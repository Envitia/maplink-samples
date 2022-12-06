/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSSERVICEDIMENSIONINFOMODEL_H
#define WMSSERVICEDIMENSIONINFOMODEL_H

#include "services/service.h"

namespace Services
{

  class WMSServiceDimensionsModel;

  // Implementation of a Qt model that displays a table of information
  // about a dimension on a WMS layer.

  class WMSServiceDimensionInfoModel : public Service::ServiceDimensionInfoModel
  {
    Q_OBJECT
    public:
      WMSServiceDimensionInfoModel();
      virtual ~WMSServiceDimensionInfoModel();

      virtual void setSelectedDimension( const QModelIndex &layer );

      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    private:
      const WMSServiceDimensionsModel *m_dimensionsModel;
      int m_selectedDimension;
  };

};
#endif
