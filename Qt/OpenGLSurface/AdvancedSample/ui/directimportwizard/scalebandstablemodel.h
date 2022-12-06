/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SCALEBANDSTABLEMODEL_H
#define SCALEBANDSTABLEMODEL_H

#include "MapLinkDirectImport.h"

#include <QAbstractItemModel>
#include <QAbstractTableModel>

// This class represents the underlying data model for the scale bands table.

class ScaleBandsTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  enum Column
  {
    ColumnName,
    ColumnMinScale,
    ColumnAutomaticTiling,
    ColumnPolarTiling,
    ColumnTilesX,
    ColumnTilesY,

    ColumnMax
  };

  struct ScaleBandTableItem
  {
    QString m_name;
    double m_minScale;
    QString m_minScaleStr;
    unsigned int m_tilesX;
    unsigned int m_tilesY;
    bool m_automaticTiling;
    bool m_polarTiling;
  };
  struct ScaleBandTableItemNamePredicate
  {
    ScaleBandTableItemNamePredicate( const QString& name )
      : m_name( name )
    {
    }

    bool operator()( const ScaleBandTableItem& a ) const
    {
      return a.m_name == m_name;
    }
    QString m_name;
  };

  ScaleBandsTableModel( QObject* parent );
  ~ScaleBandsTableModel();

  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
  virtual Qt::ItemFlags flags(const QModelIndex& index ) const;
  virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
  virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());



  //! Set the datalayer for the model and populate m_data
  void dataLayer( TSLDirectImportDataLayer* layer );
  //! Check whether the contents of m_data is valid
  bool scaleBandsValid() const;
  //! Save the contents of m_data to m_layer
  bool saveToLayer();

private:
  TSLDirectImportDataLayer* m_layer;
  std::vector<ScaleBandTableItem> m_data;
};

#endif // SCALEBANDSTABLEMODEL_H
