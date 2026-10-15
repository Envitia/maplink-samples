using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.OpenGLSurface;

namespace DrawingSurfacePanel
{
#if false
    using DrawingSurfaceClass = TSLNDrawingSurface;
#else
    using DrawingSurfaceClass = TSLNOpenGLSurface;
#endif


    /// <summary>
    /// Summary description for drawingSurfacePanel.
    /// </summary>
    public class MapViewerPanel : Panel
    {
        #region Panel_Fileds
        #region Panel_InteractionMode_Parameters
        public enum InteractionModeEnum
        {
            InValid = -1,

            TOOLS_ZOOM = 1,
            TOOLS_PAN,
            TOOLS_MAGNIFY,
            TOOLS_GRAB,
            TOOLS_ZOOM_PAN
        }

        struct contextMenuTool
        {
            public TSLNInteractionMode interactionMode;
            public InteractionModeEnum mode;
            public string text;
            public bool ischecked;
        }

        contextMenuTool[] m_contextMenuTools;

        private InteractionModeEnum m_currentInteractionMode;

        //Create out request receiver pointer
        private InteractionModeRequestReceiver m_updateReceiver = null;

        //Create interaction modes' pointers
        private TSLNInteractionModeManagerGeneric m_modeManager = null; //private TSLNInteractionModeManager m_modeManager = null;
        
        #endregion

        //Create the usual MapLink Item pointers
        private DrawingSurfaceClass m_drawingSurface = null;
        private TSLNMapDataLayer m_mapDataLayer = null;

        //Variables to hold if the control/shift button are currently pressed
        private bool m_controlButtonDown = false;
        private bool m_shiftButtonDown = false;

        //Short name of the loaded map
        private string m_shortfilename = "";
        private string m_mapFilename = "";

        /// <summary>
        /// if set to true, it shows context menu when mouse right click.
        /// if set to false, it resets the tool to zoom tool when mouse right click.
        /// </summary>
        private bool m_showRightClickContextMenu;

        // context menu when right click is clicked
        ContextMenuStrip m_contexMenuuu = null;

        public event rightClickCurrentModeChangeHandler RightClickCurrentModeChange;
        public delegate void rightClickCurrentModeChangeHandler();
        #endregion

        #region Panel_Parameters
        public String MapFileName
        {
            get
            {
                return m_mapFilename;
            }
            set
            {
               m_mapFilename = value;
            }
        }
        public String ShortFileName
        {
            get
            {
                return m_shortfilename;
            }
        }

        /// <summary>
        /// if set to true, it shows context menu when mouse right click.
        /// if set to false, it resets the tool to zoom tool when mouse right click.
        /// </summary>
        public bool showRightClickContextMenu
        {
            get
            {
                return m_showRightClickContextMenu;
            }

            set
            {
                m_showRightClickContextMenu = value;
            }
        }
        #endregion

        #region Panel_Contructors
        public MapViewerPanel() : this("", true)
        {
        }

        public MapViewerPanel(string mapFile, bool showRightClickContextMenu = true)
        {
            m_showRightClickContextMenu = showRightClickContextMenu;
            m_mapFilename = mapFile;
            initializeComponent();
            //! Set Maplink Home directory here ...
            string maplHome = TSLNUtilityFunctions.getMapLinkHome();
            if (string.IsNullOrEmpty(maplHome))
            {
              TSLNUtilityFunctions.setMapLinkHome("../");
            }
            //! Must be done in order to display maps through the new maplink display helper
            bool load = Envitia.MapLink.TSLNDrawingSurface.loadStandardConfig();
            InitializaPanel();
        }

        ~MapViewerPanel()
        {
            cleanUp();
        }
        #endregion

        #region Panel_Controls
        // panel controls
        //private Button button1;
        private MapViewerParentPanel ViewerPanel;

        private void initializeComponent()
        {
            //! define the panel's controls
            //this.button1 = new System.Windows.Forms.Button();
            this.ViewerPanel = new MapViewerParentPanel();
            this.SuspendLayout();

            //// 
            //// button1
            //// 
            //this.button1.Location = new System.Drawing.Point(0, 0);
            //this.button1.Name = "button1";
            //this.button1.Size = new System.Drawing.Size(75, 25);
            //this.button1.TabIndex = 0;
            //this.button1.Text = "button1";
            //this.button1.UseVisualStyleBackColor = true;

            // 
            // ViewerPanel
            // 
            this.ViewerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewerPanel.Location = new System.Drawing.Point(0, 35);
            this.ViewerPanel.Name = "ViewerPanel";
            this.ViewerPanel.Size = new System.Drawing.Size(783, 495);
            this.ViewerPanel.TabIndex = 2;
            //! override interaction functions
            this.ViewerPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
            this.ViewerPanel.Resize += new System.EventHandler(this.OnReSize);
            this.ViewerPanel.HandleDestroyed += new System.EventHandler(this.OnDestroyed);
            this.ViewerPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.ViewerPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
            this.ViewerPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            this.ViewerPanel.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.OnMouseWheel);
            this.ViewerPanel.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            this.ViewerPanel.KeyUp += new System.Windows.Forms.KeyEventHandler(this.OnKeyUp);

            //! Add controls to the panel
            this.Controls.Add(this.ViewerPanel);
            //this.Controls.Add(this.button1);
            this.ResumeLayout(false);
        }
        #endregion

        #region Panel_Basic_Functions
        //Paint of the ViewerPanel
        private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            if (m_drawingSurface != null)
            {
                m_drawingSurface.drawDU(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom, false, false);
            }

            // Don't forget to draw any echo rectangle that may be active.
            if (m_modeManager != null)
            {
                m_modeManager.onDraw(e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Bottom);
            }
        }

        //Resize of the ViewerPanel
        private void OnReSize(object sender, System.EventArgs e)
        {
            if (m_drawingSurface != null)
            {
                m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
            }
            if (m_modeManager != null)
            {
                m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);
            }

            // Without the invalidate, the paint is only called when the window gets bigger
            ViewerPanel.Invalidate();
        }

        //On mouse down on the ViewerPanel
        private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (m_modeManager == null)
            {
                return;
            }

            bool invalidate = false;

            switch (e.Button)
            {
                case MouseButtons.Left:
                    {
                        invalidate = m_modeManager.onLButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButtons.Middle:
                    {
                        invalidate = m_modeManager.onMButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButtons.Right:
                    {
                        invalidate = m_modeManager.onRButtonDown(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
            }
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
        }
        //On mouse move on the ViewerPanel
        private void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (m_modeManager == null)
            {
                return;
            }
            TSLNButtonType button = TSLNButtonType.TSLNButtonNone;

            if (e.Button == MouseButtons.Left)
                button = TSLNButtonType.TSLNButtonLeft;
            else if (e.Button == MouseButtons.Middle)
                button = TSLNButtonType.TSLNButtonCentre;
            else if (e.Button == MouseButtons.Right)
                button = TSLNButtonType.TSLNButtonRight;

            if (m_modeManager.onMouseMove(button, e.X, e.Y, m_shiftButtonDown, m_controlButtonDown))
            {
                // Request a redraw if the interaction hander requires it
                ViewerPanel.Invalidate();
            }
        }

        //On mouse up on the ViewerPanel
        private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (m_modeManager == null)
            {
                return;
            }

            bool invalidate = false;
            switch (e.Button)
            {
                case MouseButtons.Left:
                    {
                        invalidate = m_modeManager.onLButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButtons.Middle:
                    {
                        invalidate = m_modeManager.onMButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButtons.Right:
                    {
                        if (m_showRightClickContextMenu)
                        {
                            showRClickContextMenu(e);
                        }
                        else
                        {
                            invalidate = m_modeManager.onRButtonUp(e.X, e.Y, m_shiftButtonDown, m_controlButtonDown);
                            updateRightClickModeChangeEvent();
                        }

                        break;
                    }
            }

            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
        }

        //On mouse wheel on the ViewerPanel
        private void OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (m_modeManager == null)
            {
                return;
            }
            if (m_modeManager.onMouseWheel((short)e.Delta, e.X, e.Y))
            {
                ViewerPanel.Invalidate();
            }
        }

        //On key down anywhere in the app
        private void OnKeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.Control == true)
            {
                m_controlButtonDown = true;
            }
            else if (e.Shift == true)
            {
                m_shiftButtonDown = true;
            }
        }

        //On key up anywhere in the app
        private void OnKeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (m_controlButtonDown && e.Control == false)
            {
                m_controlButtonDown = false;
            }
            else if (m_shiftButtonDown && e.Shift == false)
            {
                m_shiftButtonDown = false;
            }
        }

        //protected override void OnLoad(EventArgs e)
        //{
        //    Init();
        //    base.OnLoad(e);
        //}

        //private void OnClosed(object sender, FormClosedEventArgs e)
        //{
        //    CleanUp();
        //}

        private void OnDestroyed(object sender, System.EventArgs e)
        {
            cleanUp();
        }

        /// <summary>
        /// initialize the context menu by setting the options
        /// </summary>
        private void initializeRClickContextMenu()
        {
            if (m_contexMenuuu != null)
                return;

            m_contexMenuuu = new ContextMenuStrip();
            for (int idx = 0; idx < m_contextMenuTools.Length; ++idx)
            {
                m_contexMenuuu.Items.Add(m_contextMenuTools[idx].text);
                ((ToolStripMenuItem)m_contexMenuuu.Items[idx]).Checked = m_contextMenuTools[idx].ischecked;
            }
            m_contexMenuuu.Items.Add("-");//< seperator
            m_contexMenuuu.Items.Add("Zoom In");
            m_contexMenuuu.Items.Add("Zoom Out");
            m_contexMenuuu.Items.Add("Reset Zoom");
        }

        /// <summary>
        /// when a check menu item is chosen, it unchecks other options
        /// </summary>
        /// <param name="text"></param>
        /// <param name="contexMenuuu"></param>
        /// <param name="tools"></param>
        private void updateContextMenuToolChecked(InteractionModeEnum mode, ContextMenuStrip contexMenuuu, contextMenuTool[] tools)
        {
            if (contexMenuuu == null)
                return;

            for (int idx = 0; idx < m_contextMenuTools.Length; ++idx)
            {
                if (m_contextMenuTools[idx].mode == mode)
                {
                    ((ToolStripMenuItem)contexMenuuu.Items[idx]).Checked = true;
                }
                else
                {
                    ((ToolStripMenuItem)contexMenuuu.Items[idx]).Checked = false;
                }
            }     
        }

        private void updateRightClickModeChangeEvent()
        {
            if (this.RightClickCurrentModeChange != null)
            {
                this.RightClickCurrentModeChange();
            }
        }

        /// <summary>
        /// show right click context menu
        /// </summary>
        /// <param name="e"></param>
        private void showRClickContextMenu(System.Windows.Forms.MouseEventArgs e)
        {
            if (m_contexMenuuu == null)
                return;

            m_contexMenuuu.Show(PointToScreen(e.Location));
            m_contexMenuuu.ItemClicked += new ToolStripItemClickedEventHandler(
                contexMenuuu_ItemClicked);
        }

        /// <summary>
        /// handles when an option is chosen from the right click context menu.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void contexMenuuu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            ToolStripItem item = e.ClickedItem;
            // your code here
            switch (item.Text)
            {
                case "Zoom Tool":
                    if(m_currentInteractionMode != InteractionModeEnum.TOOLS_ZOOM)
                    {
                        setCurrentMode(InteractionModeEnum.TOOLS_ZOOM);
                        updateRightClickModeChangeEvent();
                    }
                    break;

                case "Pan Tool":
                    if (m_currentInteractionMode != InteractionModeEnum.TOOLS_PAN)
                    {
                        setCurrentMode(InteractionModeEnum.TOOLS_PAN);
                        updateRightClickModeChangeEvent();
                    }
                    break;

                case "Magnify Tool":
                    if (m_currentInteractionMode != InteractionModeEnum.TOOLS_MAGNIFY)
                    {
                        setCurrentMode(InteractionModeEnum.TOOLS_MAGNIFY);
                        updateRightClickModeChangeEvent();
                    }
                    break;

                case "Grab Tool":
                    if (m_currentInteractionMode != InteractionModeEnum.TOOLS_GRAB)
                    {
                        setCurrentMode(InteractionModeEnum.TOOLS_GRAB);
                        updateRightClickModeChangeEvent();
                    }
                    break;

                case "ZoomPan Tool":
                    if (m_currentInteractionMode != InteractionModeEnum.TOOLS_ZOOM_PAN)
                    {
                        setCurrentMode(InteractionModeEnum.TOOLS_ZOOM_PAN);
                        updateRightClickModeChangeEvent();
                    }
                    break;

                case "Zoom In":
                    zoomIn(30, true);
                    break;

                case "Zoom Out":
                    zoomOut(30, true);
                    break;

                case "Reset Zoom":
                    resetToFullExtent(true);
                    break;
            }

        }
        #endregion

        #region Panel_Functions
        public void InitializaPanel()
        {
            bool status = DrawingSurfaceClass.loadStandardConfig();
#if false
            m_drawingSurface = new DrawingSurfaceClass((IntPtr)ViewerPanel.Handle, false);
#else
            TSLNOpenGLSurfaceCreationParameters creationOptions = new TSLNOpenGLSurfaceCreationParameters();
            creationOptions.createCoreProfile(true);
            creationOptions.enableDoubleBuffering(true);
            m_drawingSurface = new TSLNWGLSurface(ViewerPanel.Handle, false, creationOptions);
#endif

            m_mapDataLayer = new TSLNMapDataLayer();
            m_updateReceiver = new InteractionModeRequestReceiver();
            // Attach the drawing surface to the form and set it's initial size...
            // This must be done before the drawingsurface is given to the
            // InteractionModeManager
            m_drawingSurface.attach(ViewerPanel.Handle, false);

            // Create our user defined request receiver
            TSLNInteractionModeRequest request = (TSLNInteractionModeRequest)m_updateReceiver;

            // Now create the mode manager
            //m_modeManager = new TSLNInteractionModeManager(request, m_drawingSurface, ViewerPanel.Handle);
            m_modeManager = new TSLNInteractionModeManagerGeneric(request, m_drawingSurface, 5, 5, 30, true);

            // Next create our modes
            InitializeInteractionModes();

            //Give them to the modeManager
            //Remember we don't own the modes anymore after this point
            //but we must keep a reference to them or c# won't deleted them properly
            foreach (var tool in m_contextMenuTools)
            {
                m_modeManager.addMode(tool.interactionMode, tool.ischecked);
            }

            m_drawingSurface.wndResize(ViewerPanel.DisplayRectangle.Left, ViewerPanel.DisplayRectangle.Top, ViewerPanel.DisplayRectangle.Right, ViewerPanel.DisplayRectangle.Bottom, false, TSLNResizeActionEnum.TSLNResizeActionMaintainCentre);
            m_modeManager.onSize(ViewerPanel.DisplayRectangle.Width, ViewerPanel.DisplayRectangle.Height);

            // In MapLink 5.0+, this will only work AFTER the drawing surface has been attached to a window
            status = m_drawingSurface.addDataLayer(m_mapDataLayer, "map");
            m_drawingSurface.setDataLayerProps("map", TSLNPropertyEnum.TSLNPropertyBuffered, 1);

            m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);

            m_drawingSurface.reset(false);

            //activate zoom imode
            setCurrentMode(InteractionModeEnum.TOOLS_ZOOM);

            if(m_showRightClickContextMenu)
            {
                initializeRClickContextMenu();
            }           

            //Open the current map file, if passed on command line
            fileOpen(false);
        }

        private void InitializeInteractionModes()
        {
            m_contextMenuTools = new contextMenuTool[]
            {
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_ZOOM, text = "Zoom Tool", ischecked = true},
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_PAN, text = "Pan Tool", ischecked = false},
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_MAGNIFY, text = "Magnify Tool", ischecked = false},
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_GRAB, text = "Grab Tool", ischecked = false},
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_ZOOM_PAN, text = "ZoomPan Tool", ischecked = false}
            };

            for (int i = 0; i < m_contextMenuTools.Length; ++i)
            {
                switch (m_contextMenuTools[i].text)
                {
                    case "Zoom Tool":
                        m_contextMenuTools[i].interactionMode = new TSLNInteractionModeZoom((int)InteractionModeEnum.TOOLS_ZOOM, 30, "Left button drag zoom to rectangle, Left button click anchored zoom-in, Shift-Left button click anchored zoom-out, Right button click to finish", true);
                        break;

                    case "Pan Tool":
                        m_contextMenuTools[i].interactionMode = new TSLNInteractionModePan((int)InteractionModeEnum.TOOLS_PAN, "Left button click pans to point, Right button click to finish", true);
                        break;
                    case "Magnify Tool":
                        m_contextMenuTools[i].interactionMode = new TSLNInteractionModeMagnify((int)InteractionModeEnum.TOOLS_MAGNIFY, "Mouse move to change magnified area, Right button click to finish", true);
                        break;
                    case "Grab Tool":
                        m_contextMenuTools[i].interactionMode = new TSLNInteractionModeGrab((int)InteractionModeEnum.TOOLS_GRAB, true, "Left button drag move view, Right button click to finish", true);
                        break;
                    case "ZoomPan Tool":
                        m_contextMenuTools[i].interactionMode = new CustomInteractionModeZoomPan((int)InteractionModeEnum.TOOLS_ZOOM_PAN, 30, true, "Left button click pans to point and zooms in, Right button click pans to point and zooms out");
                        break;
                }
            }
        }

        private void cleanUp()
        {
            DrawingSurfaceClass.cleanup();
            m_drawingSurface.Dispose();
            m_drawingSurface = null;
        }

        //Called from either the menu or toolbar print buttons
        public void printMap()
        {
            TSLNPrintItem[] printItems = new TSLNPrintItem[1];
            TSLNPrintItem printItem = new TSLNPrintItem();
            TSLNEnvelope envelope = new TSLNEnvelope();
            printItems.SetValue(printItem, 0);

            double uux1 = 0, uuy1 = 0, uux2 = 0, uuy2 = 0;
            int tmcx1 = 0, tmcy1 = 0, tmcx2 = 0, tmcy2 = 0;
            string templateFile = "";
            bool isLandscape;

            if (ViewerPanel.Width > ViewerPanel.Height)
            {
                templateFile = "landscape.tpt";
                isLandscape = true;
            }
            else
            {
                templateFile = "portrait.tpt";
                isLandscape = false;
            }

            string templateFilePath = DrawingSurfaceClass.findFile(templateFile);

            TSLNEntitySet entitySet = TSLNPrintDocument.loadTemplate(templateFilePath, DrawingSurfaceClass.fileLoader, null);

            if (entitySet.size == 0)
            {
                MessageBox.Show(ViewerPanel, "MapLink could not load printer template", "Error", MessageBoxButtons.OK);
                return;
            }

            m_drawingSurface.getUUExtent(out uux1, out uuy1, out uux2, out uuy2);
            m_drawingSurface.UUToTMC(uux1, uuy1, out tmcx1, out tmcy1);
            m_drawingSurface.UUToTMC(uux2, uuy2, out tmcx2, out tmcy2);
            envelope.bottomLeft.x = tmcx1;
            envelope.bottomLeft.y = tmcy1;
            envelope.topRight.x = tmcx2;
            envelope.topRight.y = tmcy2;
            long tmcPerMetre = printItem.tmcPerMetre;
            printItem.drawingSurface = m_drawingSurface;
            printItem.extent(envelope);
            printItem.borderColour = Color.Black;
            printItem.borderStyle = 1;
            printItem.borderThickness = 2;
            printItem.templateName = "map";

            string[] textMappings = (string[])System.Array.CreateInstance(typeof(string), 4);
            textMappings.SetValue("MapLink Pro", 0);    // Default
            textMappings.SetValue(m_shortfilename, 1);    // New text
            textMappings.SetValue("date", 2);    // Default
            System.DateTime dt = System.DateTime.Now;
            textMappings.SetValue(dt.ToShortDateString(), 3);    // Default

            try
            {
                TSLNPrintDocument pd = new TSLNPrintDocument(m_drawingSurface, entitySet, printItems, textMappings, isLandscape);
                PrintDialog dlg = new PrintDialog();
                dlg.Document = pd;
                DialogResult result = dlg.ShowDialog();

                if (result == DialogResult.OK)
                {
                    pd.Print();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "An error occurred attempting to print -  " + ex.Message);
            }
        }

        //Open a Map
        public void fileOpen(bool showDialog)
        {
            if (showDialog)
            {
                OpenFileDialog FileOpenDialog = new OpenFileDialog();
                FileOpenDialog.ShowDialog();
                m_mapFilename = FileOpenDialog.FileName;
            }
            if (m_mapFilename == "")
            {
                return;
            }

            int indexOfLastSlash = m_mapFilename.LastIndexOf('\\') + 1;
            m_shortfilename = m_mapFilename.Substring(indexOfLastSlash, m_mapFilename.Length - indexOfLastSlash);

            this.Text = "C# .NET Interaction Modes Sample - " + m_shortfilename;

            if (!m_mapDataLayer.loadData(m_mapFilename))
            {
                string error = TSLNErrorStack.errorString("Errors:\n", TSLNErrorCategory.TSLNErrorCategoryError | TSLNErrorCategory.TSLNErrorCategoryFatal);
                if (error != "")
                {
                    MessageBox.Show(this, "MapLink has reported an error during loading of the selected map", "Error", MessageBoxButtons.OK);
                }
            }
            m_drawingSurface.backgroundColour = m_mapDataLayer.backgroundColour;
            m_drawingSurface.reset(true);
        }

        [DllImport("gdi32.dll")]
        private static extern bool
        BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);

        //Called from either the menu or toolbar copy buttons
        public void editCopy()
        {
            Graphics g = ViewerPanel.CreateGraphics();
            Bitmap image = new Bitmap(ViewerPanel.Width, ViewerPanel.Height);
            Graphics dest = Graphics.FromImage(image);
            IntPtr hdc = g.GetHdc();
            IntPtr hdcDest = dest.GetHdc();
            BitBlt(hdcDest, 0, 0, ViewerPanel.Width, ViewerPanel.Height, hdc, 0, 0, 0x00CC0020);
            g.ReleaseHdc(hdc);
            dest.ReleaseHdc(hdcDest);
            Clipboard.SetDataObject(image, true);
        }
        #endregion

        #region Panel_ZoomPan_Functions
        public bool zoomIn(int zoomPercentage, bool addToViewStack)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.zoomIn(zoomPercentage, addToViewStack);
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
            return invalidate;
        }

        public bool zoomOut(int zoomPercentage, bool addToViewStack)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.zoomOut(zoomPercentage, addToViewStack);
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
            return invalidate;
        }

        public bool resetToFullExtent(bool addToViewStack)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.resetToFullExtent(addToViewStack);
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
            return invalidate;
        }

        public bool setCurrentMode(InteractionModeEnum mode)
        {
            if (m_modeManager == null)
            {
                return false;
            }

            bool invalidate = false;
            for (int i = 0; i < m_contextMenuTools.Length; ++i)
            {
                if (mode == m_contextMenuTools[i].mode)
                {
                    invalidate = m_modeManager.setCurrentMode((int)mode);
                    break;
                }
            }

            if (invalidate)
            {
                m_currentInteractionMode = mode;
                updateContextMenuToolChecked(mode, m_contexMenuuu, m_contextMenuTools);
                ViewerPanel.Invalidate();
            }

            return invalidate;
        }

        public InteractionModeEnum getCurrentMode()
        {
            if (m_modeManager == null)
            {
                return InteractionModeEnum.InValid;
            }

            TSLNInteractionMode cmode = null;
            long id = m_modeManager.getCurrentMode(out cmode);
            foreach (var tool in m_contextMenuTools)
            {
                if(id == (int)tool.mode)
                {
                    return tool.mode;
                }
            }

            return InteractionModeEnum.InValid;
        }
        #endregion

        #region Panel_StackViews_Functions
        public bool saveView(int index)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.savedViewSetToCurrent(index);
            return invalidate;
        }

        public bool viewStackReset()
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.savedViewReset();
            return invalidate;
        }

        public bool gotoSavedView(int index)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.savedViewGoto(index);
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
            return invalidate;
        }

        public bool viewStackGotoPrevious()
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.viewStackGotoPrevious();
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
            return invalidate;
        }

        public bool viewStackGotoNext()
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.viewStackGotoNext();
            if (invalidate)
            {
                ViewerPanel.Invalidate();
            }
            return invalidate;
        }
        #endregion
    }
}
