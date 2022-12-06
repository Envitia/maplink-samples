/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "urldialog.h"


URLDialog::URLDialog( QWidget *parent )
  : QDialog( parent )
{
  setupUi( this );
  setWindowModality( Qt::WindowModal );
}

URLDialog::~URLDialog()
{

}

void URLDialog::setupDialog(  )
{

}

const QString& URLDialog::getURLTextbox()
{
  const QString& urlLink( urlTextbox->text() );

  return urlLink;
}

QDialogButtonBox* URLDialog::getButtonBox()
{
  return buttonBox;
}
