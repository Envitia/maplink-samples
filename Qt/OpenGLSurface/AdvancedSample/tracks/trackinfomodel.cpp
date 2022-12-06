/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "trackinfomodel.h"
#include "trackmanager.h"
#include "MapLink.h"
#include "MapLinkDrawing.h"
#include "tslapp6ahelper.h"

#ifdef _MSC_VER
# define snprintf _snprintf
#endif

TrackInfoModel::TrackInfoModel()
{
  // Populate the names of each of the row identifiers for the table
  m_headerNames.push_back( QVariant( "Track ID" ) );
  m_headerNames.push_back( QVariant( "Heading" ) );
  m_headerNames.push_back( QVariant( "Velocity" ) );
  m_headerNames.push_back( QVariant( "Altitude" ) );
  m_headerNames.push_back( QVariant( "Track Type" ) );
  m_headerNames.push_back( QVariant( "Hostility" ) );
  m_headerNames.push_back( QVariant( "Latitude" ) );
  m_headerNames.push_back( QVariant( "Longitude" ) );

  // Populate the units for each row in the table
  m_rowUnits.push_back( QVariant() );
  m_rowUnits.push_back( QVariant( "Degrees" ) );
  m_rowUnits.push_back( QVariant( "m/s" ) );
  m_rowUnits.push_back( QVariant( "Meters" ) );
  m_rowUnits.push_back( QVariant() );
  m_rowUnits.push_back( QVariant() );
  m_rowUnits.push_back( QVariant() );
  m_rowUnits.push_back( QVariant() );
}

TrackInfoModel::~TrackInfoModel()
{
}

int TrackInfoModel::rowCount( const QModelIndex& /*parent*/ ) const
{
  return (int)m_headerNames.size();
}

int TrackInfoModel::columnCount( const QModelIndex& /*parent*/ ) const
{
  return 2;
}

Qt::ItemFlags TrackInfoModel::flags(const QModelIndex &index) const
{
  if( index.isValid() && index.column() == 0 && index.row() == TrackHostility )
  {
    // Make the hostility cell editable
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  }

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TrackInfoModel::data(const QModelIndex &index, int role) const
{
  if( !index.isValid() || index.row() >= m_headerNames.size() )
  {
    return QVariant();
  }

  if( index.column() == 0 )
  {
    // Display information about the selected track, if there is one
    const TrackManager::DisplayInfo *displayInfo = TrackManager::instance().displayInformation();
    if( !displayInfo || displayInfo->m_selectedTrack >= displayInfo->m_tracks.size() )
    {
      // No selected track
      return QVariant();
    }

    const Track::DisplayInfo &selectedTrack = displayInfo->m_tracks[displayInfo->m_selectedTrack];

    if( role == Qt::DisplayRole )
    {
      switch( index.row() )
      {
      case TrackID:
        return QVariant( (unsigned int)displayInfo->m_selectedTrack );

      case TrackHeading:
        return QString(QStringLiteral("%1")).arg( selectedTrack.m_heading, 0, 'f', 2 );

      case TrackVelocity:
        return QString(QStringLiteral("%1")).arg( selectedTrack.m_speed, 0, 'f', 2 );

      case TrackAltitude:
        return QString(QStringLiteral("%1")).arg( selectedTrack.m_altitude, 0, 'f', 2 );

      case TrackType:
        {
          // Look up the symbol type based on the key
          TSLAPP6ASymbol symbolInfo;
          selectedTrack.m_symbolKey;
          TrackManager::instance().symbolHelper()->getSymbol( selectedTrack.m_symbolKey, symbolInfo );

          return QVariant( QString::fromUtf8(symbolInfo.name()) );
        }

      case TrackHostility:
        {
          switch( selectedTrack.m_hostility )
          {
          case TSLAPP6ASymbol::HostilityNone:
            return QStringLiteral( "None" );

          case TSLAPP6ASymbol::HostilityPending:
            return QStringLiteral( "Pending" );

          case TSLAPP6ASymbol::HostilityUnknown:
            return QStringLiteral( "Unknown" );

          case TSLAPP6ASymbol::HostilityAssumedFriend:
            return QStringLiteral( "Assumed Friend" );

          case TSLAPP6ASymbol::HostilityFriend:
            return QStringLiteral( "Friend" );

          case TSLAPP6ASymbol::HostilityNeutral:
            return QStringLiteral( "Neutral" );

          case TSLAPP6ASymbol::HostilitySuspect:
            return QStringLiteral( "Suspect" );

          case TSLAPP6ASymbol::HostilityHostile:
            return QStringLiteral( "Hostile" );

          case TSLAPP6ASymbol::HostilityJoker:
            return QStringLiteral( "Joker" );

          case TSLAPP6ASymbol::HostilityFaker:
            return QStringLiteral( "Faker" );

          default:
            return QStringLiteral( "Unspecified" );
          }
        }

      case TrackLatitude:
        {
          // Display in degrees, minutes, seconds instead of decimal
          double latitude = fabs( selectedTrack.m_lat );
          int degrees = (int)latitude;
          int minutes = (60.0 * latitude) - (60.0 * degrees);
          double seconds = (3600.0 * latitude) - (3600.0 * degrees) - (60.0 * minutes);

          char formattedLatitude[128];
          snprintf( formattedLatitude, sizeof(formattedLatitude), "%3.2d\xC2\xB0%3.2d' %05.02lf\" %c",
                    degrees, minutes, seconds, selectedTrack.m_lat < 0.0 ? 'S' : 'N' );
          return QVariant( QString::fromUtf8(formattedLatitude) );
        }

      case TrackLongitude:
        {
          // Display in degrees, minutes, seconds instead of decimal
          double longitude = fabs( selectedTrack.m_lon );
          int degrees = (int)longitude;
          int minutes = (60.0 * longitude) - (60.0 * degrees);
          double seconds = (3600.0 * longitude) - (3600.0 * degrees) - (60.0 * minutes);

          char formattedLongitude[128];
          snprintf( formattedLongitude, sizeof(formattedLongitude), "%3.2d\xC2\xB0%3.2d' %05.02lf\" %c",
                    degrees, minutes, seconds, selectedTrack.m_lon < 0.0 ? 'W' : 'E' );
          return QVariant( QString::fromUtf8(formattedLongitude) );
        }

      default:
        return QVariant();
      }
    }
    else if( role == Qt::UserRole && index.row() == TrackHostility )
    {
      // Store the actual track's hostility in the user role - this is used by
      // the hostility delegate for setting the right default value in the combo box.
      return QVariant( selectedTrack.m_hostility );
    }
  }
  else if( index.column() == 1 && role == Qt::DisplayRole )
  {
    // This column is the units of the row
    return m_rowUnits[index.row()];
  }
  
  return QVariant();
}

QVariant TrackInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if( role != Qt::DisplayRole || orientation != Qt::Vertical || section >= m_headerNames.size() )
  {
    return QVariant();
  }

  return m_headerNames[section];
}

bool TrackInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  // Only the hostility cell is editable
  if( !index.isValid() || index.column() != 0 || index.row() != TrackHostility || role != Qt::EditRole )
  {
    return false;
  }

  const TrackManager::DisplayInfo *trackInfo = TrackManager::instance().displayInformation();
  if( !trackInfo || trackInfo->m_selectedTrack >= trackInfo->m_tracks.size() )
  {
    // The current track selection is not valid
    return false;
  }

  TrackManager::instance().changeTrackHostility( (quint32)trackInfo->m_selectedTrack, value.toInt() );
  dataChanged( index, index );

  return true;
}

bool TrackInfoModel::isHostilityCell( const QModelIndex &index ) const
{
  return index.isValid() && index.column() == 0 && index.row() == TrackHostility;
}

void TrackInfoModel::reloadData()
{
  beginResetModel();
  endResetModel();
}

void TrackInfoModel::refreshTrackDisplay()
{
  // Only mark the cells that contain information that changes without user input as having
  // changed
  QVector<int> roles;
  roles.push_back( Qt::DisplayRole );

  dataChanged( createIndex( 1, 0, (void*)NULL ), createIndex( 3, 0, (void*)NULL ), roles );
  dataChanged( createIndex( 6, 0, (void*)NULL ), createIndex( 7, 0, (void*)NULL ), roles );
}
