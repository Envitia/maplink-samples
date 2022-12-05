/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef SERVICELISTMODEL_H
#define SERVICELISTMODEL_H

#include <QAbstractItemModel>
#include <QIcon>
#include "tslatomic.h"

class TSLDrawingSurface;
class QMenu;

namespace Services
{

  class ServiceList;
  class Service;

  // Implementation of a Qt model that displays a tree of all the services currently
  // loaded into the model and the layers in each service.

  class ServiceListModel : public QAbstractItemModel
  {
    Q_OBJECT
    public:
      enum ServiceNodeType
      {
        NodeTypeRoot,
        NodeTypeService,
        NodeTypeLayer,
        NodeTypeInvalid
      };

      ServiceListModel( ServiceList *services );
      virtual ~ServiceListModel();

      void addService( Service *service );

      ServiceList* serviceList();

      // Returns the TMC extent of the item in the model identified by index, or false if the item has no extent
      bool getItemTMCExtent( const QModelIndex &index, TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const;
      bool getItemTMCExtent( const QVariant &index, TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const;

      // Returns the type of item represented by index. This is used by the attached tree view to display the correct type
      // of context menu when right-clicking a node in the tree.
      ServiceNodeType getNodeTypeForIndex( const QModelIndex &index ) const;

      // Returns a list of layer names that are both not currently visible and 
      // compatible with the layers currently visible in the service identified by index
      void getPotentialLayersForDisplay( const QModelIndex &index, std::vector< std::string > &layerDisplayNames ) const;

      void getLayerDimensionNames( const QModelIndex &index, std::vector< std::string > &dimensionNames ) const;
      void getLayerStyleNames( const QModelIndex &index, std::vector< std::string > &styleNames ) const;

      // Removes the item from the model identified by the given variant, as returned from data( index, Qt::UserRole )
      void removeItem( const QVariant &nodeVariant );

      // Shows/hides the named layer on the service identified by the given variant, as returned from data( index, Qt::UserRole )
      void setLayerVisibility( const QVariant &nodeVariant, const QString &layerName, bool visible );

      // Sets the given style on the layer identified by the given variant, as returned from data( index, Qt::UserRole )
      void setLayerStyle( const QVariant &nodeVariant, const QString &styleName );

      // Gets the units of the dimension from the layer identified by the given variant, as returned from data( index, Qt::UserRole )
      const char* getLayerDimensionUnits( const QVariant &nodeVariant, const QString &name ) const;

      // Returns the service object for the given variant, as returned from data( index, Qt::UserRole )
      Service* getService( const QVariant &nodeVariant );

      // Qt model functions
      virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
      virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
      virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
      virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
      virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;

      virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
      virtual QModelIndex parent ( const QModelIndex &index ) const;

      // Drag and drop support functions
      virtual QStringList mimeTypes() const;
      virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
      virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
      virtual bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;

      virtual Qt::DropActions supportedDropActions() const;

    private:
      class ServiceListNode
      {
        public:
          ServiceListNode();
          ~ServiceListNode();

          ServiceListNode *m_parent;
          int m_index;
          ServiceNodeType m_dataType;
          void *m_data;
          std::vector< ServiceListNode* > m_children;
      };

      void updateChildNodeIndices( ServiceListNode *node );

      ServiceList *m_services;
      ServiceListNode *m_rootNode;

      QIcon m_serviceIcon;
      QIcon m_layersIcon;
  };

  inline ServiceList* ServiceListModel::serviceList()
  {
    return m_services;
  }

};
#endif
