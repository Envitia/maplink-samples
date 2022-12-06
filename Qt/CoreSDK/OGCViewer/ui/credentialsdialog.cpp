/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "credentialsdialog.h"

CredentialsDialog::CredentialsDialog( QWidget *parent )
{
  setupUi( this );

  // Turn off the help button as there is no additional help to display
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

CredentialsDialog::~CredentialsDialog()
{
}

void CredentialsDialog::accept()
{
  QDialog::accept();
}

