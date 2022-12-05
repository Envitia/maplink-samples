/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKHOSTILITYDELEGATE_H
#define TRACKHOSTILITYDELEGATE_H

// A simple Qt delegate class for providing a combobox to change the hostility of a track

#include <QStyledItemDelegate>

class TrackHostilityDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  TrackHostilityDelegate( QWidget *parent = 0 );
  virtual ~TrackHostilityDelegate();

  virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
  virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
