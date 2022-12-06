/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMTSSERVICEDIMENSIONINFOMODEL_H
#define WMTSSERVICEDIMENSIONINFOMODEL_H

#include "services/service.h"

namespace Services
{
  class WMTSServiceDimensionsModel;

  // Implementation of a Qt model that displays a table of information
  // about a dimension on a WMTS layer.

  class WMTSServiceDimensionInfoModel : public Service::ServiceDimensionInfoModel
  {
    Q_OBJECT
    public:
      WMTSServiceDimensionInfoModel();
      virtual ~WMTSServiceDimensionInfoModel();

      virtual void setSelectedDimension( const QModelIndex &layer );

      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    private:
      const WMTSServiceDimensionsModel *m_dimensionsModel;
      int m_selectedDimension;
  };
};
#endif
