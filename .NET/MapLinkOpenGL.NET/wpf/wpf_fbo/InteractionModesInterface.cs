using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

using windowsForms = System.Windows.Forms;

using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.OpenGLSurface;

namespace CSharpWPF_FrameBufferObject
{
    class InteractionModesInterface
    {
        #region Events
        public event refreshContainer m_refreshContainer;
        public delegate void refreshContainer();

        public event rightClickCurrentModeChangeHandler RightClickCurrentModeChange;
        public delegate void rightClickCurrentModeChangeHandler();
        #endregion

        #region Fields
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
        public TSLNInteractionModeManagerGeneric m_modeManager = null; //private TSLNInteractionModeManager m_modeManager = null;

        public bool m_controlButtonDown = false;
        public bool m_shiftButtonDown = false;

        // context menu when right click is clicked
        private windowsForms.ContextMenuStrip m_contexMenuuu = null;
        private bool m_showRightClickContextMenu;

        Grid m_mainContainer;
        #endregion
        #endregion

        #region Parameters
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

        #region Constructors and Destructors
        public InteractionModesInterface(Grid mainContainer, TSLNOpenGLSurface drawingSurface, bool showRightClickContextMenu = true)
        {
            m_mainContainer = mainContainer;
            m_showRightClickContextMenu = showRightClickContextMenu;

            m_updateReceiver = new InteractionModeRequestReceiver();
            TSLNInteractionModeRequest request = (TSLNInteractionModeRequest)m_updateReceiver;
            m_modeManager = new TSLNInteractionModeManagerGeneric(request, drawingSurface, 5, 5, 30, true);

            // Next create our modes
            InitializeInteractionModes();

            //Give them to the modeManager
            //Remember we don't own the modes anymore after this point
            //but we must keep a reference to them or c# won't deleted them properly
            foreach (var tool in m_contextMenuTools)
            {
                m_modeManager.addMode(tool.interactionMode, tool.ischecked);
            }

            m_modeManager.onSize(600, 600);

            setCurrentMode(InteractionModeEnum.TOOLS_ZOOM);

            if (m_showRightClickContextMenu)
            {
                initializeRClickContextMenu();
            }
        }
        #endregion

        #region ContextMenu
        private void initializeRClickContextMenu()
        {
            if (m_contexMenuuu != null)
                return;

            m_contexMenuuu = new windowsForms.ContextMenuStrip();
            for (int idx = 0; idx < m_contextMenuTools.Length; ++idx)
            {
                m_contexMenuuu.Items.Add(m_contextMenuTools[idx].text);
                ((windowsForms.ToolStripMenuItem)m_contexMenuuu.Items[idx]).Checked = m_contextMenuTools[idx].ischecked;
            }
            m_contexMenuuu.Items.Add("-");//< seperator
            m_contexMenuuu.Items.Add("Zoom In");
            m_contexMenuuu.Items.Add("Zoom Out");
            m_contexMenuuu.Items.Add("Reset Zoom");
        }

        /// <summary>
        /// show right click context menu
        /// </summary>
        /// <param name="e"></param>
        public void showRClickContextMenu(int screenLocationX, int screenLocationY)
        {
            if (m_contexMenuuu == null)
                return;

            System.Drawing.Point screenLocation = new System.Drawing.Point(screenLocationX, screenLocationY);
            m_contexMenuuu.Show(screenLocation);
            m_contexMenuuu.ItemClicked += new windowsForms.ToolStripItemClickedEventHandler(
                contexMenuuu_ItemClicked);
        }

        private void contexMenuuu_ItemClicked(object sender, windowsForms.ToolStripItemClickedEventArgs e)
        {
            windowsForms.ToolStripItem item = e.ClickedItem;
            // your code here
            switch (item.Text)
            {
                case "Zoom Tool":
                    if (m_currentInteractionMode != InteractionModeEnum.TOOLS_ZOOM)
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

        #region InteractionModes
        private void InitializeInteractionModes()
        {
            m_contextMenuTools = new contextMenuTool[]
            {
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_ZOOM, text = "Zoom Tool", ischecked = true},
                new contextMenuTool { mode = InteractionModeEnum.TOOLS_PAN, text = "Pan Tool", ischecked = false},
                //new contextMenuTool { mode = InteractionModeEnum.TOOLS_MAGNIFY, text = "Magnify Tool", ischecked = false},
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
                if (this.m_refreshContainer != null)
                    m_refreshContainer();
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
                if (id == (int)tool.mode)
                {
                    return tool.mode;
                }
            }

            return InteractionModeEnum.InValid;
        }

        private void updateContextMenuToolChecked(InteractionModeEnum mode, windowsForms.ContextMenuStrip contexMenuuu, contextMenuTool[] tools)
        {
            if (contexMenuuu == null)
                return;

            for (int idx = 0; idx < m_contextMenuTools.Length; ++idx)
            {
                if (m_contextMenuTools[idx].mode == mode)
                {
                    ((windowsForms.ToolStripMenuItem)contexMenuuu.Items[idx]).Checked = true;
                }
                else
                {
                    ((windowsForms.ToolStripMenuItem)contexMenuuu.Items[idx]).Checked = false;
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
        #endregion

        #region MainInteractionHandles
        public bool OnMouseDown(MouseButtonEventArgs e)
        {
            if (m_modeManager == null)
            {
                return false;
            }

            //Control src = e.Source as Control;
            //if (src == null)
            //    return false;

            bool invalidate = false;

            switch (e.ChangedButton)
            {
                case MouseButton.Left:
                    {
                        invalidate = m_modeManager.onLButtonDown((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButton.Middle:
                    {
                        invalidate = m_modeManager.onMButtonDown((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButton.Right:
                    {
                        invalidate = m_modeManager.onRButtonDown((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
            }

            if (invalidate)
            {
                m_refreshContainer();
            }
            return (invalidate);
        }

        public bool OnMouseMove(MouseEventArgs e)
        {
            if (m_modeManager == null)
            {
                return false;
            }

            //Control src = e.Source as Control;
            //if (src == null)
            //    return false;

            TSLNButtonType button = TSLNButtonType.TSLNButtonNone;
            if (e.LeftButton == MouseButtonState.Pressed)
                button = TSLNButtonType.TSLNButtonLeft;
            else if (e.MiddleButton == MouseButtonState.Pressed)
                button = TSLNButtonType.TSLNButtonCentre;
            else if (e.RightButton == MouseButtonState.Pressed)
                button = TSLNButtonType.TSLNButtonRight;

            bool invalidate = m_modeManager.onMouseMove(button, (int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);


            if (invalidate)
            {
                m_refreshContainer();
            }

            return invalidate;
        }

        public bool OnMouseUp(MouseButtonEventArgs e)
        {
            if (m_modeManager == null)
            {
                return false;
            }

            //Control src = e.Source as Control;
            //if (src == null)
            //    return false;

            bool invalidate = false;
            switch (e.ChangedButton)
            {
                case MouseButton.Left:
                    {
                        invalidate = m_modeManager.onLButtonUp((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButton.Middle:
                    {
                        invalidate = m_modeManager.onMButtonUp((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);
                        break;
                    }
                case MouseButton.Right:
                    {
                        if (showRightClickContextMenu)
                        {
                            showRClickContextMenu((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y);
                        }
                        else
                        {
                            invalidate = m_modeManager.onRButtonUp((int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y, m_shiftButtonDown, m_controlButtonDown);
                            updateRightClickModeChangeEvent();
                        }

                        break;
                    }
            }

            if (invalidate)
            {
                m_refreshContainer();
            }

            return invalidate;
        }

        public bool OnMouseWheel(MouseWheelEventArgs e)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.onMouseWheel((short)e.Delta, (int)e.GetPosition(m_mainContainer).X, (int)e.GetPosition(m_mainContainer).Y);

            if (invalidate)
            {
                m_refreshContainer();
            }

            return invalidate;
        }

        public void OnKeyDown(bool control, bool shift)
        {
            if (control == true)
            {
                m_controlButtonDown = true;
            }
            else if (shift == true)
            {
                m_shiftButtonDown = true;
            }
        }

        //On key up anywhere in the app
        public void OnKeyUp(bool control, bool shift)
        {
            if (m_controlButtonDown && control == false)
            {
                m_controlButtonDown = false;
            }
            else if (m_shiftButtonDown && shift == false)
            {
                m_shiftButtonDown = false;
            }
        }
        #endregion

        #region Methods
        public bool zoomIn(int zoomPercentage, bool addToViewStack)
        {
            if (m_modeManager == null)
            {
                return false;
            }
            bool invalidate = m_modeManager.zoomIn(zoomPercentage, addToViewStack);
            if (invalidate)
            {
                m_refreshContainer();
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
                m_refreshContainer();
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
                m_refreshContainer();
            }
            return invalidate;
        }



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
                m_refreshContainer();
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
                m_refreshContainer();
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
                m_refreshContainer();
            }
            return invalidate;
        }
        #endregion
    }
}
