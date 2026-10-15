using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using Envitia.MapLink;
using Envitia.MapLink.OpenGLSurface;

using windowsForms = System.Windows.Forms;

namespace CSharpWPF_FrameBufferObject
{
  class MaplinkSurfaceInterface
  {
    #region Fields
    TSLNOpenGLDisplayHelper maplingSurfaceInterface;
    public TSLNOpenGLSurface m_drawingSurface;
    String m_shortfilename;
    #endregion

    #region Constructors and Destructors
    public MaplinkSurfaceInterface()
    {
      TSLNOpenGLSurfaceCreationParameters creationOptions = new TSLNOpenGLSurfaceCreationParameters();
      creationOptions.useVSync(false);
      creationOptions.numMultisampleSamples(0);
      creationOptions.swapBuffersManually(true);
      creationOptions.createCoreProfile(false);
      creationOptions.enableDoubleBuffering(true);

      bool useTexture = false;
      maplingSurfaceInterface = new TSLNOpenGLDisplayHelper(IntPtr.Zero, false, creationOptions, false, false, useTexture, 600, 600, IntPtr.Zero);
      m_drawingSurface = maplingSurfaceInterface.drawingSurface;
      m_drawingSurface.setOption(TSLNOptionEnum.TSLNOptionDoubleBuffered, true);
#if false
      String mapPath = "\\\\qnap\\testdata\\OpenGLSurface/StaticLayerTestMaps/RenderStyles/RenderStyles.map";
      addMapDataLayer(mapPath, "staticmap");

      maplingSurfaceInterface.resizeWindow(600, 600);
      m_drawingSurface.drawDU(0, 0, 600, 600, true, true);
      String resultImgPath = "C:/Temp2/createBitmap2.tif";
      maplingSurfaceInterface.createBitmap( resultImgPath);
#endif
    }

    public void dispose()
    {
      maplingSurfaceInterface.Dispose();
    }
    #endregion

    #region Methods
    public void addMapDataLayer(String mapPath, String layerName)
    {
      TSLNMapDataLayer mapDataLayer;
      mapDataLayer = new TSLNMapDataLayer();
      m_drawingSurface.removeDataLayer(layerName);
      bool ret1 = m_drawingSurface.addDataLayer(mapDataLayer, layerName);

      m_drawingSurface.setDataLayerProps(layerName, TSLNPropertyEnum.TSLNPropertyBuffered, 1);
      m_shortfilename = System.IO.Path.GetFileName(mapPath);
      bool ret2 = mapDataLayer.loadData(mapPath);
      m_drawingSurface.backgroundColour = mapDataLayer.backgroundColour;
      bool ret3 = m_drawingSurface.reset(false);
    }

    public void readPixels(ref WriteableBitmap backbuffer, int _imgWidth, int _imgHeight)
    {
      maplingSurfaceInterface.resizeWindow(_imgWidth, _imgHeight); //maplingSurfaceInterface.resize(0, 0, _imgWidth, _imgHeight, false);
      bool ret4 = m_drawingSurface.drawDU(0, 0, _imgWidth, _imgHeight, true, true);

      int totalBytes = _imgWidth * _imgHeight * 4;

#if false
      byte[] _readBuf;
      bool ret5 = maplingSurfaceInterface.readPixels(out _readBuf);
#else
      byte[] _readBuf = new byte[totalBytes];
      for(int i=0; i< 2; ++ i)
      {
        if (maplingSurfaceInterface.readPixels(out _readBuf, totalBytes, 0))
          break;
      }
#endif

      //bool ret6 = maplingSurfaceInterface.createBitmap("C:/Temp3/Test3/tempImg3.tif", false);

      Cleanup(ref backbuffer, ref _readBuf, _imgWidth, _imgHeight);
    }

    private void Cleanup(ref WriteableBitmap backbuffer, ref byte[] _readBuf, int _imgWidth, int _imgHeight)
    {
      if (_readBuf == null)
        return;

      if (backbuffer == null || backbuffer.Width != _imgWidth || backbuffer.Height != _imgHeight)
      {
        backbuffer = new WriteableBitmap(
            _imgWidth,
            _imgHeight,
            96,
            96,
            PixelFormats.Pbgra32,
            BitmapPalettes.WebPalette);
      }

      // copy pixels upside down
      backbuffer.Lock();

      var src = new Int32Rect(0, 0, (int)backbuffer.Width, 1);
      for (int y = 0; y < (int)backbuffer.Height; y++)
      {
        src.Y = (int)backbuffer.Height - y - 1;
        backbuffer.WritePixels(src, _readBuf, backbuffer.BackBufferStride, 0, y);
      }

      backbuffer.AddDirtyRect(new Int32Rect(0, 0, (int)backbuffer.Width, (int)backbuffer.Height));
      backbuffer.Unlock();
    }

    public void printMap(int _imgWidth, int _imgHeight)

    {
      TSLNPrintItem[] printItems = new TSLNPrintItem[1];
      TSLNPrintItem printItem = new TSLNPrintItem();
      TSLNEnvelope envelope = new TSLNEnvelope();
      printItems.SetValue(printItem, 0);

      double uux1 = 0, uuy1 = 0, uux2 = 0, uuy2 = 0;
      int tmcx1 = 0, tmcy1 = 0, tmcx2 = 0, tmcy2 = 0;
      string templateFile = "";
      bool isLandscape;

      if (_imgWidth > _imgHeight)
      {
        templateFile = "landscape.tpt";
        isLandscape = true;
      }
      else
      {
        templateFile = "portrait.tpt";
        isLandscape = false;
      }

      string templateFilePath = TSLNOpenGLSurface.findFile(templateFile);

      TSLNEntitySet entitySet = TSLNPrintDocument.loadTemplate(templateFilePath, TSLNOpenGLSurface.fileLoader, null);

      if (entitySet.size == 0)
      {
        windowsForms.MessageBox.Show("MapLink could not load printer template", "Error");
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
      printItem.borderColour = System.Drawing.Color.Black;
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
        windowsForms.PrintDialog dlg = new windowsForms.PrintDialog();
        dlg.Document = pd;
        windowsForms.DialogResult result = dlg.ShowDialog();

        if (result == windowsForms.DialogResult.OK)
        {
          pd.Print();
        }
      }
      catch (Exception ex)
      {
        MessageBox.Show("An error occurred attempting to print -  " + ex.Message);
      }
    }
#endregion

  }
}
