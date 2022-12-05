/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSSERVICEDIMENSIONMODEL_H
#define WMSSERVICEDIMENSIONMODEL_H

#include <map>

#include "ui/datedelegate.h"
#include "ui/prescribedvaluedelegate.h"

#include "services/service.h"

class TSLWMSServiceLayer;
class QAbstractItemView;

// Implementation of a Qt model that displays a table of the dimensions
// on a WMS layer.

namespace Services
{

  class WMSServiceDimensionsModel : public Service::ServiceDimensionsModel
  {
    Q_OBJECT
    public:
      WMSServiceDimensionsModel( TSLWMSServiceLayer *rootLayer );
      virtual ~WMSServiceDimensionsModel();

      virtual bool configurationValid() const;
      virtual void setDelegates( QAbstractItemView *view );
      virtual bool getPossibleValues( const QModelIndex &index, std::vector< const char* > &values ) const;

      const std::pair< int, TSLWMSServiceLayer* >& getDimensionInfo( int rowIndex ) const;
      virtual QModelIndex findDimensionByName( const QString &dimensionName ) const;

      virtual Qt::ItemFlags flags(const QModelIndex & index) const;
      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
      virtual QVariant data(const QModelIndex &index, int role) const;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    private:
      void extractVisibleLayerDimensions( TSLWMSServiceLayer *layer );

      TSLWMSServiceLayer *m_rootLayer;

      // Stores the set of dimensions that are applicable to selected layers in the service. The first part of the
      // pair contains the index into the TSLWMSServiceLayer for the dimension, and the second part of the pair stores
      // the TSLWMSServiceLayer for which the dimension applies.
      std::vector< std::pair< int, TSLWMSServiceLayer* > > m_dimensions;

      DateDelegate m_dateDelegate;
      PrescribedValueDelegate m_prescribedValueDelegate;
  };

  inline const std::pair< int, TSLWMSServiceLayer* >& WMSServiceDimensionsModel::getDimensionInfo( int rowIndex ) const
  {
    return m_dimensions[rowIndex];
  }
};
#endif
