#pragma once
#include "ContextsManager.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace MaplinkWinformsGLOverlay;

namespace CliWinFormsHost_UsrControl {

  /// <summary>
  /// Summary for CliWinFormsHost_UsrControlControl
  /// </summary>
  public ref class CliWinFormsHost_UsrControlControl : public System::Windows::Forms::UserControl
  {
#pragma region Constructor_Destructor
  public:
    CliWinFormsHost_UsrControlControl(void)
      : m_contextManager(nullptr)
    {
      InitializeComponent();

      //! Initialize contexts when form is loaded.
      initialize();
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~CliWinFormsHost_UsrControlControl()
    {
      if (components)
      {
        delete components;
      }
      if (m_contextManager)
      {
        delete m_contextManager;
      }
    }
#pragma endregion

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->SuspendLayout();
      // 
      // CliWinformsMLOpenglOverlayUserControlControl
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->Name = L"CliWinformsMLOpenglOverlayUserControlControl";
      this->Resize += gcnew System::EventHandler(this, &CliWinFormsHost_UsrControlControl::OnReSize);
      this->ResumeLayout(false);
    }
#pragma endregion

#pragma region Windows Form Methods
  /// <summary>
  /// Initialize contexts when form is loaded.
  /// </summary>
  private: System::Void initialize()
  {
    //! load maplink standard configuration and initialize contextsd.
    m_contextManager = gcnew ContextsManager(this);
  }

  /// <summary>
  /// Handle resizing the window.
  /// </summary>
  private: System::Void OnReSize(System::Object^  sender, System::EventArgs^  e)
  {
    if (m_contextManager)
    {
      m_contextManager->resize(this->Width, this->Height);
      //m_contextManager->render();
    }

  }

#pragma endregion

#pragma region Application_Specific_OpenGL_Methods
  public:
    /// <summary>
    /// Set opengl interface to implement your application opengl code from the application.
    /// </summary> 
    void setApplicationGLInterface(ApplicationGLInterface^ appGlInterface)
    {
      if (m_contextManager)
      {
        m_contextManager->setApplicationGLInterface(appGlInterface);
      }
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
      if (m_contextManager)
      {
        m_contextManager->fileOpen(openFileDialog);
      }
    }

    /// <summary>
    /// Get current map file name.
    /// </summary>
    String^ getMapFileName()
    {
      if (m_contextManager)
      {
        return m_contextManager->m_mapFileName;
      }
      return nullptr;
    }

    /// <summary>
    /// Set current map file name.
    /// </summary>
    void setMapFileName(String^ fileName)
    {
      if (m_contextManager)
      {
        m_contextManager->m_mapFileName = fileName;
      }
    }

    /// <summary>
    /// Print displayed map in pdf.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    void printMap()
    {
      if (m_contextManager)
      {
        m_contextManager->printMap();
      }
    }
#pragma endregion

#pragma region Panel_ZoomPan_Methods
  public:
    /// <summary>
    /// Zoom in.
    /// </summary>
    System::Void zoomIn(int zoomPercentage, bool addToViewStack)
    {
      if (m_contextManager)
      {
        m_contextManager->zoomIn(30, true);
        m_contextManager->render();
      }
    }

    /// <summary>
    /// Zoom out.
    /// </summary>
    System::Void zoomOut(int zoomPercentage, bool addToViewStack)
    {
      if (m_contextManager)
      {
        m_contextManager->zoomOut(30, true);
        m_contextManager->render();
      }
    }

    /// <summary>
    /// Reset zoom.
    /// </summary>
    System::Void resetToFullExtent(bool addToViewStack)
    {
      if (m_contextManager)
      {
        m_contextManager->resetToFullExtent(true);
        m_contextManager->render();
      }
    }

    /// <summary>
    /// Set interaction mode.
    /// </summary>
    System::Void setCurrentMode(InteractionModesEnum mode)
    {
      //activate zoom imode
      if (m_contextManager)
      {
        m_contextManager->setInteractionMode((InteractionModesEnum)mode);
      }
    }

    /// <summary>
    /// Get current interaction mode.
    /// </summary>
    InteractionModesEnum getCurrentMode()
    {
      if (m_contextManager)
      {
        return m_contextManager->getInteractionMode();
      }
      return InteractionModesEnum::TOOLS_GRAB;
    }

    /// <summary>
    /// Set interface class to handle mouse right click call back function [optional].
    /// </summary>
    void setRightClickCallBackDelegate(RightClickContextMenuInterface^ rightClickCallBackDelegate)
    {
      if (m_contextManager)
      {
        m_contextManager->setRightClickCallBackDelegate(rightClickCallBackDelegate);
      }
    }
#pragma endregion

#pragma region Panel_StackViews_Methods
  public:
    /// <summary>
    /// Save the current view as view[index] in the views stack.
    /// </summary>
    bool saveView(int index)
    {
      if (!m_contextManager)
      {
        return false;
      }

      return m_contextManager->saveView(index);
    }

    /// <summary>
    /// Reset the saved views in the views stack.
    /// </summary>
    bool viewStackReset()
    {
      if (!m_contextManager)
      {
        return false;
      }

      return m_contextManager->viewStackReset();
    }

    /// <summary>
    /// Go to save view [index] in the views stack.
    /// </summary>
    bool gotoSavedView(int index)
    {
      if (!m_contextManager)
      {
        return false;
      }

      return m_contextManager->gotoSavedView(index);
    }

    /// <summary>
    /// Go to the previous view in the views stack.
    /// </summary>
    bool viewStackGotoPrevious()
    {
      if (!m_contextManager)
      {
        return false;
      }

      return m_contextManager->viewStackGotoPrevious();
    }

    /// <summary>
    /// Go to the next view in the views stack.
    /// </summary>
    bool viewStackGotoNext()
    {
      if (!m_contextManager)
      {
        return false;
      }

      return m_contextManager->viewStackGotoNext();
    }
#pragma endregion
#pragma endregion

#pragma region Parameters
  private:
    ContextsManager ^ m_contextManager;//< context manager that manages the initialization and main functions of multiple contexts.

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container^ components;
#pragma endregion
  };
}
