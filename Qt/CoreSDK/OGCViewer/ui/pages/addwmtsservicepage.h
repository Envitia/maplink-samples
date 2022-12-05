/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef ADDWMTSSERVICEPAGE_H
#define ADDWMTSSERVICEPAGE_H

#include <QWizardPage>
#include "ui_wmtsserviceaddress.h"
#include "services/wmts/wmtsservice.h"
#include "tslsimplestring.h"

// A page of the service wizard that lets the user enter the
// service address of the Web Map Tiling Service they want to connect to

class AddWMTSServicePage : public QWizardPage, public Services::Service::ServiceActionCallback, private Ui_addWMTSServicePage
{
    Q_OBJECT
public:
    AddWMTSServicePage(QWidget *parent = 0);
    virtual ~AddWMTSServicePage();

    virtual void initializePage();
    virtual bool isComplete() const;
    virtual bool validatePage();
    virtual void cleanupPage();
    virtual int nextId() const;

    void setService( Services::WMTSService *service );

signals:
    void signalShowError( const QString &message );
    void signalServiceConnectionEstablished();

private slots:
    void serviceURLChanged( const QString &url );
    void serviceURLSelectionChanged( int seletedIndex );
    void showError( const QString& message );
    void serviceConnectionEstablished();

private:
  // Callbacks made from the data layer being used to connect to the service. These will be made from a seperate thread
  // and require some user interface action.
  virtual void onError( const std::string &message);
  virtual void onNextSequenceAction();

  QMovie *m_loadingMovie;
  bool m_loadInProgress;
  bool m_serviceLoaded;

  bool m_editInProgress;

  Services::WMTSService *m_service;
};

inline void AddWMTSServicePage::setService( Services::WMTSService *service )
{
  m_service = service;
}

#endif // ADDSERVICEPAGE_H
