#include "stdafx.h"
#include "AnalysisCallbacks.h"

void AnalysisCallbacks::onAnalysisStarted(const TSLDirectImportDataSet * dataSet)
{
}

void AnalysisCallbacks::onAnalysisFailed(const TSLDirectImportDataSet * dataSet)
{
  m_event->SetEvent();
}

void AnalysisCallbacks::onAnalysisCancelled(const TSLDirectImportDataSet * dataSet)
{
  m_event->SetEvent();
}

void AnalysisCallbacks::onAnalysisComplete(const TSLDirectImportDataSet * dataSet, const TSLFeatureList * featureList)
{
  if (featureList)
  {
    // Successfully completed analysis, so we have the top level features in the feature list parameter.
    // These need copying into the feature class config, along with any classification and/or rendering
    // Create some basic rendering and apply it to the top level features, then trigger the return
    TSLRenderingAttributes basicAttribs;

    // Symbols
    basicAttribs.m_symbolStyle = 4;
    basicAttribs.m_symbolColour = TSLComposeRGB(0x00, 0x00, 0x00);
    basicAttribs.m_symbolSizeFactor = 10.0;
    basicAttribs.m_symbolSizeFactorUnits = TSLDimensionUnitsPixels;
    // lines
    basicAttribs.m_edgeStyle = 1;
    basicAttribs.m_edgeColour = TSLComposeRGB(0x00, 0x00, 0x00);
    basicAttribs.m_edgeThickness = 1.0;
    basicAttribs.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
    // Polygons
    basicAttribs.m_exteriorEdgeStyle = 1;
    basicAttribs.m_exteriorEdgeColour = TSLComposeRGB(0xCC, 0xCC, 0xCC);
    basicAttribs.m_exteriorEdgeThickness = 1.0;
    basicAttribs.m_exteriorEdgeThicknessUnits = TSLDimensionUnitsPixels;
    basicAttribs.m_fillStyle = 1;
    basicAttribs.m_fillColour = TSLComposeRGB(0xCC, 0xCC, 0xCC);
    // Text
    basicAttribs.m_textFont = 1;
    basicAttribs.m_textColour = TSLComposeRGB(0x00, 0x00, 0x00);
    basicAttribs.m_textSizeFactor = 15.0;
    basicAttribs.m_textSizeFactorUnits = TSLDimensionUnitsPixels;

    m_fcc->featureList().append( *featureList );
    for( size_t i(0); i < m_fcc->featureList().size(); ++i)
    {
      TSLFeature* feature( m_fcc->featureList()[i] );
      feature->setRendering(basicAttribs);
    }
  }

  m_event->SetEvent();
}
