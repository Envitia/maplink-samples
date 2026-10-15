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
using DrawingSurfacePanel;

namespace CSharpCustomPanel_WPF_Sample
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        DrawingSurfacePanel.MapViewerPanel mapPanel;

        public MainWindow()
        {
            InitializeComponent();
            mapPanel = (wfhSample3.Child as DrawingSurfacePanel.MapViewerPanel);
            mapPanel.RightClickCurrentModeChange += UpdateToolsAndMenuButtons;
            zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
        }

        public void openInitialMap( string mapPath )
        {
          mapPanel.MapFileName = mapPath;
          mapPanel.fileOpen(false);
        }
        private void uncheckAllToolbarTools()
        {
            zoomToolBarCheck.IsChecked = false;
            panToolBarCheck.IsChecked = false;
            grabToolBarCheck.IsChecked = false;
            zoompanToolBarCheck.IsChecked = false;
        }
        private void resetAllToolbarToolsText()
        {
            zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
            pantoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
            grabtoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
            zoompantoolBorder.BorderBrush = System.Windows.Media.Brushes.Transparent;
        }

        private void UpdateToolsAndMenuButtons()
        {
            uncheckAllToolbarTools();
            resetAllToolbarToolsText();
            MapViewerPanel.InteractionModeEnum id = mapPanel.getCurrentMode();
            switch(id)
            {
                case MapViewerPanel.InteractionModeEnum.TOOLS_ZOOM:
                    zoomToolBarCheck.IsChecked = true;
                    zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
                case MapViewerPanel.InteractionModeEnum.TOOLS_PAN:
                    panToolBarCheck.IsChecked = true;
                    pantoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
                case MapViewerPanel.InteractionModeEnum.TOOLS_GRAB:
                    grabToolBarCheck.IsChecked = true;
                    grabtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
                case MapViewerPanel.InteractionModeEnum.TOOLS_ZOOM_PAN:
                    zoompanToolBarCheck.IsChecked = true;
                    zoompantoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
            }
        }

        private void FileOpen_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.fileOpen(true);
        }

        private void PrintMap_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.printMap();
        }
        private void EditCopy_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.editCopy();
        }
        private void ZoomIn_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.zoomIn(30, true);
        }
        private void ZoomOut_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.zoomOut(30, true);
        }
        private void ResetZoom_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.resetToFullExtent(true);
        }
        private void ZoomTool_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.setCurrentMode(MapViewerPanel.InteractionModeEnum.TOOLS_ZOOM);
            UpdateToolsAndMenuButtons();
        }
        private void PanTool_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.setCurrentMode(MapViewerPanel.InteractionModeEnum.TOOLS_PAN);
            UpdateToolsAndMenuButtons();
        }
        private void MagnifyTool_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.setCurrentMode(MapViewerPanel.InteractionModeEnum.TOOLS_MAGNIFY);
            UpdateToolsAndMenuButtons();
        }
        private void GrabTool_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.setCurrentMode(MapViewerPanel.InteractionModeEnum.TOOLS_GRAB);
            UpdateToolsAndMenuButtons();
        }
        private void ZoomPanTool_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.setCurrentMode(MapViewerPanel.InteractionModeEnum.TOOLS_ZOOM_PAN);
            UpdateToolsAndMenuButtons();
        }
        private void PreviousView_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.viewStackGotoPrevious();
        }
        private void NextView_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.viewStackGotoNext();
        }
        private void GotoView1_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.gotoSavedView(0);
        }
        private void GotoView2_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.gotoSavedView(1);
        }
        private void GotoView3_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.gotoSavedView(2);
        }

        private void SetView1_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.saveView(0);
        }
        private void SetView2_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.saveView(1);
        }
        private void SetView3_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.saveView(2);
        }
        private void ResetViews_Click(object sender, RoutedEventArgs e)
        {
            mapPanel.viewStackReset();
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            AboutBox inputDialog = new AboutBox();
            inputDialog.ShowDialog();
        }

        private void ExitApp_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
