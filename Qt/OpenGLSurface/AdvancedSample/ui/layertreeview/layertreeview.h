/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SERVICETREEVIEW_H
#define SERVICETREEVIEW_H

#include <QTreeView>

class TreeModel;
class DirectImportDataSetTreeItem;
class DirectImportScaleBandTreeItem;

// Promoted tree view class to allow for creation of dynamic context menus on items in the service tree

class LayerTreeView : public QTreeView
{
  Q_OBJECT
public:
  LayerTreeView( QWidget *parent = 0 );
  virtual ~LayerTreeView();
  void clearSelectedLayer();

private slots:
  void removeItem();
  void loadLayer();
  void loadAttributeData();
  void editCacheSize();
  void itemClicked( const QModelIndex& );
#ifdef HAVE_DIRECT_IMPORT_SDK
  void removeDirectImportData();
  void editDirectImportData();
#endif

public slots:
  void zoomToExtentClicked();
  void selectionChanged( const QItemSelection &selected, const QItemSelection &deselected );

signals:
  void modifyData( const QModelIndex&, long value );
  void removeTreeLayer( const std::string& name, const std::string& treeAttribute );
  void zoomToLayerExtent( const std::string& );
  void editCacheSize( const std::string& name );
  void selectedAttributeData( const std::string& name, const std::string& treeAttribute );
  void addLayer();
#ifdef HAVE_DIRECT_IMPORT_SDK
  void removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
  void editDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
  void zoomToLayerDataSetExtent( DirectImportDataSetTreeItem* dataSet );
  void zoomToLayerScaleBandExtent( DirectImportScaleBandTreeItem* scaleBand );
#endif

protected:
  virtual void dragMoveEvent( QDragMoveEvent * event );
  virtual void contextMenuEvent( QContextMenuEvent * event );

private:
  void showContextMenu( TreeModel *model, const QModelIndex &layerIndex, const QPoint &menuLocation );

  std::string m_selectedLayer;
  std::string m_selectedType;
#ifdef HAVE_DIRECT_IMPORT_SDK
  DirectImportDataSetTreeItem* m_dataSetSelected;
  DirectImportScaleBandTreeItem* m_scaleBandSelected;
  DirectImportDataSetTreeItem* m_dataSetToEdit;
#endif
};

#endif
