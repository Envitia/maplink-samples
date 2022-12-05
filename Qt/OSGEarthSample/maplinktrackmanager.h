#ifndef MAPLINKTRACKMANAGER_H
#define MAPLINKTRACKMANAGER_H
/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include <QWidget>

#include <osgEarthMapLink/MilitarySymbols.h>
#include "maplinktrackobject.h"

typedef std::vector< osg::ref_ptr<MaplinkTrackObject> > MaplinkTracks;

class MaplinkTrackManager : public osg::Operation
{
public:
  MaplinkTrackManager(int numTracks, osg::Group* rootNode, osgEarth::MapNode* mapNode, bool declutter, QWidget* mainWindow);
  ~MaplinkTrackManager();

  virtual void operator()(osg::Object* obj);

  void decluttering(bool enabled);
  void showSimulationOptions();

private:
  void addOrRemoveTracks(int targetNumber);

  osgEarth::Annotation::TrackNodeFieldSchema m_trackNodeSchema;
  MaplinkTracks m_tracks;
  int m_numberOfTracks;
  int m_simulationSpeed;
  MaplinkTrackObject::PositionFormat m_positionFormat;

  // A pointer to the map node. Only used for creating the tracks group
  osgEarth::MapNode* m_mapNode;
  // The root tracks group
  // This is a ref_ptr to ensure the group isn't deleted before we have removed all the tracks
  osg::ref_ptr<osg::Group> m_tracksGroup;

  // Used to convert MapLink APP6A symbols to 
  // an osg::Image
  envitia::MapLink::MilitarySymbols m_maplinkSymbols;

  // Used for constructing modal dialogs
  QWidget* m_mainWindow;

  // This should be set while performing expensive operations, 
  // such as adding/removing tracks.
  bool m_skipUpdate;
};

#endif
