using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using DrawingSurfacePanel;

namespace CSharpCustomPanel_Winforms_Sample
{
    /// <summary>
    /// MapViewerForm
    /// </summary>
    public partial class MapViewerForm : Form
    {
        //Holds information about each interaction mode
        private class InteractionMode
        {
            //The id
            public MapViewerPanel.InteractionModeEnum id;
            //The button on the tool bar that activates this mode
            public System.Windows.Forms.ToolBarButton button;
            //The button on the menu bar that activates this mode
            public System.Windows.Forms.MenuItem menubutton;
        }

        //Create our array pointer of interaction mode information
        private InteractionMode[] m_imodeset = null;

        //Short name of the loaded map
        private string m_mapFilename;

        public MapViewerForm(string mapFile)
        {
            m_mapFilename = mapFile;
            InitializeComponent();
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            string mapFile = "";
            if (args.Length > 0)
            {
                mapFile = args[0];
            }
            Application.Run(new MapViewerForm(mapFile));
        }

        protected override void OnLoad(EventArgs e)
        {
            m_imodeset = new InteractionMode[5];
            //Create the mode sets. These are use by the toolbar and the menu to identify the
            //modes
            m_imodeset[0] = new InteractionMode();
            m_imodeset[0].id = MapViewerPanel.InteractionModeEnum.TOOLS_ZOOM;
            m_imodeset[0].button = ZoomTool;
            m_imodeset[0].menubutton = ToolsZoom;
            m_imodeset[1] = new MapViewerForm.InteractionMode();
            m_imodeset[1].id = MapViewerPanel.InteractionModeEnum.TOOLS_PAN;
            m_imodeset[1].button = PanTool;
            m_imodeset[1].menubutton = ToolsPan;
            m_imodeset[2] = new MapViewerForm.InteractionMode();
            m_imodeset[2].id = MapViewerPanel.InteractionModeEnum.TOOLS_MAGNIFY;
            m_imodeset[2].button = MagnifyTool;
            m_imodeset[2].menubutton = ToolsMagnify;
            m_imodeset[3] = new MapViewerForm.InteractionMode();
            m_imodeset[3].id = MapViewerPanel.InteractionModeEnum.TOOLS_GRAB;
            m_imodeset[3].button = GrabTool;
            m_imodeset[3].menubutton = ToolsGrab;
            m_imodeset[4] = new MapViewerForm.InteractionMode();
            m_imodeset[4].id = MapViewerPanel.InteractionModeEnum.TOOLS_ZOOM_PAN;
            m_imodeset[4].button = ZoomPanTool;
            m_imodeset[4].menubutton = ToolsZoomPan;

            this.mapViewerPanel.RightClickCurrentModeChange += UpdateToolsAndMenuButtons;
        }

        //Resize of the ViewerPanel
        //protected override void OnResize(EventArgs e)
        //{
        //    //this.mapViewerPanel.ResizePanel(this.mapViewerPanel.DisplayRectangle);
        //    this.mapViewerPanel.onSize(this.mapViewerPanel.DisplayRectangle.Width, this.mapViewerPanel.DisplayRectangle.Height);
        //    //base.OnResize(e);
        //}

        //Open a Map
        private void FileOpen(bool showDialog)
        {
            this.mapViewerPanel.fileOpen(showDialog);
        }

        private void FileOpenMenu_Click(object sender, System.EventArgs e)
        {
            FileOpen(true);
        }

        //This is called when any of the buttons on the Toolbar is clicked
        private void TheToolBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
        {
            bool invalidate = false;
            if (e.Button == FileOpenButton)
            {
                FileOpen(true);
            }
            else if (e.Button == FilePrintButton)
            {
                PrintMap();
            }
            else if (e.Button == CopyButton)
            {
                EditCopy();
            }
            else if (e.Button == ZoomIn)
            {
                this.mapViewerPanel.zoomIn(30, true);
            }
            else if (e.Button == ZoomOut)
            {
                this.mapViewerPanel.zoomOut(30, true);
            }
            else if (e.Button == ResetButton)
            {
                invalidate = this.mapViewerPanel.resetToFullExtent(true);
            }
            else if (e.Button == previousView)
            {
                invalidate = this.mapViewerPanel.viewStackGotoPrevious();
            }
            else if (e.Button == nextView)
            {
                invalidate = this.mapViewerPanel.viewStackGotoNext();
            }
            else if (e.Button == savedView1)
            {
                invalidate = GotoSavedView(0);
            }
            else if (e.Button == savedView2)
            {
                invalidate = GotoSavedView(1);
            }
            else if (e.Button == savedView3)
            {
                invalidate = GotoSavedView(2);
            }
            else
            {
                for (int i = 0; i < m_imodeset.Length; ++i)
                {
                    if (e.Button == m_imodeset[i].button)
                    {
                        invalidate = this.mapViewerPanel.setCurrentMode(m_imodeset[i].id);
                        break;
                    }
                }
                UpdateToolsAndMenuButtons();
            }
        }

        //called whenever a new imode is selected
        private void UpdateToolsAndMenuButtons()
        {
            //TSLNInteractionMode cmode = null;
            MapViewerPanel.InteractionModeEnum id = this.mapViewerPanel.getCurrentMode();

            for (int i = 0; i < m_imodeset.Length; ++i)
            {
                if (id == m_imodeset[i].id)
                {
                    m_imodeset[i].button.Pushed = true;
                    m_imodeset[i].menubutton.Checked = true;
                }
                else
                {
                    m_imodeset[i].button.Pushed = false;
                    m_imodeset[i].menubutton.Checked = false;
                }
            }
        }

        //called when one of the imode menu items are clicked
        private void MenuImode_Click(object sender, System.EventArgs e)
        {
            for (int i = 0; i < m_imodeset.Length; ++i)
            {
                if (sender == m_imodeset[i].menubutton)
                {
                    this.mapViewerPanel.setCurrentMode(m_imodeset[i].id);
                    break;
                }
            }
            UpdateToolsAndMenuButtons();
        }

        //called whenever one of the base-level non imode menu buttons on the Tools
        //menu are clicked
        private void MenuNonImode_Click(object sender, System.EventArgs e)
        {
            if (sender == this.ToolsZoomIn)
            {
                this.mapViewerPanel.zoomIn(30, true);
            }
            else if (sender == this.ToolsZoomOut)
            {
                this.mapViewerPanel.zoomOut(30, true);
            }
            else if (sender == this.ToolsReset)
            {
                this.mapViewerPanel.resetToFullExtent(true);
            }

        }

        //called when Help->About is clicked from the menu
        private void HelpAboutMenu_Click(object sender, System.EventArgs e)
        {
            AboutBox abox = new AboutBox();
            abox.ShowDialog(this);
        }

        //called when File->Print is clicked from the menu
        private void FilePrintMenu_Click(object sender, System.EventArgs e)
        {
            PrintMap();
        }

        //Called from either the menu or toolbar print buttons
        private void PrintMap()
        {
            this.mapViewerPanel.printMap();
        }

        //called when Edit->Copy is clicked from the menu
        private void EditCopy_Click(object sender, System.EventArgs e)
        {
            EditCopy();
        }

        //Called from either the menu or toolbar copy buttons
        private void EditCopy()
        {
            this.mapViewerPanel.editCopy();
        }

        //called when Tools->Saved Views->* is clicked from the menu
        private void OnSavedViewsMenuClick(object sender, System.EventArgs e)
        {
            bool invalidate = false;

            if (sender == ToolsGoto1Menu)
            {
                invalidate = GotoSavedView(0);
            }
            else if (sender == ToolsGoto1Menu)
            {
                invalidate = GotoSavedView(1);
            }
            else if (sender == ToolsGoto1Menu)
            {
                invalidate = GotoSavedView(2);
            }
            else if (sender == ToolsSet1Menu)
            {
                SaveView(0);
            }
            else if (sender == ToolsSet2Menu)
            {
                SaveView(1);
            }
            else if (sender == ToolsSet3Menu)
            {
                SaveView(2);
            }
            else if (sender == ToolsViewsResetMenu)
            {
                ViewStackReset();
            }
        }

        private bool GotoSavedView(int index)
        {
            return this.mapViewerPanel.gotoSavedView(index);
        }

        private void SaveView(int index)
        {
            this.mapViewerPanel.saveView(index);
        }

        private void ViewStackReset()
        {
            this.mapViewerPanel.viewStackReset();
        }

        private void OnViewStackMenuClick(object sender, System.EventArgs e)
        {

            bool invalidate = false;

            if (sender == ToolsPreviousViewMenu)
            {
                this.mapViewerPanel.viewStackGotoPrevious();
            }
            else if (sender == ToolsNextViewMenu)
            {
                this.mapViewerPanel.viewStackGotoNext();
            }
        }

        private void FileExitMenu_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}
