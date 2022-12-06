/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "editdimensiondialog.h"
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QBoxLayout>
#include <QComboBox>
#include <vector>

#include "services/servicelist.h"

#include "tsltmsapi.h"
#include "tslatomic.h"
#include "tslplatformhelper.h"

using namespace Services;

EditDimensionDialog::EditDimensionDialog( ServiceList *serviceList, Service *service, const QString &dimensionName,
                                          const char *dimensionUnits, QWidget *parent )
  : QDialog( parent )
  , m_serviceList( serviceList )
  , m_service( service )
  , m_dateEdit( NULL )
  , m_generalEdit( NULL )
  , m_comboEdit( NULL )
  , m_layout( new QVBoxLayout() )
{
  setupUi( this );

  m_dimensionModel = m_service->getDimensionsModel();

  Service::ServiceDimensionInfoModel *dimensionInfoModel = m_service->getDimensionInfoModel();
  dimensionInfoModel->setParent( dimensionInfoTable );
  dimensionInfoTable->setModel( dimensionInfoModel );

  m_dimensionModelIndex = m_dimensionModel->findDimensionByName( dimensionName );
  dimensionInfoModel->setSelectedDimension( m_dimensionModelIndex );

  // Ensure all rows are large enough to display the full contents - some items may be multi-line
  // so the default sizes may not be large enough
  dimensionInfoTable->resizeRowsToContents();

  QVariant currentDimensionValue( m_dimensionModel->data( m_dimensionModelIndex, Qt::EditRole ) );
  
  // Since we want to use different editing widget types depending on the units of the dimension,
  // we programmatically generate the editing widget here rather than adding it in the designer
  editLocatorWidget->setLayout( m_layout );

  if( TSLPlatformHelper::stricmp( dimensionUnits, "ISO8601" ) == 0 )
  {
    m_dateEdit = new QDateTimeEdit();
    m_dateEdit->setTimeSpec( Qt::UTC );
    m_dateEdit->setDateTime( QDateTime::fromString( currentDimensionValue.toString(), Qt::ISODate ) );
    m_dateEdit->setCalendarPopup( true );

    m_layout->addWidget( m_dateEdit );
  }
  else
  {
    std::vector< const char* > dimensionValues;
    bool allowsUserValues = m_dimensionModel->getPossibleValues( m_dimensionModelIndex, dimensionValues );
    if( dimensionValues.empty() )
    {
      // If the service doesn't specify any allowed values, just present a basic text edit box
      m_generalEdit = new QLineEdit();
      m_generalEdit->setText( currentDimensionValue.toString() );

      m_layout->addWidget( m_generalEdit );
    }
    else
    {
      // Otherwise, present a combobox populated with the possible values
      m_comboEdit = new QComboBox();

      size_t numDimensionValues =  dimensionValues.size();
      for( size_t i = 0; i < numDimensionValues; ++i )
      {
        m_comboEdit->addItem( QString::fromUtf8( dimensionValues[i] ) );
      }

      m_comboEdit->setCurrentText( currentDimensionValue.toString() );
      m_comboEdit->setEditable( allowsUserValues );

      m_layout->addWidget( m_comboEdit );
    }
  }
}

EditDimensionDialog::~EditDimensionDialog()
{
  delete m_dateEdit;
  delete m_generalEdit;
  delete m_comboEdit;
  delete m_layout;
  delete m_dimensionModel;
}

void EditDimensionDialog::accept()
{
  // Propogate the dimension value from the edit widget back into the model, which will update the
  // data layer for us
  if( m_dateEdit )
  {
     m_dimensionModel->setData( m_dimensionModelIndex, m_dateEdit->dateTime().toString( Qt::ISODate ), Qt::EditRole );
  }
  else if( m_generalEdit )
  {
    m_dimensionModel->setData( m_dimensionModelIndex, m_generalEdit->text(), Qt::EditRole );
  }
  else if( m_comboEdit )
  {
    m_dimensionModel->setData( m_dimensionModelIndex, m_comboEdit->currentText(), Qt::EditRole );
  }

  // Since we've changed the value of a dimension, ask for a redraw of the relevant drawing surfaces
  m_serviceList->redrawAttachedSurface();

  QDialog::accept();
}
