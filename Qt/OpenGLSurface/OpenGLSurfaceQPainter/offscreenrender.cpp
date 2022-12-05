//
// Copyright (c) 2017 by Envitia Group PLC
//
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOffscreenSurface>
#include <QOpenGLFunctions>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>


#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN	// Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif

# include <windows.h>
# include <direct.h>
# include <tchar.h>
# include <iostream>

#else

// These are defined by X11 which interfere with the Qt definitions
# ifdef KeyPress
#  undef KeyPress
# endif
# ifdef KeyRelease
#  undef KeyRelease
# endif
# ifdef Bool
#  undef Bool
# endif

# include <X11/Xlib.h>
#endif /* !WIN32 */


// Include MapLink
#include "MapLink.h"
#include "MapLinkOpenGLSurface.h"
#include "MapLinkIMode.h"

// include the header for this file.
#include "offscreenrender.h"


#ifndef MM2INCH
# define MM2INCH (0.0393700787)
#endif
#ifndef DPI_CONSTANT
# define DPI_CONSTANT (96.0)
#endif


//!
//! This class hides the platform specific variables
//!
class OffScreenData
{
public:
  OffScreenData();
  ~OffScreenData();

#ifdef WIN32
  WNDCLASSEX m_winClass;
  HWND m_hiddenWnd;
  HDC m_hiddenWndHDC;
#else
  Display *m_display;
  Screen *m_screen;
  Window m_window;
#endif
};


// The following blog post may help with understanding what we are doing:
//  - https://dangelog.wordpress.com/2013/02/10/using-fbos-instead-of-pbuffers-in-qt-5-2/
//
OffScreenHelper::OffScreenHelper() : m_glContext(NULL), m_qSurface(NULL), m_renderFbo(NULL), m_mSurface(NULL), m_data(new OffScreenData)
{
}

OffScreenHelper::~OffScreenHelper()
{
  // The order of deletion is important as OpenGL context and resources need
  // to be cleaned up in the correct order.
  if (m_glContext)
  {
    makeCurrent();
  }

  if (m_renderFbo)
  {
    m_renderFbo->bindDefault();
    delete m_renderFbo;
  }
  if (m_qSurface)
  {
    delete m_qSurface;
  }
  
  if (m_mSurface)
  {
    // Tell MapLink to re-read the OpenGL state
    m_mSurface->stateTracker()->reset();
    delete m_mSurface;
  }

  if (m_glContext)
  {
    doneCurrent();
    delete m_glContext;
  }

  m_glContext = NULL;
  m_renderFbo = NULL;
  m_qSurface = NULL;
  m_mSurface = NULL;

  delete m_data;
  m_data = NULL;
}


TSLOpenGLSurface *OffScreenHelper::createSurface(QOpenGLContext *contextToShareWith, int width, int height)
{
  if (!contextToShareWith || width <= 0 || height <= 0)
  {
    assert(false);
    return NULL;
  }

  // Create an OpenGL Context, QOffscreenSurface
  // The created context will be shared with the one passed in.
  setup(contextToShareWith);

  assert(m_qSurface->isValid());

  makeCurrent();

  if (!prepareMapLinkSurface(width, width))
  {
    assert(false);
    return NULL;
  }

  bool result = wndResize(width, height);
  assert(result);

  doneCurrent();
  return m_mSurface;
}

void OffScreenHelper::setup(QOpenGLContext *contextToShareWith)
{
  // Qt will be creating the OpenGL context for us. In order for the drawing surface to
  // work at its best we ask it to choose a framebuffer configuration with a specific set of
  // parameters.
  QSurfaceFormat format;

  // Want Desktop OpenGL
  format.setRenderableType( QSurfaceFormat::OpenGL );

  // Set the minimum OpenGL version to use
  format.setMajorVersion(3); // has to be >= 3.2
  format.setMinorVersion(3);
  format.setProfile( QSurfaceFormat::CompatibilityProfile );   // required for QPainter limitations (until Qt5.9)

#ifndef NDEBUG
  // Debug if building debug
  format.setOption( QSurfaceFormat::DebugContext );
#endif

  // Set the application default options
  //QSurfaceFormat::setDefaultFormat( format ); // we don't set the defaults here because of QPainter requirements.

  // The following settings are used for MapLink Pro

  // Request a 24-bit depth buffer. A 16-bit depth buffer will also work.
  format.setDepthBufferSize( 24 );

  // Set the RGBA channel sizes
  format.setGreenBufferSize( 8 );
  format.setBlueBufferSize( 8 );
  format.setRedBufferSize( 8 );
  format.setAlphaBufferSize( 8 );

  // For offscreen rendering we can't use samples as it stops us getting a texture quickly.
  format.setSamples( 0 );

  // The application is managing the buffering using FBO and textures.
  format.setSwapBehavior( QSurfaceFormat::SingleBuffer );

  // Create an OpenGL Context.
  //
  // We need our own context because MapLink will change a lot of the OpenGL state.
  // Qt also changes a lot of the OpenGL State.
  // It is not possible to track the state changes occuring via Qt.
  //
  m_glContext = new QOpenGLContext;
  m_glContext->setFormat(format);

  // Some OpenGL drivers require the contexting being shared with to not be current
  contextToShareWith->doneCurrent();
  m_glContext->setShareContext(contextToShareWith);
  m_glContext->create();
  assert(m_glContext->isValid());

  // Create an Offscreen Surface for drawing MapLink into.
  // This may or may not actually have any Window's associated with it.
  m_qSurface = new QOffscreenSurface();
  m_qSurface->setFormat(m_glContext->format());
  m_qSurface->create();
  assert(m_qSurface->isValid());
}


bool OffScreenHelper::wndResize(int width, int height, TSLResizeActionEnum action)
{
  if (!m_glContext || !m_mSurface || !m_qSurface)
    return false;

  makeCurrent();

  if (width != m_size.width() || height != m_size.height())
  {
    // We need to resize the off-screen area we are drawing to.
    if (!m_renderFbo)
    {
      // size of draw area has been changed.
      delete m_renderFbo;
      m_renderFbo = NULL;
    }

    // Note:
    // If we set samples to be non-zero then we need to use blitFrameBuffer to
    // copy into a texture for subsequent rendering.
    //
    // This affects performance so the sample does not do this.
    //
    // You may wich to set samples to non 0 if you want to improve the display
    // via the use of anti-aliasing.
    //
    // If samples == 0 then we get a texture
    // If samples != 0 then we get a colour buffer (Render Buffer)
    QOpenGLFramebufferObjectFormat format;
    format.setMipmap(false);
    format.setSamples(0); // we require a texture
    format.setTextureTarget(GL_TEXTURE_2D);
    format.setInternalTextureFormat(GL_RGBA);

    m_size.setWidth(width);
    m_size.setHeight(height);

    m_renderFbo = new QOpenGLFramebufferObject(m_size, format);

    if (!m_renderFbo || !m_renderFbo->isValid())
    {
      doneCurrent();
      if (m_renderFbo)
      {
        delete m_renderFbo;
        m_renderFbo = NULL;
      }
      return false; // failed.
    }

    m_renderFbo->bind(); // make this the current FBO

    // Tell MapLink to re-read the OpenGL state for the current context.
    //
    // This does impact performance but there is no simple way to track
    // what Qt is doing with the OpenGL context.
    m_mSurface->stateTracker()->reset();

    m_mSurface->stateTracker()->bindFramebuffer(GL_FRAMEBUFFER, m_renderFbo->handle()); // tell MapLink what the current FBO is.

    // Set these values to ensure consistent results across machines/platforms
    int widthMM = (int)(width / (DPI_CONSTANT * MM2INCH));
    int heightMM = (Int)(height / (DPI_CONSTANT * MM2INCH));
    m_mSurface->setDeviceCapabilities(widthMM, heightMM, width, height);

    // we don't attempt to draw here.
    m_mSurface->wndResize(0, 0, width, height, false, action);
  }

  // reset the defaults
  if (m_renderFbo)
  {
    m_renderFbo->bindDefault();
    m_mSurface->stateTracker()->bindFramebuffer(GL_FRAMEBUFFER, 0); 
  }
  doneCurrent();
  return true;
}

int OffScreenHelper::draw(TSLInteractionModeManagerGeneric *modeManager)
{
  if (!m_renderFbo || !m_mSurface || !m_qSurface || !m_glContext)
  {
    return 0;
  }
  // OpenGL only has one current Context at a time.
  // We have to make the one we created our OpenGL resources with current.
  makeCurrent();

  // The following call makes the FBO the current drawing target for OpenGL
  m_renderFbo->bind();

  // Tell MapLink to re-read the OpenGL state for the current context.
  //
  // This does impact performance but there is no simple way to track
  // what Qt is doing with the OpenGL context.
  //
  // We could consider just using OpenGL directly with a separate OpenGL context
  // that we have created. This would make tracking the OpenGL context state
  // simplier. However we would have to write considerably more code to handle
  // such things as FBO, offscreen setup as well as using something like GLEW
  // to handle the initialisation of the OpenGL function pointers and extensions.
  //
  m_mSurface->stateTracker()->reset();

  // Tell MapLink what the current FBO is.
  m_mSurface->stateTracker()->bindFramebuffer(GL_FRAMEBUFFER, m_renderFbo->handle());

  int textureId =  m_renderFbo->texture();
  m_mSurface->stateTracker()->bindTexture(GL_TEXTURE0, GL_TEXTURE_2D, textureId);

  // Do maplink drawing
  m_mSurface->drawDU(0, 0, m_size.width(), m_size.height(), true);

  // Mode Manager drawing.
  if (modeManager)
  {
    modeManager->onDraw( 0, 0, m_size.width(), m_size.height() );
  }

  // Flush the drawing 
  m_mSurface->swapBuffers();
  m_glContext->functions()->glFlush();  // should be a no-op

  // If we called setSamples() with a non-zero value then we would need to use
  //  - QOpenGLFramebufferObject::hasOpenGLFramebufferBlit() 
  //  - QOpenGLFramebufferObject::blitFramebuffer(...)
  // To copy the contents of the sample buffers into a FBO with a single texture.
  //
  // If hasOpenGLFramebufferBlit() returns false then you will not be able to copy the
  // sample buffers.
  // 
  // For this sample we assume setSamples(0).
  //
  // Note: OpenGLFramebufferBlit is an extension to OpenGL. It may not be implemented.
 
  // Get the OpenGL texture ID.
  textureId =  m_renderFbo->texture();

  // reset the defaults
  m_renderFbo->bindDefault();
  doneCurrent();
  return textureId;
}

void OffScreenHelper::makeCurrent()
{
  if (m_glContext && m_qSurface)
    m_glContext->makeCurrent(m_qSurface);
}

void OffScreenHelper::doneCurrent()
{
  if (m_glContext)
    m_glContext->doneCurrent();
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// Platform specific setup code
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef WINNT
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_CLOSE:
    PostQuitMessage(0);
    return 0;

  default:
    break;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif


bool OffScreenHelper::prepareMapLinkSurface(int width, int height)
{
  if (!m_data)
  {
    return false;
  }

  // Note: the width and height passed in are never changed.
  // These are just starting points for creating a window.

  // Set the MapLink Drawing Surface creation options
  TSLOpenGLSurfaceCreationParameters creationOptions;
  creationOptions.useVSync(false); // Vsync is unnecessary for tests as the window is invisible
  creationOptions.numMultisampleSamples(0);
  creationOptions.swapBuffersManually(true);
  creationOptions.createCoreProfile(false);
  creationOptions.enableDoubleBuffering(false);

#ifdef WIN32
  //The displayDevice is used to retrieve the device name
  DISPLAY_DEVICE displayDevice;
  displayDevice.cb = sizeof(DISPLAY_DEVICE);

  //device index, this code finds only the first monitor
  DWORD devIndex = 0;
  EnumDisplayDevices(0, devIndex, &displayDevice, 0);

  //Making sure we have the correct device.
  if (displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
  {
    std::cout << "Device attached to desktop" << std::endl;
  }
  else
  {
    std::cout << "Device not attached to desktop" << std::endl;
  }

  // We need to have a valid rendering context in order to initialise OpenGL,
  // under windows this requires us to have a window. Therefore create an invisible window
  m_data->m_winClass.lpszClassName = (LPCTSTR)_T("WINDOW_CLASS");
  m_data->m_winClass.cbSize = sizeof(WNDCLASSEX);
  m_data->m_winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  m_data->m_winClass.lpfnWndProc = &WindowProc;
  m_data->m_winClass.hInstance = 0;
  m_data->m_winClass.hIcon = 0;
  m_data->m_winClass.hIconSm = 0;
  m_data->m_winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  m_data->m_winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  m_data->m_winClass.lpszMenuName = NULL;
  m_data->m_winClass.cbClsExtra = 0;
  m_data->m_winClass.cbWndExtra = 0;

  RegisterClassEx(&m_data->m_winClass);

  m_data->m_hiddenWnd = CreateWindowEx(NULL, (LPCTSTR)_T("WINDOW_CLASS"), (LPCTSTR)_T("Rendering context window"), /*WS_OVERLAPPEDWINDOW*/0,
      0, 0, width, height, NULL, NULL, 0, NULL);
  if (m_data->m_hiddenWnd == NULL)
  {
    return false;
  }
  UpdateWindow(m_data->m_hiddenWnd);
  m_data->m_hiddenWndHDC = GetDC(m_data->m_hiddenWnd);
 
  // Create the plaform specific MapLink Drawing Surface,
  // using the constructor that takes an OpenGL context.
  HGLRC currentContext = wglCreateContext(m_data->m_hiddenWndHDC);
  m_mSurface = new TSLWGLSurface(m_data->m_hiddenWndHDC, true, currentContext, creationOptions);

  HGLRC context = ((TSLWGLSurface*)m_mSurface)->context();
#else 
  m_data->m_display = XOpenDisplay(NULL);
  if (m_data->m_display == NULL)
  {
    return false;
  }
  m_data->m_screen = DefaultScreenOfDisplay(m_data->m_display);

  if (getenv("SYNC"))
  {
    printf("Synchronise display on\n");
    XSynchronize(m_data->m_display, True);
  }

  int visualID = TSLGLXSurface::preferredVisualID(m_data->m_display, m_data->m_screen, creationOptions);

  // Look up visual for returned configuration
  XVisualInfo visualTemplate;
  visualTemplate.screen = XScreenNumberOfScreen(m_data->m_screen);
  visualTemplate.visualid = visualID;
  int numVisualMatches = 0;
  XVisualInfo *visualData = XGetVisualInfo(m_data->m_display, VisualIDMask | VisualScreenMask, &visualTemplate, &numVisualMatches);
  XVisualInfo chosenVisualData = visualData[0];
  XFree(visualData);

  XSetWindowAttributes attr;
  attr.border_pixel = 0;
  attr.background_pixel = 0;
  attr.colormap = XCreateColormap(m_data->m_display, RootWindow(m_data->m_display, chosenVisualData.screen), chosenVisualData.visual, AllocNone);
  attr.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
  m_data->m_window = XCreateWindow(m_data->m_display, RootWindow(m_data->m_display, chosenVisualData.screen),
    0, 0, width, height, 0, chosenVisualData.depth, InputOutput,
    chosenVisualData.visual,
    CWBorderPixel | CWColormap | CWEventMask, &attr);

  XSetStandardProperties(m_data->m_display, m_data->m_window, "MapLink Pro Rendering", "MapLink Pro Rendering", None, NULL, 0, NULL);
  //XMapWindow( m_display, m_window ); // Don't map the window - it's not actually ever drawn to

  GLXDrawable drawable = glXGetCurrentDrawable();
  GLXContext currentContext = glXGetCurrentContext();

  m_mSurface = new TSLGLXSurface(m_data->m_display, m_data->m_screen, drawable, currentContext, creationOptions, m_data->m_window);
  GLXContext context = ((TSLGLXSurface*)m_mSurface)->context();
#endif

  if( !context )
  {
    // The drawing surface failed to attach to the context - show the error from the error stack.
    // This means the drawing surface cannot be used, so exit the sample
    const char *msg = TSLErrorStack::errorString() ;
    if ( msg )
    {
      std::cout << "Failed to attach drawing surface : " << msg << std::endl; 
    }
    else
    {
      std::cout << "Failed to attach drawing surface : Unknown error" << std::endl;
    }
    return false;
  }

  return true;
}

OffScreenData::~OffScreenData()
{
#ifdef WIN32
  if (m_hiddenWndHDC)
  {
    ReleaseDC(m_hiddenWnd, m_hiddenWndHDC);
  }
  if (m_hiddenWnd)
  {
    DestroyWindow(m_hiddenWnd);
    UnregisterClass(m_winClass.lpszClassName, m_winClass.hInstance);
  }
  m_hiddenWnd = 0;
  m_hiddenWndHDC = 0;
#else
  if (m_display && m_window)
  {
    XDestroyWindow(m_display, m_window);
  }
  if (m_display)
  {
    XCloseDisplay(m_display);
  }
  m_display = NULL;
  m_screen = NULL;
  m_window = 0;
#endif
}


OffScreenData::OffScreenData()
{
#ifdef WIN32
  // Hidden window objects
  m_hiddenWnd = 0;
  m_hiddenWndHDC = 0;
#else
  m_display = NULL;
  m_screen = NULL;
  m_window = None;
#endif
}

