#ifndef MAPLINKTRACKOBJECT_H
#define MAPLINKTRACKOBJECT_H
/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <osg/ref_ptr>
#include <osgEarth/Units>
#include <osgEarth/MapNode>
#include <osgEarthAnnotation/TrackNode>
#include <osgEarthMapLink/MilitarySymbols.h>
#include <MapLink.h>

#define ICONSIZE 50
#define SCHEMAFIELD_NAME     "name"
#define SCHEMAFIELD_POSITION "position"

struct MaplinkTrackObjectProperties
{
  const char* id;       // APP6A id string
  const int   altitude; // Meters, used for drawing and decluttering priority
  const int   speed;    // Meters per second
};


class MaplinkTrackObject : public osg::Referenced
{
public:
  // Matched to the SimulationOptionsDialog.comboBox_PositionFormat indexes
  enum PositionFormat
  {
    FORMAT_NONE,
    FORMAT_GARS,
    FORMAT_MGRS,
    FORMAT_LATLON
  };

  MaplinkTrackObject(osgEarth::MapNode* mapNode, osg::Group* parent, int trackIndex, osgEarth::Annotation::TrackNodeFieldSchema& schema, envitia::MapLink::MilitarySymbols& maplinkSymbols);
  ~MaplinkTrackObject();

  void update(const double& time, const int& simulationSpeed, const PositionFormat& positionFormat);

private:
  static const MaplinkTrackObjectProperties& getRandomPossibleObject();
  static const TSLAPP6ASymbol::HostilityEnum getRandomPossibleHostility();
  static const double random(double min, double max);

  osgEarth::Angular m_startLatitude;
  osgEarth::Angular m_startLongitude;
  osgEarth::Angular m_endLatitude;
  osgEarth::Angular m_endLongitude;

  osg::ref_ptr<osgEarth::Annotation::TrackNode> m_trackNode;
  osg::ref_ptr<osg::Group> m_parent;

  TSLAPP6ASymbol m_app6aSymbol;

  double m_greatCircleDistance;

  unsigned int m_speed;
  unsigned int m_eta;
  unsigned int m_altitude;
  double m_startTime;
};

#endif
