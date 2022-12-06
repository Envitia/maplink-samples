/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SELECTCOORDSYSPAGE_H
#define SELECTCOORDSYSPAGE_H

#include <QWizardPage>
#include "ui_coordinatesystemselect.h"
#include "services/service.h"

#include "tgmapidll.h"
#include "tslsimplestring.h"

// A page of the service wizard that lets the user choose the coordinate system to
// display the service in.
// This page may not be shown if the layers the user chose in a previous page only have
// one possible coordinate system

class SelectCoordSysPage : public QWizardPage, public Services::Service::ServiceActionCallback, private Ui_coordinateSystemPage
{
    Q_OBJECT
public:
    SelectCoordSysPage(QWidget *parent = 0);
    virtual ~SelectCoordSysPage();

    virtual void initializePage();
    virtual bool validatePage();
    virtual bool isComplete() const;
    virtual void cleanupPage();

    virtual int nextId() const;

    void setService( Services::Service *service );

signals:
    void signalShowServiceOptionsPage();
    void signalShowError( const QString &message );

private slots:
  void showServiceOptionsPage();
  void showError( const QString& message );

private:
  // Callbacks made from the data layer being used to connect to the service. These will be made from a seperate thread
  // and require some user interface action.
  virtual void onError( const std::string &message);
  virtual void onNextSequenceAction();

  Services::Service *m_service;
  bool m_loadInProgress;
};

inline void SelectCoordSysPage::setService( Services::Service *service )
{
  m_service = service;
}

#endif // SELECTCOORDSYSPAGE_H
