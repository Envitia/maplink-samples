/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "simulationoptionsdialog.h"

SimulationOptionsDialog::SimulationOptionsDialog(QWidget *parent)
{
  setupUi(this);
}

SimulationOptionsDialog::~SimulationOptionsDialog()
{
}

void SimulationOptionsDialog::numTracks(int numTracks)
{
  spinBox_NumTracks->setValue(numTracks);
}

void SimulationOptionsDialog::simulationSpeed(int simSpeed)
{
  spinBox_SimulationSpeed->setValue(simSpeed);
}

void SimulationOptionsDialog::positionFormat(const MaplinkTrackObject::PositionFormat format)
{
  comboBox_PositionFormat->setCurrentIndex((int)format);
}

int SimulationOptionsDialog::numTracks()
{
  return spinBox_NumTracks->value();
}

int SimulationOptionsDialog::simulationSpeed()
{
  return spinBox_SimulationSpeed->value();
}

const MaplinkTrackObject::PositionFormat SimulationOptionsDialog::positionFormat()
{
  return (MaplinkTrackObject::PositionFormat)comboBox_PositionFormat->currentIndex();
}