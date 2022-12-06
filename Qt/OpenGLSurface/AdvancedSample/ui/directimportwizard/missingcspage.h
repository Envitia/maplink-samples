/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MISSINGCSPAGE_H
#define MISSINGCSPAGE_H

#include <QWizardPage>
#include "ui_missingcspage.h"

class MissingCSPage : public QWizardPage, private Ui_MissingCSPage
{
  Q_OBJECT
public:
  MissingCSPage( QWidget* parent = 0 );

  int nextId() const;

private:

  //QWidgets

};


#endif // MISSINGCSPAGE_H