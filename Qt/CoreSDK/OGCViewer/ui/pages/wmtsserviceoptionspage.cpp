/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "wmtsserviceoptionspage.h"
#include "services/wmts/wmtsservice.h"

#include "MapLink.h"

#include <QColorDialog>

WMTSServiceOptionsPage::WMTSServiceOptionsPage(QWidget *parent)
  : QWizardPage( parent )
  , m_service( NULL )
{
  setupUi(this);

  connect( tileRequestOrder, SIGNAL(currentIndexChanged(int)), this, SLOT(tileLoadOrderSettingChanged(int)) );
  connect( availableImageFormats, SIGNAL(activated(int)), this, SLOT(imageFormatChanged(int)) );
}

WMTSServiceOptionsPage::~WMTSServiceOptionsPage()
{
}

void WMTSServiceOptionsPage::initializePage()
{
  // This page is shown after the callback loop has finished, so we do not need to update
  // the service callback object here.

  // These are added programmatically rather than through the designer so that we can associate the enum value with the entry to
  // make it easy to identify the item the user selects
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the centre", QVariant( TSLTileLoadOrderStrategy_ClockwiseSpiral_CentreStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the bottom left", QVariant( TSLTileLoadOrderStrategy_ClockwiseSpiral_BottomLeftStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the bottom right", QVariant( TSLTileLoadOrderStrategy_ClockwiseSpiral_BottomRightStart ));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the top left", QVariant( TSLTileLoadOrderStrategy_ClockwiseSpiral_TopLeftStart));
  tileRequestOrder->addItem( "Clockwise Spiral, starting from the top right", QVariant( TSLTileLoadOrderStrategy_ClockwiseSpiral_TopRightStart ));

  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the centre", QVariant( TSLTileLoadOrderStrategy_AntiClockwiseSpiral_CentreStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the bottom left", QVariant( TSLTileLoadOrderStrategy_AntiClockwiseSpiral_BottomLeftStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the bottom right", QVariant( TSLTileLoadOrderStrategy_AntiClockwiseSpiral_BottomRightStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the top left", QVariant( TSLTileLoadOrderStrategy_AntiClockwiseSpiral_TopLeftStart ));
  tileRequestOrder->addItem( "Anti-Clockwise Spiral, starting from the top right", QVariant( TSLTileLoadOrderStrategy_AntiClockwiseSpiral_TopRightStart ));

  tileRequestOrder->addItem( "Vertical Stipes, starting from the bottom left", QVariant( TSLTileLoadOrderStrategy_VerticallyStriped_BottomLeftStart ));
  tileRequestOrder->addItem( "Vertical Stipes, starting from the bottom right", QVariant( TSLTileLoadOrderStrategy_VerticallyStriped_BottomRightStart ));
  tileRequestOrder->addItem( "Vertical Stipes, starting from the top left", QVariant( TSLTileLoadOrderStrategy_VerticallyStriped_TopLeftStart ));
  tileRequestOrder->addItem( "Vertical Stipes, starting from the top right", QVariant( TSLTileLoadOrderStrategy_VerticallyStriped_TopRightStart ));

  tileRequestOrder->addItem( "Horizontal Stipes, starting from the bottom left", QVariant( TSLTileLoadOrderStrategy_HorizontallyStriped_BottomLeftStart ));
  tileRequestOrder->addItem( "Horizontal Stipes, starting from the bottom right", QVariant( TSLTileLoadOrderStrategy_HorizontallyStriped_BottomRightStart ));
  tileRequestOrder->addItem( "Horizontal Stipes, starting from the top left", QVariant( TSLTileLoadOrderStrategy_HorizontallyStriped_TopLeftStart ));
  tileRequestOrder->addItem( "Horizontal Stipes, starting from the top right", QVariant( TSLTileLoadOrderStrategy_HorizontallyStriped_TopRightStart ));

  // Create the list of common supported image formats for the visible layers in the service
  TSLWMTSDataLayer *wmtsLayer = reinterpret_cast< TSLWMTSDataLayer* >( m_service->dataLayer() );
  TSLWMTSServiceInfo *serviceInfo = wmtsLayer->serviceInformation();
  int numLayers = serviceInfo->numLayers();
  std::set< std::string > commonFormats;

  int layerIndex = 0;
  for( ; layerIndex < numLayers; ++layerIndex )
  {
    TSLWMTSServiceLayer *layer = serviceInfo->getLayerAt(layerIndex);
    if( layer->getVisibility() )
    {
      int numFormats = layer->numImageFormats();
      for( int j = 0; j < numFormats; ++j )
      {
        commonFormats.insert( layer->getImageFormatAt( j ) );
      }

      ++layerIndex;
      break;
    }
  }

  for( ; layerIndex < numLayers; ++layerIndex )
  {
    TSLWMTSServiceLayer *layer = serviceInfo->getLayerAt(layerIndex);
    if( !layer->getVisibility() )
    {
      continue;
    }

    int numFormats = layer->numImageFormats();
    std::set< std::string > thisLayerFormats;
    for( int j = 0; j < numFormats; ++j )
    {
      thisLayerFormats.insert( layer->getImageFormatAt( j ) );
    }

    std::set< std::string > allCommonFormats;
    std::set_intersection( commonFormats.begin(), commonFormats.end(), thisLayerFormats.begin(), thisLayerFormats.end(),
                           std::inserter( allCommonFormats, allCommonFormats.begin() ) );
    swap( allCommonFormats, commonFormats );
  }

  // Populate the format selection combobox with the choice of formats
  std::set< std::string >::iterator formatIt( commonFormats.begin() );
  std::set< std::string >::iterator formatItE( commonFormats.end() );
  for( ; formatIt != formatItE; ++formatIt )
  {
    availableImageFormats->addItem( QString::fromUtf8( formatIt->c_str() ) );
  }

  // Default the selection to image/png if available
  int currentFormatIndex = availableImageFormats->findText( tr( "image/png" ) );
  if( currentFormatIndex >= 0 )
  {
    availableImageFormats->setCurrentIndex( currentFormatIndex );
  }
  else
  {
    availableImageFormats->setCurrentIndex( 0 );
  }
}

int WMTSServiceOptionsPage::nextId() const
{
  // This page is always the last page
  return -1;
}

void WMTSServiceOptionsPage::tileLoadOrderSettingChanged( int newIndex )
{
  TSLWMTSDataLayer *wmtsLayer = reinterpret_cast< TSLWMTSDataLayer* >( m_service->dataLayer() );
  wmtsLayer->tileLoadOrder( (TSLTileLoadOrderStrategyEnum)tileRequestOrder->itemData( newIndex ).toInt() );
}


void WMTSServiceOptionsPage::imageFormatChanged( int newIndex )
{
  QByteArray selectedFormat( availableImageFormats->itemText( newIndex ).toUtf8() );

  // Set the new image request format on all layers from the service
  TSLWMTSDataLayer *wmtsLayer = reinterpret_cast< TSLWMTSDataLayer* >( m_service->dataLayer() );
  TSLWMTSServiceInfo *serviceInfo = wmtsLayer->serviceInformation();
  int numLayers = serviceInfo->numLayers();
  for( int layerIndex = 0; layerIndex < numLayers; ++layerIndex )
  {
    serviceInfo->getLayerAt( layerIndex )->setImageFormat( selectedFormat.constData() );
  }
}
