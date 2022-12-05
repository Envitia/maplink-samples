/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WMSSERVICEOPTIONSPAGE_H
#define WMSSERVICEOPTIONSPAGE_H

#include <QWizardPage>
#include "ui_wmsserviceoptions.h"

namespace Services
{
  class WMSService;
};

// A page of the service wizard that offers general settings for Web Map Services

class WMSServiceOptionsPage : public QWizardPage, private Ui_wmsServiceOptionsPage
{
    Q_OBJECT
public:
    WMSServiceOptionsPage(QWidget *parent = 0);
    virtual ~WMSServiceOptionsPage();

    virtual void initializePage();
    virtual int nextId() const;

    void setService( Services::WMSService *service );

private slots:
  void tileLevelSettingChanged( int newIndex );
  void tileLoadOrderSettingChanged( int newIndex );
  void setBackgroundColourButtonState( int enabled );
  void setBackgroundColour();
  void setNewBackgroundColour( const QColor &newColour );
  void setTransparentRequests( int enabled );
  void imageFormatChanged( int newIndex );

private:
  Services::WMSService *m_service;
  QColor m_backgroundColour;
};

inline void WMSServiceOptionsPage::setService( Services::WMSService *service )
{
  // This page only applies to WMS services
  m_service = reinterpret_cast< Services::WMSService* >( service );
}

#endif // WMSSERVICEOPTIONSPAGE_H
