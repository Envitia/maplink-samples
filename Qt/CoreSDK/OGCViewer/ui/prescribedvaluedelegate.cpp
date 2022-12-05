/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "prescribedvaluedelegate.h"
#include "services/service.h"
#include <QComboBox>
#include <QAbstractItemView>

using namespace Services;

PrescribedValueDelegate::PrescribedValueDelegate( QWidget *parent )
  : QStyledItemDelegate( parent )
{
}

PrescribedValueDelegate::~PrescribedValueDelegate()
{
}

QWidget* PrescribedValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& /*option*/, const QModelIndex &index) const
{
  QComboBox *fixedValueEdit = new QComboBox( parent );
  fixedValueEdit->setFrame( false );

  // Use a service value model to populate the dropdown with the dimension values extracted from the specific data layer
  // being used
  const Service::ServiceDimensionsModel *model = reinterpret_cast< const Service::ServiceDimensionsModel* >( index.model() );

  std::vector< const char* > dimensionValues;
  model->getPossibleValues( index, dimensionValues );
  
  size_t numValues = dimensionValues.size();
  for( size_t i = 0; i < numValues; ++i )
  {
    fixedValueEdit->addItem( QString::fromUtf8( dimensionValues[i] ) );
  }

  return fixedValueEdit;
}

void PrescribedValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *fixedValueEdit = reinterpret_cast< QComboBox* >( editor );
  QVariant editData( index.model()->data( index, Qt::EditRole) );
  fixedValueEdit->setCurrentText( editData.toString() );
}

void PrescribedValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *fixedValueEdit = reinterpret_cast< QComboBox* >( editor );
  model->setData( index, fixedValueEdit->currentText(), Qt::EditRole );
}

void PrescribedValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
  editor->setGeometry( option.rect );
}
