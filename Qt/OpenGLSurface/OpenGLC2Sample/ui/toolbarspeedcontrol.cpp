/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "toolbarspeedcontrol.h"
#include "tracks/trackmanager.h"

#include "MapLink.h"
#include "tslapp6ahelper.h"

ToolbarSpeedControl::ToolbarSpeedControl(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);

  connect( simulationSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSpinnerValue(int)) );
}
  
ToolbarSpeedControl::~ToolbarSpeedControl()
{
}

void ToolbarSpeedControl::updateSpinnerValue( int value )
{
  // Inform the track manager of the new playback speed - the slider value is 10 times the actual compression factor
  TrackManager::instance().setSimulationTimeCompression( value / 10.0 );
}
