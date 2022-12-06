/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SCALEBANDSPAGE_H
#define SCALEBANDSPAGE_H

#include <QWizardPage>
#include <QComboBox>
#include "ui_scalebandspage.h"

#include "scalebandstable.h"
#include "scalebandstablemodel.h"

// Direct import wizard page to allow the user to add or remove 
// scale bands.

class ScaleBandsPage : public QWizardPage, private Ui_ScaleBandsPage
{
  Q_OBJECT
public:
  ScaleBandsPage( QWidget* parent = 0 );
  virtual ~ScaleBandsPage();

  virtual void initializePage();
  virtual bool validatePage();

private slots:
  void addScaleBand();
  void removeScaleBand();

private:
  ScaleBandsTableModel m_scaleBandsTableModel;

  //QWidgets

};

#endif // SCALEBANDSPAGE_H
