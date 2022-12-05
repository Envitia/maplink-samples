/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "datasetinformationpage.h"
#include "ui_datasetinformationpage.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>

#include "directimportwizard.h"
#include "wizardpageenum.h"
#include "layermanager.h"

#include "MapLink.h"
#include "MapLinkDirectImport.h"

#include <sstream>

DataSetInformationPage::DataSetInformationPage(QWidget *parent)
  : QWizardPage( parent )
  , m_existingDataSet( NULL )
  , m_selectedDataSetIndex( 0 )
  , m_selectedDataSet ( NULL )
{
  setupUi(this);

  // Required to be able to change the background colour
  dataSetInfoRasterMaskGenerationColourValueLabel->setAutoFillBackground(true);

  connect( dataSetSelectionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(dataSetSelectionSpinBoxValueChanged(int)));
  connect( dataSetLoadCheckBox, SIGNAL(toggled(bool)), this, SLOT(dataSetLoadCheckBoxToggled(bool)));
  connect( dataSetAutoScaleBandCheckBox, SIGNAL(toggled(bool)), this, SLOT(dataSetAutoScaleBandCheckBoxToggled(bool)));
  connect( dataSetScaleBandComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT( dataSetScaleBandComboBoxCurrentIndexChanged(int)));
  connect( dataSetSLDPushButton, SIGNAL(pressed()), this, SLOT(dataSetSLDPushButtonPressed()));
  connect( dataSetInfoRasterBrightnessValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(dataSetInfoRasterBrightnessValueSpinBoxChanged(int)));
  connect( dataSetInfoRasterContrastValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(dataSetInfoRasterContrastValueSpinBoxChanged(int)));
  connect( dataSetInfoRasterGammaValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(dataSetInfoRasterGammaValueSpinBoxChanged(int)));
  connect( dataSetInfoRasterMaskGenerationModeValueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataSetInfoRasterMaskGenerationModeComboBoxCurrentIndexChanged(int)));
  connect( dataSetInfoRasterMaskGenerationColourValuePushButton, SIGNAL(pressed()), this, SLOT(dataSetInfoRasterMaskGenerationColourValuePushButtonPressed()));
  connect( dataSetInfoRasterMaskGenerationFileNameValuePushButton, SIGNAL(pressed()), this, SLOT(dataSetInfoRasterMaskGenerationFileNameValuePushButtonPressed()));
  connect( dataSetInfoRasterPyramidGenerationModeValueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataSetInfoRasterPyramidGenerationModeValueComboBoxCurrentIndexChanged(int)));
  connect( dataSetInfoRasterPyramidInterpolationValueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataSetInfoRasterPyramidInterpolationValueComboBoxCurrentIndexChanged(int)));
  connect( dataSetInfoRasterMaxTileSizeValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(dataSetInfoRasterMaxTileSizeValueSpinBoxValueChanged(int)));
  connect( dataSetInfoRasterPyramidGenerationMaxLevelsValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(dataSetInfoRasterPyramidGenerationMaxLevelsValueSpinBoxValueChanged(int)));
  connect( dataSetInfoRasterNormaliseValueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataSetInfoRasterNormaliseValueComboBoxCurrentIndexChanged(int)));
  connect( dataSetInfoRasterMinMaxChannelSelectComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataSetInfoRasterMinMaxChannelSelectComboBoxCurrentIndexChanged(int)));
  connect( dataSetInfoRasterMinValueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(dataSetInfoRasterMinValueSpinBoxValueChanged(double)));
  connect( dataSetInfoRasterMaxValueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(dataSetInfoRasterMaxValueSpinBoxValueChanged(double)));
}

void DataSetInformationPage::editExistingDataSet( TSLDirectImportDataSet* dataSet )
{
  m_existingDataSet = dataSet;
  // Disable the settings that can't be changed once the dataset is loaded
  dataSetLoadCheckBox->setEnabled(false);
  dataSetAutoScaleBandCheckBox->setEnabled(false);
}

void DataSetInformationPage::initializePage()
{
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));
  const TSLvector<TSLDirectImportDataSet*>* dataSets = wiz->dataSets();
  if( !dataSets )
  {
    return;
  }

  unsigned int numDataSets(0);
  if( m_existingDataSet )
  {
    numDataSets = 1;
  }
  else
  {
    numDataSets = dataSets->size();
  }

  TSLDirectImportDataLayer* layer( wiz->directImportLayer() );

  dataSetSelectionSpinBox->setValue( 1 );
  dataSetSelectionSpinBox->setMinimum( 1 );
  dataSetSelectionSpinBox->setMaximum( numDataSets );

  QString ofNStr("of ");
  ofNStr += QString::number( numDataSets );
  dataSetSelectionOfNLabel->setText( ofNStr );

  //! Populate the list of scale bands
  dataSetScaleBandComboBox->clear();
  for( unsigned int i(0); i < layer->numScaleBands(); ++i )
  {
    TSLDirectImportScaleBand* band( layer->getScaleBand(i) );
    QString comboEntry( band->name() );
    comboEntry += " - 1:";
    if( band->minScale() == 0.0 )
    {
      comboEntry += "Infinity";
    }
    else
    {
      comboEntry += QString::number( (int)(1.0 / band->minScale()), 'f', 1 );
    }
    dataSetScaleBandComboBox->addItem( comboEntry );
  }

  if( m_existingDataSet )
  {
    DirectImportDataSetSettings settings;
    settings.m_autoScaleBand = true;
    settings.m_load = true;
    // This will be changed by dataSetSelectionSpinBoxValueChanged
    // to set which scale band will be used.
    settings.m_scaleBandIndex = 0;
    m_dataSetSettings.push_back( settings );
  }
  else
  {
    //! Populate m_dataSetSettings
    for( unsigned int i(0); i < dataSets->size(); ++i )
    {
      DirectImportDataSetSettings settings;
      settings.m_autoScaleBand = true;
      settings.m_load = true;
      // This will be changed by dataSetSelectionSpinBoxValueChanged
      // to set which scale band will be used.
      settings.m_scaleBandIndex = 0;
      m_dataSetSettings.push_back( settings );
    }
  }

  //! Display informatino for the first dataset
  dataSetSelectionSpinBoxValueChanged( 1 );

  emit completeChanged();
}

bool DataSetInformationPage::validatePage()
{
  // If editing an existing dataset simply request a reprocess and accept
  if( m_existingDataSet && m_existingDataSet->type() == TSLDirectImportDataSet::DataSetTypeVector )
  {
    DirectImportDataSetSettings& settings(m_dataSetSettings[0]);

    TSLFeatureClassConfig* featureConfig(NULL);
    if (settings.m_featureClassConfigFile.isEmpty())//Default feature config
    {
      DirectImportWizard* wiz(qobject_cast<DirectImportWizard*>(wizard()));
      LayerManager* layerManager(wiz->layerManager());
      featureConfig = layerManager->getDefaultFeatureConfig(m_existingDataSet);
    }
    else
    {
      featureConfig = &(settings.m_featureClassConfig);
    }

    if (featureConfig)
    {
      // Request that the feature configuration be updated
      // This will take effect when the dataset is reprocessed
      TSLDirectImportVectorSettings* vectorSettings((reinterpret_cast<TSLDirectImportDataSetVector*>(m_existingDataSet))->vectorSettings());
      if (vectorSettings)
      {
        vectorSettings->updateFeatureClassConfig(featureConfig);
      }
    }

    m_existingDataSet->reprocess();
    return true;
  }

  // This page is always valid
  // The last-minute validation is used to add datasets to the layer.
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));
  const TSLvector<TSLDirectImportDataSet*>* dataSets( wiz->dataSets() );
  if( !dataSets )
  {
    return false;
  }
  LayerManager* layerManager( wiz->layerManager() );
  TSLDirectImportDataLayer* layer( wiz->directImportLayer() );

  for( unsigned int i(0); i < dataSets->size(); ++i )
  {
    DirectImportDataSetSettings& settings( m_dataSetSettings[i] );
    if( settings.m_load == false )
    {
      continue;
    }

    TSLDirectImportDataSet* dataSet( dataSets->operator[](i) );
    bool dataSetLoaded(false);
    TSLFeatureClassConfig* featureConfig( NULL );
    if( settings.m_featureClassConfigFile.isEmpty() == false )
    {
      featureConfig = &(settings.m_featureClassConfig);
    }

    if( settings.m_autoScaleBand )
    {
      dataSetLoaded = layerManager->loadDataSetIntoDirectImportLayer( dataSet, NULL, featureConfig );
    }
    else
    {
      TSLDirectImportScaleBand* band( layer->getScaleBand(settings.m_scaleBandIndex) );
      dataSetLoaded = layerManager->loadDataSetIntoDirectImportLayer( dataSet, band, featureConfig );
    }

    // This wizard doesn't allow the user to navigate backwards, so there's not much we can
    // do in the case of a failure.
    // A failure at this point is unlikely, and should only be possible if invalid parameters
    // are passed while loading the dataset.
    if( !dataSetLoaded )
    {
      std::stringstream str;
      str << "TSLDirectImportDataLayer/ScaleBand::loadData returned false for data set " << i << ".\nError stack contents: ";
      TSLSimpleString errorMessage;
      if( TSLThreadedErrorStack::errorString( errorMessage ) )
      {
        str << errorMessage.c_str();
      }
      QMessageBox::warning(this, "A Data Set failed to load",
                           QString(str.str().c_str()));
    }
  }

  return true;
}

int DataSetInformationPage::nextId() const
{
  // This is the final page
  return -1;
}

void DataSetInformationPage::dataSetLoadCheckBoxToggled(bool checked)
{
  unsigned int i( dataSetSelectionSpinBox->value() - 1 );
  if( i >= m_dataSetSettings.size() )
  {
    return;
  }
  DirectImportDataSetSettings& settings( m_dataSetSettings[i] );
  settings.m_load = checked;
}

void DataSetInformationPage::dataSetAutoScaleBandCheckBoxToggled(bool checked)
{
  unsigned int i( dataSetSelectionSpinBox->value() - 1 );
  if( i >= m_dataSetSettings.size() )
  {
    return;
  }
  DirectImportDataSetSettings& settings( m_dataSetSettings[i] );
  settings.m_autoScaleBand = checked;

  if( checked )
  {
    settings.m_scaleBandIndex = 0;
    DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));
    const TSLvector<TSLDirectImportDataSet*>* dataSets( wiz->dataSets() );
    if( !dataSets )
    {
      return;
    }
    TSLDirectImportDataLayer* layer( wiz->directImportLayer() );
    TSLDirectImportDataSet* dataSet( dataSets->operator[](i) );

    // Iterate over the scale bands and stop once a match is found
    unsigned int bandIndexToDisplay(0);
    for( unsigned int bandIndex(0); bandIndex < layer->numScaleBands(); ++bandIndex )
    {
      TSLDirectImportScaleBand* band( layer->getScaleBand(bandIndex) );
      if( dataSet->scale() < band->minScale() )
      {
        break;
      }
      else
      {
        bandIndexToDisplay = bandIndex;
      }
    }
    dataSetScaleBandComboBox->setCurrentIndex( bandIndexToDisplay );

    dataSetScaleBandComboBox->setEnabled(false);
  }
  else
  {
    dataSetScaleBandComboBox->setEnabled(true);
  }
}

void DataSetInformationPage::dataSetScaleBandComboBoxCurrentIndexChanged(int index)
{
  unsigned int i( dataSetSelectionSpinBox->value() - 1 );
  if( i >= m_dataSetSettings.size() )
  {
    return;
  }
  DirectImportDataSetSettings& settings( m_dataSetSettings[i] );
  settings.m_scaleBandIndex = index;
}

void DataSetInformationPage::dataSetSelectionSpinBoxValueChanged(int value)
{
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));
  const TSLvector<TSLDirectImportDataSet*>* dataSets( wiz->dataSets() );
  if( !dataSets )
  {
    return;
  }
  TSLDirectImportDataLayer* layer( wiz->directImportLayer() );

  // Dialog isn't ready yet
  // This function will be called at the end of initialisation
  if( (dataSets->empty() && !m_existingDataSet) ||
      !layer ||
      m_dataSetSettings.empty() )
  {
    return;
  }

  std::stringstream str;

  unsigned int i( value - 1 );
  TSLDirectImportDataSet* dataSet( NULL );
  if( m_existingDataSet )
  {
    dataSet = m_existingDataSet;
  }
  else
  {
    if( i >= dataSets->size() )
    {
      return;
    }
    dataSet = dataSets->operator[](i);
  }

  m_selectedDataSetIndex = i;
  m_selectedDataSet = dataSet;

  DirectImportDataSetSettings& settings( m_dataSetSettings[i] );

  // Default to the common settings tab
  dataSetInfoTabWidget->setCurrentIndex( 0 );

  // Set the checkboxes and list the correct scale band in the combo box
  // If loading automatically the wizard mimics the calculation performed by the
  // direct import layer, however when loading the data set the scale band
  // will be determined by the layer itself
  dataSetLoadCheckBox->setChecked( settings.m_load ? Qt::Checked : Qt::Unchecked );
  dataSetAutoScaleBandCheckBox->setChecked( settings.m_autoScaleBand ? Qt::Checked : Qt::Unchecked );
  if( settings.m_autoScaleBand )
  {
    dataSetScaleBandComboBox->setEnabled(false);
    // Iterate over the scale bands and stop once a match is found
    unsigned int bandIndexToDisplay(0);
    for( unsigned int bandIndex(0); bandIndex < layer->numScaleBands(); ++bandIndex )
    {
      TSLDirectImportScaleBand* band( layer->getScaleBand(bandIndex) );
      if( dataSet->scale() < band->minScale() )
      {
        break;
      }
      else
      {
        bandIndexToDisplay = bandIndex;
      }
    }
    dataSetScaleBandComboBox->setCurrentIndex( bandIndexToDisplay );
  }
  else
  {
    dataSetScaleBandComboBox->setEnabled(true);
    dataSetScaleBandComboBox->setCurrentIndex( settings.m_scaleBandIndex );
  }

  dataSetInfoNameValueLabel->setText( dataSet->name() );

  double minLat, minLon, maxLat, maxLon;
  const TSLEnvelope& extent = dataSet->extentTMC();

  dataSet->coordinateSystem()->TMCToLatLong(extent.xMin(), extent.yMin(), &minLat, &minLon); 
  dataSet->coordinateSystem()->TMCToLatLong(extent.xMax(), extent.yMax(), &maxLat, &maxLon);
  str << minLat << ", " << minLon << "  " << maxLat << ", " << maxLon;
  dataSetInfoExtentValueLabel->setText( QString( str.str().c_str() ));
  str.str(std::string());

  const TSLCoordinateSystem* cs( dataSet->coordinateSystem() );
  if( cs )
  {
    const char* csName( cs->name() );
    dataSetInfoCSNameValueLabel->setText( QString( csName ? csName : "" ) );
  }
  else
  {
    dataSetInfoCSNameValueLabel->setText( QString( "No Coordinate System" ) );
  }


  if( dataSet->type() == TSLDirectImportDataSet::DataSetTypeMultiLevelRaster )
  {
    // There are multiple overview types availble for raster data sets
    // All raster data sets inherit from TSLDirectImportDataSetRaster
    const TSLDirectImportDataSetMultiLevelRaster* rasterDataSet( reinterpret_cast<const TSLDirectImportDataSetMultiLevelRaster*>( dataSet ) );
    if( rasterDataSet )
    {
      switch( rasterDataSet->overviewType() )
      {
        case TSLDirectImportDriver::OverviewTypeNone:
          dataSetInfoIsOverviewValueLabel->setText( QString( "NO" ) );
          break;
        case TSLDirectImportDriver::OverviewTypeNative:
          dataSetInfoIsOverviewValueLabel->setText( QString( "Native Overview" ) );
          break;
        case TSLDirectImportDriver::OverviewTypeGenerated:
          dataSetInfoIsOverviewValueLabel->setText( QString( "Generated Overview" ) );
          break;
        case TSLDirectImportDriver::OverviewTypeAny:
        default:
          dataSetInfoIsOverviewValueLabel->setText( QString( "Not Applicable" ) );
          break;
      }
    }    
  }
  else
  {
    dataSetInfoIsOverviewValueLabel->setText( QString( "Not Applicable" ) );
  }
  
  if( dataSet->scale() != 0.0 )
  {
    dataSetInfoScaleValueLabel->setText( QString("1 : ") + QString::number( (int)(1.0 / dataSet->scale())));
  }
  else
  {
    dataSetInfoScaleValueLabel->setText( "Unknown (0.0)" );
  }

  switch( dataSet->dataType() )
  {
    case TSLDirectImportDriver::DataTypeVector:
      dataSetInfoTypeValueLabel->setText( QString("Vector") );
      break;
    case TSLDirectImportDriver::DataTypeRaster:
      dataSetInfoTypeValueLabel->setText( QString("Raster") );
      break;
    default:
      dataSetInfoTypeValueLabel->setText( QString("Unknown") );
  }


  if( dataSet->dataType() == TSLDirectImportDriver::DataTypeRaster )
  {
    TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(dataSet))->rasterSettings() );
    if( rasterSettings )
    {
      dataSetInfoRasterSettingsTab->setEnabled(true);

      str << rasterSettings->width() << " x " << rasterSettings->height();
      dataSetInfoRasterSizeValueLabel->setText( QString(str.str().c_str()) );
      str.str(std::string());

      str << rasterSettings->numChannels();
      dataSetInfoRasterNumChannelsValueLabel->setText( QString(str.str().c_str()) );
      str.str(std::string());

      TSLDirectImportRaster::RasterChannelFormat channelFormat( rasterSettings->channelFormat() );
      switch( channelFormat )
      {
        case TSLDirectImportRaster::RasterChannelFormatUInt8: str << "Unsigned 8-bit Integer"; break;
        case TSLDirectImportRaster::RasterChannelFormatInt16: str << "Signed 16-bit Integer"; break;
        case TSLDirectImportRaster::RasterChannelFormatUInt16: str << "Unsigned 16-bit Integer"; break;
        case TSLDirectImportRaster::RasterChannelFormatInt32: str << "Signed 32-bit Integer"; break;
        case TSLDirectImportRaster::RasterChannelFormatUInt32: str << "Unsigned 32-bit Integer"; break;
        case TSLDirectImportRaster::RasterChannelFormatFloat32: str << "32-bit floating point"; break;
        case TSLDirectImportRaster::RasterChannelFormatFloat64: str << "64-bit floating point"; break;
        case TSLDirectImportRaster::RasterChannelFormatUnknown:
        default: str << "Unknown"; break;
      }
      dataSetInfoRasterChannelFormatValueLabel->setText( QString(str.str().c_str()) );
      str.str(std::string());

      dataSetInfoRasterBrightnessValueSpinBox->setValue( rasterSettings->brightness() );
      dataSetInfoRasterContrastValueSpinBox->setValue( rasterSettings->contrast() );
      dataSetInfoRasterGammaValueSpinBox->setValue( rasterSettings->gamma() );

      TSLDirectImportRasterSettings::MaskGenerationMode maskMode( rasterSettings->maskGenerationMode() );
      dataSetInfoRasterMaskGenerationModeValueComboBox->setCurrentIndex( (int)maskMode );
      setMaskGenerationModeState( maskMode );

      TSLRGB maskColour( rasterSettings->maskGenerationColour() );
      setMaskGenerationColourState( QColor( maskColour.m_r, maskColour.m_g, maskColour.m_b ) );

      const char* maskFileName( rasterSettings->maskGenerationFileName());
      dataSetInfoRasterMaskGenerationFileNameValueLineEdit->setText( maskFileName ? maskFileName : "" );

      dataSetInfoRasterPyramidGenerationModeValueComboBox->setCurrentIndex( rasterSettings->pyramidGeneration() ? 1 : 0 );
      dataSetInfoRasterPyramidInterpolationValueComboBox->setCurrentIndex((int)rasterSettings->pyramidInterpolation());

      dataSetInfoRasterMaxTileSizeValueSpinBox->setValue(rasterSettings->maxTileSize());
      dataSetInfoRasterPyramidGenerationMaxLevelsValueSpinBox->setValue( rasterSettings->pyramidGenerationMaxLevels() );

      dataSetInfoRasterNormaliseValueComboBox->setCurrentIndex( (int)rasterSettings->normalisationMode() );

      dataSetInfoRasterMinMaxChannelSelectComboBox->clear();
      for( unsigned int channel(0); channel < rasterSettings->numChannels(); ++channel )
      {
        str << channel << ": ";
        switch (rasterSettings->channelDefinition(channel))
        {
        case TSLDirectImportRaster::RasterChannelDefinitionPaletted:
          str << "Paletted"; 
          break;
        case TSLDirectImportRaster::RasterChannelDefinitionGreyScale: 
          str << "GreyScale"; 
          break;
        case TSLDirectImportRaster::RasterChannelDefinitionRed:
          str << "Red"; 
          break;
        case TSLDirectImportRaster::RasterChannelDefinitionGreen:
          str << "Green"; 
          break;
        case TSLDirectImportRaster::RasterChannelDefinitionBlue:
          str << "Blue"; 
          break;
        case TSLDirectImportRaster::RasterChannelDefinitionAlpha:
          str << "Alpha"; 
          break;
        case TSLDirectImportRaster::RasterChannelDefinitionUnknown:
        default: 
          str << "Unknown"; 
          break;
        }
        dataSetInfoRasterMinMaxChannelSelectComboBox->addItem( QString(str.str().c_str()) );
        str.str(std::string());
      }

      if( rasterSettings->rasterType() != TSLDirectImportRaster::RasterTypePaletted )
      {
        dataSetInfoRasterMinValueSpinBox->setEnabled(true);
        dataSetInfoRasterMaxValueSpinBox->setEnabled(true);
      }
      else
      {
        dataSetInfoRasterMinValueSpinBox->setEnabled(false);
        dataSetInfoRasterMaxValueSpinBox->setEnabled(false);
      }
    }
  }
  else
  {
    dataSetInfoRasterSettingsTab->setEnabled(false);
    dataSetInfoTabWidget->setCurrentIndex(0);
  }

  if( dataSet->dataType() == TSLDirectImportDriver::DataTypeVector )
  {
    dataSetSLDPushButton->setEnabled(true);
  }
  else
  {
    dataSetSLDPushButton->setEnabled(false);
  }

  if( settings.m_featureClassConfigFile.isEmpty() )
  {
    dataSetSLDLineEdit->setText( QString("Using Default Feature Configuration"));
  }
  else
  {
    dataSetSLDLineEdit->setText( settings.m_featureClassConfigFile );
  }

  TSLDirectImportDriver* driver( dataSet->driver() );
  if( driver )
  {
    const char* name( driver->name() );
    const char* desc( driver->description() );
    dataSetInfoDriverNameValueLabel->setText( QString( name ? name : "" ) );
    dataSetInfoDriverDescValueLabel->setText( QString( desc ? desc : "" ) );
  }

}

void DataSetInformationPage::dataSetInfoRasterBrightnessValueSpinBoxChanged(int value)
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->brightness( value );
}

void DataSetInformationPage::dataSetInfoRasterContrastValueSpinBoxChanged(int value)
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->contrast( value );
}

void DataSetInformationPage::dataSetInfoRasterGammaValueSpinBoxChanged(int value)
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->gamma( value );
}

void DataSetInformationPage::dataSetInfoRasterMaskGenerationModeComboBoxCurrentIndexChanged(int index)
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  TSLDirectImportRasterSettings::MaskGenerationMode maskMode( (TSLDirectImportRasterSettings::MaskGenerationMode)index );

  rasterSettings->maskGenerationMode( maskMode );
  setMaskGenerationModeState( maskMode );
}

void DataSetInformationPage::setMaskGenerationModeState( TSLDirectImportRasterSettings::MaskGenerationMode maskMode )
{
  switch( maskMode )
  {
    case TSLDirectImportRasterSettings::MaskGenerationReplaceAll:
    case TSLDirectImportRasterSettings::MaskGenerationReplaceAtEdges:
      dataSetInfoRasterMaskGenerationColourLabel->setEnabled(true);
      dataSetInfoRasterMaskGenerationColourValueLabel->setEnabled(true);
      dataSetInfoRasterMaskGenerationColourValuePushButton->setEnabled(true);
      dataSetInfoRasterMaskGenerationFileNameLabel->setEnabled(false);
      dataSetInfoRasterMaskGenerationFileNameValueLineEdit->setEnabled(false);
      dataSetInfoRasterMaskGenerationFileNameValuePushButton->setEnabled(false);
      break;
    case TSLDirectImportRasterSettings::MaskGenerationLoadFromFile:
      dataSetInfoRasterMaskGenerationColourLabel->setEnabled(false);
      dataSetInfoRasterMaskGenerationColourValueLabel->setEnabled(false);
      dataSetInfoRasterMaskGenerationColourValuePushButton->setEnabled(false);
      dataSetInfoRasterMaskGenerationFileNameLabel->setEnabled(true);
      dataSetInfoRasterMaskGenerationFileNameValueLineEdit->setEnabled(true);
      dataSetInfoRasterMaskGenerationFileNameValuePushButton->setEnabled(true);
      break;
    case TSLDirectImportRasterSettings::MaskGenerationNone:
    default:
      dataSetInfoRasterMaskGenerationColourLabel->setEnabled(false);
      dataSetInfoRasterMaskGenerationColourValueLabel->setEnabled(false);
      dataSetInfoRasterMaskGenerationColourValuePushButton->setEnabled(false);
      dataSetInfoRasterMaskGenerationFileNameLabel->setEnabled(false);
      dataSetInfoRasterMaskGenerationFileNameValueLineEdit->setEnabled(false);
      dataSetInfoRasterMaskGenerationFileNameValuePushButton->setEnabled(false);
      break;
  }
}

void DataSetInformationPage::setMaskGenerationColourState( QColor maskColour )
{
  dataSetInfoRasterMaskGenerationColourValueLabel->setStyleSheet(
        "QLabel { background-color :" + QVariant(maskColour).toString() + "; }" );
}

void DataSetInformationPage::dataSetSLDPushButtonPressed()
{
  unsigned int i( dataSetSelectionSpinBox->value() - 1 );
  if( i >= m_dataSetSettings.size() )
  {
    return;
  }
  DirectImportDataSetSettings& settings( m_dataSetSettings[i] );

  // Display a file dialog and allow the user to select an SLD
  // This may be loaded into a TSLFeatureClassConfig for use with the Direct Import SDK
#ifdef _MSC_VER
  QString qFileName = QFileDialog::getOpenFileName( this, tr( "Load SLD for vector data set" ), QString(),
    tr( "Styled Layer Descriptor ( *.sld );;All Files ( * );" ) );
#else
  QString qFileName = QFileDialog::getOpenFileName( this, tr( "Load SLD for vector data set" ), QString(),
    tr( "Styled Layer Descriptor ( *.sld );;All Files ( * );" ), 0, QFileDialog::DontUseNativeDialog );
#endif

  if( qFileName.isEmpty() )
  {
    return;
  }

  if( settings.m_featureClassConfig.load( qFileName.toUtf8() ) == false )
  {
    QMessageBox::warning(this, "Failed to load SLD",
                         "Failed to load the specified SLD.");
    return;
  }

  settings.m_featureClassConfigFile = qFileName;

  dataSetSLDLineEdit->setText( qFileName );
}

void DataSetInformationPage::dataSetInfoRasterMaskGenerationColourValuePushButtonPressed()
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  TSLRGB currentColour( rasterSettings->maskGenerationColour() );

  QColor colour = QColorDialog::getColor( QColor(currentColour.m_r, currentColour.m_g, currentColour.m_b), this );
  setMaskGenerationColourState( colour );

  rasterSettings->maskGenerationColour( TSLRGB( colour.red(), colour.green(), colour.blue() ));
}

void DataSetInformationPage::dataSetInfoRasterMaskGenerationFileNameValuePushButtonPressed()
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );

  // Display a file dialog and allow the user to select an SLD
  // This may be loaded into a TSLFeatureClassConfig for use with the Direct Import SDK
#ifdef _MSC_VER
  QString qFileName = QFileDialog::getOpenFileName( this, tr( "Load SLD for vector data set" ), QString(),
    tr( "Mask Image ( *.jpg *.jpeg *.png *.tif *.bmp );;All Files ( * );" ) );
#else
  QString qFileName = QFileDialog::getOpenFileName( this, tr( "Load SLD for vector data set" ), QString(),
    tr( "Mask Image ( *.jpg *.jpeg *.png *.tif *.bmp );;All Files ( * );" ), 0, QFileDialog::DontUseNativeDialog );
#endif

  if( qFileName.isEmpty() )
  {
    return;
  }

  rasterSettings->maskGenerationFileName( qFileName.toUtf8() );
  dataSetInfoRasterMaskGenerationFileNameValueLineEdit->setText( qFileName );
}

void DataSetInformationPage::dataSetInfoRasterPyramidGenerationModeValueComboBoxCurrentIndexChanged( int index )
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  // 0 - None
  // 1 - Pyramids
  rasterSettings->pyramidGeneration( (bool)index );
}

void DataSetInformationPage::dataSetInfoRasterPyramidInterpolationValueComboBoxCurrentIndexChanged( int index )
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->pyramidInterpolation( (TSLRasterInterpolation)index );
}

void DataSetInformationPage::dataSetInfoRasterMaxTileSizeValueSpinBoxValueChanged( int value )
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->maxTileSize( value );
}

void DataSetInformationPage::dataSetInfoRasterPyramidGenerationMaxLevelsValueSpinBoxValueChanged( int value )
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->pyramidGenerationMaxLevels( value );
}

void DataSetInformationPage::dataSetInfoRasterNormaliseValueComboBoxCurrentIndexChanged( int index )
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  rasterSettings->normalisationMode( (TSLDirectImportRasterSettings::NormalisationMode)index );
}

void DataSetInformationPage::dataSetInfoRasterMinMaxChannelSelectComboBoxCurrentIndexChanged( int index )
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  double minimum(0.0);
  double maximum(0.0);
  if( !rasterSettings->getChannelMinMax( index, minimum, maximum ) )
  {
    minimum = 0.0;
    maximum = 0.0;
  }

  dataSetInfoRasterMinValueSpinBox->setValue( minimum );
  dataSetInfoRasterMaxValueSpinBox->setValue( maximum );
}

void DataSetInformationPage::dataSetInfoRasterMinValueSpinBoxValueChanged(double value)
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  double minimum(0.0);
  double maximum(0.0);
  if( !rasterSettings->getChannelMinMax( dataSetInfoRasterMinMaxChannelSelectComboBox->currentIndex(), minimum, maximum ) )
  {
    minimum = 0.0;
    maximum = 0.0;
  }
  minimum = value;
  if( !rasterSettings->setChannelMinMax( dataSetInfoRasterMinMaxChannelSelectComboBox->currentIndex(), minimum, maximum ) &&
      maximum < minimum )
  {
    maximum = minimum;
    dataSetInfoRasterMaxValueSpinBox->setValue(maximum);
    // valueChanged will set min/max in raster settings
  }
}

void DataSetInformationPage::dataSetInfoRasterMaxValueSpinBoxValueChanged(double value)
{
  if( !m_selectedDataSet || m_selectedDataSet->dataType() != TSLDirectImportDriver::DataTypeRaster )
  {
    return;
  }

  TSLDirectImportRasterSettings* rasterSettings( (reinterpret_cast<TSLDirectImportDataSetRaster*>(m_selectedDataSet))->rasterSettings() );
  double minimum(0.0);
  double maximum(0.0);
  if( !rasterSettings->getChannelMinMax( dataSetInfoRasterMinMaxChannelSelectComboBox->currentIndex(), minimum, maximum ) )
  {
    minimum = 0.0;
    maximum = 0.0;
  }
  maximum = value;
  if( !rasterSettings->setChannelMinMax( dataSetInfoRasterMinMaxChannelSelectComboBox->currentIndex(), minimum, maximum ) &&
      maximum < minimum )
  {
    minimum = maximum;
    dataSetInfoRasterMinValueSpinBox->setValue(minimum);
  }
}

