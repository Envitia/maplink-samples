/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TMFLAYERDIALOG_H
#define TMFLAYERDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "ui_tmflayerdialog.h"

class TMFLayerDialog : public QDialog, private Ui_TMFLayerDialog
{
  Q_OBJECT
public:
  TMFLayerDialog( QWidget *parent = 0 );
  ~TMFLayerDialog();

  void setDataLabel( QString );

  void setLayerNameBox( QString );
  void setCoordinateSystemBox( QString );
  void setStylingText( QString );

  QString getLayerNameBox();
  QString getCoordinateSystemBox();
  QString getStylingText();

  QPushButton* getBrowseButton();
  QDialogButtonBox* getButtonBox();

};

#endif // TMFLAYERDIALOG_H
