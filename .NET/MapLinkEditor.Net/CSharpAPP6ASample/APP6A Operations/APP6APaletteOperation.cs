using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample.APP6A_Operations
{
  class APP6APaletteOperation : APP6AOperation
  {
    // Number from messages file
    private const int AP_APP6A_PALETTE = 300;
    private const int AE_APP6A_PALETTE = 301;
    private const int AE_APP6A_NOTHING_SELECTED = 302;

    public APP6APaletteOperation()
    {
    }

    public APP6APaletteOperation(TSLNAPP6ARequest additionalPropertiesCallback)
    {
      m_additionalPropertiesCallback = additionalPropertiesCallback;
    }

    public override int activate(object data)
    {
      if ( m_dlg == null )
      {

        TSLNErrorStack.clear() ;

        IntPtr parentHandle = (IntPtr) data ;
        
        // Set the callback to be this. Note that from MapLink 5.0 onwards
        // this will automatically update the internal properties window so that
        // properties callback messages are handled here.
        m_dlg = new TSLNAPP6APalette ( this, false, parentHandle ) ;

        if ( !m_dlg.valid )
        {
          // The create may have failed in the constructor so
          // we need to check the valid() method for this. The app
          // won't fall over, it just won't do anything.
          string msg = TSLNErrorStack.errorString() ;
          if (msg != null)
          {
            System.Windows.Forms.MessageBox.Show(msg);
          }
        }
      }

      // If you want to use the decluttering facility then you need
      // to set up the feature codes for the different symbol parts.
      // By default you can set the text on or off, graphics on or
      // off and the frame on or off. If you want to declutter individual
      // symbols parts then you must set the featureID before calling
      // getSymbolAsEntitySet. TSLNAPP6ASymbol.TSLNAPP6A_DEFAULT_TEXT, 
      // TSLNAPP6ASymbol.TSLNAPP6A_DEFAULT_GRAPHIC and 
      // TSLNAPP6ASymbol.TSLNAPP6A_DEFAULT_FRAME are provided as static
      // properties.
      TSLNStandardDataLayer layer = m_handler.DataLayer ;
      layer.addFeatureRendering(MapViewerForm.FN_TEXT, TSLNAPP6ASymbol.TSLNAPP6A_DEFAULT_TEXT);
      layer.addFeatureRendering(MapViewerForm.FN_GRAPHIC, TSLNAPP6ASymbol.TSLNAPP6A_DEFAULT_GRAPHIC);
      layer.addFeatureRendering(MapViewerForm.FN_FRAME, TSLNAPP6ASymbol.TSLNAPP6A_DEFAULT_FRAME);

      if (m_dlg != null)
      {
        m_dlg.showWindow(true);
      }

      return AP_APP6A_PALETTE ;
    }

    public override int done()
    {
      if (m_dlg != null)
      {
        m_dlg.showWindow(false);
      }

      m_handler.deactivate();

      return 0;
    }

    public override int locator(int x, int y, TSLNLocatorType type, TSLNButtonType button, TSLNModifierType modifier)
    {
      if (type != TSLNLocatorType.TSLNLocatorRelease || 
          button != TSLNButtonType.TSLNButtonLeft)
        return 0;

      if (m_dlg != null)
      {
        // Grap the symbol from the palette.
        TSLNAPP6ASymbol tmp = new TSLNAPP6ASymbol();
        if (!m_dlg.getSelectedSymbol(ref tmp))
        {
          return AE_APP6A_NOTHING_SELECTED;
        }

        // Set up the location.
        tmp.x = x;
        tmp.y = y;
        tmp.smallText = false;

        // Convert it to an entityset.
        TSLNEntitySet es = Helper.getSymbolAsEntitySet(tmp);

        if (es == null)
        {
          return AE_APP6A_PALETTE;
        }

        TSLNEntitySet dles = m_handler.EntitySet;

        if (dles == null)
        {
          return AE_APP6A_PALETTE;
        }

        if (!dles.insert(es, -1))
        {
          return AE_APP6A_PALETTE;
        }

        // Update the extent.
        m_handler.updateEntityExtent(es);
        // Redraw the extent of the entity.
        m_handler.redrawEntityExtent(es, true);

      }
      return AP_APP6A_PALETTE;
    }

    public override void requestHandler(TSLNUserOperationRequest handler)
    {
      m_handler = handler;
    }

    public override void onPaletteClose(TSLNAPP6ARequest.RequestFlags flags)
    {
      // Called when the use clicks the close cross, or the ok
      // button within the palette window. All we need to do is
      // redirect the call to the done() method.

      // Check that this is not the result of our the done()s showWindow() method
      // or we will get in a loop. 
      if (flags != TSLNAPP6ARequest.RequestFlags.APP6A_SHOW_WINDOW_CLOSE)
      {
        done();
      }
    }

    public override bool onPropertiesUpdate(TSLNAPP6ARequest.RequestFlags flags, TSLNAPP6ASymbol symbol)
    {
      // This example shows how changes in the hostility combo can be trapped here. 
      // Note that these messages will always appear with the APP6A_PROPS_UPDATING
      // flag set to true.
      if ((flags & TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_UPDATING) == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_UPDATING)
      {

        if ((flags & TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_SELCHANGED) == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_SELCHANGED)
        {
          // Check for the hostility field (see app6aincludes.h for all fields).
          if ((flags & TSLNAPP6ARequest.RequestFlags.APP6A_FLD_HOSTILITY) == TSLNAPP6ARequest.RequestFlags.APP6A_FLD_HOSTILITY)
          {
            TSLNAPP6ASymbol.HostilityEnum host = symbol.hostility;
            // Handle changes here...
          }
        }

        // You can also trap the KILL_FOCUS windows message. In the case of combos and buttons
        // you would be better to trap the selection changes as shown above. However, you
        // may not want to trap the selection changes in an edit control as this will change
        // on each key press. Therefore trap the kill focus.
        if ((flags & TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_KILLFOCUS) == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_KILLFOCUS)
        {
          // Check for the staff comments being changes.
          if ((flags & TSLNAPP6ARequest.RequestFlags.APP6A_FLD_STAFFCOMMENTS) == TSLNAPP6ARequest.RequestFlags.APP6A_FLD_STAFFCOMMENTS)
          {
            string staffComments = symbol.staffComments;
            // Handle changes here.
          }
        }

        return true;
      }

      // Note that the APP6A_PROPS_OK flag is not guarateed to be unique for all
      // the field flags, but they are guarateed not to appear with an APP6A_PROPS_UPDATING
      // flag. They also always appear as a single flag thus you can use the = operator.

      if (flags == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_OK)
      {
        // Do something

      }

      if (flags == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_APPLY)
      {
        // Do something
      }

      return true; 
    }

    private TSLNUserOperationRequest m_handler = null;
    private TSLNAPP6APalette m_dlg = null;
    private TSLNAPP6ARequest m_additionalPropertiesCallback = null;
  }
}
