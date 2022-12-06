/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSSERVICELAYERSTYLESMODEL_H
#define WMSSERVICELAYERSTYLESMODEL_H

#include "services/service.h"

class TSLWMSServiceLayer;

// Implementation of a Qt model that displays a list of the styles available
// for a specific WMS layer.
namespace Services
{
  class WMSServiceLayerStylesModel : public Service::ServiceLayerStylesModel
  {
    Q_OBJECT
    public:
      WMSServiceLayerStylesModel();
      virtual ~WMSServiceLayerStylesModel();

      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

      virtual void setSelectedLayer( const QModelIndex &layer );
      virtual const char* currentStyle() const;
      virtual void setStyle( const char *name );

    private:
      TSLWMSServiceLayer *m_layer;
  };
};
#endif
