/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef CREDENTIALSDIALOG_H
#define CREDENTIALSDIALOG_H

#include "ui_credentialsdialog.h"

// A Qt dialog that requests credentials from the user, for a given URL.

class CredentialsDialog : public QDialog, public Ui_credentialsDialog
{
  Q_OBJECT
public:
  CredentialsDialog( QWidget *parent = 0 );
  virtual ~CredentialsDialog();

  virtual void accept();

private:
};

#endif
