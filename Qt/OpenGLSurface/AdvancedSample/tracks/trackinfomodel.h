/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKINFOMODEL_H
#define TRACKINFOMODEL_H


#include <QMainWindow>
#include <QWidget>

#include <QAbstractTableModel>
#include <vector>


class TSLDrawingSurface;


// This class maps information about the currently selected track to a Qt UI widget 
// for display

class TrackInfoModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  // Maps table row numbers to the information to display in that row
  enum TrackInformationRow
  {
    TrackID = 0,
    TrackHeading = 1,
    TrackVelocity = 2,
    TrackAltitude = 3,
    TrackType = 4,
    TrackHostility = 5,
    TrackLatitude = 6,
    TrackLongitude = 7
  };

  TrackInfoModel();
  virtual ~TrackInfoModel();

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

  // Returns true if the given index is the table cell that contains the selected track's hostility
  bool isHostilityCell( const QModelIndex &index ) const;

  // Refreshes the entire model. Invoked when the currently selected track changes.
  void reloadData();

  // Refreshes cells in the table that change as the selected track moves, i.e. heading, speed and position.
  void refreshTrackDisplay();

private:
  std::vector< QVariant > m_headerNames;
  std::vector< QVariant > m_rowUnits;
};


#endif
