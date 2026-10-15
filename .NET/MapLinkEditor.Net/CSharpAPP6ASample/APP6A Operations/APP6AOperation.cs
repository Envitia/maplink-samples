using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample.APP6A_Operations
{
  abstract class APP6AOperation : TSLNUserOperation, TSLNAPP6ARequest
  {
    static APP6AOperation()
    {
      TSLNErrorStack.clear();

      string maplinkHome = TSLNUtilityFunctions.getMapLinkHome();
      string filename = "";
      if (maplinkHome != null)
      {
          // Setup Symbol csv configuration file to load
          filename = maplinkHome;
          filename += "/config/app6aConfig.csv";
//          filename += "/config/2525bunfilledConfig.csv";
//          filename += "/config/2525bConfig.csv" ;
      }

      // Pass Configuration File to Helper
      // Helper will default to APP6Aconfig.csv if no filename passed
      m_helper = new TSLNAPP6AHelper(filename);

      if (!m_helper.valid)
      {
        String errorMsg = TSLNErrorStack.errorString();
        if (errorMsg != null)
        {
          System.Windows.Forms.MessageBox.Show(errorMsg);
        }
      }

    }

    public virtual bool onHelp(TSLNAPP6ARequest.RequestFlags flags, bool help, bool f1)
    {
      return true;
    }

    public virtual void onPaletteClose(TSLNAPP6ARequest.RequestFlags flags)
    {
    }

    public virtual void onPaletteLoad(TSLNAPP6ARequest.RequestFlags flags, string filename, string paletteName)
    {
    }

    public virtual void onPaletteSave(TSLNAPP6ARequest.RequestFlags flags, string filename, string paletteName)
    {
    }

    public virtual bool onPaletteUpdate(TSLNAPP6ARequest.RequestFlags flags, TSLNAPP6ASymbol symbol)
    {
      return true;
    }

    public virtual void onPropertiesClose(TSLNAPP6ARequest.RequestFlags flags)
    {
    }

    public virtual void onPropertiesOpening(TSLNAPP6ARequest.RequestFlags flags)
    {
    }

    public virtual bool onPropertiesUpdate(TSLNAPP6ARequest.RequestFlags flags, TSLNAPP6ASymbol symbol)
    {
      return true;
    }

    public virtual void onSelectorClose(TSLNAPP6ARequest.RequestFlags flags)
    {
    }

    public virtual bool onSelectorUpdate(TSLNAPP6ARequest.RequestFlags flags, TSLNAPP6ASymbol symbol)
    {
        return true;
    }

    public virtual TSLNAPP6AHelper onInitialise()
    {
        return m_helper;
    }
    public TSLNAPP6AHelper Helper
    {
      get
      {
        return m_helper;
      }
    }

    private static TSLNAPP6AHelper m_helper;
  }
}
