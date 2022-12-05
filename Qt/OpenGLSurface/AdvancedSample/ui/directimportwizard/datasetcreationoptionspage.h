/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DATASETCREATIONOPTIONSPAGE_H
#define DATASETCREATIONOPTIONSPAGE_H

#include <QWizardPage>
#include "ui_datasetcreationoptionspage.h"

#include "wizardpageenum.h"

// Direct import wizard page that creates the datasets.

class DataSetCreationOptionsPage : public QWizardPage, private Ui_datasetcreationoptionspage
{
  Q_OBJECT

public:
  DataSetCreationOptionsPage(QWidget *parent = 0);

  virtual void initializePage();
  virtual bool validatePage();
  virtual int nextId() const;

private:
  DirectImportWizardPage m_nextPage;
};

#endif // DATASETCREATIONOPTIONSPAGE_H

