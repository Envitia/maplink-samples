#ifndef _QT_OFFSCREEN_RENDERER_H_
#define _QT_OFFSCREEN_RENDERER_H_
//
// Copyright (c) 2017 by Envitia Group PLC
//

#if (QT_VERSION <= 0x50500)
# pragma error
#endif

class OffScreenData;
class QOpenGLContext;
class QOffscreenSurface;
class QOpenGLFramebufferObject;
class TSLOpenGLSurface;
class TSLInteractionModeManagerGeneric;

//!
//! This class wraps the necessary setup for drawing MapLink into an OpenGL 
//! texture for subsequence display using Qt.
//!
//! The aim of the class is to allow the user to also render using the QtPainter
//! class.
//!
//! The helper can be created thus:
//!
//! \code
//! OffScreenHelper *helper = new OffScreenHelper;
//! TSLOpenGLSurface *surface = helper->createSurface(oglContextToShareWith, 600, 600);
//! \endcode
//!
//! The helper returns a MapLink OpenGL Drawing Surface. You should add your layers to this
//! instance.
//!
//! The helper will delete the surface instance when it is deleted.
//!
//!
//! To resize the area that is drawn to you should call the helper rather than the 
//! MapLink OpenGL Drawing Surface as the the helper manages the sizing of the FBO.
//!
//! \code
//! helper->wndResize(600, 600);
//! \endcode
//!
//!
//! To draw the contents of the MapLink OpenGL Drawing Surface you should call the helper
//! as this sets up the Qt objects correctly before attaching and drawing via MapLink the
//! contents of the MapLink drawing surface.
//
//! \code
//! GLuint textureID = helper->draw();
//! \endcode
//!
//! If you call one of the methods directly on the MapLink OpenGL Drawing surface that causes
//! a draw to occur the effects are undefined as the Frame Buffer Object (FBO) or OpenGL Context
//! currently bound may not be correct ones.
//!
//!
//! \subsection Threading
//!
//! This class assumes that it is used from within the main Qt GUI thread.
//!
//! If you wish to use this class from other threads then you will need to modify the logic
//! in the class.
//!
//! For more information on how to do this please refer to the Qt documentation on:
//!  - QObject::moveToThread
//!  - QObject::deleteLater
//!
//!
class OffScreenHelper
{
public:
  //!
  //! Light weight Constructor.
  //!
  //! @see createSurface()
  //!
  OffScreenHelper();
  //!
  //! Deletes all allocated objects.
  //!
  //! This method should be called from the main Qt GUI thread.
  //!
  virtual ~OffScreenHelper();

  //!
  //! Creates a MapLink OpenGL Drawing Surface.
  //!
  //! Only the drawing mechanism is setup. Loading of resources or adding layers or loaders
  //! is not performed.
  //!
  //! This method creates the Qt objects that are required for drawing into an Offscreen
  //! OpenGL Frame Buffer Object (FBO).
  //!
  //! This method should be called from the main Qt GUI thread.
  //!
  //! The current OpenGL context will be undefined on return from this method.
  //!
  //! @param contextToShareWith is the context from the QOpenGLWidget or
  //!        QOpenGLWindow. This is required or the texture MapLink rendering
  //!        has been drawn into will not be capable of being drawn in the
  //!        calling widget/window.
  //! @param width  of the texture to be drawn into.
  //! @param height of the texture to be drawn into.
  //! @return the MapLink OpenGL drawing surface to draw into.
  //!
  TSLOpenGLSurface *createSurface(QOpenGLContext *contextToShareWith, int width, int height);

  //!
  //! This method manages the drawing of the MapLink drawing surface into an
  //! OpenGL texture.
  //!
  //! The current OpenGL context will be undefined on return from this method.
  //!
  //! The current FBO will be undefined on return from this method.
  //!
  //! @param modeManager ensures feedback from the MapLink modes is drawn into the same FBO
  //!        as the layers.
  //!
  //! @return the OpenGL texture ID.
  //!
  int draw(TSLInteractionModeManagerGeneric *modeManager = NULL);

  //!
  //! Resize the MapLink OpenGL Drawing Surface.
  //!
  //! This method has to be called to resize the drawing area as it will
  //! resize the associated OpenGL Frame Buffer Object.
  //!
  //! This method does not cause a draw to occur.
  //!
  //! @param width  of the texture to be drawn into.
  //! @param height of the texture to be drawn into.
  //! @param action  One of TSLResizeActionEnum values.  Determines if the uuExtent is
  //! adjusted automatically.  Typically, this is set to TSLResizeActionMaintainTopLeft.
  //! The default value, for compatibility is TSLResizeActionNone.  Note,
  //! that this parameter is presently ignored for Dynamic Arc maps when the
  //! TSLOptionAdjustForDynamicArc option has been set on the Drawing Surface.
  //! @return true if the resize occurred.
  //!
  bool wndResize(int width, int height, TSLResizeActionEnum action = TSLResizeActionNone);

  //!
  //! Make the OpenGL context wrapped by this class the current context.
  //!
  void makeCurrent();

  //!
  //! Stop the OpenGL context wrapped by this class from being the current context.
  //!
  //! You will need to makeCurrent() an OpenGL context before using OpenGL again.
  //!
  void doneCurrent();
protected:
  //!
  //! This method creates the necessary Qt objects to help with drawing
  //! MapLink into an offscreen texture.
  //!
  //! @param contextToShareWith is the context from the QOpenGLWidget or
  //!        QOpenGLWindow. This is required or the texture MapLink rendering
  //!        has been drawn into will not be capable of being drawn in the
  //!        calling widget/window.
  void setup(QOpenGLContext *contextToShareWith);

private:
  // OpenGL Context
  QOpenGLContext *m_glContext;
  // Offscreen surface
  QOffscreenSurface *m_qSurface;

  // Size of the area to draw to.
  QSize m_size;

  // OpenGL Frame Buffer object - this wraps an OpenGL texture.
  QOpenGLFramebufferObject *m_renderFbo;

  // MapLink OpenGL Drawing Surface.
  TSLOpenGLSurface *m_mSurface;

  //!
  //! Create a MapLink Drawing Surface
  //!
  //! An invisible window is created and the surface is attached to this.
  //! The size is arbitary as the actual rendering occurs to a FBO.
  //!
  //! @param width
  //! @param height
  //! @return true if the surface was created.
  bool prepareMapLinkSurface(int width, int height);

  // Platform Specific variables - we hide the implementation of the class
  // as exposing Windows or X11 types causes problems with Qt.
  OffScreenData *m_data;
};

#endif

