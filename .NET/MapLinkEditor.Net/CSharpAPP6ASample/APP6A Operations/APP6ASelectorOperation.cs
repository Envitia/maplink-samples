using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample.APP6A_Operations
{
  class APP6ASelectorOperation : APP6AOperation
  {
    public APP6ASelectorOperation()
    {

    }

    public override int activate(object data)
    {
      m_selectorOperationActive = true ;

      if ( m_dlg == null )
      {

        TSLNErrorStack.clear() ;

        IntPtr parentHandle = (IntPtr) data ;
        m_dlg = new TSLNAPP6ASelector ( this, false, parentHandle ) ;
        if ( !m_dlg.valid )
        {
          // The create may have failed in the constructor so
          // we need to check the valid property for this. The app
          // won't fall over, it just won't do anything.
          string msg = TSLNErrorStack.errorString() ;
          if (msg != null)
          {
            MessageBox.Show(msg);
          }
        }
      }

      if (m_dlg != null)
      {
        m_dlg.showWindow(true);
      }
      return 1 ;
    }

    public override int done()
    {
      if (m_dlg != null)
      {
        m_dlg.showWindow(false);
      }

      m_handler.deactivate();

      return 1;
    }

    public override void deactivate()
    {
      m_selectorOperationActive = false;
    }

    public override void requestHandler(TSLNUserOperationRequest handler)
    {
      m_handler = handler;
    }

    public override void onSelectorClose(TSLNAPP6ARequest.RequestFlags flags)
    {
      // Only do anything if this is the current operation,
      // since these callbacks can be triggered from the palette
      if (!m_selectorOperationActive)
        return;

      // Called when the use clicks the close cross, or the ok
      // button within the selector window. All we need to do is
      // redirect the call to the done() method.

      // Check that this is not the result of our the done()s showWindow() method
      // or we will get in a loop. 
      if (flags != TSLNAPP6ARequest.RequestFlags.APP6A_SHOW_WINDOW_CLOSE)
      {
        done();
      }
    }

    public override bool onSelectorUpdate(TSLNAPP6ARequest.RequestFlags flags, TSLNAPP6ASymbol symbol)
    {
      // Only do anything if this is the current operation,
      // since these callbacks can be triggered from the palette
      if ( !m_selectorOperationActive )
        return true ;
      
      // This method demostrates how the symbol selector updates and 
      // also how to write symbols to a stream. We will open a stream
      // and save each symbol in turn to it.

      if ( flags == TSLNAPP6ARequest.RequestFlags.APP6A_FIRST || 
           flags == TSLNAPP6ARequest.RequestFlags.APP6A_FIRST_AND_LAST )
      {
        SaveFileDialog dlg = new SaveFileDialog();
        dlg.Filter =  "APP6A Palette Files (*.app6a)|*.app6a";
        dlg.FilterIndex = 0;
        dlg.RestoreDirectory = true;
        dlg.DefaultExt = "app6a";

        if ( dlg.ShowDialog() == DialogResult.OK )
        {
          m_stream = new FileStream(dlg.FileName, FileMode.Create, FileAccess.Write);
        }
      }

      if (m_stream != null)
      {
        // Save it.
        symbol.save(ref m_stream);

        // And close, if this is the last symbol.
        if (flags == TSLNAPP6ARequest.RequestFlags.APP6A_LAST ||
            flags == TSLNAPP6ARequest.RequestFlags.APP6A_FIRST_AND_LAST)
        {
          m_stream.Close();
        }
      }

      return true ;
    }

    private TSLNUserOperationRequest m_handler = null;
    private TSLNAPP6ASelector m_dlg = null;
    private bool m_selectorOperationActive = false;
    private FileStream m_stream = null;
  }
}
