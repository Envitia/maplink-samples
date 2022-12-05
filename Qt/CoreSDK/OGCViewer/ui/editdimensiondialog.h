/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef EDITDIMENSIONDIALOG_H
#define EDITDIMENSIONDIALOG_H

#include "ui_editdimensiondialog.h"
#include "services/service.h"

class QDateTimeEdit;
class QLineEdit;
class QVBoxLayout;
class QComboBox;

namespace Services
{
  class ServiceList;
};

// A Qt dialog that provides the ability to edit WMS or WMTS layer dimensions.

class EditDimensionDialog : public QDialog, public Ui_editDimensionDialog
{
  Q_OBJECT
public:
  EditDimensionDialog( Services::ServiceList *serviceList, Services::Service *service, const QString &dimensionName, const char *dimensionUnits, QWidget *parent = 0 );
  virtual ~EditDimensionDialog();

  virtual void accept();

private:
  Services::ServiceList *m_serviceList;
  Services::Service *m_service;
  Services::Service::ServiceDimensionsModel *m_dimensionModel;

  QModelIndex m_dimensionModelIndex;

  QDateTimeEdit *m_dateEdit;
  QLineEdit *m_generalEdit;
  QVBoxLayout *m_layout;
  QComboBox *m_comboEdit;
};

#endif
