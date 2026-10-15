#pragma once

class TSLFeatureClassConfig;
class TSLDirectImportDataSet;

class  AnalysisCallbacks : public TSLDirectImportDataLayerAnalysisCallbacks
{
public:
  AnalysisCallbacks() : m_fcc(0)
  {
    m_event = new CEvent();
  }

  ~AnalysisCallbacks()
  {
    delete m_event;
  }

  void onAnalysisStarted(const TSLDirectImportDataSet * dataSet);
  void onAnalysisFailed(const TSLDirectImportDataSet * dataSet);
  void onAnalysisCancelled(const TSLDirectImportDataSet * dataSet);
  void onAnalysisComplete(const TSLDirectImportDataSet * dataSet, const TSLFeatureList * featureList);

  void setFCC( TSLFeatureClassConfig * fcc )
  {
    m_fcc = fcc;
  }
  CEvent *m_event;

private:
  TSLFeatureClassConfig * m_fcc;
};
