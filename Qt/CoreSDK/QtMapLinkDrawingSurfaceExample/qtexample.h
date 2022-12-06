/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef QTEXAMPLE_H
#define QTEXAMPLE_H

#include <QMainWindow>
#include "ui_qtexample.h"

class QtExample : public QMainWindow
{
    Q_OBJECT

public:
    QtExample(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QtExample();

private:
    Ui::QtExampleClass ui;
};

#endif // QTEXAMPLE_H
