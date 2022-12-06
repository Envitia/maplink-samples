/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "newlayerpage.h"
#include "directimportwizard.h"
#include "layermanager.h"

#include <QMessageBox>
#include <QFileDialog>

#include "MapLink.h"
#include "MapLinkDirectImport.h"

NewLayerPage::NewLayerPage( QWidget *parent )
  : QWizardPage( parent )
{
  setupUi( this );

  connect(layerNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(layerNameChanged(QString)));
  connect(csysUseMapCheckBox, SIGNAL(toggled(bool)), this, SLOT(csysUseMapCheckBoxChanged(bool)));
  connect(csysIDLineEdit, SIGNAL(textChanged(QString)), this, SLOT(csysIDLineEditChanged(QString)));
  connect(csysTMCPerMULineEdit, SIGNAL(textChanged(QString)), this, SLOT(csysTMCPerMULineEditChanged(QString)));

  connect(memCacheSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(memCacheSizeSpinBoxChanged(int)));
  connect(memCacheSizeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(memCacheSizeComboBoxChanged(QString)));

  connect(onDiskCacheDirLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onDiskCacheDirLineEditChanged(QString)));
  connect(onDiskCacheDirBrowseButton, SIGNAL(clicked()), this, SLOT(onDiskCacheDirBrowseButtonClicked()));
  connect(onDiskCacheSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDiskCacheSizeSpinBoxChanged(int)));
  connect(onDiskCacheSizeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onDiskCacheSizeComboBoxChanged(QString)));

  connect(numProcessingThreadsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onNumProcessingThreadsSpinBoxChanged(int)));
}

void NewLayerPage::initializePage()
{
  emit completeChanged();
}

bool NewLayerPage::isComplete() const
{
  // Fast validation of required fields
  if( layerNameLineEdit->text().isEmpty() )
  {
    return false;
  }

  if( csysUseMapCheckBox->checkState() == Qt::Unchecked )
  {
    if( csysIDLineEdit->text().isEmpty() )
    {
      return false;
    }
    if( csysTMCPerMULineEdit->text().isEmpty() )
    {
      return false;
    }
  }

  return true;
}

bool NewLayerPage::validatePage()
{
  // If all settings are valid, create the TSLDirectImportDataLayer
  // and add it to the LayerManager
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()) );
  LayerManager* layerManager( wiz->layerManager() );

  QString layerName( layerNameLineEdit->text() );
  if( layerName.isEmpty() )
  {
    return false;
  }

  QString memCacheMult( memCacheSizeComboBox->currentText() );
  int memCacheSize( memCacheSizeSpinBox->value() );
  if( memCacheMult == "MB" )
  {
    memCacheSize *= 1024;
  }
  else if( memCacheMult == "GB" )
  {
    memCacheSize *= 1024 * 1024;
  }

  // The coordinate system for the layer
  // This is either taken from the 'coordinate providing layer' (The map)
  // or specified by the wizard
  TSLCoordinateSystem* csys( NULL );
  if( csysUseMapCheckBox->checkState() == Qt::Checked )
  {
    const TSLCoordinateSystem* mapCsys( wiz->mapCoordinateSystem() );
    if( mapCsys )
    {
      csys = mapCsys->clone(1);
    }
    else
    {
      QMessageBox::warning( this, "Invalid coordinate system settings",
                            "No map coordinate system is available, please specify a coordinate systed ID and TMCPerMU value.");
      return false;
    }
  }
  else
  {
    QString csysID( csysIDLineEdit->text() );
    QString csysTMCPerMUStr( csysTMCPerMULineEdit->text() );
    if( csysID.isEmpty() || csysTMCPerMUStr.isEmpty() )
    {
      return false;
    }
    double tmcPerMU( csysTMCPerMUStr.toDouble() );
    if( tmcPerMU < 1.0 || tmcPerMU > 6000000.0 )
    {
      QMessageBox::warning( this, "Invalid Coordinate System settings",
                            "The provided TMC per MU value must be between 1 and 6,000,000.");
      return false;
    }
    // Lookup coordinate system by name
    const TSLCoordinateSystem* foundCsys( TSLCoordinateSystem::findByName( csysID.toUtf8() ) );
    if( foundCsys == NULL )
    {
      // Maybe it's an EPSG id
      int epsgID( csysID.toInt() );
      foundCsys = TSLCoordinateSystem::findByEPSG( epsgID );
    }
    if( foundCsys == NULL )
    {
      QMessageBox::warning( this, "Invalid coordinate System settings",
                            "The specified coordinate system ID must be a MapLink coordinate system name, or EPSG ID");
      return false;
    }

    // Need to clone to set the TMCPerMU value
    csys = foundCsys->clone(1);
    csys->setTMCperMU( tmcPerMU );
  }

  std::string onDiskCacheDir( onDiskCacheDirLineEdit->text().toStdString() );
  bool onDiskCacheFlushOnExit( onDiskCacheFlushOnExitCheckBox->checkState() == Qt::Checked );
  QString diskCacheMult( onDiskCacheSizeComboBox->currentText() );
  int diskCacheSize( onDiskCacheSizeSpinBox->value() );
  if( diskCacheMult == "MB" )
  {
    diskCacheSize *= 1024;
  }
  else if( diskCacheMult == "GB" )
  {
    diskCacheSize *= 1024 * 1024;
  }

  int numProcessingThreads( numProcessingThreadsSpinBox->value() );

  // Settings are valid, ask the LayerManager to create the layer
  std::string layerNameStr( layerName.toUtf8() );
  TSLDirectImportDataLayer* layer(
    layerManager->createDirectImportLayer( layerNameStr, memCacheSize, numProcessingThreads, onDiskCacheDir, onDiskCacheFlushOnExit, diskCacheSize, csys, wiz->layerCallbacks() ) );
  if( layer == NULL )
  {
    QMessageBox::critical( this, "Failed to create direct import layer",
                           "Unable to create direct import datalayer, please verify all settings are valid");
  }
  else
  {
    // Store the layer for the rest of the wizard
    wiz->directImportLayer( layer );
  }

  csys->destroy();
  return layer != NULL ;
}

void NewLayerPage::layerNameChanged(const QString& layerName)
{
  emit completeChanged();
}

void NewLayerPage::csysUseMapCheckBoxChanged(bool checked)
{
  emit completeChanged();
}

void NewLayerPage::csysIDLineEditChanged(const QString& id)
{
  emit completeChanged();
}

void NewLayerPage::csysTMCPerMULineEditChanged(const QString& tmcPerMU)
{
  emit completeChanged();
}

void NewLayerPage::memCacheSizeSpinBoxChanged(int val)
{
  emit completeChanged();
}

void NewLayerPage::memCacheSizeComboBoxChanged(const QString& str)
{
  emit completeChanged();
}

void NewLayerPage::onDiskCacheFlushOnExitCheckBoxChanged(bool checked)
{
  emit completeChanged();
}

void NewLayerPage::onDiskCacheDirBrowseButtonClicked()
{
  QFileDialog qf(this);
  qf.setFileMode( QFileDialog::Directory );
  qf.setOption( QFileDialog::ShowDirsOnly );
  qf.setAcceptMode( QFileDialog::AcceptSave );
#ifndef _MSC_VER
  qf.setOption( QFileDialog::DontUseNativeDialog );
#endif
  qf.setDirectory( QDir::currentPath() );

  if( qf.exec() != QFileDialog::Rejected )
  {
    QStringList files( qf.selectedFiles() );
    if( !files.empty() )
    {
      onDiskCacheDirLineEdit->setText( files[0] );
    }
  }

  emit completeChanged();
}

void NewLayerPage::onDiskCacheDirLineEditChanged(const QString& dir)
{
  emit completeChanged();
}

void NewLayerPage::onDiskCacheSizeSpinBoxChanged(int val)
{
  emit completeChanged();
}

void NewLayerPage::onDiskCacheSizeComboBoxChanged(const QString& str)
{
  emit completeChanged();
}

void NewLayerPage::onNumProcessingThreadsSpinBoxChanged(int val)
{
  emit completeChanged();
}

