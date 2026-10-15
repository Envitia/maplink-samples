#pragma once

#include "InteractionModeRequestReceiver.h"
#include "CustomInteractionModeZoomPan.h"
#include "ModeInteractions.h"

using namespace System::Drawing;
using namespace System;
using namespace System::Windows::Forms;
using namespace Envitia::MapLink;
using namespace ModeInteractions;

namespace MaplinkWinformsGLOverlay
{
  /// <summary>
  /// Class that contains maplink display helper which has its own context.
  /// </summary>
  ref class MaplinkInterface
  {
#pragma region Constructor_Destructor
  public:
    /// <summary>
    /// Constructor to initialize the creation options, create context for the drawing surface, and create dispay helper class.
    /// </summary>
    MaplinkInterface(HDC hDC, GLsizei iWidth, GLsizei iHeight, bool createExternalContext)
      : m_modeManager(nullptr)
      , m_updateReceiver(nullptr)
      , m_zoomMode(nullptr)
      , m_panMode(nullptr)
      , m_grabMode(nullptr)
      , m_myCustomMode(nullptr)
      , m_shiftButtonDown(false)
      , m_controlButtonDown(false)
    {
      m_hDC = hDC;
      m_width = iWidth;
      m_height = iHeight;
      m_createExternalContext = createExternalContext;
    }

    /// <summary>
    /// destructor to delete the maplink helper interface
    /// </summary>
    ~MaplinkInterface()
    {
      if (m_baseHelper)
      {
        delete m_baseHelper;
      }
    }
#pragma endregion

#pragma region PublicMainMethods
  public:
    /// <summary>
    /// Create off screen helper and add data to its drawing surface.
    /// </summary>
    void createOffScreenHelper(IntPtr sharedHandle, void *sharedContext)
    {
      //! initialize the creation options
      TSLNOpenGLSurfaceCreationParameters^ creationOptions = gcnew TSLNOpenGLSurfaceCreationParameters();
      creationOptions->useVSync(false);
      creationOptions->numMultisampleSamples(0);
      creationOptions->swapBuffersManually(true);
      creationOptions->createCoreProfile(false);
      creationOptions->enableDoubleBuffering(false);
      creationOptions->shareContext(sharedContext);

      IntPtr hglrc = IntPtr::Zero;
      if (m_createExternalContext)
      {
        //! create context for the drawing surface
        m_hglrc = createCompatibleContext(m_hDC, sharedContext);
        hglrc = IntPtr((void*)m_hglrc);
      }

      //! create dispay helper class
      bool useTexture = true;
      m_baseHelper = gcnew TSLNOpenGLDisplayHelper(sharedHandle, false, creationOptions, false, false, useTexture, m_width, m_height, hglrc);
      m_surface = m_baseHelper->drawingSurface;
      m_surface->setOption(TSLNOptionEnum::TSLNOptionDoubleBuffered, true);

      //! setup interaction modes for mouse moves and clicks on the drawing surface to apply zoom/pan..
      setupInteractionModes();
    }

    /// <summary>
    /// Load and add datalayer to the drawing surface
    /// </summary>
    bool loadMap(String^ mapLocation, String^% msgErr)
    {
      //! load map to the drawing surface
      String^ LayerName = "themap";
      if (!addDataLayerToSurface(m_surface, mapLocation, LayerName, msgErr))
      {
        return false;
      }

      int indexOfLastSlash = mapLocation->LastIndexOf('\\') + 1;
      m_shortfilename = mapLocation->Substring(indexOfLastSlash, mapLocation->Length - indexOfLastSlash);

      return true;
    }

    /// <summary>
    /// Get position on the map given latitude and longitude.
    /// </summary>
    bool getLatLonPosition(double latitude, double longitude, Point% position)
    {
      int x = -1, y = -1;
      bool ret = m_surface->latLongToDU(latitude, longitude, x, y);
      if (ret)
      {
        position = Point(x, y);
      }
      return ret;
    }

    /// <summary>
    /// Render the maplink drawing surface
    /// </summary>
    System::Void render()
    {
      drawMaplinkMap(m_width, m_height);
    }

    /// <summary>
    /// Set window size.
    /// </summary>
    void setSize(GLsizei iWidth, GLsizei iHeight)
    {
      m_width = iWidth;
      m_height = iHeight;
    }

    /// <summary>
    /// Make maplink context current before data layer drawing
    /// </summary>
    bool makeCurrent()
    {
      if (m_hglrc != NULL)
      {
        return wglMakeCurrent(m_hDC, m_hglrc);
      }

      try
      {
        m_baseHelper->makeContextCurrent();
      }
      catch (...)
      {
      }
      return false;
    }

    /// <summary>
    /// Get texture id from maplink drawing surface
    /// </summary>
    GLuint textureId()
    {
      return m_textureId;
    }

    /// <summary>
    /// create compatible opengl context
    /// </summary>
    static HGLRC createCompatibleContext(HDC hdc, void *sharedContext)
    {
      TSLNWGLContextOptions^ options = gcnew TSLNWGLContextOptions(hdc, NULL);
      options->setOpenGLVersion(3, 3);
      if (sharedContext != NULL)
        options->Context = (sharedContext);
      //! create context for the drawing surface
      HGLRC hglrc = (HGLRC)TSLNOpenGLDisplayHelper::createContext(options);

      if (hglrc == NULL)
      {
        MessageBox::Show("wglCreateContext Failed");
        return 0;
      }

      return hglrc;
    }

    /// <summary>
    /// Print displayed map as pdf.
    /// </summary>
    void printMap()
    {
      cli::array<TSLNPrintItem^>^ printItems = gcnew cli::array<TSLNPrintItem^>(1);
      TSLNPrintItem^ printItem = gcnew TSLNPrintItem();
      TSLNEnvelope^ envelope = gcnew TSLNEnvelope();

      printItems->SetValue(printItem, 0);

      double uux1 = 0, uuy1 = 0, uux2 = 0, uuy2 = 0;
      int tmcx1 = 0, tmcy1 = 0, tmcx2 = 0, tmcy2 = 0;
      String^ templateFile = "";
      bool isLandscape;

      if (m_width > m_height)
      {
        templateFile = "landscape.tpt";
        isLandscape = true;
      }
      else
      {
        templateFile = "portrait.tpt";
        isLandscape = false;
      }

      String^ templateFilePath = DrawingSurfaceClass::findFile(templateFile);

      TSLNEntitySet^ entitySet = TSLNPrintDocument::loadTemplate(templateFilePath, DrawingSurfaceClass::fileLoader, nullptr);

      if (entitySet->size == 0)
      {
        MessageBox::Show("MapLink could not load printer template");
        return;
      }

      m_surface->getUUExtent(uux1, uuy1, uux2, uuy2);
      m_surface->UUToTMC(uux1, uuy1, tmcx1, tmcy1);
      m_surface->UUToTMC(uux2, uuy2, tmcx2, tmcy2);
      envelope->bottomLeft->x = tmcx1;
      envelope->bottomLeft->y = tmcy1;
      envelope->topRight->x = tmcx2;
      envelope->topRight->y = tmcy2;
      long tmcPerMetre = printItem->tmcPerMetre;
      printItem->drawingSurface = m_surface;
      printItem->extent(envelope);
      printItem->borderColour = Color::Black;
      printItem->borderStyle = 1;
      printItem->borderThickness = 2;
      printItem->templateName = "map";

      cli::array<String^>^ textMappings = gcnew cli::array<String^>(4);
      textMappings->SetValue("MapLink Pro", 0);    // Default
      textMappings->SetValue(m_shortfilename, 1);    // New text
      textMappings->SetValue("date", 2);    // Default
      System::DateTime dt = System::DateTime::Now;
      textMappings->SetValue(dt.ToShortDateString(), 3);    // Default

      try
      {
        TSLNPrintDocument^ pd = gcnew TSLNPrintDocument(m_surface, entitySet, printItems, textMappings, isLandscape);
        PrintDialog^ dlg = gcnew PrintDialog();
        dlg->Document = pd;
        DialogResult result = dlg->ShowDialog();

        if (result == DialogResult::OK)
        {
          pd->Print();
        }
      }
      catch (Exception^ ex)
      {
        MessageBox::Show("An error occurred attempting to print -  " + ex->Message);
      }

    }
#pragma endregion

#pragma region PrivateMaplinkMethods
  private:
    /// <summary>
    /// Load and add datalayer to the drawing surface
    /// </summary>
    bool addDataLayerToSurface(DrawingSurfaceClass^ surface, String^ mapPath, String^ layerName, String^% msgErr)
    {
      //! add data layer to surface
      TSLNMapDataLayer^ mapDataLayer = gcnew TSLNMapDataLayer();
      surface->backgroundColour = System::Drawing::Color::White;
      surface->removeDataLayer(layerName);
      if (!surface->addDataLayer(mapDataLayer, layerName))
      {
        msgErr = "Failed to add data layer to surface";
        return false;
      }

      surface->setDataLayerProps(layerName, TSLNPropertyEnum::TSLNPropertyBuffered, 1);

      //! load map to the data layer
      if (!mapDataLayer->loadData(mapPath))
      {
        msgErr = "Failed to load map";
        return false;
      }

      surface->backgroundColour = mapDataLayer->backgroundColour;
      if (!surface->reset(false))
      {
        msgErr = "Failed to reset surface";
        return false;
      }

      return true;
    }

    /// <summary>
    /// Draw drawing surface
    /// </summary>
    bool drawSurface(TSLNOpenGLDisplayHelper^ baseHelper, DrawingSurfaceClass^ surface, int width, int height)
    {
      //! draw the data layer
      bool ret1 = baseHelper->resizeWindow(width, height);
      bool ret2 = surface->drawDU(0, 0, width, height, true, true);

      //! update the mode manager
      if (m_modeManager != nullptr)
      {
        m_modeManager->onSize(width, height);
        m_modeManager->onDraw(0, 0, width, height);
      }

      return ret1 && ret2;
    }

    /// <summary>
    /// Draw the data layer to the drawing surface
    /// </summary>
    bool drawMaplinkMap(GLsizei iWidth, GLsizei iHeight)
    {
      makeCurrent();

      //! Draw the data layer
      bool retDraw = drawSurface(m_baseHelper, m_surface, iWidth, iHeight);
      if (!retDraw)
      {
        //return false;
      }

      m_baseHelper->swapBuffers();
      glFlush();

      m_textureId = m_baseHelper->textureID();

      //m_baseHelper->createBitmap("C:/Temp2/createBitmap.tif");

      return true;
    }
#pragma endregion

#pragma region InteractionModes
  public:
    TSLNInteractionModeManagerGeneric^ m_modeManager;//< interaction modes manager.

    /// <summary>
    /// set interaction mode
    /// </summary>
    void setInteractionMode(InteractionModesEnum mode)
    {
      m_currentMode = mode;
      m_modeManager->setCurrentMode((int)m_currentMode);
    }

    /// <summary>
    /// get interaction mode
    /// </summary>
    InteractionModesEnum getInteractionMode()
    {
      return m_currentMode;
    }

    /// <summary>
    /// Handle key and mouse clicks/movements.
    /// </summary>
    bool processWndMessage(Message % m)
    {
      if (m_modeManager == nullptr)
      {
        return false;
      }

      bool invalidate = false;
      // Listen for messages that are sent to the button window. Some messages are sent
      // to the parent window instead of the button's window.
      switch (m.Msg)
      {
      case WM_LBUTTONDOWN:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onLButtonDown(position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          break;
        }

      case WM_RBUTTONDOWN:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onRButtonDown(position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          break;
        }
      case WM_MBUTTONDOWN:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onMButtonDown(position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          break;
        }

      case WM_LBUTTONUP:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onLButtonUp(position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          break;
        }
      case WM_RBUTTONUP:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          if (m_rightClickCallBackDelegate)
          {
            m_rightClickCallBackDelegate->handleMouseRightClick();
          }
          else
          {
            invalidate = m_modeManager->onRButtonUp(position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          }
          break;
        }
      case WM_MBUTTONUP:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onMButtonUp(position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          break;
        }

      case WM_SYSKEYDOWN:
        if (true)
        {
          int keyPressed = (int)m.WParam;
          if (keyPressed == VK_CONTROL)
          {
            m_controlButtonDown = true;
          }
          else if (keyPressed == VK_SHIFT)
          {
            m_shiftButtonDown = true;
          }
          break;
        }

      case WM_SYSKEYUP:
        if (true)
        {
          int keyPressed = (int)m.WParam;
          if (m_controlButtonDown && keyPressed != VK_CONTROL)
          {
            m_controlButtonDown = false;
          }
          else if (m_shiftButtonDown && keyPressed != VK_SHIFT)
          {
            m_shiftButtonDown = false;
          }

          break;
        }

      case WM_MOUSEMOVE:
        if (true)
        {
          TSLNButtonType button = TSLNButtonType::TSLNButtonNone;
          switch (m.WParam.ToInt32())
          {
          case MK_LBUTTON: // the left button was clicked
            button = TSLNButtonType::TSLNButtonLeft;
            break;
          case MK_RBUTTON: // the left button was clicked
            button = TSLNButtonType::TSLNButtonRight;
            break;
          case MK_MBUTTON: // the left button was clicked
            button = TSLNButtonType::TSLNButtonCentre;
            break;
          }

          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onMouseMove(button, position.X, position.Y, m_shiftButtonDown, m_controlButtonDown);
          if (button == TSLNButtonType::TSLNButtonLeft && !invalidate)
            invalidate = true;

          break;
        }


      case WM_MOUSEWHEEL:
        if (true)
        {
          Point position = getPosition(m);
          makeCurrent();
          invalidate = m_modeManager->onMouseWheel(5, position.X, position.Y);
          break;
        }
      }

      return invalidate;
    }

    /// <summary>
    /// Set interface class to handle mouse right click call back function [optional].
    /// </summary>
    void setRightClickCallBackDelegate(RightClickContextMenuInterface^ rightClickCallBackDelegate)
    {
      m_rightClickCallBackDelegate = rightClickCallBackDelegate;
    }

  private:
    /// <summary>
    /// Setup interaction modes for mouse moves and clicks on the drawing surface to apply zoom/pan..
    /// </summary>
    void setupInteractionModes()
    {
      //! setup interaction modes
      m_updateReceiver = gcnew InteractionModeRequestReceiver();
      TSLNInteractionModeRequest^ request = (TSLNInteractionModeRequest^)m_updateReceiver;
      m_modeManager = gcnew TSLNInteractionModeManagerGeneric(request, m_surface, 5, 5, 30, true);
      // Next create our modes
      m_zoomMode = gcnew TSLNInteractionModeZoom((int)InteractionModesEnum::TOOLS_ZOOM, 30, "Left button drag zoom to rectangle, Left button click anchored zoom-in, Shift-Left button click anchored zoom-out, Right button click to finish", true);
      m_panMode = gcnew TSLNInteractionModePan((int)InteractionModesEnum::TOOLS_PAN, "Left button click pans to point, Right button click to finish", true);
      m_grabMode = gcnew TSLNInteractionModeGrab((int)InteractionModesEnum::TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true);
      m_myCustomMode = gcnew CustomInteractionModeZoomPan((int)InteractionModesEnum::TOOLS_ZOOM_PAN, 30, true, "Left button click pans to point and zooms in, Right button click pans to point and zooms out");

      //Give them to the modeManager
      //Remember we don't own the modes anymore after this point
      //but we must keep a reference to them or c# won't deleted them properly
      m_modeManager->addMode(m_zoomMode, true); //set zoom as default
      m_modeManager->addMode(m_panMode, false);
      m_modeManager->addMode(m_grabMode, false);
      m_modeManager->addMode(m_myCustomMode, false);

      //activate zoom imode
      setInteractionMode(InteractionModesEnum::TOOLS_ZOOM);
    }

    /// <summary>
    /// Get mouse position.
    /// </summary>
    Point getPosition(Message % m)
    {
      int x = m.LParam.ToInt32() & 0x0000FFFF;
      int y = (int)((m.LParam.ToInt32() & 0xFFFF0000) >> 16);
      Point position(x, y);
      //Point position = Cursor::Position;
      return position;
    }

  private:
    //! Interaction modes request receiver.
    InteractionModeRequestReceiver^ m_updateReceiver;
    //! Interaction modes.
    TSLNInteractionModeZoom^ m_zoomMode;
    TSLNInteractionModePan^ m_panMode;
    TSLNInteractionModeGrab^ m_grabMode;
    TSLNInteractionMode^ m_myCustomMode;

    InteractionModesEnum m_currentMode;//< Current interaction mode.
    bool m_shiftButtonDown;//< flag to be set when 'SHIFT' key is being pressed.
    bool m_controlButtonDown;//< flag to be set when 'CONTROL' key is being pressed.
    RightClickContextMenuInterface^ m_rightClickCallBackDelegate;//<Interface class to handle mouse right click call back function [optional].
#pragma endregion

#pragma region Parameters
  private:
    TSLNOpenGLDisplayHelper^  m_baseHelper;//< Maplink display helper  
    DrawingSurfaceClass^      m_surface;//< Maplink drawing surface
    GLuint                    m_textureId;//< Maplink shared texture id

    HGLRC   m_hglrc;//< Maplink drawing context
    HDC     m_hDC;//< Maplink window HDC [shared with the opengl window]                          
    bool    m_createExternalContext;//< Flag to be set to create the context from the application and pass it to the maplink helper. otherwise, maplink will create its own context
    int     m_width, m_height;//< Drawing surface size
    String^ m_shortfilename;//< Current map file name
#pragma endregion
  };
}