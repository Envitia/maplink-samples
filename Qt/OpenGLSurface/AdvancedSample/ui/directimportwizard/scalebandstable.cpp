/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "scalebandstable.h"

#include <QHeaderView>

ScaleBandsTable::ScaleBandsTable( QWidget *parent )
  : QTableView( parent )
{
  QTableView::horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  QTableView::verticalHeader()->hide();

}

ScaleBandsTable::~ScaleBandsTable()
{

}
