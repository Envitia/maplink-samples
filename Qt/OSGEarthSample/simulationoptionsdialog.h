/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SIMULATIONOPTIONSDIALOG_H
#define SIMULATIONOPTIONSDIALOG_H

#include <QDialog>
#include "ui_simulationOptions.h"
#include "maplinktrackobject.h"

class SimulationOptionsDialog : public QDialog, private Ui_SimulationOptionsDialog
{
    Q_OBJECT
public:
    SimulationOptionsDialog(QWidget *parent = 0);
    ~SimulationOptionsDialog();

    void numTracks(int numTracks);
    void simulationSpeed(int simSpeed);
    void positionFormat(const MaplinkTrackObject::PositionFormat format);

    int numTracks();
    int simulationSpeed();
    const MaplinkTrackObject::PositionFormat positionFormat();  
};

#endif // SIMULATIONOPTIONSDIALOG_H
