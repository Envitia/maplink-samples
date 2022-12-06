#ifndef ATTRIBUTETREEWIDGET_H
#define ATTRIBUTETREEWIDGET_H
/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <qtreewidget.h>
#include "MapLink.h"

class AttributeTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
    AttributeTreeWidget(QWidget *parent = 0);
    virtual ~AttributeTreeWidget();

    void AddEntitySet( const TSLEntitySet* set, QTreeWidgetItem* item = NULL );

    bool m_initialised;
private:
  
};

#endif
