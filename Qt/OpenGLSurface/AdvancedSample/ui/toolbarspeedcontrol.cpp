/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "toolbarspeedcontrol.h"
#include "tracks/trackmanager.h"

ToolbarSpeedControl::ToolbarSpeedControl( QWidget *parent )
  : QWidget( parent )
{
  setupUi( this );

  connect( simulationSpeedSlider, SIGNAL( valueChanged( int ) ), this, SLOT( updateSpinnerValue( int ) ) );
}

ToolbarSpeedControl::~ToolbarSpeedControl()
{
}

void ToolbarSpeedControl::updateSpinnerValue( int value )
{
  // Inform the track manager of the new playback speed 
  TrackManager::instance().setSimulationTimeCompression( value );
}