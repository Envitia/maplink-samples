using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using System.Threading;
using Envitia.MapLink;
using Envitia.MapLink.DirectImport;

namespace CSharpDirectImportSample
{
  // An example implementation of TSLNDirectImportDataLayerAnalysisCallabcks
  //
  // This implementation simply takes the results of an analysis process and appends
  // the results in m_fcc.
  //
  // A condition variable is provided in order to allow the foreground thread to wait for
  // the results
  class DirectImportAnalysisCallbacks : TSLNDirectImportDataLayerAnalysisCallbacks
  {
    private object m_condVar = new object();
    private bool m_condVarSignalled = false;
    public TSLNFeatureClassConfig m_fcc = new TSLNFeatureClassConfig();
    
    public void condVarReset()
    {
      lock(m_condVar)
      {
        m_condVarSignalled = false;
      }
    }
    public void condVarWait()
    {
      lock(m_condVar)
      {
        if( m_condVarSignalled )
        {
          return;
        }
        Monitor.Wait(m_condVar);
      }
    }

    private void condVarNotifyAll()
    {
      lock(m_condVar)
      {
        m_condVarSignalled = true;
        Monitor.PulseAll(m_condVar);
      }
    }

    public override void onAnalysisStarted(TSLNDirectImportDataSet dataSet)
    {
    }

    public override void onAnalysisCancelled(TSLNDirectImportDataSet dataSet)
    {
    }

    public override void onAnalysisFailed(TSLNDirectImportDataSet dataSet)
    {
    }

    public override void onAnalysisComplete(TSLNDirectImportDataSet dataSet, TSLNFeatureList featureList)
    {
      // featureList contains any features present in the analysed data
      TSLNFeatureList fc = m_fcc.featureList();
      // Append this to any existing features in the feature class config
      fc.append(featureList);

      // Iterate through each of the features and set basic styling
      // This feature configuration may also be setup to classify the features
      // and setup other styling such as feature masking.
      TSLNRenderingAttributes basicAttribs = new TSLNRenderingAttributes();
      basicAttribs.symbolStyle = 4;
      basicAttribs.symbolColour = Color.FromArgb(0x00, 0x00, 0x00).ToArgb();
      basicAttribs.symbolSizeFactor = 10.0;
      basicAttribs.symbolSizeFactorUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;

      basicAttribs.edgeStyle = 1;
      basicAttribs.edgeColour = Color.FromArgb(0x00, 0x00, 0x00).ToArgb();
      basicAttribs.edgeThickness = 1.0;
      basicAttribs.edgeThicknessUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;

      basicAttribs.exteriorEdgeStyle = 1;
      basicAttribs.exteriorEdgeColour = Color.FromArgb(0xCC, 0xCC, 0xCC).ToArgb();
      basicAttribs.exteriorEdgeThickness = 1.0;
      basicAttribs.exteriorEdgeThicknessUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;
      basicAttribs.fillStyle = 1;
      basicAttribs.fillColour = Color.FromArgb(0xCC, 0xCC, 0xCC).ToArgb();

      basicAttribs.textFont = 1;
      basicAttribs.textColour = Color.FromArgb(0x00, 0x00, 0x00).ToArgb();
      basicAttribs.textSizeFactor = 15.0;
      basicAttribs.textSizeFactorUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;

      for (uint i = 0; i < m_fcc.featureList().size(); ++i)
      {
        TSLNFeature feature = m_fcc.featureList().queryFeature(i);
        feature.setRendering(basicAttribs);
      }
      
      // And notify any waiting threads
      condVarNotifyAll();
    }

  }
}