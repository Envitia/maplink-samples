using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample.APP6A_Operations
{
  class APP6APropertiesOperation : APP6AOperation
  {
    public APP6APropertiesOperation()
    {

    }

    public override int activate(object data)
    {
      m_propertiesOperationActive = true;

      if (m_dlg == null)
      {
        TSLNErrorStack.clear();

        IntPtr parentHandle = (IntPtr)data;

        m_dlg = new TSLNAPP6AProperties(this, false, parentHandle);

        if (!m_dlg.valid)
        {
          string errorMsg = TSLNErrorStack.errorString();
          if (errorMsg != null)
          {
            System.Windows.Forms.MessageBox.Show(errorMsg);
          }
        }
      }

      if (m_dlg != null)
      {
        m_dlg.showWindow(true);
      }

      m_symbols.Clear();

      if (m_dlg != null)
      {
        m_dlg.clear();
      }

      int num = m_handler.NumberOfSelectedEntities;

      for (int i = 0; i < num; ++i)
      {
        TSLNEntity ent = m_handler.getSelection(i);

        TSLNEntitySet entSet = (TSLNEntitySet)ent;

        if (entSet == null)
        {
          continue;
        }

        if (!Helper.isAPP6A(entSet))
        {
          continue;
        }

        if (m_dlg == null || !m_dlg.load(entSet, false))
          continue;

        m_symbols.Add(entSet);
      }

      return 0;
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

    public override void deactivate()
    {
      m_propertiesOperationActive = false;

      if (m_dlg != null)
      {
        m_dlg.showWindow(false);
      }
    }

    public override void requestHandler(TSLNUserOperationRequest handler)
    {
      m_handler = handler;
    }

    public override void onPropertiesClose(TSLNAPP6ARequest.RequestFlags flags)
    {
      // Only do anything if this is the current operation,
      // since these callbacks can be triggered from the palette
      if (!m_propertiesOperationActive)
        return;

      // Called when the use clicks the close cross, or the ok
      // button within the properties window. All we need to do is
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
      // Only do anything if this is the current operation,
      // since these callbacks can be triggered from the palette
      if (!m_propertiesOperationActive)
        return true;

      // Possible flags: 

      // APP6A_PROPS_UPDATING    
      // APP6A_PROPS_OK          
      // APP6A_PROPS_APPLY
      // APP6A_PROPS_UNDO
      if (flags == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_OK ||
          flags == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_APPLY ||
          flags == TSLNAPP6ARequest.RequestFlags.APP6A_PROPS_UNDO)
      {
        // This method will be called once for each symbol
        // we have in the list. We want to overwrite
        // the correct item in the m_symbols vector and 
        // put the new one in its place.

        m_handler.displayHighlights(false);

        int numInList = m_dlg != null ? m_dlg.listSize : 0;

        // This will match our selected symbols list.
        if (numInList != m_symbols.Count)
          return false;

        for (int ii = 0; ii < numInList; ++ii)
        {
          // Grap the symbol.
          TSLNAPP6ASymbol tmp = null;
          if (m_dlg == null || !m_dlg.getSelectedSymbol(ref tmp, ii))
            return false;

          // Convert it to an entityset.
          TSLNEntitySet es = Helper.getSymbolAsEntitySet(tmp);

          if (es == null)
            return false;

          // Replace the old symbol with the new.
          TSLNEntitySet ent = m_symbols[ii];
          TSLNEnvelope oldExtent = ent.envelope();
          ent.clear();

          while (es.size != 0)
          {
            ent.insert(es[0], -1);
          }

          // We need to redraw both extents because the symbol
          // may have got smaller.
          m_handler.updateEntityExtent(ent);

          TSLNEnvelope newExtent = ent.envelope();

          m_handler.redraw2Extents(newExtent, oldExtent, true);

          m_handler.displayHighlights(true);
        }
      }

      // As we have updated all the symbols we don't want
      // any more calls to this method. Stop them be returning false.
      return false;
    }

    private TSLNUserOperationRequest m_handler = null;
    private TSLNAPP6AProperties m_dlg = null;
    private List<TSLNEntitySet> m_symbols = new List<TSLNEntitySet>();
    private bool m_propertiesOperationActive = false;

  }
}
