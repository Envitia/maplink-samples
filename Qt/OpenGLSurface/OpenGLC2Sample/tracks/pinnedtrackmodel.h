/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef PINNEDTRACKMODEL_H
#define PINNEDTRACKMODEL_H

// This class maps a subset of information about a list of tracks that have been
// selected for monitoring to a Qt UI widget for display.

#include <QAbstractTableModel>
#include <vector>

class TSLDrawingSurface;

class PinnedTrackModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  // Event filter class to use with the model - removes rows from the model when the delete key is pressed.
  class DeleteKeyEvent : public QObject
  {
  public:
    DeleteKeyEvent();
    virtual ~DeleteKeyEvent();

  protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
  };

  PinnedTrackModel();
  virtual ~PinnedTrackModel();

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

  // Adds the track currently selected according to the track manager to the list of tracks
  // to display information about.
  void pinSelectedTrack();

  // Updates the data displayed in cells that change over time as tracks move
  void refreshTrackData();

private:
  // Maps table column numbers to the information to display in that column
  enum TrackInformationColumn
  {
    TrackID = 0,
    TrackHeading = 1,
    TrackVelocity = 2,
    TrackAltitude = 3,
  };

  std::vector< QVariant > m_headerNames;
  std::vector< size_t > m_pinnedTracks;
};


#endif
