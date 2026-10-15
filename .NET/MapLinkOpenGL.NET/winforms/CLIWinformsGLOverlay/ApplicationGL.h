#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "MaplinkInterface.h"
#include "ApplicationGLInterface.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Envitia::MapLink;

namespace MaplinkWinformsGLOverlay
{
  ref class MaplinkInterface;

#if 0
  typedef System::Windows::Forms::UserControl ParentControlClass;//< to use Windows::Forms::UserControl as parent control.
#else
  typedef System::Windows::Forms::Form ParentControlClass;//< to use Windows::Forms::Form as parent control.
#endif

  /// <summary>
  /// class to display native opengl in winforms window
  /// </summary>
  ref class ApplicationGL : public System::Windows::Forms::NativeWindow
  {
#pragma region Constructor_Destructor
  public:
    /// <summary>
    /// Constructor to create window handle, opengl context
    /// </summary>
    ApplicationGL(ParentControlClass ^ parentForm, GLsizei iWidth, GLsizei iHeight)
    {
      m_parentForm = parentForm;
      m_cp = gcnew CreateParams;

      // Set the position on the form
      m_cp->X = 0;
      m_cp->Y = 0;
      m_cp->Height = iHeight;
      m_cp->Width = iWidth;

      // Specify the form as the parent.
      m_cp->Parent = parentForm->Handle;

      // Create as a child of the specified parent and make OpenGL compliant (no clipping)
      m_cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

      // Create the actual window
      this->CreateHandle(m_cp);

      m_handle = this->Handle;
      m_hDC = GetDC((HWND)this->Handle.ToPointer());
      if (m_hDC == NULL)
      {
        MessageBox::Show("Failed to create HDC from window handle");
        return;
      }

      //! initialize opengl context
      m_hglrc = MaplinkInterface::createCompatibleContext(m_hDC, NULL);
      initializeAppGL();
      resizeAppGL(iWidth, iHeight, true);

      //!
      resizeWindow(iWidth, iHeight);
    }

  protected:
    /// <summary>
    /// Destructor to delete opengl handle
    /// </summary>
    ~ApplicationGL(System::Void)
    {
      this->DestroyHandle();
      TSLNOpenGLDisplayHelper::deleteContext(m_hglrc);
    }
#pragma endregion

#pragma region PublicMainMethods
  public:
    /// <summary>
    /// Rendering function to make context current -> bind maplink shared texture -> draw cube of this texture -> draw another native cube.
    /// </summary>
    System::Void render(GLsizei iWidth, GLsizei iHeight)
    {
      //! make application context current
      makeCurrent();

      //! clear color and resize context
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
      resizeAppGL(iWidth, iHeight, true);

      //! set opengl settings
      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);
      glCullFace(GL_FRONT);
      glEnable(GL_DEPTH_TEST);

      //! enable and bind maplink texture
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, m_maplInterface->textureId());

      //! draw map texture
      const double nums = 1.0;
      float w = m_width;
      float h = m_height;
      glBegin(GL_QUADS);
      glTexCoord2d(0.0, 0.0); glVertex2f(w, 0.0f);
      glTexCoord2d(nums, 0.0); glVertex2f(0, 0);
      glTexCoord2d(nums, nums); glVertex2f(0, h);
      glTexCoord2d(0.0, nums); glVertex2f(w, h);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      //! draw application's native opengl
      //! get location of London given its Lat-Lon -> Lat = 51.509865   Lon = -0.118092
      double latitude = 51.509865, longitude = -0.118092;
      Point position;
      if (m_maplInterface->getLatLonPosition(latitude, longitude, position))
      {
        draweNativeGLRect(position.X, position.Y);//(150,200);
      }

      //! If you set an opengl interface to implement your application opengl code from the application.
      if (m_appGlInterface)
      {
        m_appGlInterface->draweNativeGL();
      }

      //! swap native opengl buffer
      SwapBuffers(m_hDC);
    }

    /// <summary>
    /// Resize native window.
    /// </summary>
    void resizeWindow(GLsizei iWidth, GLsizei iHeight)
    {
      MoveWindow(
        (HWND)this->Handle.ToPointer(),
        0,
        0,
        iWidth,
        iHeight,
        true
      );
      //m_parentForm->Invalidate();
    }
#pragma endregion

#pragma region Application_Specific_OpenGL_Methods
  public:
    /// <summary>
    /// Draw native opengl rectangle [Do your magic here...]
    /// </summary> 
    void draweNativeGLRect(int centerX, int centerY)
    {
      //glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(0.0, 1.0, 0.0);
      glLineWidth(10);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      glBegin(GL_POLYGON);
      glVertex2i(centerX - 25, centerY - 25);
      glVertex2i(centerX - 25, centerY + 25);
      glVertex2i(centerX + 25, centerY + 25);
      glVertex2i(centerX + 25, centerY - 25);
      glEnd();

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3f(1.0, 1.0, 1.0);
    }

    /// <summary>
    /// Set opengl interface to implement your application opengl code from the application.
    /// </summary> 
    void setApplicationGLInterface(ApplicationGLInterface^ appGlInterface)
    {
      m_appGlInterface = appGlInterface;
    }
#pragma endregion

#pragma region ApplicationOpenGlMethods
  protected:
    /// <summary>
    /// Make opengl context current before native drawing
    /// </summary>
    bool makeCurrent()
    {
      return wglMakeCurrent(m_hDC, m_hglrc);
    }

    /// <summary>
    /// Initialize opengl
    /// </summary>
    bool initializeAppGL(GLvoid)										// All setup for opengl goes here
    {
      if (!makeCurrent())
      {
        MessageBox::Show("wglMakeCurrent Failed");
        return false;
      }

      glShadeModel(GL_SMOOTH);							// Enable smooth shading
      glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
      glClearDepth(1.0f);									// Depth buffer setup
      glEnable(GL_DEPTH_TEST);							// Enables depth testing
      glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
      return true;										// Initialisation went ok
    }

    /// <summary>
    /// Resize opengl scene
    /// </summary>
    GLvoid resizeAppGL(GLsizei width, GLsizei height, bool ortho)		// Resize and initialise the gl window
    {
      //if (!makeCurrent())
      //{
      //  MessageBox::Show("wglMakeCurrent Failed");
      //  return;
      //}

      if (height == 0)										// Prevent A Divide By Zero By
      {
        height = 1;										// Making Height Equal One
      }

      m_height = height;
      m_width = width;

      glViewport(0, 0, width, height);						// Reset The Current Viewport

      //! projection
      glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
      glLoadIdentity();									// Reset The Projection Matrix

      // Calculate The Aspect Ratio Of The Window
      if (ortho)
      {
        glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
      }
      else
      {
        gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
      }

      //! texture
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glScalef(-1.0f, 1.0f, 1.0f);

      //! Model view
      glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
      glLoadIdentity();									// Reset The Modelview Matrix
    }
#pragma endregion

#pragma region WindowsMouseInteraction
  public:
    /// <summary>
    /// Handle key and mouse clicks/movements.
    /// </summary>
    virtual void WndProc(Message % m) override
    {
      if (m_maplInterface != nullptr)
      {
        bool invalidate = m_maplInterface->processWndMessage(m);
        if (invalidate)
        {
          //m_parentForm->Invalidate();

          //! draw maplink surface
          m_maplInterface->render();

          //! draw opengl surface
          render(m_width, m_height);
        }
      }
      NativeWindow::WndProc(m);
    }
#pragma endregion

#pragma region PublicProperties
  public:
    /// <summary>
    /// Get opengl context to share it with maplink.
    /// </summary>
    HGLRC getContext()
    {
      return m_hglrc;
    }

    /// <summary>
    /// Get window hdc.
    /// </summary>
    HDC getHdc()
    {
      return m_hDC;
    }

    /// <summary>
    /// Get wndow handle.
    /// </summary>
    IntPtr getHandle()
    {
      return m_handle;
    }

    /// <summary>
    /// Get a reference of the maplink interface class.
    /// </summary>
    void maplInterface(MaplinkInterface ^ maplInterface)
    {
      m_maplInterface = maplInterface;
    }
#pragma endregion

#pragma region Parameters
  private:
    ParentControlClass^       m_parentForm;//< parent user control.
    IntPtr                    m_handle;//< window handle.
    HDC                       m_hDC;//< window hdc.
    HGLRC                     m_hglrc;//< opengl context.
    CreateParams^             m_cp;//< create params to create the window handle.

    MaplinkInterface ^        m_maplInterface;//< maplink display helper interface.
    GLsizei                   m_height, m_width;//< opengl window height&width.
    ApplicationGLInterface^   m_appGlInterface;//< opengl interface to implement your application opengl code from the application
#pragma endregion
  };
}