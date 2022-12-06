/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "layerpropertiesdialog.h"
#include "services/servicelist.h"
#include "services/service.h"

using namespace Services;

LayerPropertiesDialog::LayerPropertiesDialog( ServiceList *serviceList, Service *service, QWidget *parent )
  : QDialog( parent )
  , m_serviceList( serviceList )
  , m_service( service )
{
  setupUi( this );

  m_originalTransparency = m_serviceList->getServiceDataLayerTransparency( service );
  transparencySlider->setValue( m_originalTransparency );

  connect( transparencySlider, SIGNAL(valueChanged(int)), this, SLOT(transparencyChanged(int)) );
}

LayerPropertiesDialog::~LayerPropertiesDialog()
{
}

void LayerPropertiesDialog::reject()
{
  // Undo any transparency changes that were made
  m_serviceList->setServiceDataLayerTransparency( m_service, m_originalTransparency );
  QDialog::reject();
}

void LayerPropertiesDialog::transparencyChanged( int value )
{
  m_serviceList->setServiceDataLayerTransparency( m_service, value );
}
