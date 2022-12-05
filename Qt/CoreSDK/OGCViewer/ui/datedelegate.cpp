/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "datedelegate.h"
#include <QDateTimeEdit>

DateDelegate::DateDelegate( QWidget *parent )
  : QStyledItemDelegate( parent )
{
}

DateDelegate::~DateDelegate()
{
}

QWidget* DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
  QString dateString( index.data( Qt::EditRole ).toString() );

  QDateTimeEdit *dateEdit = new QDateTimeEdit( parent );

  // Set the default date value to the current value in the model
  dateEdit->setTimeSpec( Qt::UTC );

  dateEdit->setFrame( false );
  dateEdit->setCalendarPopup( true );

  return dateEdit;
}

void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QVariant editData( index.model()->data( index, Qt::EditRole) );
  QDateTimeEdit *dateEdit = reinterpret_cast< QDateTimeEdit* >( editor );
  dateEdit->setDateTime( QDateTime::fromString( editData.toString(), Qt::ISODate ) );
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QDateTimeEdit *dateEdit = reinterpret_cast< QDateTimeEdit* >( editor );
  model->setData( index, dateEdit->dateTime().toString( Qt::ISODate ), Qt::EditRole );
}

void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
  editor->setGeometry( option.rect );
}
