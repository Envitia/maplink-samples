#pragma once

#include "ContextsManager.h"

namespace CLIWinformsGLOverlay {

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;
  using namespace MaplinkWinformsGLOverlay;


  /// <summary>
  /// Summary for Form1
  ///
  /// WARNING: If you change the name of this class, you will need to change the
  ///          'Resource File Name' property for the managed resource compiler tool
  ///          associated with all .resx files this class depends on.  Otherwise,
  ///          the designers will not be able to interact properly with localized
  ///          resources associated with this form.
  /// </summary>
  public ref class Form1 : public System::Windows::Forms::Form
  {
#pragma region Constructor_Destructor
  public:
    Form1(void)
    {
      InitializeComponent();

      //! load maplink standard configuration and initialize contextsd.
      contextManager = gcnew ContextsManager(this);
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~Form1()
    {
      if (components)
      {
        delete components;
      }
      if (contextManager)
      {
        delete contextManager;
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
      this->components = (gcnew System::ComponentModel::Container());
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
      this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
      this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
      this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripButton5 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripButton7 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripButton8 = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStrip1->SuspendLayout();
      this->SuspendLayout();
      // 
      // timer1
      // 
      this->timer1->Enabled = true;
      this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
      // 
      // toolStrip1
      // 
      this->toolStrip1->ImageScalingSize = System::Drawing::Size(24, 24);
      this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {
        this->toolStripButton1,
          this->toolStripSeparator1, this->toolStripButton2, this->toolStripButton3, this->toolStripButton4, this->toolStripSeparator2,
          this->toolStripButton5, this->toolStripButton6, this->toolStripButton7, this->toolStripButton8, this->toolStripSeparator3
      });
      this->toolStrip1->Location = System::Drawing::Point(0, 0);
      this->toolStrip1->Name = L"toolStrip1";
      this->toolStrip1->Size = System::Drawing::Size(1018, 31);
      this->toolStrip1->TabIndex = 0;
      this->toolStrip1->Text = L"toolStrip1";
      // 
      // toolStripButton1
      // 
      this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton1.Image")));
      this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton1->Name = L"toolStripButton1";
      this->toolStripButton1->Size = System::Drawing::Size(28, 28);
      this->toolStripButton1->Text = L"toolStripButton1";
      this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
      // 
      // toolStripSeparator1
      // 
      this->toolStripSeparator1->Name = L"toolStripSeparator1";
      this->toolStripSeparator1->Size = System::Drawing::Size(6, 31);
      // 
      // toolStripButton2
      // 
      this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton2.Image")));
      this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton2->Name = L"toolStripButton2";
      this->toolStripButton2->Size = System::Drawing::Size(28, 28);
      this->toolStripButton2->Text = L"toolStripButton2";
      this->toolStripButton2->Click += gcnew System::EventHandler(this, &Form1::toolStripButton2_Click);
      // 
      // toolStripButton3
      // 
      this->toolStripButton3->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton3.Image")));
      this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton3->Name = L"toolStripButton3";
      this->toolStripButton3->Size = System::Drawing::Size(28, 28);
      this->toolStripButton3->Text = L"toolStripButton3";
      this->toolStripButton3->Click += gcnew System::EventHandler(this, &Form1::toolStripButton3_Click);
      // 
      // toolStripButton4
      // 
      this->toolStripButton4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton4.Image")));
      this->toolStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton4->Name = L"toolStripButton4";
      this->toolStripButton4->Size = System::Drawing::Size(28, 28);
      this->toolStripButton4->Text = L"toolStripButton4";
      this->toolStripButton4->Click += gcnew System::EventHandler(this, &Form1::toolStripButton4_Click);
      // 
      // toolStripSeparator2
      // 
      this->toolStripSeparator2->Name = L"toolStripSeparator2";
      this->toolStripSeparator2->Size = System::Drawing::Size(6, 31);
      // 
      // toolStripButton5
      // 
      this->toolStripButton5->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton5->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton5.Image")));
      this->toolStripButton5->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton5->Name = L"toolStripButton5";
      this->toolStripButton5->Size = System::Drawing::Size(28, 28);
      this->toolStripButton5->Text = L"toolStripButton5";
      this->toolStripButton5->Click += gcnew System::EventHandler(this, &Form1::toolStripButton5_Click);
      // 
      // toolStripButton6
      // 
      this->toolStripButton6->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton6.Image")));
      this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton6->Name = L"toolStripButton6";
      this->toolStripButton6->Size = System::Drawing::Size(28, 28);
      this->toolStripButton6->Text = L"toolStripButton6";
      this->toolStripButton6->Click += gcnew System::EventHandler(this, &Form1::toolStripButton6_Click);
      // 
      // toolStripButton7
      // 
      this->toolStripButton7->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton7->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton7.Image")));
      this->toolStripButton7->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton7->Name = L"toolStripButton7";
      this->toolStripButton7->Size = System::Drawing::Size(28, 28);
      this->toolStripButton7->Text = L"toolStripButton7";
      this->toolStripButton7->Click += gcnew System::EventHandler(this, &Form1::toolStripButton7_Click);
      // 
      // toolStripButton8
      // 
      this->toolStripButton8->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->toolStripButton8->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton8.Image")));
      this->toolStripButton8->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->toolStripButton8->Name = L"toolStripButton8";
      this->toolStripButton8->Size = System::Drawing::Size(28, 28);
      this->toolStripButton8->Text = L"toolStripButton8";
      this->toolStripButton8->Click += gcnew System::EventHandler(this, &Form1::toolStripButton8_Click);
      // 
      // toolStripSeparator3
      // 
      this->toolStripSeparator3->Name = L"toolStripSeparator3";
      this->toolStripSeparator3->Size = System::Drawing::Size(6, 31);
      // 
      // Form1
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(1018, 726);
      this->Controls->Add(this->toolStrip1);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
      this->Name = L"Form1";
      this->Text = L"CppCli Winforms Integration With Opengl Overlay";
      this->Resize += gcnew System::EventHandler(this, &Form1::OnReSize);
      this->toolStrip1->ResumeLayout(false);
      this->toolStrip1->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

#pragma region Windows Form Methods
  private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
  {
    //UNREFERENCED_PARAMETER(sender);
    //UNREFERENCED_PARAMETER(e);

    //! render with each timer tick
    //contextManager->render();
  }

           /// <summary>
           /// handle resizing the window.
           /// </summary>
  private: System::Void OnReSize(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->resize(this->Width, this->Height);
  }

#pragma endregion

#pragma region ToolbarButtons
  private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->fileOpen(true);
  }
  private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->zoomIn(30, true);
    contextManager->render();
  }
  private: System::Void toolStripButton3_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->zoomOut(30, true);
    contextManager->render();
  }
  private: System::Void toolStripButton4_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->resetToFullExtent(true);
    contextManager->render();
  }
  private: System::Void toolStripButton5_Click(System::Object^  sender, System::EventArgs^  e)
  {
    //activate zoom imode
    contextManager->setInteractionMode(InteractionModesEnum::TOOLS_ZOOM);
  }
  private: System::Void toolStripButton6_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->setInteractionMode(InteractionModesEnum::TOOLS_PAN);
  }
  private: System::Void toolStripButton7_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->setInteractionMode(InteractionModesEnum::TOOLS_GRAB);
  }
  private: System::Void toolStripButton8_Click(System::Object^  sender, System::EventArgs^  e)
  {
    contextManager->setInteractionMode(InteractionModesEnum::TOOLS_ZOOM_PAN);
  }
#pragma endregion

#pragma region Parameters
  private:
    ContextsManager ^ contextManager;//< context manager that manages the initialization and main functions of the contexts.

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
  private: System::Windows::Forms::Timer^  timer1;
  private: System::Windows::Forms::ToolStrip^  toolStrip1;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton3;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton4;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton5;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton6;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton7;
  private: System::Windows::Forms::ToolStripButton^  toolStripButton8;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
  private: System::ComponentModel::IContainer^  components;
#pragma endregion
  };
}

