/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMTSSERVICEDIMENSIONMODEL_H
#define WMTSSERVICEDIMENSIONMODEL_H

#include <map>
#include "ui/datedelegate.h"
#include "ui/prescribedvaluedelegate.h"
#include "services/service.h"

class TSLWMTSServiceLayer;
class TSLWMTSServiceInfo;
class QAbstractItemView;

// Implementation of a Qt model that displays a table of the dimensions
// on a WMTS layer.
namespace Services
{
  class WMTSServiceDimensionsModel : public Service::ServiceDimensionsModel
  {
    Q_OBJECT
    public:
      WMTSServiceDimensionsModel( TSLWMTSServiceInfo *serviceInfo );
      virtual ~WMTSServiceDimensionsModel();

      virtual bool configurationValid() const;
      virtual void setDelegates( QAbstractItemView *view );
      virtual bool getPossibleValues( const QModelIndex &index, std::vector< const char* > &values ) const;

      const std::pair< int, TSLWMTSServiceLayer* >& getDimensionInfo( int rowIndex ) const;
      virtual QModelIndex findDimensionByName( const QString &dimensionName ) const;

      virtual Qt::ItemFlags flags(const QModelIndex & index) const;
      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    private:
      void extractVisibleLayerDimensions( TSLWMTSServiceInfo *serviceInfo );

      const TSLWMTSServiceInfo *m_serviceInfo;
      std::vector< std::pair< int, TSLWMTSServiceLayer* > > m_dimensions;

      DateDelegate m_dateDelegate;
      PrescribedValueDelegate m_prescribedValueDelegate;
  };

  inline const std::pair< int, TSLWMTSServiceLayer* >& WMTSServiceDimensionsModel::getDimensionInfo( int rowIndex ) const
  {
    return m_dimensions[rowIndex];
  }
};
#endif
