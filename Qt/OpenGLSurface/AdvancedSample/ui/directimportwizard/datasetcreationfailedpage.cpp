/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "datasetcreationfailedpage.h"
#include "ui_datasetcreationfailedpage.h"

#include "MapLink.h"

DataSetCreationFailedPage::DataSetCreationFailedPage(QWidget *parent)
  : QWizardPage(parent)
{
  setupUi(this);
}

int DataSetCreationFailedPage::nextId() const
{
  return -1;
}

void DataSetCreationFailedPage::initializePage()
{
  // Dataset creation has failed. Display the contents of the
  // MapLink error stack to provide more information about the failure
  // A more complicated example woul use the MapLink error callback to maintain
  // an application log window.
  TSLSimpleString errStr;
  if( TSLThreadedErrorStack::errorString(errStr) )
  {
    errorLogTextEdit->setText( errStr.c_str() );
  }

  emit completeChanged();
}
