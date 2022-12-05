/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "datalayerdialog.h"

DataLayerDialog::DataLayerDialog( QWidget *parent )
  : QDialog( parent )
{
  setupUi( this );
  setWindowModality( Qt::WindowModal );
  QStringList items;
  items << "KB" << "MB" << "GB";
  byteSizeBox->addItems( items );
  byteSizeBox->setCurrentIndex( 1 );

  items.clear();
  items << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256" << "512" << "1024";
  cacheSizeBox->addItems( items );
  cacheSizeBox->setCurrentIndex( 8 );

  initialiseCacheSizes();

  connect(checkBox_sharedCache, SIGNAL(toggled(bool)), this, SLOT(checkBoxSharedCacheToggled(bool)));
  connect(cacheSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(cacheSizeComboBoxCurrentIndexChanged(int)));
  connect(byteSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(byteSizeComboBoxCurrentIndexChanged(int)));
}

DataLayerDialog::~DataLayerDialog()
{

}

void DataLayerDialog::setDataLabel( QString labelText )
{
  labelDataType->setText( labelText );
}

void DataLayerDialog::setLayerNameBox( QString layerName )
{
  layerNameText->setText( layerName );
}

void DataLayerDialog::setCacheSizeBox( QString cacheSize )
{
  cacheSizeBox->setCurrentText( cacheSize );
}

void DataLayerDialog::setByteSizeBox( QString byteSize )
{
  byteSizeBox->setCurrentText( byteSize );
}

void DataLayerDialog::setUseSharedCache(bool useSharedCache)
{
  checkBox_sharedCache->setChecked(useSharedCache);
}

long DataLayerDialog::getCacheSizeBox() const
{
  return cacheSizeBox->currentText().toLong();
}

QString DataLayerDialog::getLayerNameBox() const
{
  return layerNameText->text();
}

QString DataLayerDialog::getByteSizeBox() const
{
  return byteSizeBox->currentText();
}

QDialogButtonBox* DataLayerDialog::getButtonBox() const
{
  return buttonBox;
}

bool DataLayerDialog::getUseSharedCache() const
{
  return checkBox_sharedCache->isChecked();
}

void DataLayerDialog::checkBoxSharedCacheToggled(bool checked)
{
  int idx = getUseSharedCache() ? 1 : 0;

  // Save the settings for the other cache type
  int other_idx = abs(idx - 1);
  m_cacheSizes[other_idx].m_value = cacheSizeBox->currentText();
  m_cacheSizes[other_idx].m_byteSize = getByteSizeBox();
  m_cacheSizes[other_idx].m_selected = false;

  // Restore the previous settings
  setCacheSizeBox(m_cacheSizes[idx].m_value);
  setByteSizeBox(m_cacheSizes[idx].m_byteSize);
  m_cacheSizes[idx].m_selected = true;
}

void DataLayerDialog::cacheSizeComboBoxCurrentIndexChanged(int index)
{
  int idx = getUseSharedCache() ? 1 : 0;
  m_cacheSizes[idx].m_value = cacheSizeBox->currentText();
}

void DataLayerDialog::byteSizeComboBoxCurrentIndexChanged(int index)
{
  int idx = getUseSharedCache() ? 1 : 0;
  m_cacheSizes[idx].m_byteSize = getByteSizeBox();
}

void DataLayerDialog::initialiseCacheSizes()
{
  m_cacheSizes[0].m_selected = false;
  m_cacheSizes[0].m_value = "256";
  m_cacheSizes[0].m_byteSize = "MB";
  m_cacheSizes[1].m_selected = true;
  m_cacheSizes[1].m_value = "1";
  m_cacheSizes[1].m_byteSize = "GB";

  setCacheSizeBox(m_cacheSizes[1].m_value);
  setByteSizeBox(m_cacheSizes[1].m_byteSize);
  setUseSharedCache(true);
}

void DataLayerDialog::showEvent(QShowEvent* event)
{
  // Restore the previous settings
  int idx = getUseSharedCache() ? 1 : 0;
  setCacheSizeBox(m_cacheSizes[idx].m_value);
  setByteSizeBox(m_cacheSizes[idx].m_byteSize);
  setUseSharedCache(m_cacheSizes[idx].m_selected);
}