/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DIMENSIONSPAGE_H
#define DIMENSIONSPAGE_H

#include <QWizardPage>
#include "ui_dimensions.h"
#include "services/service.h"

// A page of the service wizard that lets the user enter initial values
// for any dimensions available on the service the are connecting to.
// This page may not be shown if the service has no dimensions.

class DimensionsPage : public QWizardPage, public Services::Service::ServiceActionCallback, private Ui_dimensionsPage
{
    Q_OBJECT
public:
    DimensionsPage(QWidget *parent = 0);
    virtual ~DimensionsPage();

    virtual void initializePage();
    virtual bool isComplete() const;
    virtual bool validatePage();
    virtual int nextId() const;
    virtual void cleanupPage();

    void setService( Services::Service *service );

signals:
    void signalShowCoordinateSystemPage();
    void signalShowNextPage();

private slots:
    // Invoked by signalShowCoordinateSystemPage in the UI thread to indicate that the next page should be the coordinate
    // system selection page
    void showCoordinateSystemPage();
    void showNextPage();

    // Called whenever the currently selected row in the dimensions list changes. This is used to update the 2nd table
    // that shows information about the selected dimension
    void dimensionSelected( const QModelIndex &current, const QModelIndex &previous ) ;

    // Called when the value of a dimension is changed. This function is used to change the completion state
    // of the page depending on all dimensions have a value or not.
    void dimensionValueChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );

private:
  // Callbacks made from the data layer being used to connect to the service. These will be made from a seperate thread
  // and require some user interface action.
  virtual void onError( const std::string &message);
  virtual void onNextSequenceAction();
  virtual void coordinateSystemChoiceRequired();

  Services::Service *m_service;
  bool m_loadInProgress;
  bool m_showCoordSysSelectionPage;
};

inline void DimensionsPage::setService( Services::Service *service )
{
  m_service = service;
}

#endif // ADDSERVICEPAGE_H
