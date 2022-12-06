/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef FRACTIONSPINBOX_H
#define FRACTIONSPINBOX_H

#include <QSpinBox>

class FractionSpinBox : public QSpinBox
{
  Q_OBJECT
public:
  FractionSpinBox(QWidget *parent = 0);
  virtual ~FractionSpinBox();

  virtual int valueFromText(const QString & text) const;
  virtual QString textFromValue(int value) const;
  virtual QValidator::State validate(QString & text, int & pos) const;
};

#endif
