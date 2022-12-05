/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef ACTIONTYPEPAGE_H
#define ACTIONTYPEPAGE_H

#include <QWizardPage>
#include "ui_actiontype.h"

// A page of the service wizard that lets the user choose what type of service
// they want to connect to.

class ActionTypePage : public QWizardPage, private Ui_actionTypePage
{
    Q_OBJECT
public:
    ActionTypePage(QWidget *parent = 0);
    virtual ~ActionTypePage();

    virtual bool validatePage();

    virtual int nextId() const;
};

#endif // ACTIONTYPEPAGE_H
