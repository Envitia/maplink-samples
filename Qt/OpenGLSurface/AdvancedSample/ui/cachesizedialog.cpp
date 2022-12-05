/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "cachesizedialog.h"

enum { KB, MB, GB };

CacheSizeDialog::CacheSizeDialog( QWidget *parent )
  : QDialog( parent )
{
  setupUi( this );
  setWindowModality( Qt::WindowModal );
  QStringList items;
  items << "KB" << "MB" << "GB";
  byteBox->addItems( items );
  byteBox->setCurrentIndex( KB );

  items.clear();
  items << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256" << "512" << "1024";
  cacheSizeBox->addItems( items );
  cacheSizeBox->setCurrentIndex( 8 );
}

CacheSizeDialog::~CacheSizeDialog()
{

}

void CacheSizeDialog::setupDialog( const std::string& name, int currentCache )
{
  m_layerName = name;

  int byteSize = 0;
  int cache = currentCache;

  while( cache >= 1024 )
  {
    cache /= 1024;
    byteSize++;
  }

  cacheSizeBox->setCurrentText( QString::number( cache ) );

  byteBox->setCurrentIndex( byteSize );

}

unsigned int CacheSizeDialog::cacheSizeInKB()
{
  unsigned int cacheSize( cacheSizeBox->currentText().toUInt() );
  const QString& bytes( byteBox->currentText() );
  if( bytes == "MB" )
  {
    cacheSize *= 1024;
  }
  else if( bytes == "GB" )
  {
    cacheSize *= 1024 * 1024;
  }
  return cacheSize;
}

QDialogButtonBox* CacheSizeDialog::getButtonBox()
{
  return buttonBox;
}

const std::string& CacheSizeDialog::getLayerName()
{
  return m_layerName;
}
