/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <string>

class TSLDrawingSurface;

class TreeItem;

class LayerTreeView;

// Data model for the layer tree

class TreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:

  explicit TreeModel( const QString &data, QObject *parent = 0 );
  ~TreeModel();

  virtual QVariant data( const QModelIndex &index, int role ) const Q_DECL_OVERRIDE;
  virtual Qt::ItemFlags flags( const QModelIndex &index ) const Q_DECL_OVERRIDE;
  virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const Q_DECL_OVERRIDE;
  virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const Q_DECL_OVERRIDE;
  virtual QModelIndex parent( const QModelIndex &index ) const Q_DECL_OVERRIDE;
  virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const Q_DECL_OVERRIDE;
  virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const Q_DECL_OVERRIDE;

  virtual Qt::DropActions supportedDragActions() const;
  virtual Qt::DropActions supportedDropActions() const;

  virtual QMimeData* mimeData( const QModelIndexList &indexes ) const;
  virtual QStringList mimeTypes() const;

  virtual bool canDropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent ) const;
  virtual bool dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent );

  virtual bool setData( const QModelIndex &index, const QVariant &value, int role );

  void setupModelData( const QStringList &lines, TreeItem *parent );

  void refreshFromSurface( TSLDrawingSurface* drawingSurface, QString layerType = "" );
  TreeItem* getRootItem();

signals:
  void moveLayerToIndex( const char* moveLayer, const char* targetLayer, int row );
  void modelModified( const QModelIndex &index, long value );
  void closeProgressDialog();
#ifdef HAVE_DIRECT_IMPORT_SDK
  void moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo );
#endif

private:
  TSLDrawingSurface* m_drawingSurface;
  void createRootItem( const QString &data );
  TreeItem* m_rootItem;

};

#endif // TREEMODEL_H
