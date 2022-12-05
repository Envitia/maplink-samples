/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef ATTRIBUTETREEWIDGET_H
#define ATTRIBUTETREEWIDGET_H

#include <qtreewidget.h>

class TSLEntitySet;
class TSLEntity;
class TSLEntityIterator;

// A GUI widget used to show the hierachy of an entity set (TSLEntitySet)
// as a tree structure.

class AttributeTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  AttributeTreeWidget( QWidget *parent = 0 );
  virtual ~AttributeTreeWidget();

  // Constructing the tree from entities/entity sets
  void addEntitySet( const TSLEntitySet* set, unsigned int& numAdded, QTreeWidgetItem* item = NULL );
  QTreeWidgetItem* addEntity( const TSLEntity* ent, unsigned int& numAdded, QTreeWidgetItem* parentItem = NULL );
  void addEntityIterator( TSLEntityIterator* it );
  
  bool initialised() const;
  void initialised(bool initialised);

private:

  bool m_initialised;

};

inline 
bool AttributeTreeWidget::initialised() const
{
  return m_initialised;
}

inline
void AttributeTreeWidget::initialised(bool initialised)
{
  m_initialised = initialised;
}

#endif
