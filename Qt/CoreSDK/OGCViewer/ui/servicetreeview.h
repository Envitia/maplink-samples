/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SERVICETREEVIEW_H
#define SERVICETREEVIEW_H

#include <QTreeView>

namespace Services
{
  class ServiceListModel;
};

// Promoted tree view class to allow for creation of dynamic context menus on items in the service tree

class ServiceTreeView : public QTreeView
{
  Q_OBJECT
public:
  ServiceTreeView( QWidget *parent = 0 );
  virtual ~ServiceTreeView();

private slots:
  void removeItem();
  void showLayerOnService();
  void changeDimensionValue();
  void changeStyleValue();
  void zoomToSelection();
  void setServiceProperties();

protected:
  virtual void contextMenuEvent(QContextMenuEvent * event);
  virtual void dragMoveEvent(QDragMoveEvent * event);

  void showContextMenuForService( Services::ServiceListModel *model, const QModelIndex &serviceIndex, const QPoint &menuLocation );
  void showContextMenuForLayer( Services::ServiceListModel *model, const QModelIndex &layerIndex, const QPoint &menuLocation );
};

#endif
