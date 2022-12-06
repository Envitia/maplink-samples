/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "generaloptionsdialog.h"
#include "services/servicelist.h"
#include <QMessageBox>

using namespace Services;

GeneralOptionsDialog::GeneralOptionsDialog( ServiceList *serviceList, QWidget *parent )
  : QDialog( parent )
  , m_serviceList( serviceList )
{
  setupUi( this );

  // Turn off the help button as there is no additional help to display
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  connect(pushButtonClearCredentials, SIGNAL(clicked(bool)), this, SLOT(clearCredentials()));

  numConnections->setValue( m_serviceList->numConnections() );
  cacheSize->setValue( m_serviceList->cacheSizes() );
}

GeneralOptionsDialog::~GeneralOptionsDialog()
{
}

void GeneralOptionsDialog::accept()
{
  m_serviceList->setNumConnections( numConnections->value() );
  m_serviceList->setCacheSizes( cacheSize->value() );

  QDialog::accept();
}

void GeneralOptionsDialog::clearCredentials()
{
  m_serviceList->clearCachedCredentials();
  QMessageBox msgBox;
  msgBox.setText("The credentials cache has been cleared.");
  msgBox.exec();
}
