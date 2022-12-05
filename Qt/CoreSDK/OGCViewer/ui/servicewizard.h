/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SERVICEWIZARD_H
#define SERVICEWIZARD_H

#include <QWizard>
#include "servicewizardpageenum.h"
#include "ui_servicewizard.h"

#include "services/service.h"

namespace Services
{
  class ServiceList;
};
class TSLDataLayer;

class ServiceWizard : public QWizard, private Ui_serviceWizard, public Services::Service::ServiceCredentialsCallback
{
    Q_OBJECT
public:
    ServiceWizard( Services::ServiceList *serviceList, TSLDataLayer *coordinateProvidingLayer, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~ServiceWizard();

    // Called by the ActionTypePage wizard page when the type of service to connect to
    // has been decided.
    void setService( Services::Service *newService );

    virtual void accept();
    virtual void reject();

    virtual void onCredentialsRequired (TSLSimpleString& username, TSLSimpleString& password);

signals:
    void signalShowCredentialsDialog( QString* username, QString* password );

private slots:
    void showCredentialsDialog( QString* username, QString* password );

private:
    void recordLoadedServices();

    Services::Service *m_connectedService;
    Services::ServiceList *m_serviceList;
    TSLDataLayer *m_coordinateProvidingLayer;

    QMutex m_mutex;
    QWaitCondition m_waitCond;
};

#endif // MAINWINDOW_H
