/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SCALEBANDSTABLE_H
#define SCALEBANDSTABLE_H

#include <QTableView>
#include <QComboBox>

#include "scalebandstablemodel.h"

// This class represents a tabular view to display scale bands.

class ScaleBandsTable : public QTableView
{
  Q_OBJECT

private:

public:
  ScaleBandsTable( QWidget *parent = 0 );
  ~ScaleBandsTable();


private:

};

#endif // SCALEBANDSTABLE_H
