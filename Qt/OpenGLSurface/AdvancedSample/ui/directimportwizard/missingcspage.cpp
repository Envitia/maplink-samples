/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "missingcspage.h"
#include "wizardpageenum.h"

MissingCSPage::MissingCSPage( QWidget *parent )
  : QWizardPage( parent )
{
  setupUi( this );
  registerField( "dataCoordinateSystem", dataCoordinateSystemCombo );
}

int MissingCSPage::nextId() const
{
  return -1;
}