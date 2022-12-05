/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "actiontypepage.h"
#include "servicewizardpageenum.h"
#include "servicewizard.h"
#include "services/wms/wmsservice.h"
#include "services/wmts/wmtsservice.h"

using namespace Services;

ActionTypePage::ActionTypePage(QWidget *parent)
  : QWizardPage( parent )
{
  setupUi(this);
}

ActionTypePage::~ActionTypePage()
{
}

int ActionTypePage::nextId() const
{
  if( addWMSButton->isChecked() )
  {
    return AddWMSService;
  }
  else if( addWMTSButton->isChecked() )
  {
    return AddWMTSService;
  }

  return -1;
}

bool ActionTypePage::validatePage()
{
  ServiceWizard *parentWizard = reinterpret_cast< ServiceWizard* >( wizard() );
  if( addWMSButton->isChecked() )
  {
    parentWizard->setService( new WMSService() );
    return true;
  }
  else if( addWMTSButton->isChecked() )
  {
    parentWizard->setService( new WMTSService() );
    return true;
  }

  return false;
}
