#pragma once

#include "ApplicationGL.h"
#include "MaplinkInterface.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace MaplinkWinformsGLOverlay
{
  /// <summary>
  /// Class that manages the initialization and main functions of multiple contexts.
  /// </summary>
  ref class ContextsManager
  {
#pragma region Constructor_Destructor
  public:
    ContextsManager(ParentControlClass^ parentForm)
      : m_width(600)
      , m_height(600)
      , m_appGL(nullptr)
      , m_maplInterface(nullptr)
    {
      //! load maplink standard configuration
      String^ maplHome = TSLNUtilityFunctions::getMapLinkHome();
      if (maplHome == nullptr)
      {
        TSLNUtilityFunctions::setMapLinkHome("../");// ("C:/secure/source/maplink");
      }
      if (!TSLNDrawingSurface::loadStandardConfig())
      {
        MessageBox::Show("Failed to load maplink standard configuration");
      }

      //! Initialize contexts when form is loaded.
      initializeContexts(parentForm);
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~ContextsManager()
    {
      if (m_maplInterface)
      {
        delete m_maplInterface;
      }
      if (m_appGL)
      {
        delete m_appGL;
      }
    }
#pragma endregion

#pragma region Windows Form Methods
  public:
    /// <summary>
    /// Initialize contexts when form is loaded.
    /// </summary>
    System::Void initializeContexts(ParentControlClass^ parentForm)
    {
      //! initialize opengl class
      m_width = parentForm->Width;
      m_height = parentForm->Height;
      m_appGL = gcnew ApplicationGL(parentForm, m_width, m_height);

      //! initialize maplink interface class
      m_maplInterface = gcnew MaplinkInterface(
        m_appGL->getHdc(),
        m_width,
        m_height,
        false);

      //! Create off screen helper and add data to its drawing surface.
      m_maplInterface->createOffScreenHelper(
        m_appGL->getHandle(),
        m_appGL->getContext());

      //! set maplink interface reference in the opengl class to use its texture id.
      m_appGL->maplInterface(m_maplInterface);
    }

    /// <summary>
    /// Handle resizing the window.
    /// </summary>
    System::Void resize(GLsizei iWidth, GLsizei iHeight)
    {
      if (m_appGL == nullptr || m_maplInterface == nullptr)
        return;
      //! resize the window
      m_width = iWidth;
      m_height = iHeight;

      if (m_appGL != nullptr)
      {
        m_appGL->resizeWindow(m_width, m_height);
      }

      if (m_maplInterface != nullptr)
      {
        m_maplInterface->setSize(m_width, m_height);
      }

      //! draw the window with the new size
      render();
    }

    /// <summary>
    /// render the maplink drawing surface and application native opengl.
    /// </summary>
    System::Void render()
    {
      if (m_appGL == nullptr || m_maplInterface == nullptr)
        return;
      //! draw maplink surface
      m_maplInterface->render();

      //! draw opengl surface
      m_appGL->render(m_width, m_height);
    }
#pragma endregion

#pragma region Application_Specific_OpenGL_Methods
  public:
    /// <summary>
    /// Set opengl interface to implement your application opengl code from the application.
    /// </summary> 
    void setApplicationGLInterface(ApplicationGLInterface^ appGlInterface)
    {
      m_appGL->setApplicationGLInterface(appGlInterface);
    }
#pragma endregion

#pragma region Maplink_Context_Methods
#pragma region Panel_FileOpen_Methods
  public:
    /// <summary>
    /// Load and render map. If parameter is true, open a file open dialog to select the map file path. If set to false, use the last opened map file name.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    System::Void fileOpen(bool openFileDialog)
    {
      String^ msgErr;
      //! Choose map file
      if (openFileDialog)
      {
        OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
        openFileDialog1->InitialDirectory = "C:\\Program Files\\Envitia\\MapLink Pro\\10.0\\Maps";
        openFileDialog1->Filter = "map files (*.map)|*.map|All files (*.*)|*.*";
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
          m_mapFileName = openFileDialog1->FileName;
        }
        else
        {
          return;
        }
      }

      //! load and add datalayer to the drawing surface
      if (m_mapFileName)
      {
        m_maplInterface->makeCurrent();
        if (!m_maplInterface->loadMap(m_mapFileName, msgErr))
        {
          MessageBox::Show("Failed to load map. Error: " + msgErr);
        }

        //! draw window
        render();
      }
    }

    /// <summary>
    /// Print displayed map in pdf.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    System::Void printMap()
    {
      m_maplInterface->printMap();
    }
#pragma endregion

#pragma region Panel_ZoomPan_Methods
  public:
    /// <summary>
    /// Zoom in.
    /// </summary>
    System::Void zoomIn(int zoomPercentage, bool addToViewStack)
    {
      m_maplInterface->m_modeManager->zoomIn(zoomPercentage, addToViewStack);
    }

    /// <summary>
    /// Zoom out.
    /// </summary>
    System::Void zoomOut(int zoomPercentage, bool addToViewStack)
    {
      m_maplInterface->m_modeManager->zoomOut(zoomPercentage, addToViewStack);
    }

    /// <summary>
    /// Reset zoom.
    /// </summary>
    System::Void resetToFullExtent(bool addToViewStack)
    {
      m_maplInterface->m_modeManager->resetToFullExtent(addToViewStack);
    }

    /// <summary>
    /// Set interaction mode.
    /// </summary>
    System::Void setInteractionMode(InteractionModesEnum mode)
    {
      //activate zoom imode
      m_maplInterface->setInteractionMode(mode);
    }

    /// <summary>
    /// Get current interaction mode.
    /// </summary>
    InteractionModesEnum getInteractionMode()
    {
      return m_maplInterface->getInteractionMode();
    }

    /// <summary>
    /// Set interface class to handle mouse right click call back function [optional].
    /// </summary>
    void setRightClickCallBackDelegate(RightClickContextMenuInterface^ rightClickCallBackDelegate)
    {
      m_maplInterface->setRightClickCallBackDelegate(rightClickCallBackDelegate);
    }
#pragma endregion

#pragma region Panel_StackViews_Methods
  public:
    /// <summary>
    /// Save the current view as view[index] in the views stack.
    /// </summary>
    bool saveView(int index)
    {
      if (m_maplInterface->m_modeManager == nullptr)
      {
        return false;
      }
      bool invalidate = m_maplInterface->m_modeManager->savedViewSetToCurrent(index);
      return invalidate;
    }

    /// <summary>
    /// Reset the saved views in the views stack.
    /// </summary>
    bool viewStackReset()
    {
      if (m_maplInterface->m_modeManager == nullptr)
      {
        return false;
      }
      bool invalidate = m_maplInterface->m_modeManager->savedViewReset();
      return invalidate;
    }

    /// <summary>
    /// Go to save view [index] in the views stack.
    /// </summary>
    bool gotoSavedView(int index)
    {
      if (m_maplInterface->m_modeManager == nullptr)
      {
        return false;
      }
      bool invalidate = m_maplInterface->m_modeManager->savedViewGoto(index);
      if (invalidate)
      {
        render();
      }
      return invalidate;
    }

    /// <summary>
    /// Go to the previous view in the views stack.
    /// </summary>
    bool viewStackGotoPrevious()
    {
      if (m_maplInterface->m_modeManager == nullptr)
      {
        return false;
      }
      bool invalidate = m_maplInterface->m_modeManager->viewStackGotoPrevious();
      if (invalidate)
      {
        render();
      }
      return invalidate;
    }

    /// <summary>
    /// Go to the next view in the views stack.
    /// </summary>
    bool viewStackGotoNext()
    {
      if (m_maplInterface->m_modeManager == nullptr)
      {
        return false;
      }
      bool invalidate = m_maplInterface->m_modeManager->viewStackGotoNext();
      if (invalidate)
      {
        render();
      }
      return invalidate;
    }
#pragma endregion
#pragma endregion

#pragma region Parameters
  private:
    ApplicationGL^      m_appGL;//< application opengl class
    MaplinkInterface^   m_maplInterface;//< maplink drawing helper
    int                 m_width, m_height;//< drawing size

  public:
    String^             m_mapFileName;//< map file name
#pragma endregion
  };
}