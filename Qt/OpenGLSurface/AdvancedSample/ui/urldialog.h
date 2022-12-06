/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef URLDIALOG_H
#define URLDIALOG_H

#include <QDialog>

#include "ui_urldialog.h"

class URLDialog : public QDialog, private Ui_URLDialog
{
  Q_OBJECT
public:
  URLDialog( QWidget *parent = 0 );
  ~URLDialog();

  void setupDialog( );

  const QString& getURLTextbox();

  //unsigned int cacheSizeInKB();

  QDialogButtonBox* getButtonBox();

private:
  //std::string m_layerName;

};

#endif // URLDIALOG_H
