/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TOOLBARSPEEDCONTROL_H
#define TOOLBARSPEEDCONTROL_H

#include <QWidget>
#include "ui_toolbarspeedcontrol.h"

class ToolbarSpeedControl : public QWidget, private Ui_ToolbarSpeedControl
{
  Q_OBJECT
public:
  ToolbarSpeedControl(QWidget *parent = 0);
  virtual ~ToolbarSpeedControl();

public slots:
  void updateSpinnerValue( int value );
};

#endif
