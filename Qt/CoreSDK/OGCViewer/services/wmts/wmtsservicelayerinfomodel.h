/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WMTSSERVICELAYERINFOMODEL_H
#define WMTSSERVICELAYERINFOMODEL_H

#include "services/service.h"

class TSLWMTSServiceLayer;

// Implementation of a Qt model that displays a table of information about
// a layer on a WMTS service.
namespace Services
{
  class WMTSServiceLayerInfoModel : public Service::ServiceLayerInfoModel
  {
    Q_OBJECT
    public:
      WMTSServiceLayerInfoModel();
      virtual ~WMTSServiceLayerInfoModel();

      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

      virtual void setSelectedLayer( const QModelIndex &layer );

    private:
      const TSLWMTSServiceLayer *m_layer;
  };
};
#endif
