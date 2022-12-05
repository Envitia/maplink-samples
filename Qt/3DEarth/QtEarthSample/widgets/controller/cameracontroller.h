#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QWidget>
#include <memory>
#include "interactions/cameramanager.h"

class MapLinkWidget;

namespace Ui {
class CameraController;
}

class CameraController : public QWidget
{
    Q_OBJECT

public:
    explicit CameraController(QWidget *parent = nullptr);
    ~CameraController();

	// attach maplink surface widget
	void attachMapLinkWidget(MapLinkWidget *maplinkWidget);

	// setup the cameratools gui
	void setup_CameraControls_Signals();

	// start/pause tracks simulation
	void startTracks();
	void pauseTracks();
	void resumeTracks();

public://camera tools methods
	// Updates the camera on the surface with the current values we have stored.
	void updateCamera();

	// update dislog with camera parameters
	void updateDialog(bool doingTrackUpdate = false);

	// Sets the track that the dialog text will update to follow
	// The camera will also follow this track if the "Follow with Camera" box is checked
	void selectedTrack(envitia::maplink::earth::Track* track, uint16_t id);

	// Called by MainFrm when it receives the EARTH_TRACKS_UPDATED message
	bool OnSelectedTrackViewChanged();

	// handle view changes
	void handleViewChange();

	// reset surface view
	void resetView();
	
private slots:
	// Functions for moving the camera in Geocentric space
	void camForward();
	void camBack();
	void camLeft();
	void camRight();
	void camUp();
	void camDown();
	void camRollUp();
	void camRollDown();
	void camFovUp();
	void camFovDown();

	// Functions for moving the camera in Geodetic space
	void camLatUp();
	void camLatDown();
	void camLonUp();
	void camLonDown();
	void camAltUp();
	void camAltDown();
	void targetLatUp();
	void targetLatDown();
	void targetLonUp();
	void targetLonDown();
	void targetAltUp();
	void targetAltDown();

	// Functions for handling track dialog functions
	void startPauseTracks();
	void removeTracks();
	void followtrackChecked(int);

	// line edits text change
	void camposLat_TextChanged(const QString&);
	void camposLon_TextChanged(const QString&);
	void camposAlt_TextChanged(const QString&);
	void camtarLat_TextChanged(const QString&);
	void camtarLon_TextChanged(const QString&);
	void camtarAlt_TextChanged(const QString&);
	void camroll_TextChanged(const QString&);
	void camfov_TextChanged(const QString&);
	void numtracks_TextChanged(const QString&);

	// timer event to update the surface with the moving tracks
	void updateTracksWithTimer();

private:
	Ui::CameraController *ui;

	// MapLink widget surface to draw the map
	MapLinkWidget *m_maplinkWidget;

	// camera manager to control the camera
	std::unique_ptr<CameraManager> m_camManager;

	// Track data
	bool m_editingDialogParams = false;
	uint16_t m_numTracks;
	bool m_followTrack;
	std::string m_trackID;
	std::string m_trackPosition;
	std::string m_trackSpeed;
	envitia::maplink::earth::Track* m_selectedTrack;
	uint16_t m_selectedTrackID;
	QTimer *m_tracksTimer;
};

#endif // CAMERACONTROLLER_H
