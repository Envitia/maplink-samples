#include <QIntValidator>
#include <QTimer>
#include "cameracontroller.h"
#include "ui_cameracontroller.h"
#include "maplinkwidget.h"
#include <sstream>
#include "surfacecontroller.h"
#include <iostream>

CameraController::CameraController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraController)
	, m_maplinkWidget(nullptr)
	, m_camManager(nullptr)
	, m_numTracks(250)
	, m_followTrack(false)
	, m_trackID("")
	, m_trackPosition("")
	, m_trackSpeed("")
	, m_selectedTrack(nullptr)
{
    ui->setupUi(this);
	setup_CameraControls_Signals();
}

CameraController::~CameraController()
{
	if (m_tracksTimer)
		m_tracksTimer->stop();
    delete ui;
}

void CameraController::attachMapLinkWidget(MapLinkWidget *maplinkWidget)
{
	m_maplinkWidget = maplinkWidget;
	m_camManager.reset(new CameraManager(m_maplinkWidget->drawingSurface()));
	updateDialog();
}

void CameraController::setup_CameraControls_Signals()
{
	// connect slots to button signals
	connect(ui->pushButton_cammove_forward, SIGNAL(clicked()), this, SLOT(camForward()));
	connect(ui->pushButton_cammove_back, SIGNAL(clicked()), this, SLOT(camBack()));
	connect(ui->pushButton_cammove_left, SIGNAL(clicked()), this, SLOT(camLeft()));
	connect(ui->pushButton_cammove_right, SIGNAL(clicked()), this, SLOT(camRight()));
	connect(ui->pushButton_cammove_up, SIGNAL(clicked()), this, SLOT(camUp()));
	connect(ui->pushButton_cammove_down, SIGNAL(clicked()), this, SLOT(camDown()));
	connect(ui->pushButton_camroll_up, SIGNAL(clicked()), this, SLOT(camRollUp()));
	connect(ui->pushButton_camroll_down, SIGNAL(clicked()), this, SLOT(camRollDown()));
	connect(ui->pushButton_camfov_up, SIGNAL(clicked()), this, SLOT(camFovUp()));
	connect(ui->pushButton_camfov_down, SIGNAL(clicked()), this, SLOT(camFovDown()));
	connect(ui->pushButton_campos_lat_up, SIGNAL(clicked()), this, SLOT(camLatUp()));
	connect(ui->pushButton_campos_lat_down, SIGNAL(clicked()), this, SLOT(camLatDown()));
	connect(ui->pushButton_campos_lon_up, SIGNAL(clicked()), this, SLOT(camLonUp()));
	connect(ui->pushButton_campos_lon_down, SIGNAL(clicked()), this, SLOT(camLonDown()));
	connect(ui->pushButton_campos_alt_up, SIGNAL(clicked()), this, SLOT(camAltUp()));
	connect(ui->pushButton_campos_alt_down, SIGNAL(clicked()), this, SLOT(camAltDown()));
	connect(ui->pushButton_camtar_lat_up, SIGNAL(clicked()), this, SLOT(targetLatUp()));
	connect(ui->pushButton_camtar_lat_down, SIGNAL(clicked()), this, SLOT(targetLatDown()));
	connect(ui->pushButton_camtar_lon_up, SIGNAL(clicked()), this, SLOT(targetLonUp()));
	connect(ui->pushButton_camtar_lon_down, SIGNAL(clicked()), this, SLOT(targetLonDown()));
	connect(ui->pushButton_camtar_alt_up, SIGNAL(clicked()), this, SLOT(targetAltUp()));
	connect(ui->pushButton_camtar_alt_down, SIGNAL(clicked()), this, SLOT(targetAltDown()));

	connect(ui->pushButton_starttracks, SIGNAL(clicked()), this, SLOT(startPauseTracks()));
	connect(ui->pushButton_removetracks, SIGNAL(clicked()), this, SLOT(removeTracks()));
	connect(ui->checkBox_followtrack, SIGNAL(stateChanged(int)), this, SLOT(followtrackChecked(int)));

	connect(ui->lineEdit_campos_lat, SIGNAL(textChanged(const QString&)), this, SLOT(camposLat_TextChanged(const QString&)));
	connect(ui->lineEdit_campos_lon, SIGNAL(textChanged(const QString&)), this, SLOT(camposLon_TextChanged(const QString&)));
	connect(ui->lineEdit_campos_alt, SIGNAL(textChanged(const QString&)), this, SLOT(camposAlt_TextChanged(const QString&)));
	connect(ui->lineEdit_camtar_lat, SIGNAL(textChanged(const QString&)), this, SLOT(camtarLat_TextChanged(const QString&)));
	connect(ui->lineEdit_camtar_lon, SIGNAL(textChanged(const QString&)), this, SLOT(camtarLon_TextChanged(const QString&)));
	connect(ui->lineEdit_camtar_alt, SIGNAL(textChanged(const QString&)), this, SLOT(camtarAlt_TextChanged(const QString&)));
	connect(ui->lineEdit_camroll, SIGNAL(textChanged(const QString&)), this, SLOT(camroll_TextChanged(const QString&)));
	connect(ui->lineEdit_camfov, SIGNAL(textChanged(const QString&)), this, SLOT(camfov_TextChanged(const QString&)));
	connect(ui->lineEdit_numtracks, SIGNAL(textChanged(const QString&)), this, SLOT(numtracks_TextChanged(const QString&)));

	// move lineedit limits
	ui->lineEdit_campos_lat->setValidator(new QIntValidator(-90, 90, this));
	ui->lineEdit_campos_lon->setValidator(new QIntValidator(-180, 180, this));
	ui->lineEdit_camtar_lat->setValidator(new QIntValidator(-90, 90, this));
	ui->lineEdit_camtar_lon->setValidator(new QIntValidator(-180, 180, this));
	ui->lineEdit_camfov->setValidator(new QIntValidator(15, 120, this));
	ui->lineEdit_camroll->setValidator(new QIntValidator(-360, 360, this));

	// selected track line edits 
	QPalette palette = ui->lineEdit_trackid->palette();
	palette.setColor(ui->lineEdit_trackid->foregroundRole(), Qt::blue);
	ui->lineEdit_trackid->setPalette(palette);
	ui->lineEdit_trackpos->setPalette(palette);
	ui->lineEdit_trackspeed->setPalette(palette);

	// tracks timer
	m_tracksTimer = new QTimer(this);
	connect(m_tracksTimer, SIGNAL(timeout()), this, SLOT(updateTracksWithTimer()));
}

void CameraController::resetView()
{
	// reset camera manager
	if (m_camManager)
		m_camManager->resetCamera();
	updateDialog();
}



void CameraController::updateCamera()
{
	if (m_camManager)
	{
		m_camManager->updateCamera();
	}
}

void CameraController::updateDialog(bool doingTrackUpdate)
{
	if (!m_camManager)
		return;

	m_editingDialogParams = true;
	if (doingTrackUpdate)
	{
		ui->lineEdit_trackid->setText(m_trackID.c_str());
		ui->lineEdit_trackpos->setText(m_trackPosition.c_str());
		ui->lineEdit_trackspeed->setText(m_trackSpeed.c_str());

		if (m_followTrack)
		{
			ui->lineEdit_campos_lat->setText(std::to_string(m_camManager->camPosLat()).c_str());
			ui->lineEdit_campos_lon->setText(std::to_string(m_camManager->camPosLon()).c_str());
			ui->lineEdit_campos_alt->setText(std::to_string(m_camManager->camPosAlt()).c_str());

			ui->lineEdit_camtar_lat->setText(std::to_string(m_camManager->camTargetLat()).c_str());
			ui->lineEdit_camtar_lon->setText(std::to_string(m_camManager->camTargetLon()).c_str());
			ui->lineEdit_camtar_alt->setText(std::to_string(m_camManager->camTargetAlt()).c_str());
		}
	}
	else
	{
		ui->lineEdit_campos_lat->setText(std::to_string(m_camManager->camPosLat()).c_str());
		ui->lineEdit_campos_lon->setText(std::to_string(m_camManager->camPosLon()).c_str());
		ui->lineEdit_campos_alt->setText(std::to_string(m_camManager->camPosAlt()).c_str());

		ui->lineEdit_camtar_lat->setText(std::to_string(m_camManager->camTargetLat()).c_str());
		ui->lineEdit_camtar_lon->setText(std::to_string(m_camManager->camTargetLon()).c_str());
		ui->lineEdit_camtar_alt->setText(std::to_string(m_camManager->camTargetAlt()).c_str());

		ui->lineEdit_camroll->setText(std::to_string(m_camManager->camRollAngle()).c_str());
		ui->lineEdit_camfov->setText(std::to_string(m_camManager->camFOV()).c_str());

		ui->lineEdit_numtracks->setText(std::to_string(m_numTracks).c_str());

		ui->checkBox_followtrack->setChecked(m_followTrack);
	}
	m_editingDialogParams = false;
}

void CameraController::selectedTrack(envitia::maplink::earth::Track* track, uint16_t id)
{
	m_selectedTrack = track;
	m_selectedTrackID = id;

	if (m_selectedTrack == nullptr)
	{
		m_trackPosition = "";
		m_trackSpeed = "";
		m_trackID = "";

		updateDialog(true);
	}
}

bool CameraController::OnSelectedTrackViewChanged()
{
	if (m_selectedTrack)
	{
		if (m_followTrack)
		{
			// Move the camera target to follow the track
			if (m_camManager)
			{
				m_camManager->followTrack(m_selectedTrack);
			}
		}

		// Update the text in the dialog to match the track's properties
		auto pos = m_selectedTrack->position();
		std::stringstream ss("");
		ss << pos.y() << ", " << pos.x();
		m_trackPosition = ss.str().c_str();
		m_trackSpeed = m_selectedTrack->attributeValue(1);
		m_trackID = std::to_string(m_selectedTrackID).c_str();
		updateDialog(true);

		return true;
	}
	return false;
}

void CameraController::handleViewChange()
{
	if (m_camManager)
	{
		m_camManager->handleViewChange();
	}

	// Update the dialog
  updateDialog();


}


void CameraController::camForward()
{
	if (m_camManager)
	{
		m_camManager->camForward();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camBack()
{
	if (m_camManager)
	{
		m_camManager->camBack();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camLeft()
{
	if (m_camManager)
	{
		m_camManager->camLeft();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camRight()
{
	if (m_camManager)
	{
		m_camManager->camRight();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camUp()
{
	if (m_camManager)
	{
		m_camManager->camUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camDown()
{
	if (m_camManager)
	{
		m_camManager->camDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camRollUp() {
	if (m_camManager)
	{
		m_camManager->camRollUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camRollDown() {
	if (m_camManager)
	{
		m_camManager->camRollDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camFovUp() {
	if (m_camManager)
	{
		m_camManager->camFovUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camFovDown() {
	if (m_camManager)
	{
		m_camManager->camFovDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

// Functions for moving the camera in Geodetic space
void CameraController::camLatUp() {
	if (m_camManager)
	{
		m_camManager->camLatUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }

}
void CameraController::camLatDown() {
	if (m_camManager)
	{
		m_camManager->camLatDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camLonUp() {
	if (m_camManager)
	{
		m_camManager->camLonUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camLonDown() {
	if (m_camManager)
	{
		m_camManager->camLonDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camAltUp() {
	if (m_camManager)
	{
		m_camManager->camAltUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camAltDown() {
	if (m_camManager)
	{
		m_camManager->camAltDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::targetLatUp() {
	if (m_camManager)
	{
		m_camManager->targetLatUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::targetLatDown() {
	if (m_camManager)
	{
		m_camManager->targetLatDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::targetLonUp() {
	if (m_camManager)
	{
		m_camManager->targetLonUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::targetLonDown() {
	if (m_camManager)
	{
		m_camManager->targetLonDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::targetAltUp() {
	if (m_camManager)
	{
		m_camManager->targetAltUp();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::targetAltDown() {
	if (m_camManager)
	{
		m_camManager->targetAltDown();
	}
	// Update the dialog
	updateDialog();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}


void CameraController::camposLat_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camPosLat(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camposLon_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camPosLon(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camposAlt_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camPosAlt(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::camtarLat_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camTargetLat(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camtarLon_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camTargetLon(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camtarAlt_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camTargetAlt(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camroll_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camRollAngle(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}
void CameraController::camfov_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_camManager->camFOV(txt.toDouble());
	m_camManager->updateCamera();

  if(parent())
  {
    ((SurfaceController*)parent())->reactivateTrackball();
  }
}

void CameraController::numtracks_TextChanged(const QString& txt)
{
	if (m_editingDialogParams || !m_camManager)
		return;
	m_numTracks = txt.toInt();
}

void CameraController::startPauseTracks() {
	QString text = ui->pushButton_starttracks->text();
	if (text == "Pause Simulation")
		pauseTracks();
	else if (text == "Resume Simulation")
		resumeTracks();
	else
		startTracks();
}

void CameraController::startTracks() {
	// remove previous tracks
	removeTracks();

	// Initialise the track simulation
	if (m_maplinkWidget)
	{
		m_maplinkWidget->startTracks(m_numTracks);

		if (m_tracksTimer)
			m_tracksTimer->start(30);
	}

	// change the button's text to pause
	ui->pushButton_starttracks->setText("Pause Simulation");
}

void CameraController::removeTracks() {
	// remove tracks
	m_selectedTrack = nullptr;
	if (m_maplinkWidget)
		m_maplinkWidget->removeTracks();

	// reset the selected track info
	m_trackPosition = "";
	m_trackSpeed = "";
	m_trackID = "";

	// stop timer
	if(m_tracksTimer)
		m_tracksTimer->stop();

	// change the button's text to resume
	ui->pushButton_starttracks->setText("Start Simulation");
	updateDialog(true);
}

void CameraController::pauseTracks() {
	if (m_tracksTimer)
		m_tracksTimer->stop();

	// change the button's text to resume
	ui->pushButton_starttracks->setText("Resume Simulation");
}

void CameraController::resumeTracks() {
	// reset last updated track time to now in order to resume the tracks if paused
	if (m_maplinkWidget)
		m_maplinkWidget->tracksManager().resetLastTrackUpdateTime();

	// start the timer
	if (m_tracksTimer)
		m_tracksTimer->start(30);

	// change the button's text to resume
	ui->pushButton_starttracks->setText("Pause Simulation");
}

void CameraController::followtrackChecked(int)
{
	m_followTrack = ui->checkBox_followtrack->isChecked();
}

void CameraController::updateTracksWithTimer()
{
	// Update the track simulation
	// Note: Updating all tracks on every frame may affect performance,
	// instead tracks should be updated as and when needed, as part of the application's
	// event handling
	if (m_maplinkWidget)
	{
		m_maplinkWidget->tracksManager().updateTracks();
		OnSelectedTrackViewChanged();
		m_maplinkWidget->update();
	}
}
