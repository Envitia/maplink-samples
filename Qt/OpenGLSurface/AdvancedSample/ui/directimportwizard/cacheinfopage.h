/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef CACHEINFOPAGE_H
#define CACHEINFOPAGE_H

#include <QWizardPage>
#include "ui_cacheinfopage.h"

class CacheInfoPage : public QWizardPage, private Ui_CacheInfoPage
{
  Q_OBJECT
public:
  CacheInfoPage( QWidget* parent = 0 );

  int nextId() const;

  unsigned int cacheSizeInKB();

private:

  //QWidgets

};

#endif // CACHEINFOPAGE_H