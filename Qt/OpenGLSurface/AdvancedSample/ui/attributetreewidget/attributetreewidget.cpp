/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "attributetreewidget.h"

#include "MapLink.h"



  AttributeTreeWidget::AttributeTreeWidget( QWidget *parent )
: m_initialised( false )
{

}

AttributeTreeWidget::~AttributeTreeWidget()
{

}

void AttributeTreeWidget::addEntitySet( const TSLEntitySet* set, unsigned int& numAdded, QTreeWidgetItem* item )
{
  // This will display the hierarchy of the entitySet in the tree widget
  // Along with any attributes stored in the data.

  // Takes the TSLEntitySet and populates the treeview with its contents.
  //
  // The contents of each TSLEntity's dataset is displayed.

  if (!set)
  {
    return;
  }

  if( item == NULL )
  {
    QTreeWidgetItem* topItem = new QTreeWidgetItem();
    const char* name( set->name() );
    if( name == NULL || name[0] == '\0' )
    {
      topItem->setText( 0, QString( "Unnamed Entity Set" ) );
    }
    else
    {
      topItem->setText( 0, QString( name ) );
    }
    topItem->setText( 1, QString( "" ) );

    const TSLDataSet* dataSet = set->dataSet();
    if( dataSet )
    {
      for( int j( 0 ); j < dataSet->numAvailableFields(); ++j )
      {
        TSLSimpleString field;
        dataSet->getAvailableField( j, field );
        const TSLVariant *variant = dataSet->getData( field.c_str() );
        QString str = "";
        if( variant )
        {
          int len = variant->getValueAsString( 0, 0, 0 );
          char * buf = new char[len + 1];
          variant->getValueAsString( buf, len );
          str = buf;
          delete[] buf;
        }

        QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
        fieldItem->setText( 0, QString( field.c_str() ) );
        fieldItem->setText( 1, str );
        topItem->addChild( fieldItem );
      }
    }

    addTopLevelItem( topItem );
    item = topItem;
    item->setExpanded( true );
    ++numAdded;
  }

  QTreeWidgetItem *newItem = NULL;

  for( int i( 0 ); i < set->size(); ++i )
  {
    const TSLEntity* entity = ( *set )[i];

    newItem = addEntity( entity, numAdded, item );

    if( entity->type() == TSLGeometryTypeEntitySet )
    {
      addEntitySet( (const TSLEntitySet*)entity, numAdded, newItem );
    }
  }
}

QTreeWidgetItem* AttributeTreeWidget::addEntity( const TSLEntity* ent, unsigned int& numAdded, QTreeWidgetItem* parentItem )
{
  if( !ent )
  {
    return NULL;
  }

  QTreeWidgetItem* newItem = new QTreeWidgetItem();
  const char* name( ent->name() );
  if( name == NULL || name[0] == '\0' )
  {
    newItem->setText( 0, QString( "Unnamed Entity" ) );
  }
  else
  {
    newItem->setText( 0, QString( name ) );
  }
  newItem->setText( 1, QString( "" ) );

  if( ent->type() == TSLGeometryTypeText )
  {
    const TSLText* text( reinterpret_cast<const TSLText*>( ent ) );
    QTreeWidgetItem* valueItem = new QTreeWidgetItem();
    const char* textVal( text->value() );
    valueItem->setText( 0, QString( "Value" ) );
    valueItem->setText( 1, textVal ? textVal : "" );
    newItem->addChild( valueItem );
  }
  else if( ent->type() == TSLGeometryTypeGeodeticText )
  {
    const TSLGeodeticText* text( reinterpret_cast<const TSLGeodeticText*>( ent ) );
    QTreeWidgetItem* valueItem = new QTreeWidgetItem();
    const char* textVal( text->value() );
    valueItem->setText( 0, QString( "Value" ) );
    valueItem->setText( 1, textVal ? textVal : "" );
    newItem->addChild( valueItem );
  }

  const TSLDataSet* dataSet = ent->dataSet();
  if( dataSet )
  {
    for( int j( 0 ); j < dataSet->numAvailableFields(); ++j )
    {
      TSLSimpleString field;
      dataSet->getAvailableField( j, field );
      const TSLVariant *variant = dataSet->getData( field.c_str() );
      QString str = "";
      if( variant )
      {
        int len = variant->getValueAsString( 0, 0, 0 );
        char * buf = new char[len + 1];
        variant->getValueAsString( buf, len );
        str = buf;
        delete[] buf;
      }

      QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
      fieldItem->setText( 0, QString( field.c_str() ) );
      fieldItem->setText( 1, str );
      newItem->addChild( fieldItem );
    }
  }

  if( parentItem )
  {
    parentItem->addChild( newItem );
  }
  else
  {
    addTopLevelItem( newItem );
  }

  ++numAdded;
  return newItem;
}

void AttributeTreeWidget::addEntityIterator( TSLEntityIterator* it )
{
  if (!it)
  {
    return;
  }

  const TSLEntity* ent( it->getNextEntity() );
  while( ent )
  {
    unsigned int numAdded(0);
    if ( const TSLEntitySet* es = TSLEntitySet::isEntitySet(ent) )
    {
      addEntitySet( es, numAdded );
    }
    else
    {
      addEntity( ent, numAdded );
    }
    if( numAdded )
    {
      while( ent && numAdded )
      {
        --numAdded;
        ent = it->getNextEntity();
      }
    }
    else
    {
      ent = it->getNextEntity();
    }
  }
}
