/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSSERVICELAYERINFOMODEL_H
#define WMSSERVICELAYERINFOMODEL_H

#include "services/service.h"

class TSLWMSServiceLayer;

// Implementation of a Qt model that displays a table of information about
// a layer on a WMS service.
namespace Services
{
  class WMSServiceLayerInfoModel : public Service::ServiceLayerInfoModel
  {
    Q_OBJECT
    public:
      WMSServiceLayerInfoModel();
      virtual ~WMSServiceLayerInfoModel();

      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

      virtual void setSelectedLayer( const QModelIndex &layer );

    private:
      TSLWMSServiceLayer *m_layer;
  };
};
#endif
