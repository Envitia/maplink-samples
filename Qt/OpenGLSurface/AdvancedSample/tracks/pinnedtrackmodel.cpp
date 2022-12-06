/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "pinnedtrackmodel.h"
#include "trackmanager.h"
#include "MapLink.h"
#include <algorithm>
#include <QKeyEvent>
#include <QAbstractItemView>



PinnedTrackModel::DeleteKeyEvent::DeleteKeyEvent()
{
}

PinnedTrackModel::DeleteKeyEvent::~DeleteKeyEvent()
{
}

bool PinnedTrackModel::DeleteKeyEvent::eventFilter(QObject *obj, QEvent *event)
{
  if( event->type() == QEvent::KeyPress )
  {
    QKeyEvent *keyEvent = reinterpret_cast<QKeyEvent*>( event );
    if( keyEvent->matches( QKeySequence::Delete ) )
    {
      // Delete key was pressed, remove the selected rows from the table
      QAbstractItemView *view = reinterpret_cast<QAbstractItemView*>( obj );

      QModelIndexList selectedIndices = view->selectionModel()->selectedIndexes();
      if( !selectedIndices.isEmpty() )
      {
        view->model()->removeRow( selectedIndices.first().row() );
      }
    }

    return true;
  }
  else
  {
    return QObject::eventFilter( obj, event );
  }
}

PinnedTrackModel::PinnedTrackModel()
{
  // Populate the names of each of the row identifiers for the table
  m_headerNames.push_back( QVariant( "ID" ) );
  m_headerNames.push_back( QVariant( "Heading" ) );
  m_headerNames.push_back( QVariant( "Velocity" ) );
  m_headerNames.push_back( QVariant( "Altitude" ) );
}

PinnedTrackModel::~PinnedTrackModel()
{
}

int PinnedTrackModel::rowCount( const QModelIndex& /*parent*/ ) const
{
  return (int)m_pinnedTracks.size();
}

int PinnedTrackModel::columnCount( const QModelIndex& /*parent*/ ) const
{
  return (int)m_headerNames.size();
}

QVariant PinnedTrackModel::data(const QModelIndex &index, int role) const
{
  if( !index.isValid() || index.row() >= m_pinnedTracks.size() || index.column() >= m_headerNames.size() ||
      role != Qt::DisplayRole || m_pinnedTracks.empty() )
  {
    return QVariant();
  }


  // Display a limited set of information about the currently pinned tracks
  const TrackManager::DisplayInfo *displayInfo = TrackManager::instance().displayInformation();
  if( !displayInfo )
  {
    // No tracks currently exist
    return QVariant();
  }

  const Track::DisplayInfo &pinnedTrack = displayInfo->m_tracks[m_pinnedTracks[index.row()]];

  switch( index.column() )
  {
  case TrackID:
    return QVariant( (unsigned int) m_pinnedTracks[index.row()] );

  case TrackHeading:
    return QString(QStringLiteral("%1")).arg( pinnedTrack.m_heading, 0, 'f', 2 );

  case TrackVelocity:
    return QString(QStringLiteral("%1")).arg( pinnedTrack.m_speed, 0, 'f', 2 );

  case TrackAltitude:
    return QString(QStringLiteral("%1")).arg( pinnedTrack.m_altitude, 0, 'f', 2 );

  default:
    return QVariant();
  }

  return QVariant();
}

QVariant PinnedTrackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if( role != Qt::DisplayRole || orientation != Qt::Horizontal || section >= m_headerNames.size() )
  {
    return QVariant();
  }

  return m_headerNames[section];
}

bool PinnedTrackModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if( row >= m_pinnedTracks.size() )
  {
    return false;
  }

  // Remove the requested tracks from the pined list
  beginRemoveRows( parent, row, row + count - 1 );

  for( int currentRow = row + count - 1; currentRow >= row; --currentRow )
  {
    m_pinnedTracks.erase( m_pinnedTracks.begin() + currentRow );
  }

  endRemoveRows();

  return true;
}

void PinnedTrackModel::pinSelectedTrack()
{
  const TrackManager::DisplayInfo *displayInfo = TrackManager::instance().displayInformation();
  if( !displayInfo || displayInfo->m_selectedTrack >= displayInfo->m_tracks.size() )
  {
    // No selected track, disregard
    return;
  }

  if( std::find( m_pinnedTracks.begin(), m_pinnedTracks.end(), displayInfo->m_selectedTrack ) !=
      m_pinnedTracks.end() )
  {
    // This track is already pinned, don't re-add it to the list
  }

  beginInsertRows( QModelIndex(), (int)m_pinnedTracks.size(), (int)m_pinnedTracks.size() );
  m_pinnedTracks.push_back( displayInfo->m_selectedTrack );
  endInsertRows();
}

void PinnedTrackModel::refreshTrackData()
{
  if( !m_pinnedTracks.empty() )
  {
    QVector<int> roles;
    roles.push_back( Qt::DisplayRole );
    dataChanged( createIndex( 0, TrackHeading, (void*)NULL ), createIndex( (int)(m_pinnedTracks.size()-1), TrackAltitude, (void*)NULL ),
                 roles );
  }
}
