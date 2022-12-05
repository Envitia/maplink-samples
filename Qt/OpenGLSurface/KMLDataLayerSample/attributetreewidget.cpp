/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include "attributetreewidget.h"


AttributeTreeWidget::AttributeTreeWidget(QWidget *parent)
: m_initialised(false)
{

}

AttributeTreeWidget::~AttributeTreeWidget()
{

}

void AttributeTreeWidget::AddEntitySet( const TSLEntitySet* set, QTreeWidgetItem* item )
{
  // This will display the hierarchy of the entitySet in the tree widget
  // Along with any attributes stored in the data.

  // Takes the TSLEntitySet and populates the treeview with its contents.
  //
  // The contents of each TSLEntity's dataset is displayed.

  if( item == NULL )
  {
    QTreeWidgetItem* topItem = new QTreeWidgetItem();
    topItem->setText(0, QString( set->name() ) );
    topItem->setText(1, QString( "" ) );

    const TSLDataSet* dataSet = set->dataSet();
    if( dataSet )
    {
      for( int j(0); j<dataSet->numAvailableFields(); ++j )
      {
        TSLSimpleString field;
        dataSet->getAvailableField( j, field );
        const TSLVariant *variant = dataSet->getData( field.c_str() );
        QString str = "";
        if( variant )
        {
          int len = variant->getValueAsString( 0, 0, 0 ) ;
          char * buf = new char[ len + 1 ] ;
          variant->getValueAsString( buf, len ) ;
          str = buf ;
          delete[] buf ;
        }

        QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
        fieldItem->setText( 0, QString( field.c_str() ) );
        fieldItem->setText( 1, str );
        topItem->addChild( fieldItem );
      }
    }

    addTopLevelItem( topItem );
    item = topItem;
    item->setExpanded(true);
  }

  QTreeWidgetItem *newItem = NULL;

  for( int i(0); i < set->size(); ++i )
  {
    const TSLEntity* entity = (*set)[i];

    newItem = new QTreeWidgetItem();
    newItem->setText(0, QString( entity->name() ) );
    newItem->setText(1, QString( "" ) );

    item->addChild( newItem );
    

    if( entity )
    {
      const TSLDataSet* dataSet = entity->dataSet();
      if( dataSet )
      {
        for( int j(0); j<dataSet->numAvailableFields(); ++j )
        {
          TSLSimpleString field;
          dataSet->getAvailableField( j, field );
          const TSLVariant *variant = dataSet->getData( field.c_str() );
          QString str = "";
          if( variant )
          {
            int len = variant->getValueAsString( 0, 0, 0 ) ;
            char * buf = new char[ len + 1 ] ;
            variant->getValueAsString( buf, len ) ;
            str = buf ;
            delete[] buf ;
          }

          QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
          fieldItem->setText( 0, QString( field.c_str() ) );
          fieldItem->setText( 1, str );
          newItem->addChild( fieldItem );
        }
      }

      if( entity->type() == TSLGeometryTypeEntitySet )
      {
        AddEntitySet( (const TSLEntitySet*)entity, newItem );
      }
    }
  }
}
