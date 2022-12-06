/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "selectlayerspage.h"
#include "servicewizardpageenum.h"
#include "services/service.h"
#include "services/servicelayerpreview.h"
#include <QMessageBox>

using namespace Services;

SelectLayersPage::SelectLayersPage(QWidget *parent)
  : QWizardPage(parent)
  , m_service( NULL )
  , m_previewHelper( NULL )
  , m_loadInProgress( false )
  , m_showCoordSysSelectionPage( false )
{
  setupUi(this);

  setCommitPage( true );

  connect( this, SIGNAL(signalShowCoordinateSystemPage()), this, SLOT(showCoordinateSystemPage()) );
  connect( this, SIGNAL(signalShowNextPage()), this, SLOT(showNextPage()) );
  connect( this, SIGNAL(signalShowError(const QString&)), this, SLOT(showError(const QString&)));
}

SelectLayersPage::~SelectLayersPage()
{
  delete m_previewHelper;
}

void SelectLayersPage::initializePage()
{
  // Ensure we recieve any callbacks from the service object so we can update the page accordingly
  m_service->pushCallbackObject( this );

  QAbstractItemModel *layersModel = m_service->getServiceLayerModel();
  layersModel->setParent( serviceLayersTree );
  serviceLayersTree->setModel( layersModel );

  // We must defer this until after calling setModel or there will be no selection model on the treeview
  connect( serviceLayersTree->selectionModel(), SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
           this, SLOT( layerSelected(const QModelIndex&, const QModelIndex&) ) );
  connect( layersModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)),
           this, SLOT(layerStatusChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)) );

  serviceLayersTree->expandAll();

  Service::ServiceLayerInfoModel *layerInfoModel = m_service->getServiceLayerInfoModel();
  layerInfoModel->setParent( layerInformation );
  layerInformation->setModel( layerInfoModel );

  Service::ServiceLayerStylesModel *layerStylesModel = m_service->getServiceLayerStyleModel();
  layerStylesModel->setParent( layerStyles );
  layerStyles->setModel( layerStylesModel );
  connect( layerStyles, SIGNAL(activated(const QString&)), this, SLOT(layerStyleChanged(const QString&)) );

  // Get the helper object that handles displaying layer previews for us
  if( m_previewHelper )
  {
    // Make sure we have the right helper type for this service - If the user previously navigated back to the
    // start of the wizard and changed service types then the old one would no longer work. It is easiest to just
    // get rid of any old one and start from fresh.
    delete m_previewHelper;
  }
  m_previewHelper = m_service->getLayerPreviewHelper();
  m_previewHelper->setSurfaceWidget( layerPreviewWidget );
}

bool SelectLayersPage::validatePage()
{
  if( nextId() == SetDimensions )
  {
    // If we are advancing to the dimensions page we do not need to do any
    // additional work
    return true;
  }

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

int SelectLayersPage::nextId() const
{
  // If any enabled layers advertise dimensions, the next page is the dimensions page. Otherwise
  // we need to signal the service callback thread to determine if we need to show the coordinate system
  // selection page or not.
  Service::ServiceLayerModel *layerInfoModel = (Service::ServiceLayerModel *)serviceLayersTree->model();
  if( layerInfoModel->selectionsHaveDimensions() )
  {
    return SetDimensions;
  }

  // If we're not showing the dimensions page, then the next page is either the coordinate system
  // selection page or the final service options page
  if( m_showCoordSysSelectionPage )
  {
    return SelectCoordSystem;
  }

  // Otherwise the next page is the service-specific options page
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

void SelectLayersPage::cleanupPage()
{
  // Navigating back from this page means that the models no longer correspond to any valid data,
  // clear them out.
  serviceLayersTree->setModel( NULL );
  layerInformation->setModel( NULL );

  // Tell the service loading thread to cancel the current connection sequence as navigating back
  // from this page requires starting again.
  m_service->cancelSequence();

  m_loadInProgress = false;
  m_showCoordSysSelectionPage = false;

  delete m_previewHelper;
  m_previewHelper = NULL;

  m_service->popCallbackObject();

  QWizardPage::cleanupPage();
}

bool SelectLayersPage::isComplete() const
{
  if( m_loadInProgress )
  {
    // Disable the next button to prevent multiple signals being sent to the service loading thread
    return false;
  }

  // Require at least one layer to be selected before the next page can be shown
  Service::ServiceLayerModel *layerInfoModel = (Service::ServiceLayerModel *)serviceLayersTree->model();
  if( layerInfoModel )
  {
    return layerInfoModel->configurationValid();
  }

  return false;
}

void SelectLayersPage::layerSelected( const QModelIndex &current, const QModelIndex& /*previous*/ )
{
  Service::ServiceLayerInfoModel *layerInfoModel = reinterpret_cast<Service::ServiceLayerInfoModel*>( layerInformation->model() );
  layerInfoModel->setSelectedLayer( current );

  Service::ServiceLayerStylesModel *layerStylesModel = reinterpret_cast<Service::ServiceLayerStylesModel*>( layerStyles->model() );
  layerStylesModel->setSelectedLayer( current );

  if( layerStylesModel->rowCount() != 0 )
  {
    layerStyles->setEnabled( true );

    const char *currentStyle = layerStylesModel->currentStyle();
    if( currentStyle )
    {
      // If a style is currently selected, make it the item selected in the combobox
      int index = layerStyles->findText( QString::fromUtf8( currentStyle ) );
      layerStyles->setCurrentIndex( index );
    }
  }
  else
  {
    // Disable the combo box if there are no styles for this layer
    layerStyles->setEnabled( false );
  }

  // Ensure all rows are large enough to display the full contents - some items may be multi-line
  // so the default sizes may not be large enough
  layerInformation->resizeRowsToContents();

  // Ask the preview helper to show what this layer looks like
  m_previewHelper->showLayerPreview( current, layerStyles->currentText().toUtf8() );
}

void SelectLayersPage::layerStatusChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>& /*roles*/ )
{
  if( topLeft.isValid() && bottomRight.isValid() )
  {
    // Only forward the signal if the items changed are valid
    emit completeChanged();
  }
}

void SelectLayersPage::layerStyleChanged(const QString &text)
{
  Service::ServiceLayerStylesModel *layerStylesModel = reinterpret_cast<Service::ServiceLayerStylesModel*>( layerStyles->model() );
  QByteArray styleName( text.toUtf8() );
  layerStylesModel->setStyle( !text.isEmpty() ? styleName.constData() : NULL );

  // Ask the preview helper to show what this style looks like
  m_previewHelper->showLayerPreview( serviceLayersTree->currentIndex(), styleName.constData() );
}

void SelectLayersPage::showCoordinateSystemPage()
{
  m_showCoordSysSelectionPage = true;

  emit showNextPage();
}

void SelectLayersPage::showNextPage()
{
  // Signal the wizard to advance to the next page
  wizard()->next();
}

void SelectLayersPage::showError( const QString& message )
{
  // Reset the state of the page so that the user can try a different URL
  m_loadInProgress = false;

  // Show the error message after hiding the connecting animation
  QMessageBox::critical( this, tr( "Error Configuring Service Settings" ), message );
}

void SelectLayersPage::onError( const std::string &message )
{
  // Send a signal to perform the necessary UI updates in the correct thread
  emit signalShowError( QString::fromUtf8( message.c_str() ) );
}

void SelectLayersPage::onNextSequenceAction()
{
  emit signalShowNextPage();
}

void SelectLayersPage::coordinateSystemChoiceRequired()
{
  emit signalShowCoordinateSystemPage();
}
