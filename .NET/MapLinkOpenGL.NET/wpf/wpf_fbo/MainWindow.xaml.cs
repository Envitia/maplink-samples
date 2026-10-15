using System;
using System.Windows;
using System.Windows.Media.Imaging;
using windowsForms = System.Windows.Forms;
using System.Windows.Threading;

using System.Windows.Input;
using System.Windows.Controls;

using Envitia.MapLink;

namespace CSharpWPF_FrameBufferObject
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Fields
        private int frames;
        MaplinkSurfaceInterface maplinkInterface;
        InteractionModesInterface interactionModes;
        #endregion

        #region Constructors and Destructors
        public MainWindow()
        {
            InitializeComponent();

            //! Set Maplink Home directory here ...
            string maplHome = TSLNUtilityFunctions.getMapLinkHome();
            if (string.IsNullOrEmpty(maplHome))
            {
              TSLNUtilityFunctions.setMapLinkHome("../");
            }

            //! Must be done in order to display maps through the new maplink display helper
            bool load = Envitia.MapLink.TSLNDrawingSurface.loadStandardConfig();

            maplinkInterface = new MaplinkSurfaceInterface();
            interactionModes = new InteractionModesInterface(this.imageContainer, maplinkInterface.m_drawingSurface, true);
            interactionModes.m_refreshContainer += refresh;
            interactionModes.RightClickCurrentModeChange += UpdateToolsAndMenuButtons;
            zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;

            //! to test the fps
            //setTimer(1);
    }

        #endregion

        #region Window_Events
        private void imageContainer_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            refresh();

            if (interactionModes.m_modeManager != null)
            {
                int _imgWidth = (int)this.imageContainer.ActualWidth;
                int _imgHeight = (int)this.imageContainer.ActualHeight;
                if (_imgWidth <= 0 || _imgHeight <= 0)
                {
                    return;
                }
                interactionModes.m_modeManager.onSize(_imgWidth, _imgHeight);
            }
        }

        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            //close logic here
            maplinkInterface.dispose();

            base.OnClosing(e);
        }
        #endregion

        #region ContainerInteractions
        private void imageContainer_MouseDown(object sender, MouseButtonEventArgs e)
        {
            interactionModes.OnMouseDown(e);
        }

        private void imageContainer_MouseMove(object sender, MouseEventArgs e)
        {
            interactionModes.OnMouseMove(e);
        }

        private void imageContainer_MouseUp(object sender, MouseButtonEventArgs e)
        {
            interactionModes.OnMouseUp(e);
        }

        private void imageContainer_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            interactionModes.OnMouseWheel(e);
        }

        private void imageContainer_KeyDown(object sender, KeyEventArgs e)
        {
            interactionModes.OnKeyDown((e.Key == Key.LeftCtrl || e.Key == Key.RightCtrl), (e.Key == Key.LeftShift || e.Key == Key.RightShift));
        }
        private void imageContainer_KeyUp(object sender, KeyEventArgs e)
        {
            interactionModes.OnKeyUp((e.Key == Key.LeftCtrl || e.Key == Key.RightCtrl), (e.Key == Key.LeftShift || e.Key == Key.RightShift));
        }

        public bool zoomIn(int zoomPercentage, bool addToViewStack)
        {
            bool invalidate = interactionModes.zoomIn(zoomPercentage, addToViewStack);
            return invalidate;
        }
        public bool zoomOut(int zoomPercentage, bool addToViewStack)
        {
            bool invalidate = interactionModes.zoomOut(zoomPercentage, addToViewStack);
            return invalidate;
        }

        public bool resetToFullExtent(bool addToViewStack)
        {
            bool invalidate = interactionModes.resetToFullExtent(addToViewStack);
            return invalidate;
        }
        #endregion

        #region TimerFunctions
        private DateTime lastMeasureTime;
        private void setTimer(int timerIntervalMSec)
        {
            DispatcherTimer timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(timerIntervalMSec);
            timer.Tick += this.TimerOnTick;
            timer.Start();
        }

        private void TimerOnTick(object sender, EventArgs eventArgs)
        {
            if (DateTime.Now.Subtract(this.lastMeasureTime) > TimeSpan.FromSeconds(1))
            {
                this.Title = this.frames + "fps";
                this.frames = 0;
                this.lastMeasureTime = DateTime.Now;
            }

            this.refresh();
        }
        #endregion

        #region BackgroundFunctions
        private void refresh()
        {
            int _imgWidth = (int)this.imageContainer.ActualWidth;
            int _imgHeight = (int)this.imageContainer.ActualHeight;
            if (_imgWidth <= 0 || _imgHeight <= 0)
            {
                return;
            }

            WriteableBitmap backbuffer = null;
            maplinkInterface.readPixels(ref backbuffer, _imgWidth, _imgHeight);
            if (backbuffer != null)
            {
                this.image.Source = backbuffer;
            }

            // Don't forget to draw any echo rectangle that may be active.
            if (interactionModes.m_modeManager != null)
            {
                Rect rec = GetAbsolutePlacement(this.imageContainer);
                interactionModes.m_modeManager.onDraw((int)rec.Left, (int)rec.Top, (int)rec.Right, (int)rec.Bottom);
            }

            this.frames++;
        }
    #endregion

    #region MenuandToolBat_Handling
    public void openMap(string mapPath)
    {
      maplinkInterface.addMapDataLayer(mapPath, "staticmap");
      refresh();
    }

    private void FileOpen_Click(object sender, RoutedEventArgs e)
        {
            windowsForms.OpenFileDialog openFileDialog = new windowsForms.OpenFileDialog();
            if (openFileDialog.ShowDialog() == windowsForms.DialogResult.OK)
            {
                string mapPath = openFileDialog.FileName;
                openMap(mapPath);
            }
        }

        private void PrintMap_Click(object sender, RoutedEventArgs e)
        {
            printMap();
        }

        private void ZoomIn_Click(object sender, RoutedEventArgs e)
        {
            zoomIn(30, true);
        }
        private void ZoomOut_Click(object sender, RoutedEventArgs e)
        {
            zoomOut(30, true);
        }
        private void ResetZoom_Click(object sender, RoutedEventArgs e)
        {
            resetToFullExtent(true);
        }
        private void ZoomTool_Click(object sender, RoutedEventArgs e)
        {
            this.image.Cursor = Cursors.Arrow;
            interactionModes.setCurrentMode(InteractionModesInterface.InteractionModeEnum.TOOLS_ZOOM);
            UpdateToolsAndMenuButtons();
        }
        private void PanTool_Click(object sender, RoutedEventArgs e)
        {
            this.image.Cursor = Cursors.Arrow;
            interactionModes.setCurrentMode(InteractionModesInterface.InteractionModeEnum.TOOLS_PAN);
            UpdateToolsAndMenuButtons();
        }
        private void MagnifyTool_Click(object sender, RoutedEventArgs e)
        {
            this.image.Cursor = Cursors.Arrow;
            interactionModes.setCurrentMode(InteractionModesInterface.InteractionModeEnum.TOOLS_MAGNIFY);
            UpdateToolsAndMenuButtons();
        }
        private void GrabTool_Click(object sender, RoutedEventArgs e)
        {
            this.image.Cursor = Cursors.Hand;
            interactionModes.setCurrentMode(InteractionModesInterface.InteractionModeEnum.TOOLS_GRAB);
            UpdateToolsAndMenuButtons();
        }
        private void ZoomPanTool_Click(object sender, RoutedEventArgs e)
        {
            this.image.Cursor = Cursors.Arrow;
            interactionModes.setCurrentMode(InteractionModesInterface.InteractionModeEnum.TOOLS_ZOOM_PAN);
            UpdateToolsAndMenuButtons();
        }
        private void PreviousView_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.viewStackGotoPrevious();
        }
        private void NextView_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.viewStackGotoNext();
        }
        private void GotoView1_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.gotoSavedView(0);
        }
        private void GotoView2_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.gotoSavedView(1);
        }
        private void GotoView3_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.gotoSavedView(2);
        }

        private void SetView1_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.saveView(0);
        }
        private void SetView2_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.saveView(1);
        }
        private void SetView3_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.saveView(2);
        }
        private void ResetViews_Click(object sender, RoutedEventArgs e)
        {
            interactionModes.viewStackReset();
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
        #endregion

        #region HelpingMethods
        public void printMap()
        {
            int _imgWidth = (int)this.imageContainer.ActualWidth;
            int _imgHeight = (int)this.imageContainer.ActualHeight;
            if (_imgWidth <= 0 || _imgHeight <= 0)
            {
                return;
            }
            maplinkInterface.printMap(_imgWidth, _imgHeight);
        }

        public static Rect GetAbsolutePlacement(FrameworkElement element, bool relativeToScreen = false)
        {
            var absolutePos = element.PointToScreen(new System.Windows.Point(0, 0));
            if (relativeToScreen)
            {
                return new Rect(absolutePos.X, absolutePos.Y, element.ActualWidth, element.ActualHeight);
            }
            var posMW = Application.Current.MainWindow.PointToScreen(new System.Windows.Point(0, 0));
            absolutePos = new System.Windows.Point(absolutePos.X - posMW.X, absolutePos.Y - posMW.Y);
            return new Rect(absolutePos.X, absolutePos.Y, element.ActualWidth, element.ActualHeight);
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
            InteractionModesInterface.InteractionModeEnum id = interactionModes.getCurrentMode();
            switch (id)
            {
                case InteractionModesInterface.InteractionModeEnum.TOOLS_ZOOM:
                    zoomToolBarCheck.IsChecked = true;
                    zoomtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
                case InteractionModesInterface.InteractionModeEnum.TOOLS_PAN:
                    panToolBarCheck.IsChecked = true;
                    pantoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
                case InteractionModesInterface.InteractionModeEnum.TOOLS_GRAB:
                    grabToolBarCheck.IsChecked = true;
                    grabtoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
                case InteractionModesInterface.InteractionModeEnum.TOOLS_ZOOM_PAN:
                    zoompanToolBarCheck.IsChecked = true;
                    zoompantoolBorder.BorderBrush = System.Windows.Media.Brushes.Black;
                    break;
            }
        }
        #endregion
    }
}
