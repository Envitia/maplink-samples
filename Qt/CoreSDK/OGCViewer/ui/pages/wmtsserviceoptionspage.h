/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WMTSSERVICEOPTIONSPAGE_H
#define WMTSSERVICEOPTIONSPAGE_H

#include <QWizardPage>
#include "ui_wmtsserviceoptions.h"

namespace Services
{
  class WMTSService;
};

// A page of the service wizard that offers general settings for Web Map Tile Services

class WMTSServiceOptionsPage : public QWizardPage, private Ui_wmtsServiceOptionsPage
{
    Q_OBJECT
public:
    WMTSServiceOptionsPage(QWidget *parent = 0);
    virtual ~WMTSServiceOptionsPage();

    virtual void initializePage();
    virtual int nextId() const;

    void setService( Services::WMTSService *service );

private slots:
  void tileLoadOrderSettingChanged( int newIndex );
  void imageFormatChanged( int newIndex );

private:
  Services::WMTSService *m_service;
  QColor m_backgroundColour;
};

inline void WMTSServiceOptionsPage::setService( Services::WMTSService *service )
{
  // This page only applies to WMTS services
  m_service = service;
}

#endif // WMSSERVICEOPTIONSPAGE_H
