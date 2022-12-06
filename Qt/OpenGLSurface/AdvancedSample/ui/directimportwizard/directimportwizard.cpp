/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "directimportwizard.h"

#include "layermanager.h"
#include "MapLinkDirectImport.h"

DirectImportWizard::DirectImportWizard( LayerManager* layerManager, TSLDirectImportDataLayerCallbacks* layerCallbacks, const TSLCoordinateSystem* mapCoordinateSystem,
                                        const std::string& dataPath, QWidget* parent)
  : QWizard( parent )
  , m_layerManager( layerManager )
  , m_layer( NULL )
  , m_layerCallbacks( layerCallbacks )
  , m_mapCoordinateSystem( mapCoordinateSystem )
  , m_dataPath( dataPath )
  , m_datasets( NULL )
{
  setupUi( this );
  setWindowModality( Qt::WindowModal );

  m_layer = m_layerManager->directImportLayer();
  if( m_layer == NULL )
  {
    setStartId( DirectImportWizardNewLayerPage );
  }
  else
  {
    setStartId( DirectImportWizardDataSetCreationPage );
  }

  // Disable the 'back' button for all pages
  QList<QWizard::WizardButton> layout;
  layout << QWizard::Stretch << QWizard::NextButton << QWizard::FinishButton << QWizard::CancelButton;
  setButtonLayout(layout);
}

//! Called when editing dataset settings through the LayerTreeView
DirectImportWizard::DirectImportWizard( LayerManager* layerManager, TSLDirectImportDataLayerCallbacks* layerCallbacks, const TSLCoordinateSystem* mapCoordinateSystem,
                                        TSLDirectImportDataSet* dataSet, QWidget* parent)
  : QWizard( parent )
  , m_layerManager( layerManager )
  , m_layer( NULL )
  , m_layerCallbacks( layerCallbacks )
  , m_mapCoordinateSystem( mapCoordinateSystem )
  , m_dataPath( dataSet ? dataSet->dataPath() : "" )
  , m_datasets( NULL )
{
  setupUi( this );
  setWindowModality( Qt::WindowModal );

  m_layer = m_layerManager->directImportLayer();

  m_datasets = new TSLvector<TSLDirectImportDataSet*>();
  m_datasets->push_back(dataSet);

  // Disable the 'back' button for all pages
  QList<QWizard::WizardButton> layout;
  layout << QWizard::Stretch << QWizard::NextButton << QWizard::FinishButton << QWizard::CancelButton;
  setButtonLayout(layout);

  setStartId( DirectImportWizardDataSetInformationPage );
  dataSetInformationPage->editExistingDataSet( dataSet );
}

DirectImportWizard::~DirectImportWizard()
{
  if( m_datasets )
  {
    TSLDirectImportDataLayer::destroyDataSets( m_datasets );
  }
}

void DirectImportWizard::createDataSets( TSLDirectImportDriver::OverviewType overviewTypes )
{
  // TODO: Add signals to callback implementation, and connect to slots for dialog box creation
  //       A dialog should be displayed for onNoCoordinateSystem and onNoExtent callbacks

  if( m_datasets )
  {
    TSLDirectImportDataLayer::destroyDataSets( m_datasets );
  }

  if (m_layer)
  {
    m_datasets = m_layer->createDataSets( m_dataPath.c_str(), overviewTypes );
  }
}
