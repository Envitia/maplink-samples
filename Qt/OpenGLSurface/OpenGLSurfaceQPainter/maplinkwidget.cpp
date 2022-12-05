/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#if (QT_VERSION >= 0x50000)
#include <QGuiApplication>
#endif
#include <QMessageBox>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "MapLink.h"
#include "MapLinkOpenGLSurface.h"

#include "application.h"
#include "maplinkwidget.h"
#include "offscreenrender.h"

#include <iostream>
#include <cassert>

using namespace std;

#define DRAW_OGL

// These are defined by X11 which interfere with the Qt definitions
#ifdef KeyPress
# undef KeyPress
#endif
#ifdef KeyRelease
# undef KeyRelease
#endif

#define PROGRAM_VERTEX_ATTRIBUTE     0
#define PROGRAM_TEXCOORD_ATTRIBUTE   1
#define PROGRAM_SCREENSIZE_ATTRIBUTE 2

// OpenGL vertext and fragment shaders for drawing a texture as 1:1 pixels
static const char *vertexShaderSrc =
        "attribute vec4 vertex;\n"
        "attribute vec4 texCoord;\n"
        "varying vec4 texC;\n"
        "uniform mat4 matrix;\n"
        "uniform vec2 screenSize;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vec4(screenSize * (vertex.xy), 1, 1);\n"
        "    texC = texCoord;\n"
        "}\n";

static const char *fragmentShaderSrc =
        "uniform sampler2D texture;\n"
        "varying vec4 texC;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texC.st);\n"
        "}\n";

MapLinkWidget::MapLinkWidget( QWidget * parent, Qt::WindowFlags f )
  : QOpenGLWidget(parent, f)
  , m_application( new Application( parent ) ) // We don't create any OpenGL resources in the Application class at this point in time.
  , m_texture(NULL)
  , m_program(NULL)
  , m_offScreenHelper(NULL)
  , m_clearColor(Qt::magenta) // A colour that you would not normally see in a map.
  , m_width(0)
  , m_height(0)
  , m_vShader(NULL)
  , m_fShader(NULL)
{
  setAutoFillBackground( false ); // We are using OpenGL to clear the background of this Widget

  if (parent)
  {
    // If this Widget is not a child of a MainWindow then we need
    // to configure the parent so that we get Keyboard and Mouse
    // Events.
    //
    // In Qt Designer do not set the Focus and Mouse Tracking.
    // If you do delete the entries from the 'ui' file.
    //
    parent->installEventFilter( this );
    // See below.
    parent->setFocusPolicy( Qt::WheelFocus );
    parent->setMouseTracking( true );
  }
  // Strong Focus and Wheel Mouse.
  setFocusPolicy(Qt::WheelFocus);

  // Set the mouse tracking in the designer.
  setMouseTracking( true );

  setWindowTitle(tr("SimpleGLSurfaceSample QOpenGLWidget"));
}

MapLinkWidget::~MapLinkWidget()
{
  // Clean up.
  makeCurrent();

  // The OpenGL resources have to be deleted using the correct OpenGL context.
  // Order of delete is important
  if (m_offScreenHelper)
  {
    m_offScreenHelper->makeCurrent();
  }

  if (m_application)
  {
    delete m_application;
  }

  if (m_offScreenHelper)
  {
    m_offScreenHelper->doneCurrent();
    delete m_offScreenHelper;
  }

  m_application = NULL;
  m_offScreenHelper = NULL;

  makeCurrent();

  m_vao.destroy();
  m_vbo.destroy();

  if (m_program)
  {
    m_program->removeAllShaders();
    m_program->release();
  }
  if (m_vShader)
  {
    delete m_vShader;
  }
  if (m_fShader)
  {
    delete m_fShader;
  }

  if (m_program)
  {
    delete m_program;
  }
  if (m_texture)
  {
    delete m_texture;
  }

  m_fShader = NULL;
  m_vShader = NULL;
  m_program = NULL;
  m_texture = NULL;
  
  doneCurrent();
}

QSize MapLinkWidget::minimumSizeHint() const
{
  return QSize(300, 300);
}

QSize MapLinkWidget::sizeHint() const
{
  return QSize(500, 300);
}

// Map to load.
void MapLinkWidget::loadFile( const char *filename)
{
  if( m_application )
  {
    m_application->loadFile( filename ); // Load the map/file
    m_application->resetView(); // Reset the view to show the entire extent of the new map/file
    update(); // Cause a redraw so the new map/file can be seen
  }
}

// Mouse, Keyboard and event handling
//
// We only update the display if MapLink tells us too.
void MapLinkWidget::mouseMoveEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;

  // The MapLink application class doesn't use Qt - convert the mouse button types
  // to the MapLink types
  TSLButtonType button = TSLButtonNone ;
  Qt::MouseButton buttonQt = event->button();
  switch( buttonQt )
  {
  case Qt::LeftButton:
     button = TSLButtonLeft;
     break;

  case Qt::MidButton:
    button = TSLButtonCentre;
    break;

  case Qt::RightButton:
    button = TSLButtonRight;
    break;

  default:
    break;
  }

  // Forward the event onto the application
  if( m_application->mouseMoveEvent(button, shiftPressed, controlPressed, event->x(),  event->y()) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::mousePressEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();

  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
     redraw = m_application->onLButtonDown( shiftPressed, controlPressed, x, y );
     break;

  case Qt::MidButton:
    redraw = m_application->onMButtonDown( shiftPressed, controlPressed, x, y );
    break;

  case Qt::RightButton:
    redraw = m_application->onRButtonDown( shiftPressed, controlPressed, x, y );
    break;

  default:
    break;
  }

  if( redraw )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::mouseReleaseEvent( QMouseEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x() ;
  int y = event->y() ;
  Qt::MouseButton button = event->button();
  
  // Forward the event onto the application
  bool redraw = false;
  switch( button )
  {
  case Qt::LeftButton:
     redraw = m_application->onLButtonUp( shiftPressed, controlPressed, x, y );
     break;

  case Qt::MidButton:
    redraw = m_application->onMButtonUp( shiftPressed, controlPressed, x, y );
    break;

  case Qt::RightButton:
    redraw = m_application->onRButtonUp( shiftPressed, controlPressed, x, y );
    break;

  default:
    break;
  }
  if( redraw )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::wheelEvent( QWheelEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;
  int x = event->x() ;
  int y = event->y() ;

  // Forward the event onto the application
  if( m_application->onMouseWheel(shiftPressed, controlPressed, event->delta(), x, y) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::keyPressEvent( QKeyEvent *event )
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = modifiers & Qt::ShiftModifier;
  bool controlPressed = modifiers & Qt::ControlModifier;

  // Forward the event onto the application
  if( m_application->onKeyPress( shiftPressed, controlPressed, event->key() ) )
  {
    update(); // We were asked to redraw the display
  }
}

void MapLinkWidget::redrawRequired (const TSLEnvelope&, unsigned int)
{
  // Trigger a redraw
  update();
}

// Qt calls this to initalise OpenGL
void MapLinkWidget::initializeGL()
{
  // Initialise OpenGL for Qt - this is similar to using GLEW.
  initializeOpenGLFunctions();

  // For drawing we need a Vertex Attribute Object (VAO) and a Vertex Buffer Object (VBO).
  //
  // Define the coordinates of the Rectangle we will be drawing with the MapLink
  // texture on it.

#ifdef DRAW_OGL
  m_vao.create();
  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

  //
  // We are going to use a triangle fan to draw the rectangle.
  //
  //  4
  //  +---------------------+ 3
  //  |                     |
  //  |                     |
  //  |                     |
  //  |                     |
  //  |                     |
  //  +---------------------+
  //  1                     2
  //
  // The points are defined 1, 2, 3, 4 as shown above (anti-clockwise).
  //
  // The order of the points is defined below and is significant.
  //
  // This assumes the bottom left is [0,0] and top right is [1,1]
  //
  vertData.append(0.0f);   // x
  vertData.append(0.0f);   // y
  vertData.append(0.0f);   // z
  vertData.append(0.0f);   // s - texture
  vertData.append(0.0f);   // t - texture

  vertData.append(1.0f);   // x
  vertData.append(0.0f);   // y
  vertData.append(0.0f);   // z
  vertData.append(1.0f);   // s - texture
  vertData.append(0.0f);   // t - texture

  vertData.append(1.0f);   // x
  vertData.append(1.0f);   // y
  vertData.append(0.0f);   // z
  vertData.append(1.0f);   // s - texture
  vertData.append(1.0f);   // t - texture
  
  vertData.append(0.0f);   // x
  vertData.append(1.0f);   // y
  vertData.append(0.0f);   // z
  vertData.append(0.0f);   // s - texture
  vertData.append(1.0f);   // t - texture
  // NOTE: normally we would create a VBO here and store the coordinates in the
  // VBO. However QPainter (5.8) for some reason will crash unless we delete our VBO
  // before we use an instance of the class.

  // Create Vertex and Fragment shader to copy the texture to the screen.
  // We store a pointer to these instances because the program does not take ownership.
  m_vShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  m_vShader->compileSourceCode(vertexShaderSrc);
  m_fShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  m_fShader->compileSourceCode(fragmentShaderSrc);
  m_program = new QOpenGLShaderProgram;
  m_program->addShader(m_vShader);
  m_program->addShader(m_fShader);
  // We need to set where the shader variables are for later use.
  m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
  m_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
  // Link the shader to create a program
  m_program->link();
  // Make the program the current one.
  m_program->bind();
  // Set the texture to use to be the first one.
  m_program->setUniformValue("texture", 0);
  m_program->setUniformValue("screenSize", m_width, m_height);
  m_program->release();

  assert(m_application); // created in the constructor

  // Create the Offscreen Helper class
  if (!m_offScreenHelper)
  {
    // We don't create any OpenGL resources in the OffScreenHelper class at this
    // point in time.
    m_offScreenHelper = new OffScreenHelper;
  }

  // Create the MapLink OpenGL Drawing Surface and render target objects
  // The OpenGL context will be switched to the one we will be using for MapLink Pro
  // drawing - NOTE we pass in the this Widget's OpenGL context as we want to 
  // share OpenGL texture resources between the context's.
  TSLOpenGLSurface *surface = m_offScreenHelper->createSurface(context(), 300, 300);

  // Initialise the Application Object that contains the application and
  // maplink logic.
  // The OpenGL context will not be for this Widget at this point in time.
  m_application->create(this, surface, m_offScreenHelper );

  // Set the current OpenGL Context back to this Widget.
  // We don't want to switch OpenGL Context objects too often.
  makeCurrent();
#endif
}

// resize - informs the drawing surface of any change in size of the window
// Relys on the ResizeAction to maintain the view of the map sensibly
void MapLinkWidget::resizeGL(int width, int height)
{
  // Resize the MapLink surface and Render target
  if (m_offScreenHelper)
  {
    if (width <= 0 || height <= 0)
    {
      width = height = 100; // minimum size
    }
    m_offScreenHelper->wndResize(width, height);
  }

  // Make sure we are using the OpenGL context for this Widget.
  makeCurrent();

  // Define the area to draw into for this Widget.
  // We are mapping the MapLink drawing surface texture 1:1 with the screen area
  // for this widget.
  glViewport(0, 0, width, height);

  // Store the actual size as we need this for drawing the MapLink Pro drawing
  // surface texture.
  m_width = width;
  m_height = height;
}

void MapLinkWidget::paintEvent(QPaintEvent *e) {
   paintGL();
}

// Qt calls this to draw OpenGL
void MapLinkWidget::paintGL()
{
  makeCurrent();

  QOpenGLPaintDevice device(rect().size());  // This device could be cached - only updating when the size of Widget changes.
  QPainter painter(&device);

  // Notify QPainter that we are going to start drawing with OpenGL.
  painter.beginNativePainting();

#ifdef DRAW_OGL
  // To understand what can be drawn where please refer to the Qt documentation for
  //
  // - QOpenGLWidget
  // - QOpenGLWindow
  //
  // There are a number of Qt examples that show how to use both these Qt classes.
  //

  // The draw() method causes the MapLink drawing surface to draw the attached layers.
  //
  // MapLink is using a different OpenGL Context than that of this widget. This
  // reduces the amount of interference between Qt and MapLink Pro OpenGL drawing
  // surface.
  GLuint textureID = m_application->draw();

  // Set the current OpenGL Context to that of this widget.
  makeCurrent();

  // We reset the viewport as we don't know what the state may be after a QPainter
  // has been used.
  glViewport(0, 0, m_width, m_height);
  
  // Because this Widget is drawing into a different off-screen display we need to clear
  // the display.
  //
  // If we know we are always going to draw over the whole area then we could just clear the
  // depth buffer. However clearing both the depth and colour buffers is safer.
  //
  // Note: this will clear down the whole area of drawing. If you are drawing with QPainter
  // below then consider clearing the FBO before drawing with QPainter.
  glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF()); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Some sensible OpenGL settings.
  glFrontFace(GL_CW);
  glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  if (textureID != 0) // check to make sure we have a texture to display.
  {
    // MapLink drew something.

    // Note: The code below is not strictly optimal. However the over-head of performing
    // the setup for drawing is likly to be quite minimal compared to drawing a map.
    //
    // If performance is critical and all other options have been exhausted then I would
    // suggest using either the AMD or NVIDIA GPU debuggers to look at the frame performance.
    //
 
    // We will be using the shader we created in initializeGL() to draw the texture.
    //
    // We have to define the display matrix to be a Orthographic projection as we are displaying a 2D
    // plane.
    QMatrix4x4 m;
    m.ortho(0.0f, m_width, 0.0f, m_height, -1.0f, 1.0f); // origin lower-left corner

    // Setup the VAO and the texture drawing program so that we can draw the MapLink Pro drawing surface
    // texture.
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao); // this will release the VAO.
    m_program->bind(); // activate the shader program
    m_program->setUniformValue("matrix", m); // pass in the matrix for display.
    m_program->setUniformValue("screenSize", m_width, m_height); // pass in the screen size

    // Create a VBO on the fly - this is not the correct way to do this however when using a QPainter
    // we have to clear a significant amount of resources down or we will crash inside the OpenGL
    // driver.
    //
    // I have dropped to using OpenGL directly here as it did not seem to be possible to clear
    // down resources sufficently for QPainter to work.
    //
    // This approach is not particularly efficent and should be reconsidered after Qt5.9
    // release.
    //
    GLuint m_glVBO;
    glGenBuffers(1, &m_glVBO);                                        // Create a data buffer for our rectangle coordinates
    glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);                           // bind the data buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertData.count(), vertData.constData(), GL_STATIC_DRAW); // copy the data to the GPU
    glEnableVertexAttribArray(0);                                     // Enable the first two Vertex attribute indices
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);   // point to the xyz data and define how much data per point
    glVertexAttribPointer(PROGRAM_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),(void *)(3 * sizeof(GLfloat))); // point to the st data etc...

    // Activate Texture unit 0 (see initialiseGL()).
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID); // Set the texture ID returned from m_application->draw()

    // Draw the rectangle with the texture pasted on to it.
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // disable and/or release allocated resources
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteBuffers(1, &m_glVBO);
    m_program->release();
  }

  // QPainter expects the following settings - possibly more may be required in future see the QPainter
  // documentation.
  glClear (GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_BLEND);

  // Note: Any resources you have allocated via OpenGL need to be released or turned off by this point.
#endif

  // When we have finished with OpenGL we have to call the following function to 
  // notify QPainter.
  painter.endNativePainting();

  //
  // QPainter expects all OpenGL resources to be released and OpenGL to be in a default
  // state.
  //
  // This is why we use a different OpenGL context from the Qt one and why the code for
  // displaying the texture containing the drawn contents of a MapLink drawing surface
  // has been written in an inefficent manner.

  // Over draw using QPainter
  painter.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
  painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
  painter.drawEllipse(80, 80, 200, 100);

  painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
  QFont font;
  font.setPointSize(24);
  painter.setFont(font);
  painter.setPen(QPen(Qt::red, 12, Qt::DashDotLine, Qt::RoundCap));
  painter.drawText(rect(), Qt::AlignCenter, "MapLink Pro QPainter example.");
  painter.end();
}

// Event filter - We require this when not directly attached to a
// MainWindow so that we can receive Keyboard, Mouse and Resize
// events.
bool MapLinkWidget::eventFilter(QObject *obj, QEvent *event)
{
  if (obj != parent())
  {
    return false;
  }

  switch( event->type() )
  {
  case QEvent::KeyPress:
    {
      QKeyEvent *ke = static_cast<QKeyEvent *>(event);
      keyPressEvent( ke );
      return false;
    }
  case QEvent::KeyRelease:
    {
      QKeyEvent *ke = static_cast<QKeyEvent *>(event);
      keyReleaseEvent( ke );
      return false;
    }
  case QEvent::MouseButtonPress:
    {
      QMouseEvent *ke = static_cast<QMouseEvent *>(event);
      mousePressEvent( ke );
      return false;
    }
  case QEvent::MouseButtonRelease:
    {
      QMouseEvent *ke = static_cast<QMouseEvent *>(event);
      mouseReleaseEvent( ke );
      return false;
    }
  case QEvent::MouseMove:
    {
      QMouseEvent *ke = static_cast<QMouseEvent *>(event);
      mouseMoveEvent( ke );
      return false;
    }
  case QEvent::Wheel:
    {
      QWheelEvent *ke = static_cast<QWheelEvent *>(event);
      wheelEvent( ke );
      return false;
    }
  case QEvent::Resize:
    {
      QResizeEvent *re = static_cast<QResizeEvent *>(event);

      setGeometry(0, 0, re->size().width(), re->size().height());
      return true;
    }
  case QEvent::Close:
    {
      QCloseEvent *ce = static_cast<QCloseEvent *>(event);
      closeEvent( ce );
    }
  default:
    return false;
  }
}

///////////////////////////////////////////
// Events forwarded from the main window. These are all forwarded on to
// the appliction, and we will issue a redraw request if the application asks
// us to.
void MapLinkWidget::resetView()
{
  m_application->resetView();
  update(); // The viewing extent has changed, so a redraw is always required
}

void MapLinkWidget::zoomInOnce()
{
  if( m_application->zoomIn() )
  {
    // We were asked to redraw the display
    update();
  }
}

void MapLinkWidget::zoomOutOnce()
{
  if( m_application->zoomOut() )
  {
    // We were asked to redraw the display
    update();
  }
}

void MapLinkWidget::activatePanMode()
{
  // Tell the application to activate the pan interaction mode
  m_application->activatePanMode();
}

void MapLinkWidget::activateGrabMode()
{
  // Tell the application to activate the grab interaction mode
  m_application->activateGrabMode();
}

void MapLinkWidget::activateZoomMode()
{
  // Tell the application to activate the zoom interaction mode
  m_application->activateZoomMode();
}

void MapLinkWidget::enableBufferedLayerTiling( bool enable )
{
  // Tell the application to enable/disable buffered layer tiling
  //
  // Limitation: MapLink has to create the Context at present for this to work.
  //
  //m_application->enableBufferedLayerTiling( enable );
}
