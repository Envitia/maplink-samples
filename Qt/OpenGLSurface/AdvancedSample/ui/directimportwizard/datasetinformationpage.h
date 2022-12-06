/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DATASETINFORMATIONPAGE_H
#define DATASETINFORMATIONPAGE_H

#include <QWizardPage>
#include "ui_datasetinformationpage.h"

#include <vector>

#include "featureconfiguration/tslfeatureclassconfig.h"
#include "MapLinkDirectImport.h"

// Direct import wizard page containing various options for the user
// to set that are used in dataset creation.

class DataSetInformationPage : public QWizardPage, private Ui_DataSetInformationPage
{
  Q_OBJECT

public:
  //! Settings for a dataset in the current panel
  //! The data set information is queried directly from
  //! the TSLDirectImortDataSet
  struct DirectImportDataSetSettings
  {
    //! Whether or not the dataset should be loaded
    bool m_load;
    //! true if the scale band should be selected automatically
    //! when loading
    bool m_autoScaleBand;
    //! If m_autoScaleBand == false, the index
    //! of the scale band to use
    unsigned int m_scaleBandIndex;
    //! The feature configuration to use
    //! Loaded from an SLD
    QString m_featureClassConfigFile;
    TSLFeatureClassConfig m_featureClassConfig;
  };

  DataSetInformationPage(QWidget *parent = 0);

  //! Set if an existing dataset is being edited via the LayerTreeView
  void editExistingDataSet( TSLDirectImportDataSet* dataSet );

  virtual void initializePage();
  virtual int nextId() const;
  virtual bool validatePage();

private slots:
  void dataSetSelectionSpinBoxValueChanged(int value);
  void dataSetLoadCheckBoxToggled(bool checked);
  void dataSetAutoScaleBandCheckBoxToggled(bool checked);
  void dataSetScaleBandComboBoxCurrentIndexChanged(int index);
  void dataSetSLDPushButtonPressed();
  void dataSetInfoRasterBrightnessValueSpinBoxChanged(int value);
  void dataSetInfoRasterContrastValueSpinBoxChanged(int value);
  void dataSetInfoRasterGammaValueSpinBoxChanged(int value);
  void dataSetInfoRasterMaskGenerationModeComboBoxCurrentIndexChanged(int index);
  void dataSetInfoRasterMaskGenerationColourValuePushButtonPressed();
  void dataSetInfoRasterMaskGenerationFileNameValuePushButtonPressed();
  void dataSetInfoRasterPyramidGenerationModeValueComboBoxCurrentIndexChanged(int);
  void dataSetInfoRasterPyramidInterpolationValueComboBoxCurrentIndexChanged(int);
  void dataSetInfoRasterMaxTileSizeValueSpinBoxValueChanged(int);
  void dataSetInfoRasterPyramidGenerationMaxLevelsValueSpinBoxValueChanged(int);
  void dataSetInfoRasterNormaliseValueComboBoxCurrentIndexChanged(int);
  void dataSetInfoRasterMinMaxChannelSelectComboBoxCurrentIndexChanged(int);
  void dataSetInfoRasterMinValueSpinBoxValueChanged(double value);
  void dataSetInfoRasterMaxValueSpinBoxValueChanged(double value);

private:
  //! Enable/disable the relevant properties depending on the mask generation mode
  void setMaskGenerationModeState( TSLDirectImportRasterSettings::MaskGenerationMode maskMode );
  void setMaskGenerationColourState( QColor maskColour );

  int m_nextId;
  TSLDirectImportDataSet* m_existingDataSet;

  unsigned int m_selectedDataSetIndex;
  TSLDirectImportDataSet* m_selectedDataSet;

  std::vector< DirectImportDataSetSettings > m_dataSetSettings;
};

#endif // DATASETINFORMATIONPAGE_H
