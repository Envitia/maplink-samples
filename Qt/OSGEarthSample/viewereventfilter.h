/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef VIEWEREVENTFILTER_H
#define VIEWEREVENTFILTER_H

#include <QObject>

class ViewerEventFilter : public QObject
{
  Q_OBJECT
public:
  ViewerEventFilter(QObject* parent = 0);
protected:
  bool eventFilter(QObject* object, QEvent* event);
};

#endif
