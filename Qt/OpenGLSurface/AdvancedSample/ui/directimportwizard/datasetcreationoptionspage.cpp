/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "datasetcreationoptionspage.h"
#include "directimportwizard.h"

#include "MapLink.h"

DataSetCreationOptionsPage::DataSetCreationOptionsPage(QWidget *parent)
  : QWizardPage(parent)
  , m_nextPage( DirectImportWizardDataSetInformationPage )
{
  setupUi(this);
}

void DataSetCreationOptionsPage::initializePage()
{
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));
  dataPathLineEdit->setText( wiz->dataPath().c_str() );

  emit completeChanged();
}

bool DataSetCreationOptionsPage::validatePage()
{
  // Trigger creation of the datasets
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));

  bool includeOverviews( optionsIncludeOverviewsCheckBox->checkState() == Qt::Checked );

  // Clear the error stack.
  // If dataset creation fails the next page will display the contents
  TSLThreadedErrorStack::clear();
  wiz->createDataSets( includeOverviews ? TSLDirectImportDriver::OverviewTypeAny : TSLDirectImportDriver::OverviewTypeNone );

  const TSLvector<TSLDirectImportDataSet*>* dataSets( wiz->dataSets() );
  if( !dataSets )
  {
    return false;
  }

  if( dataSets->empty() )
  {
    m_nextPage = DirectImportWizardDataSetCreationFailedPage;
  }

  return true;
}

int DataSetCreationOptionsPage::nextId() const
{
  return m_nextPage;
}
