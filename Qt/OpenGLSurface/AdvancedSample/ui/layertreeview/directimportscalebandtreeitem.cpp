/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include "directimportscalebandtreeitem.h"
#include "directimportdatasettreeitem.h"

#include "MapLinkDirectImport.h"
#include <sstream>

DirectImportScaleBandTreeItem::DirectImportScaleBandTreeItem( TreeItem* parentItem, TSLDirectImportScaleBand* scaleBand )
  : TreeItem( parentItem )
{
  const char* scaleBandName = scaleBand ? scaleBand->name() : "";
  m_itemData << scaleBandName;
  std::stringstream ss;
  ss << (scaleBand ? scaleBand->minScale() : 0.0);
  m_itemData << ss.str().c_str();
  m_layerName = scaleBandName;
  m_layerType = "Scale Band";
  m_scaleBand = scaleBand;

  // DataSets may be dragged to another position under a scale band
  m_flags |= Qt::ItemIsDropEnabled;

  // DataSets are listed in the same layout as the underlying TSLDirectImportScaleBand
  // The row numbers for each data set are used when moving data, so this layout
  // should not be modified.
  // No other children should be added as a direct child of DirectImportScaleBandTreeItem
  // without moving the datasets to another sub-tree.
  unsigned int numDataSets = scaleBand ? scaleBand->numDataSets() : 0;
  for( unsigned int i(0); i < numDataSets; ++i )
  {
    TSLDirectImportDataSet* dataSet( scaleBand->getDataSet(i) );
    appendChild( new DirectImportDataSetTreeItem( this, dataSet ) );
  }
}

DirectImportScaleBandTreeItem::~DirectImportScaleBandTreeItem()
{

}

TreeItem::TreeItemType DirectImportScaleBandTreeItem::type()
{
  return TreeItem::TreeItemTypeDirectImportScaleBand;
}

TSLDirectImportScaleBand* DirectImportScaleBandTreeItem::getScaleBand() const
{
  return m_scaleBand;
}
