/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef GENERALOPTIONSDIALOG_H
#define GENERALOPTIONSDIALOG_H

#include "ui_generaloptionsdialog.h"

namespace Services
{
  class ServiceList;
};

// A Qt dialog that provides some general configuration settings that are 
// global to the application

class GeneralOptionsDialog : public QDialog, public Ui_generalOptionsDialog
{
  Q_OBJECT
public:
  GeneralOptionsDialog( Services::ServiceList *serviceList, QWidget *parent = 0 );
  virtual ~GeneralOptionsDialog();

  virtual void accept();
    
private slots:
  void clearCredentials();

private:
  Services::ServiceList *m_serviceList;
};

#endif
