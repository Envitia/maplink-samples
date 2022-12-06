#include <cstdio>
#include "fractionspinbox.h"

FractionSpinBox::FractionSpinBox(QWidget *parent)
  : QSpinBox(parent)
{
}

FractionSpinBox::~FractionSpinBox()
{
}

int FractionSpinBox::valueFromText(const QString &text) const
{
  double value = 0.0;
  sscanf( text.toUtf8(), "%lf", &value );
  return (int)(value * 10.0);
}

QString FractionSpinBox::textFromValue(int value) const
{
  return QString::number( value / 10.0, 'f', 1 );
}

QValidator::State FractionSpinBox::validate(QString &text, int &/*pos*/) const
{
  double value = 0.0;
  sscanf( text.toUtf8(), "%lf", &value );
  if( value * 10.0 < minimum() || value * 10.0 > maximum() )
  {
    return QValidator::Invalid;
  }

  return QValidator::Acceptable;
}
