/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DATALAYERDIALOG_H
#define DATALAYERDIALOG_H

#include <QDialog>

#include "ui_datalayerdialog.h"

class DataLayerDialog : public QDialog, private Ui_dataLayerDialog
{
  Q_OBJECT
public:
  DataLayerDialog( QWidget *parent = 0 );
  ~DataLayerDialog();

  void setDataLabel( QString );

  void setLayerNameBox( QString );
  void setCacheSizeBox( QString );
  void setByteSizeBox( QString );
  void setUseSharedCache(bool);

  QString getLayerNameBox() const;
  long    getCacheSizeBox() const;
  QString getByteSizeBox() const;
  bool    getUseSharedCache() const;

  QDialogButtonBox* getButtonBox() const;

protected:
  void showEvent(QShowEvent* event);

private slots:
  void checkBoxSharedCacheToggled(bool checked);
  void cacheSizeComboBoxCurrentIndexChanged(int);
  void byteSizeComboBoxCurrentIndexChanged(int);

private:
  struct CacheSizes 
  {
    QString m_value;
    QString m_byteSize;
    bool m_selected;
  };

  CacheSizes m_cacheSizes[2];
  void initialiseCacheSizes();
 
};

#endif // DATALAYERDIALOG_H
