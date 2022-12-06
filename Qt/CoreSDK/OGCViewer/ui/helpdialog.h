/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include "ui_helpdialog.h"

// A Qt dialog that displays the applications help

class HelpDialog : public QDialog, public Ui_DialogHelp
{
  Q_OBJECT
public:
  HelpDialog( QWidget *parent = 0 );
  virtual ~HelpDialog();

};

#endif
