/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef PRESCRIBEDVALUEDELEGATE_H
#define PRESCRIBEDVALUEDELEGATE_H

#include <QStyledItemDelegate>

// A Qt delegate class used in conjunction with WMS/WMTS dimensions that have a list
// of allowed values.

class PrescribedValueDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  PrescribedValueDelegate( QWidget *parent = 0 );
  virtual ~PrescribedValueDelegate();

  virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
  virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
