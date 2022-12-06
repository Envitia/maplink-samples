/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QStyledItemDelegate>

// A Qt delegate class used in conjunction with WMS/WMTS dimensions in ISO8601 format
// that provides an easier way of editing dates in the UI.

class DateDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  DateDelegate( QWidget *parent = 0 );
  virtual ~DateDelegate();

  virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
  virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
