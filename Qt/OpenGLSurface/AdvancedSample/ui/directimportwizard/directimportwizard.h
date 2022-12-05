/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DIRECTIMPORTWIZARD_H
#define DIRECTIMPORTWIZARD_H

#include "MapLinkDirectImport.h"

#include <QWizard>

#include "ui_directimportwizard.h"

#include "wizardpageenum.h"

#include <memory>

class LayerManager;
class TSLCoordinateSystem;

// This class is a wizard that takes the user through steps to load data into
// the application via direct import.

class DirectImportWizard : public QWizard, private Ui_DirectImportWizard
{
  Q_OBJECT
public:

  DirectImportWizard( LayerManager* layerManager, TSLDirectImportDataLayerCallbacks* layerCallbacks, const TSLCoordinateSystem* mapCoordinateSystem,
                      const std::string& dataPath, QWidget* parent = 0 );
  DirectImportWizard( LayerManager* layerManager, TSLDirectImportDataLayerCallbacks* layerCallbacks, const TSLCoordinateSystem* mapCoordinateSystem,
                      TSLDirectImportDataSet* dataSet, QWidget* parent);
  virtual ~DirectImportWizard();

  //! Called by DataSetCreationOptionsPage
  void createDataSets( TSLDirectImportDriver::OverviewType overviewTypes );
  const TSLvector<TSLDirectImportDataSet*>* dataSets();

  LayerManager* layerManager();
  TSLDirectImportDataLayerCallbacks* layerCallbacks();
  const TSLCoordinateSystem* mapCoordinateSystem() const;
  TSLDirectImportDataLayer* directImportLayer();
  void directImportLayer( TSLDirectImportDataLayer* layer );

  //! The path to the data being opened
  const std::string& dataPath() const;

private:
  LayerManager* m_layerManager;
  TSLDirectImportDataLayer* m_layer;
  TSLDirectImportDataLayerCallbacks* m_layerCallbacks;
  const TSLCoordinateSystem* m_mapCoordinateSystem;

  //! The data path to load
  std::string m_dataPath;
 
  TSLvector<TSLDirectImportDataSet*>* m_datasets;
};

inline LayerManager* DirectImportWizard::layerManager()
{
  return m_layerManager;
}

inline TSLDirectImportDataLayerCallbacks* DirectImportWizard::layerCallbacks()
{
  return m_layerCallbacks;
}

inline const TSLCoordinateSystem* DirectImportWizard::mapCoordinateSystem() const
{
  return m_mapCoordinateSystem;
}

inline const std::string& DirectImportWizard::dataPath() const
{
  return m_dataPath;
}

inline TSLDirectImportDataLayer* DirectImportWizard::directImportLayer()
{
  return m_layer;
}

inline void DirectImportWizard::directImportLayer( TSLDirectImportDataLayer* layer )
{
  m_layer = layer;
}

inline const TSLvector<TSLDirectImportDataSet*>* DirectImportWizard::dataSets()
{
  return m_datasets;
}

#endif // DIRECTIMPORTWIZARD_H
