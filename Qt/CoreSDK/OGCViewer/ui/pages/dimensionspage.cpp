/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "dimensionspage.h"
#include "servicewizardpageenum.h"
#include <QMessageBox>

using namespace Services;

DimensionsPage::DimensionsPage(QWidget *parent)
  : QWizardPage(parent)
  , m_service( NULL )
  , m_loadInProgress( false )
  , m_showCoordSysSelectionPage( false )
{
  setupUi(this);

  setCommitPage( true );

  connect( this, SIGNAL(signalShowCoordinateSystemPage()), this, SLOT(showCoordinateSystemPage()) );
  connect( this, SIGNAL(signalShowNextPage()), this, SLOT(showNextPage()) );
}

DimensionsPage::~DimensionsPage()
{
}

void DimensionsPage::initializePage()
{
  m_loadInProgress = false;
  m_showCoordSysSelectionPage = false;

  // Ensure we recieve any callbacks from the service object so we can update the page accordingly
  m_service->pushCallbackObject( this );

  Service::ServiceDimensionsModel *dimensionModel = m_service->getDimensionsModel();
  dimensionModel->setParent( dimensionList );
  dimensionList->setModel( dimensionModel );

  // We must defer this until after calling setModel or there will be no selection model on the treeview
  connect( dimensionList->selectionModel(), SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
           this, SLOT(dimensionSelected(const QModelIndex&, const QModelIndex&) ) );
  connect( dimensionModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)),
           this, SLOT(dimensionValueChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)) );

  Service::ServiceDimensionInfoModel *dimensionInfoModel = m_service->getDimensionInfoModel();
  dimensionInfoModel->setParent( dimensionInfoTable );
  dimensionInfoTable->setModel( dimensionInfoModel );

  // Make the model add the appropriate delegates to the view for special editing of particular dimension
  // types (e.g. dates)
  dimensionModel->setDelegates( dimensionList );
}

bool DimensionsPage::validatePage()
{
  if( !m_loadInProgress )
  {
    // Otherwise we do not want to allow the page to be advanced directly as we need the
    // service loading thread to tell us if we need to show the coordinate system selection page or not.
    m_loadInProgress = true;
    m_service->advanceConnectionSequence();

    // Disable the next button - the page will be advanced programmatically when we get a callback from
    // the service load thread
    emit completeChanged();

    return false;
  }

  // We have had the next callback from the loading thread, allow the wizard to advance to the next page
  m_loadInProgress = false;
  return true;
}

int DimensionsPage::nextId() const
{
  // The next page is either the coordinate system selection page or the final service options page,
  // depending on whether the user has a choice of coordinate systems or not
  if( m_showCoordSysSelectionPage )
  {
    return SelectCoordSystem;
  }

  switch( m_service->type() )
  {
  case ServiceTypeWMS:
    return WMSServiceOptions;

  case ServiceTypeWMTS:
    return WMTSServiceOptions;

  default:
    return -1;
  }
}

void DimensionsPage::cleanupPage()
{
  // Navigating back from this page means that the model no longer correspond to any valid data,
  // clear it out.
  dimensionList->setModel( NULL );

  // This page and the layer selection page are part of the same service callback, so
  // don't send a cancel message to the service loading thread

  m_loadInProgress = false;
  m_showCoordSysSelectionPage = false;

  m_service->popCallbackObject();

  QWizardPage::cleanupPage();
}

bool DimensionsPage::isComplete() const
{
  if( m_loadInProgress )
  {
    // Disable the next button to prevent multiple signals being sent to the service loading thread
    return false;
  }

  // Require at least one layer to be selected before the next page can be shown
  Service::ServiceDimensionsModel *dimensionsModel = (Service::ServiceDimensionsModel *)dimensionList->model();
  if( dimensionsModel )
  {
    return dimensionsModel->configurationValid();
  }

  return false;
}

void DimensionsPage::showCoordinateSystemPage()
{
  m_showCoordSysSelectionPage = true;

  emit showNextPage();
}

void DimensionsPage::showNextPage()
{
  // Signal the wizard to advance to the next page
  wizard()->next();
}

void DimensionsPage::dimensionSelected( const QModelIndex &current, const QModelIndex &/*previous*/ )
{
  Service::ServiceDimensionInfoModel *dimensionInfoModel = 
                    reinterpret_cast< Service::ServiceDimensionInfoModel* >( dimensionInfoTable->model() );

  dimensionInfoModel->setSelectedDimension( current );

  // Ensure all rows are large enough to display the full contents - some items may be multi-line
  // so the default sizes may not be large enough
  dimensionInfoTable->resizeRowsToContents();
}

void DimensionsPage::dimensionValueChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &/*roles*/ )
{
  if( topLeft.isValid() && bottomRight.isValid() )
  {
    // Only forward the signal if the items changed are valid
    emit completeChanged();
  }
}

void DimensionsPage::onError( const std::string &message )
{
  QMessageBox::critical( this, tr( "Error Configuring Service Settings" ), message.c_str() );
}

void DimensionsPage::onNextSequenceAction()
{
  emit signalShowNextPage();
}

void DimensionsPage::coordinateSystemChoiceRequired()
{
  emit signalShowCoordinateSystemPage();
}
