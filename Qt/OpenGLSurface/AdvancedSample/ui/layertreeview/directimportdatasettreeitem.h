/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DIRECTIMPORTDATASETTREEITEM_H
#define DIRECTIMPORTDATASETTREEITEM_H

#include "treeitem.h"

class TSLDirectImportDataSet;

// A small class to implement a direct import item in the layer tree view.

class DirectImportDataSetTreeItem : public TreeItem
{
private:
  TSLDirectImportDataSet* m_dataSet;
public:
  DirectImportDataSetTreeItem( TreeItem* parentItem, TSLDirectImportDataSet* dataSet );
  virtual ~DirectImportDataSetTreeItem();

  virtual TreeItem::TreeItemType type();
  TSLDirectImportDataSet* getDataSet() const;
};

#endif // TREEITEM_H
