/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKNUMBERS_H
#define TRACKNUMBERS_H

// A simple dialog for changing the number of tracks and track variety in the simulation.

#include "ui_tracknumbers.h"
#include <QDialog>

class TrackNumbers : public QDialog, private Ui_trackNumberDialog
{
  Q_OBJECT
public:
  TrackNumbers( QWidget *parent = 0 );
  virtual ~TrackNumbers();

  virtual void accept();
};

#endif
