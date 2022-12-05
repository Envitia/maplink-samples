/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef ADDSERVICEPAGE_H
#define ADDSERVICEPAGE_H

#include <QWizardPage>
#include "ui_wmsserviceaddress.h"
#include "services/wms/wmsservice.h"
#include "tslsimplestring.h"

// A page of the service wizard that lets the user enter the
// service address of the Web Map Service they want to connect to

class AddWMSServicePage : public QWizardPage, public Services::Service::ServiceActionCallback, private Ui_addWMSServicePage
{
    Q_OBJECT
public:
    AddWMSServicePage(QWidget *parent = 0);
    virtual ~AddWMSServicePage();

    virtual void initializePage();
    virtual bool isComplete() const;
    virtual bool validatePage();
    virtual void cleanupPage();
    virtual int nextId() const;

    void setService( Services::WMSService *service );

signals:
    void signalShowError( const QString &message );
    void signalServiceConnectionEstablished();

private slots:
    void serviceURLChanged( const QString &url );
    void serviceURLSelectionChanged( int seletedIndex );
    void showError( const QString& message );
    void serviceConnectionEstablished();
    void initialWMSVersionChanged( int index );
    void axisInversionChanged( int state );

private:
  // Callbacks made from the data layer being used to connect to the service. These will be made from a seperate thread
  // and require some user interface action.
  virtual void onError( const std::string &message);
  virtual void onNextSequenceAction();

  QMovie *m_loadingMovie;
  bool m_loadInProgress;
  bool m_serviceLoaded;

  bool m_editInProgress;

  Services::WMSService *m_service;
};

inline void AddWMSServicePage::setService( Services::WMSService *service )
{
  m_service = service;
}

#endif // ADDSERVICEPAGE_H
