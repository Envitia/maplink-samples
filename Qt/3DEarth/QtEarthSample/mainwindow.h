/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_qtearthsample.h"
#include "surfacecontroller.h"

class MainWindow : public QMainWindow, private Ui_QtEarthSampleClass
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void loadLayer(const char *filename);

private slots:
	void activate_Trackball_Mode();
	void activate_Select_Mode();
	void activate_CreatePolygon_Mode();
	void activate_CreatePolyline_Mode();
	void activate_CreateText_Mode();
	void activate_CreateSymbol_Mode();
	void activate_CreateExtrudedPolygon_Mode();
	void activate_CreateExtrudedPolyline_Mode();
	void activate_DeleteGeometry_Mode();

	// load and add map to the drawing surface.
	void loadLayer();
	// reset map
	void resetView();
	// show/hide camera tool gui
	void fullScreen();

    void showAboutBox();
    void exit();

private:
  QActionGroup *m_interactionModesGroup;

  //! object of the surface controller dialog
  SurfaceController * m_surfaceController;
};

#endif
