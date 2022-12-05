/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DIRECTIMPORTSCALEBANDTREEITEM_H
#define DIRECTIMPORTSCALEBANDTREEITEM_H

#include "treeitem.h"

class TSLDirectImportScaleBand;

// A small class to implement a direct import scale band item in the layer tree view.

class DirectImportScaleBandTreeItem : public TreeItem
{
private:
  TSLDirectImportScaleBand* m_scaleBand;
public:
  DirectImportScaleBandTreeItem( TreeItem* parentItem, TSLDirectImportScaleBand* scaleBand );
  virtual ~DirectImportScaleBandTreeItem();

  virtual TreeItem::TreeItemType type();
  TSLDirectImportScaleBand* getScaleBand() const;
};

#endif // TREEITEM_H
