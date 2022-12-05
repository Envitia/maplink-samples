/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WMTSSERVICELAYERSTYLESMODEL_H
#define WMTSSERVICELAYERSTYLESMODEL_H

#include "services/service.h"

class TSLWMTSServiceLayer;

// Implementation of a Qt model that displays a list of the styles available
// for a specific WMTS layer.
namespace Services
{
  class WMTSServiceLayerStylesModel : public Service::ServiceLayerStylesModel
  {
    Q_OBJECT
    public:
      WMTSServiceLayerStylesModel();
      virtual ~WMTSServiceLayerStylesModel();

      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

      virtual void setSelectedLayer( const QModelIndex &layer );
      virtual const char* currentStyle() const;
      virtual void setStyle( const char *name );

    private:
      TSLWMTSServiceLayer *m_layer;
  };
};
#endif
