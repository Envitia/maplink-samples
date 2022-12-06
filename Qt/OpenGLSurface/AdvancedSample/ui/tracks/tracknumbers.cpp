/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "tracknumbers.h"
#include "tracks/trackmanager.h"

TrackNumbers::TrackNumbers( QWidget *parent )
  : QDialog( parent )
{
  setupUi( this );

  // Turn off the help button as there is no additional help to display
  setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

  numTracks->setValue( (int)TrackManager::instance().numRequestedTracks() );
  numTrackTypes->setValue( (int)TrackManager::instance().numRequestedTrackTypes() );
}

TrackNumbers::~TrackNumbers()
{
}

void TrackNumbers::accept()
{
  TrackManager::instance().numRequestedTrackTypes( numTrackTypes->value() );
  TrackManager::instance().numRequestedTracks( numTracks->value() );
  TrackManager::instance().createTracks( numTracks->value(), numTrackTypes->value() );
  QDialog::accept();
}
