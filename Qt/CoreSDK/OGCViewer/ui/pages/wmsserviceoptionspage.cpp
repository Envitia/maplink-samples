/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "wmsserviceoptionspage.h"
#include "services/wms/wmsservice.h"

#include "MapLink.h"

#include <QColorDialog>

WMSServiceOptionsPage::WMSServiceOptionsPage(QWidget *parent)
  : QWizardPage( parent )
  , m_service( NULL )
{
  setupUi(this);

  connect( tileLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(tileLevelSettingChanged(int)) );
  connect( tileRequestOrder, SIGNAL(currentIndexChanged(int)), this, SLOT(tileLoadOrderSettingChanged(int)) );
  connect( backgroundColourEnable, SIGNAL(stateChanged(int)), this, SLOT(setBackgroundColourButtonState(int)) );
  connect( backgroundColourDisplay, SIGNAL(clicked(bool)), this, SLOT(setBackgroundColour()) );
  connect( useTransparentImages, SIGNAL(stateChanged(int)), this, SLOT(setTransparentRequests(int)) );
  connect( availableImageFormats, SIGNAL(activated(int)), this, SLOT(imageFormatChanged(int)) );
}

WMSServiceOptionsPage::~WMSServiceOptionsPage()
{
}

void WMSServiceOptionsPage::initializePage()
{
  // This page is shown after the callback loop has finished, so we do not need to update
  // the service callback object here.

  tileLevels->addItem( "Automatic", QVariant( TSLWMSDataLayer::TileLevelStrategyDetect ) );
  tileLevels->addItem( "Use tile levels", QVariant( TSLWMSDataLayer::TileLevelStrategyUseLevels ) );
  tileLevels->addItem( "Don't use tile levels", QVariant( TSLWMSDataLayer::TileLevelStrategyUseZoomScale ) );

  // These are added programmatically rather than through the designer so that we can associate the enum value with the entry to
  // make it easy to identify the item the user selects
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the centre", QVariant( TSLWMSDataLayer::ClockwiseSpiral_CentreStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the bottom left", QVariant( TSLWMSDataLayer::ClockwiseSpiral_BottomLeftStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the bottom right", QVariant( TSLWMSDataLayer::ClockwiseSpiral_BottomRightStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the top left", QVariant( TSLWMSDataLayer::ClockwiseSpiral_TopLeftStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the top right", QVariant( TSLWMSDataLayer::ClockwiseSpiral_TopRightStart ));

  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the centre", QVariant( TSLWMSDataLayer::AntiClockwiseSpiral_CentreStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the bottom left", QVariant( TSLWMSDataLayer::AntiClockwiseSpiral_BottomLeftStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the bottom right", QVariant( TSLWMSDataLayer::AntiClockwiseSpiral_BottomRightStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the top left", QVariant( TSLWMSDataLayer::AntiClockwiseSpiral_TopLeftStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the top right", QVariant( TSLWMSDataLayer::AntiClockwiseSpiral_TopRightStart ));

  tileRequestOrder->addItem( "Vertical Stipes, starting from the bottom left", QVariant( TSLWMSDataLayer::VerticallyStriped_BottomLeftStart ));
  tileRequestOrder->addItem( "Vertical Stipes, starting from the bottom right", QVariant( TSLWMSDataLayer::VerticallyStriped_BottomRightStart ));
  tileRequestOrder->addItem( "Vertical Stipes, starting from the top left", QVariant( TSLWMSDataLayer::VerticallyStriped_TopLeftStart ));
  tileRequestOrder->addItem( "Vertical Stipes, starting from the top right", QVariant( TSLWMSDataLayer::VerticallyStriped_TopRightStart ));

  tileRequestOrder->addItem( "Horizontal Stipes, starting from the bottom left", QVariant( TSLWMSDataLayer::HorizontallyStriped_BottomLeftStart ));
  tileRequestOrder->addItem( "Horizontal Stipes, starting from the bottom right", QVariant( TSLWMSDataLayer::HorizontallyStriped_BottomRightStart ));
  tileRequestOrder->addItem( "Horizontal Stipes, starting from the top left", QVariant( TSLWMSDataLayer::HorizontallyStriped_TopLeftStart ));
  tileRequestOrder->addItem( "Horizontal Stipes, starting from the top right", QVariant( TSLWMSDataLayer::HorizontallyStriped_TopRightStart ));

  unsigned char r, g, b;
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  if( wmsLayer->getBackgroundColour( r, g, b ) )
  {
    m_backgroundColour.setRed( r );
    m_backgroundColour.setGreen( r );
    m_backgroundColour.setBlue( r );
  }

  // Ensure the default setting for transparency in the data layer matches the UI state
  wmsLayer->setTransparent( useTransparentImages->checkState() == Qt::Checked );

  // Create the list of supported image formats for this service
  QString currentImageFormat( QString::fromUtf8( wmsLayer->getCurrentImageRequestFormat() ) );

  int numImageFormats = wmsLayer->noOfImageRequestFormats();
  for( int i = 0; i < numImageFormats; ++i )
  {
    availableImageFormats->addItem( QString::fromUtf8( wmsLayer->getImageRequestFormatAt( i ) ) );
  }

  int currentFormatIndex = availableImageFormats->findText( currentImageFormat );
  availableImageFormats->setCurrentIndex( currentFormatIndex );
}

int WMSServiceOptionsPage::nextId() const
{
  // This page is always the last page
  return -1;
}

void WMSServiceOptionsPage::tileLevelSettingChanged( int newIndex )
{
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->tileLevelStrategy( (TSLWMSDataLayer::TileLevelStrategy)tileLevels->itemData( newIndex ).toInt() );
}

void WMSServiceOptionsPage::tileLoadOrderSettingChanged( int newIndex )
{
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->tileLoadOrder( (TSLWMSDataLayer::TileLoadOrderStrategy)tileRequestOrder->itemData( newIndex ).toInt() );
}

void WMSServiceOptionsPage::setBackgroundColourButtonState( int enabled )
{
  bool isEnabled = enabled == Qt::Checked;
  backgroundColourDisplay->setEnabled( isEnabled );
  if( !isEnabled )
  {
    TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
    wmsLayer->clearBackgroundColour();
  }
}

void WMSServiceOptionsPage::setBackgroundColour()
{
  // This will be deleted when the dialog is closed
  QColorDialog *colourPicker = new QColorDialog( m_backgroundColour, this );
  colourPicker->setAttribute( Qt::WA_DeleteOnClose );
  colourPicker->setModal( true );
  colourPicker->open( this, SLOT(setNewBackgroundColour(const QColor&)) );
}

void WMSServiceOptionsPage::setTransparentRequests( int enabled )
{
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->setTransparent( enabled == Qt::Checked );
}

void WMSServiceOptionsPage::setNewBackgroundColour( const QColor &newColour )
{
  // This slot is invoked by the QColorDialog created in setBackgroundColour().
  m_backgroundColour = newColour;
  QString styleString = QString( "background-color: rgb(%1, %2, %3)").arg( newColour.red() ).arg( newColour.green() ).arg( newColour.blue() );
  backgroundColourDisplay->setStyleSheet( styleString );

  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->setBackgroundColour( newColour.red(), newColour.green(), newColour.blue() );
}

void WMSServiceOptionsPage::imageFormatChanged( int newIndex )
{
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->setImageRequestFormat( newIndex );
}
