/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "cacheinfopage.h"
#include "wizardpageenum.h"

CacheInfoPage::CacheInfoPage( QWidget *parent )
  : QWizardPage( parent )
{
  setupUi( this );

  registerField( "cacheSize", cacheSizeBox );
  registerField( "cacheBytes", cacheSizeCombo );

  registerField( "diskCachePath", diskCachePathBox );
  registerField( "diskCacheSize", diskCacheSizeBox );
  registerField( "diskCacheBytes", diskCacheSizeCombo );

  diskCachePathLabel->setEnabled( false );
  diskCachePathBox->setEnabled( false );
  diskCachePathButton->setEnabled( false );

  diskCacheSizeLabel->setEnabled( false );
  diskCacheSizeBox->setEnabled( false );
  diskCacheSizeCombo->setEnabled( false );

  cacheSizeBox->setText( "256" );
  cacheSizeCombo->addItem( "KB" );
  cacheSizeCombo->addItem( "MB" );
  cacheSizeCombo->setCurrentIndex( 1 );


  diskCacheSizeCombo->addItem( "KB" );
  diskCacheSizeCombo->addItem( "MB" );
  diskCacheSizeCombo->setCurrentIndex( 1 );

}

int CacheInfoPage::nextId() const
{
  return ScaleBounds;
}

unsigned int CacheInfoPage::cacheSizeInKB()
{
  unsigned int cacheNum( cacheSizeBox->text().toUInt() );
  unsigned int cacheComboIndex( cacheSizeCombo->currentIndex() );
  if( cacheComboIndex == 1 )
  {
    cacheNum *= 1024;
  }
  return cacheNum;
}