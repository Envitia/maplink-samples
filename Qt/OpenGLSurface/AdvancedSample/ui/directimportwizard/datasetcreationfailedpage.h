/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DATASETCREATIONFAILEDPAGE_H
#define DATASETCREATIONFAILEDPAGE_H

#include <QWizardPage>
#include "ui_datasetcreationfailedpage.h"


// This class a page of the direct import wizard that is displayed with information
// describing any errors if the dataset creation failed.

class DataSetCreationFailedPage : public QWizardPage, private Ui_DataSetCreationFailedPage
{
  Q_OBJECT

public:
  DataSetCreationFailedPage(QWidget *parent = 0);

  virtual void initializePage();
  virtual int nextId() const;

};

#endif // DATASETCREATIONFAILEDPAGE_H
