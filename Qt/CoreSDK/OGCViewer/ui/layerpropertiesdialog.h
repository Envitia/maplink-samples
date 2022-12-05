/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef LAYERPROPERTIESDIALOG_H
#define LAYERPROPERTIESDIALOG_H

#include "ui_layerproperties.h"
#include "tslatomic.h"

namespace Services
{
  class ServiceList;
  class Service;
};

// A Qt dialog that provides settings for a WMS or WMTS layer. Currently this
// consists of only layer transparency.

class LayerPropertiesDialog : public QDialog, public Ui_layerPropertiesDialog
{
  Q_OBJECT
public:
  LayerPropertiesDialog( Services::ServiceList *serviceList, Services::Service *service, QWidget *parent = 0 );
  virtual ~LayerPropertiesDialog();

  virtual void reject();

private slots:
  void transparencyChanged( int value );

private:
  Services::ServiceList *m_serviceList;
  Services::Service *m_service;
  TSLPropertyValue m_originalTransparency;
};

#endif
