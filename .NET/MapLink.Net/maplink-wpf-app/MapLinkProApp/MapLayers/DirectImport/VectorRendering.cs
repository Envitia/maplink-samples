using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MapLinkProApp.DirectImport
{
  public class VectorRendering : Envitia.MapLink.DirectImport.TSLNDirectImportDataLayerAnalysisCallbacks
  {
    private object m_condVar = new object();
    private bool m_condVarSignalled = false;
    public Envitia.MapLink.TSLNFeatureClassConfig FeatureClassConfig { get; } = new Envitia.MapLink.TSLNFeatureClassConfig();

    // The rendering attributes for vector data. Can be overriden.
    public Envitia.MapLink.TSLNRenderingAttributes RenderingAttributes { get; } = new Envitia.MapLink.TSLNRenderingAttributes();

    public VectorRendering()
    {
      // Provide some default rendering

      int defaultColour = System.Drawing.Color.FromArgb(0x66, 0x66, 0x66).ToArgb();
      
      RenderingAttributes.symbolStyle = 4;
      RenderingAttributes.symbolColour = defaultColour;
      RenderingAttributes.symbolSizeFactor = 10.0;
      RenderingAttributes.symbolSizeFactorUnits = Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels;

      RenderingAttributes.edgeStyle = 1;
      RenderingAttributes.edgeColour = defaultColour;
      RenderingAttributes.edgeThickness = 1.0;
      RenderingAttributes.edgeThicknessUnits = Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels;

      RenderingAttributes.exteriorEdgeStyle = 1;
      RenderingAttributes.exteriorEdgeColour = System.Drawing.Color.FromArgb(0xff, 0x00, 0x00).ToArgb();
      RenderingAttributes.exteriorEdgeThickness = 1.0;
      RenderingAttributes.exteriorEdgeThicknessUnits = Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels;
      RenderingAttributes.fillStyle = 0;

      RenderingAttributes.textFont = 1;
      RenderingAttributes.textColour = defaultColour;
      RenderingAttributes.textSizeFactor = 15.0;
      RenderingAttributes.textSizeFactorUnits = Envitia.MapLink.TSLNDimensionUnits.TSLNDimensionUnitsPixels;
    }

    public void condVarReset()
    {
      lock (m_condVar)
      {
        m_condVarSignalled = false;
      }
    }
    private void condVarWait()
    {
      lock (m_condVar)
      {
        if (m_condVarSignalled)
        {
          return;
        }
        System.Threading.Monitor.Wait(m_condVar);
      }
    }

    private void condVarNotifyAll()
    {
      lock (m_condVar)
      {
        m_condVarSignalled = true;
        System.Threading.Monitor.PulseAll(m_condVar);
      }
    }

    public bool applyRendering(Envitia.MapLink.DirectImport.TSLNDirectImportDataSetList dataList, Envitia.MapLink.DirectImport.TSLNDirectImportDataLayer dataLayer)
    {
      condVarReset();

      dataLayer.setAnalysisCallbacks(this);
      if (!dataLayer.analyseData(dataList))
      {
        int errorCode = 0;
        string errorString;
        Envitia.MapLink.TSLNErrorStack.lastError(out errorCode, out errorString);
        System.Windows.MessageBox.Show(errorString);
        return false;
      }

      condVarWait();

      return true;
    }

    public override void onAnalysisStarted(Envitia.MapLink.DirectImport.TSLNDirectImportDataSet dataSet)
    {
    }

    public override void onAnalysisCancelled(Envitia.MapLink.DirectImport.TSLNDirectImportDataSet dataSet)
    {
    }

    public override void onAnalysisFailed(Envitia.MapLink.DirectImport.TSLNDirectImportDataSet dataSet)
    {
    }

    public override void onAnalysisComplete(Envitia.MapLink.DirectImport.TSLNDirectImportDataSet dataSet, Envitia.MapLink.TSLNFeatureList featureList)
    {
      // featureList contains any features present in the analysed data
      Envitia.MapLink.TSLNFeatureList fc = FeatureClassConfig.featureList();
      // Append this to any existing features in the feature class config
      fc.append(featureList);

      // Iterate through each of the features and set basic styling
      // This feature configuration may also be setup to classify the features
      // and setup other styling such as feature masking.
      for (uint i = 0; i < FeatureClassConfig.featureList().size(); ++i)
      {
        FeatureClassConfig.featureList().queryFeature(i).setRendering(RenderingAttributes);
      }

      // And notify any waiting threads
      condVarNotifyAll();
    }
  }
}
