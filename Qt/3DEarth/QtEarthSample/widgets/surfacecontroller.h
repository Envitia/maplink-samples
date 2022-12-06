#ifndef SURFACECONTROLLER_H
#define SURFACECONTROLLER_H
#include <QWidget>

namespace envitia {
	namespace maplink {
		namespace earth
		{
			class Track;//< Forward declaration
		}
	}
}

namespace Ui {
	class SurfaceController;
}

class SurfaceController : public QWidget
{
	Q_OBJECT

public:
	explicit SurfaceController(QWidget *parent = nullptr);
	~SurfaceController();

public://drawing surface methods
	void activate_Trackball_Mode();
	void activate_Select_Mode();
	void activate_CreatePolygon_Mode();
	void activate_CreatePolyline_Mode();
	void activate_CreateText_Mode();
	void activate_CreateSymbol_Mode();
	void activate_CreateExtrudedPolygon_Mode();
	void activate_CreateExtrudedPolyline_Mode();
	void activate_DeleteGeometry_Mode();

	void loadLayer(const char *filename);
	// load and add map to the drawing surface.
	void loadLayer();
	// reset map
	void resetView();
	// show/hide camera tool gui
	void fullScreen();

public://MapLink iwdget call back methods
	//! set the call back to update the GUI for reseting interaction modes.
	void resetInteractionModes();

	// Sets the track that the dialog text will update to follow
	// The camera will also follow this track if the "Follow with Camera" box is checked
	void selectedTrack(envitia::maplink::earth::Track* track, uint16_t id);

	// handle view changes
	void handleViewChange();

  // Reactivates the TrackBall interaction mode if it's active, which updates the values stored within from the actual Camera
  void reactivateTrackball();

private:
	Ui::SurfaceController *ui;
};

#endif // SURFACECONTROLLER_H
