/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DECLUTTERMODEL_H
#define DECLUTTERMODEL_H

// This class maps the feature list for a set of MapLink Data Layers into a Qt
// treeview widget. This allows for both display of the available features,
// and also allows decluttering of features through toggling nodes in the tree.

#include <QAbstractItemModel>
#include <vector>
#include <string>

class TSLDataLayer;
class TSLDrawingSurface;


class DeclutterModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  DeclutterModel();
  virtual ~DeclutterModel();

  // Qt model functions
  virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
  virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
  virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
  virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
  virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
  virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
  virtual QModelIndex parent ( const QModelIndex &index ) const;
  virtual bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

  // Adds the features from the given layer to the tree
  void addLayerFeatures( const QString &layerName, TSLDataLayer *layer );

  // Removes the given layer from the tree
  void removeLayerFeatures( const QString &layerName );

  // Sets the drawing surface to use when changing feature's decluttering status
  void setDrawingSurface( TSLDrawingSurface *surface );

  // Sets the widget that the model should trigger an update for when the declutter settings are modified.
  // This causes the view to refresh when the user changes the declutter settings in the drawing surface
  void setUpdateView( QWidget *widget );

private slots:
  void updateLayerFeatures( const QString& layerName );

private:
  // Internal class for each node in the tree.
  class DeclutterNode
  {
  public:
    DeclutterNode( DeclutterNode *parent, int index, const QString &displayName, const QString &featureName, const QString &layerName );
    ~DeclutterNode();

    DeclutterNode *m_parent;
    int m_index;
    QString m_displayName; // Contains only the last part of the feature name
    QString m_featureName; // Contains the full feature name
    QString m_layerName;

    std::vector< DeclutterNode* > m_children;
  };

  void updateChildNodes( DeclutterNode *node );

  DeclutterNode *m_rootNode;
  TSLDrawingSurface *m_surface;
  QWidget *m_updateView;
};

inline void DeclutterModel::setDrawingSurface( TSLDrawingSurface *surface )
{
  m_surface = surface;
}

inline void DeclutterModel::setUpdateView( QWidget *widget )
{
  m_updateView = widget;
}

#endif
