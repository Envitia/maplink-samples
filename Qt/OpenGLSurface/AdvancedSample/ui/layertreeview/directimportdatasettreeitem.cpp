/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include "directimportdatasettreeitem.h"

#include "MapLinkDirectImport.h"
#include <sstream>

DirectImportDataSetTreeItem::DirectImportDataSetTreeItem( TreeItem* parentItem, TSLDirectImportDataSet* dataSet )
  : TreeItem( parentItem )
{
  // Datasets may be drag/dropped to reorder
  m_flags |= Qt::ItemIsDragEnabled;

  const char* dataSetName = dataSet ? dataSet->name() : "";
  m_itemData << "Data Set" << dataSetName;
  m_layerName = dataSetName;
  m_layerType = "Data Set";
  m_dataSet = dataSet;
}

DirectImportDataSetTreeItem::~DirectImportDataSetTreeItem()
{

}

TreeItem::TreeItemType DirectImportDataSetTreeItem::type()
{
  return TreeItem::TreeItemTypeDirectImportDataSet;
}

TSLDirectImportDataSet* DirectImportDataSetTreeItem::getDataSet() const
{
  return m_dataSet;
}
