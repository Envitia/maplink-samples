/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef CACHESIZEDIALOG_H
#define CACHESIZEDIALOG_H

#include <QDialog>

#include "ui_cachesizedialog.h"

class CacheSizeDialog : public QDialog, private Ui_CacheSizeDialog
{
  Q_OBJECT
public:
  CacheSizeDialog( QWidget *parent = 0 );
  ~CacheSizeDialog();

  void setupDialog( const std::string& name, int val );

  const std::string& getLayerName();

  unsigned int cacheSizeInKB();

  QDialogButtonBox* getButtonBox();

private:
  std::string m_layerName;

};

#endif // CACHESIZEDIALOG_H