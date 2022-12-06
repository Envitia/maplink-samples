/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QStringList>

#include "treeitem.h"
#include "MapLink.h"

#ifdef HAVE_DIRECT_IMPORT_SDK
# include "directimportscalebandtreeitem.h"
# include "MapLinkDirectImport.h"
#endif

#include <sstream>

TreeItem::TreeItem( const QList<QVariant> &data, TreeItem *parent, QString name, QString type, bool checked )
  : m_parentItem( parent )
  , m_itemData( data )
  , m_layerName( name )
  , m_layerType( type )
  , m_layerVisible( true )
  , m_checked( checked )
  , m_flags( Qt::NoItemFlags )
{
}

TreeItem::TreeItem( TreeItem* parent )
  : m_parentItem( parent )
  , m_layerVisible( true )
  , m_checked( false )
  , m_flags( Qt::NoItemFlags )
{
}

TreeItem::TreeItem( TreeItem *parent, TSLDrawingSurface* drawingSurface, unsigned int layerIndex, const QString& layerType )
  : m_parentItem( parent )
  , m_layerVisible( true )
  , m_checked( false )
  , m_flags( Qt::NoItemFlags )
{
  // Information about the datalayer itself
  // This information is available for all datalayers
  const char* layerName = NULL;

  // Root datalayer item may be dragged/dropped
  m_flags |= Qt::ItemIsDragEnabled;

  if( !drawingSurface || !drawingSurface->getDataLayerInfo( layerIndex, NULL, &layerName ) )
  {
    return;
  }
  TSLDataLayer* dataLayer( drawingSurface->getDataLayer( layerName ) );
  if( !dataLayer )
  {
    return;
  }
  m_layerName = layerName;
  m_layerType = layerType;

  // Root item for each layer contains name and the type/group the layer
  // is assigned to in the sample
  m_itemData << m_layerName;
  m_itemData << m_layerType;

  // Each datalayer property is listed as a child of this root item
  QList<QVariant> columnData;
  TSLPropertyValue propVal = 0;

  drawingSurface->getDataLayerProps( layerName, TSLPropertyVisible, &propVal );
  columnData.clear();
  columnData << "Visible";
  columnData << "";
  TreeItem* visibilityItem( new TreeItem( columnData, this, m_layerName, m_layerType, propVal ) );
  visibilityItem->flags() |= Qt::ItemIsUserCheckable;
  appendChild( visibilityItem );

  drawingSurface->getDataLayerProps( layerName, TSLPropertyBuffered, &propVal );
  if ( !propVal )
  {
    // only allow geometry streaming for unbuffered layers

    drawingSurface->getDataLayerProps( layerName, TSLPropertyGeometryStreaming, &propVal );
    columnData.clear();
    columnData << "Geometry Streaming";
    columnData << "";
    TreeItem* geometryStreamingItem( new TreeItem( columnData, this, m_layerName, m_layerType, propVal ) );
    geometryStreamingItem->flags() |= Qt::ItemIsUserCheckable;
    appendChild( geometryStreamingItem );
  }

  drawingSurface->getDataLayerProps( layerName, TSLPropertyTransparency, &propVal );
  columnData.clear();
  columnData << "Transparency";
  columnData << propVal;
  TreeItem* transparencyItem( new TreeItem( columnData, this, m_layerName, m_layerType ) );
  transparencyItem->flags() |= Qt::ItemIsEditable;
  appendChild( transparencyItem );

  TSLTMC x1, y1, x2, y2;
  std::stringstream ss;
  dataLayer->getTMCExtent( &x1, &y1, &x2, &y2 );

  columnData.clear();
  columnData << "X BottomLeft";
  ss << x1;
  columnData << ss.str().c_str();
  appendChild( new TreeItem( columnData, this, m_layerName, m_layerType ) );

  columnData.clear();
  columnData << "Y BottomLeft";
  ss.str(std::string());
  ss << y1;
  columnData << ss.str().c_str();
  appendChild( new TreeItem( columnData, this, m_layerName, m_layerType ) );

  columnData.clear();
  columnData << "X TopRight";
  ss.str(std::string());
  ss << x2;
  columnData << ss.str().c_str();
  appendChild( new TreeItem( columnData, this, m_layerName, m_layerType ) );

  columnData.clear();
  columnData << "Y TopRight";
  ss.str(std::string());
  ss << y2;
  columnData << ss.str().c_str();
  appendChild( new TreeItem( columnData, this, m_layerName, m_layerType ) );

#ifdef HAVE_DIRECT_IMPORT_SDK
  TSLDataLayerTypeEnum dlType( dataLayer->layerType() );
  if( dlType == TSLDataLayerTypeDirectImportDataLayer )
  {
    TSLDirectImportDataLayer* directImportLayer( reinterpret_cast<TSLDirectImportDataLayer*>( dataLayer ) );
    unsigned int numScaleBands( directImportLayer->numScaleBands() );
    for( unsigned int i(0); i < numScaleBands; ++i )
    {
      TSLDirectImportScaleBand* band( directImportLayer->getScaleBand(i) );
      appendChild( new DirectImportScaleBandTreeItem( this, band ) );
    }
  }
#endif
}

TreeItem::~TreeItem()
{
  m_parentItem = NULL;
  qDeleteAll( m_childItems );
}

void TreeItem::appendChild( TreeItem *item )
{
  m_childItems.append( item );
}

TreeItem *TreeItem::child( int row )
{
  return m_childItems.value( row );
}

int TreeItem::childCount() const
{
  return m_childItems.count();
}

int TreeItem::columnCount() const
{
  return m_itemData.count();
}

QVariant TreeItem::data( int column ) const
{
  return m_itemData.value( column );
}

void TreeItem::setData( int column, QVariant data )
{
  m_itemData.replace( column, data );
}

QString TreeItem::getName()
{
  return m_layerName;
}

QString TreeItem::getType()
{
  return m_layerType;
}

TreeItem *TreeItem::parentItem()
{
  return m_parentItem;
}

int TreeItem::row() const
{
  if( m_parentItem )
    return m_parentItem->m_childItems.indexOf( const_cast<TreeItem*>( this ) );

  return 0;
}

bool TreeItem::checked() const
{
  return m_checked;
}

void TreeItem::toggleChecked()
{
  m_checked = !m_checked;
}

Qt::ItemFlags& TreeItem::flags()
{
  return m_flags;
}

TreeItem::TreeItemType TreeItem::type()
{
  return TreeItem::TreeItemTypeDefault;
}
