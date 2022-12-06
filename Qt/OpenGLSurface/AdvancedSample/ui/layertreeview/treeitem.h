/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TSLDrawingSurface;

// Base class for items in the layer tree view.

class TreeItem
{
public:
  enum TreeItemType
  {
    TreeItemTypeDefault,
    TreeItemTypeDirectImportScaleBand,
    TreeItemTypeDirectImportDataSet
  };

  TreeItem( TreeItem* parent );
  TreeItem( const QList<QVariant> &data, TreeItem *parentItem = 0, QString name = "", QString type = "", bool checked = false );
  TreeItem( TreeItem *parent, TSLDrawingSurface* drawingSurface, unsigned int layerIndex, const QString& layerType );
  virtual ~TreeItem();

  void appendChild( TreeItem *child );

  TreeItem *child( int row );
  int childCount() const;
  int columnCount() const;
  QVariant data( int column ) const;
  void setData( int column, QVariant data );
  QString getName();
  QString getType();
  int row() const;
  TreeItem *parentItem();
  bool checked() const;
  void toggleChecked();
  //! Additional flags not set by TreeModel
  //! - Qt::ItemIsDragEnabled;
  //! - Qt::ItemIsDropEnabled;
  //! - Qt::ItemIsEditable;
  Qt::ItemFlags& flags();

  virtual TreeItemType type();

protected:
  QList<TreeItem*> m_childItems;
  QList<QVariant> m_itemData;
  TreeItem *m_parentItem;
  bool m_checked;
  Qt::ItemFlags m_flags;

  // Layer properties
  QString m_layerName;
  QString m_layerType;
  bool m_layerVisible;
};

#endif // TREEITEM_H
