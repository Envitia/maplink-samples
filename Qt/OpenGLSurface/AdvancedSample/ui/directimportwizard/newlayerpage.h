/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef NEWLAYERPAGE_H
#define NEWLAYERPAGE_H

#include <QWizardPage>
#include "ui_newlayerpage.h"

// Direct import wizard page allowing the user to enter the details
// of the new layer (TSLDirectImportDataLayer).

class NewLayerPage : public QWizardPage, private Ui_NewLayerPage
{
  Q_OBJECT
public:
  NewLayerPage( QWidget* parent = 0 );

  virtual bool isComplete() const;
  virtual bool validatePage();
  virtual void initializePage();

private slots:
  void layerNameChanged(const QString& layerName);
  void csysUseMapCheckBoxChanged(bool checked);
  void csysIDLineEditChanged(const QString& id);
  void csysTMCPerMULineEditChanged(const QString& tmcPerMU);

  void memCacheSizeSpinBoxChanged(int val);
  void memCacheSizeComboBoxChanged(const QString& str);

  void onDiskCacheFlushOnExitCheckBoxChanged(bool checked);
  void onDiskCacheDirBrowseButtonClicked();
  void onDiskCacheDirLineEditChanged(const QString& dir);
  void onDiskCacheSizeSpinBoxChanged(int val);
  void onDiskCacheSizeComboBoxChanged(const QString& str);

  void onNumProcessingThreadsSpinBoxChanged(int val);
private:

};

#endif // NEWLAYERPAGE_H
