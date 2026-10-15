using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using CliWinFormsHost_UsrControl;

namespace CppWPFWinFormsHost_WPF
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    #region Parameters
    CliWinFormsHost_UsrControlControl mapPanel;//< user control object
    static MainWindow mainWindow;//< main window self object
    #endregion

    #region Constructor_Destructor
    /// <summary>
    /// Construction to initialize the user control and set its right click handling function [if required].
    /// </summary>
    public MainWindow()
    {
      InitializeComponent();
      mainWindow = this;

      mapPanel = (wfhSample3.Child as CliWinFormsHost_UsrControl.CliWinFormsHost_UsrControlControl);
      //! set the right click handling function if you want to reset the interaction mode of pop up a context menu.
      mapPanel.setRightClickCallBackDelegate(new RightClickContextMenuInterface());

      //! set the Interface class to pass your application opengl code [optional].
      mapPanel.setApplicationGLInterface(new AppGLInterface());

      zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
    }
    #endregion

    #region Panel_FileOpen_Functions
    /// <summary>
    /// Open initial map if the software has map path in its arguments.
    /// </summary>
    /// <param name="mapPath"></param>
    public void openInitialMap(string mapPath)
    {
      mapPanel.setMapFileName(mapPath);
      mapPanel.fileOpen(false);
    }

    /// <summary>
    /// Load and render map. If parameter is true, open a file open dialog to select the map file path. If set to false, use the last opened map file name.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void FileOpen_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.fileOpen(true);
    }

    /// <summary>
    /// Print displayed map in pdf.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void PrintMap_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.printMap();
    }
    #endregion

    #region Panel_General_Functions
    /// <summary>
    /// Show about dialog.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void About_Click(object sender, RoutedEventArgs e)
    {
      AboutBox inputDialog = new AboutBox();
      inputDialog.ShowDialog();
    }

    /// <summary>
    /// Exit the application.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ExitApp_Click(object sender, RoutedEventArgs e)
    {
      Application.Current.Shutdown();
    }
    #endregion

    #region Panel_ZoomPan_Functions
    /// <summary>
    /// Zoom in.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ZoomIn_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.zoomIn(30, true);
    }

    /// <summary>
    /// Zoom out.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ZoomOut_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.zoomOut(30, true);
    }

    /// <summary>
    /// Reset zoom.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ResetZoom_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.resetToFullExtent(true);
    }

    /// <summary>
    /// Select zoom tool.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ZoomTool_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.setCurrentMode(ModeInteractions.InteractionModesEnum.TOOLS_ZOOM);
      UpdateToolsAndMenuButtons();
    }

    /// <summary>
    /// Select pan tool.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void PanTool_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.setCurrentMode(ModeInteractions.InteractionModesEnum.TOOLS_PAN);
      UpdateToolsAndMenuButtons();
    }

    /// <summary>
    /// Select grab tool.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void GrabTool_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.setCurrentMode(ModeInteractions.InteractionModesEnum.TOOLS_GRAB);
      UpdateToolsAndMenuButtons();
    }

    /// <summary>
    /// Select zoom pan tool.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ZoomPanTool_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.setCurrentMode(ModeInteractions.InteractionModesEnum.TOOLS_ZOOM_PAN);
      UpdateToolsAndMenuButtons();
    }

    /// <summary>
    /// Reset all the interaction modes in the menu check boxes.
    /// </summary>
    private void uncheckAllToolbarTools()
    {
      zoomToolBarCheck.IsChecked = false;
      panToolBarCheck.IsChecked = false;
      grabToolBarCheck.IsChecked = false;
      zoompanToolBarCheck.IsChecked = false;
    }

    /// <summary>
    /// Reset all the interaction modes in the toolbar buttons.
    /// </summary>
    private void resetAllToolbarToolsText()
    {
      zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
      pantoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
      grabtoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
      zoompantoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
    }

    /// <summary>
    /// Update the selected interaction mode in the menu check boxes and the toolbar buttons.
    /// </summary>
    private void UpdateToolsAndMenuButtons()
    {
      uncheckAllToolbarTools();
      resetAllToolbarToolsText();
      ModeInteractions.InteractionModesEnum id = mapPanel.getCurrentMode();
      switch (id)
      {
        case ModeInteractions.InteractionModesEnum.TOOLS_ZOOM:
          zoomToolBarCheck.IsChecked = true;
          zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
          break;
        case ModeInteractions.InteractionModesEnum.TOOLS_PAN:
          panToolBarCheck.IsChecked = true;
          pantoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
          break;
        case ModeInteractions.InteractionModesEnum.TOOLS_GRAB:
          grabToolBarCheck.IsChecked = true;
          grabtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
          break;
        case ModeInteractions.InteractionModesEnum.TOOLS_ZOOM_PAN:
          zoompanToolBarCheck.IsChecked = true;
          zoompantoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
          break;
      }
    }
    #endregion

    #region Panel_StackViews_Functions
    /// <summary>
    /// Preview previous view.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void PreviousView_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.viewStackGotoPrevious();
    }

    /// <summary>
    /// Preview next view.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void NextView_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.viewStackGotoNext();
    }

    /// <summary>
    /// Go to view 1.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void GotoView1_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.gotoSavedView(0);
    }

    /// <summary>
    /// Go to view 2.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void GotoView2_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.gotoSavedView(1);
    }

    /// <summary>
    /// Go to view 3.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void GotoView3_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.gotoSavedView(2);
    }

    /// <summary>
    /// Set the current render as view 1.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void SetView1_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.saveView(0);
    }

    /// <summary>
    /// Set the current render as view 2.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void SetView2_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.saveView(1);
    }

    /// <summary>
    /// Set the current render as view 3.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void SetView3_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.saveView(2);
    }

    /// <summary>
    /// Reset all saved views.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ResetViews_Click(object sender, RoutedEventArgs e)
    {
      mapPanel.viewStackReset();
    }
    #endregion

    #region Panel_MouseRightClickHandle
    /// <summary>
    /// Set the right click handling function if you want to reset the interaction mode of pop up a context menu.
    /// </summary>
    class RightClickContextMenuInterface : ModeInteractions.RightClickContextMenuInterface
    {
      public override void handleMouseRightClick()
      {
        mainWindow.handleRightClickCallBack();
      }
    }

    /// <summary>
    /// Implement mouse right click handling function here.
    /// </summary>
    private void handleRightClickCallBack()
    {
      //! reset to zoom mode when right click. You can handle right click here through pop up context menu of tool bar options.
      ZoomTool_Click(null, null);
    }
    #endregion

    #region Application_Specific_OpenGL_Methods
    /// <summary>
    /// Set the Interface class to pass your application opengl code [optional].
    /// </summary>
    class AppGLInterface : MaplinkWinformsGLOverlay.ApplicationGLInterface
    {
      public override void draweNativeGL()
      {
        mainWindow.handleAppDraweNativeGL();
      }
    }

    /// <summary>
    /// Implement application opengl extra drawing here.
    /// </summary>
    private void handleAppDraweNativeGL()
    {
      int k = 0;
    }
    #endregion
  }
}
