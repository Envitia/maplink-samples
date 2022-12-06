/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef SELECTLAYERSPAGE_H
#define SELECTLAYERSPAGE_H

#include <QWizardPage>
#include "ui_selectlayers.h"
#include "services/service.h"

#include "tgmapidll.h"
#include "tslsimplestring.h"

// A page of the service wizard that lets the user choose which layers from the service they
// want to see and which styles to visualise them with, as well as showing a preview of what
// the layer looks like.

class SelectLayersPage : public QWizardPage, public Services::Service::ServiceActionCallback, private Ui_selectLayersPage
{
    Q_OBJECT
public:
    SelectLayersPage(QWidget *parent = 0);
    virtual ~SelectLayersPage();

    virtual void initializePage();
    virtual bool isComplete() const;
    virtual bool validatePage();
    virtual int nextId() const;
    virtual void cleanupPage();

    void setService( Services::Service *service );

signals:
    void signalShowCoordinateSystemPage();
    void signalShowNextPage();
    void signalShowError( const QString &message );

private slots:
    // Called when a layer in the layer tree is selected. This function updates the layer information and preview
    // areas to match the currently selected layer
    void layerSelected( const QModelIndex &current, const QModelIndex &previous );

    // Called when the checked state of a layer is changed. This function is used to change the completion state
    // of the page depending on whether any layers are selected or not.
    void layerStatusChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );

    // Called when a new style is selected in the layer's style dropdown
    void layerStyleChanged(const QString &text);

    // Invoked by signalShowCoordinateSystemPage in the UI thread to indicate that the next page should be the coordinate
    // system selection page
    void showCoordinateSystemPage();
    void showNextPage();
    void showError( const QString& message );

private:
  // Callbacks made from the data layer being used to connect to the service. These will be made from a seperate thread
  // and require some user interface action.
  virtual void onError( const std::string &message);
  virtual void onNextSequenceAction();
  virtual void coordinateSystemChoiceRequired();

  Services::Service *m_service;
  Services::ServiceLayerPreview *m_previewHelper;
  bool m_loadInProgress;
  bool m_showCoordSysSelectionPage;
};

inline void SelectLayersPage::setService( Services::Service *service )
{
  m_service = service;
}

#endif // SELECTLAYERSPAGE_H
