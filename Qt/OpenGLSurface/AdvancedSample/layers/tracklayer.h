/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKLAYER_H
#define TRACKLAYER_H

// This custom data layer is responsible for drawing all of the tracks and their associated
// annotations. This class demonstrates the use of texture atlases (http://en.wikipedia.org/wiki/Texture_atlas)
// to provide extremely high performance track rendering.

#include <QWidget>
#include "tracks/trackmanager.h"
#include "glhelpers.h"

#include <map>
#include <string>
#include <vector>


class TSLOpenGLSurface;
class TSLOpenGLTrackHelper;


class TrackLayer : public TSLClientCustomDataLayer
{
public:
  TrackLayer();
  virtual ~TrackLayer();

  // Called to create rendering resources that will be used for drawing
  bool initialise( TSLOpenGLSurface *surface );
  
  virtual bool drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler);

private:

  void applyHaloTextStyle( TSLEntitySet *set, TSLStyleID colour );

  int round( double val ) const;

  // How many tracks can be stored for display
  size_t m_bufferTrackLimit;

  // Feature IDs used to refer to hostility types. Used for decluttering tracks by hostility type
  TSLFeatureID m_friendFeatureID;
  TSLFeatureID m_hostileFeatureID;
  TSLFeatureID m_neutralFeatureID;
  TSLFeatureID m_unknownFeatureID;
  TSLFeatureID m_suspectFeatureID;
  TSLFeatureID m_assumedFriendFeatureID;
  TSLFeatureID m_pendingFeatureID;
  TSLFeatureID m_jokerFeatureID;
  TSLFeatureID m_fakerFeatureID;
  TSLFeatureID m_headingIndicatorFeatureID;
  TSLFeatureID m_historyPointsFeatureID;

  TrackAnnotationLevel m_lastAnnotationLevel;


  TSLOpenGLTrackHelper* m_trackHelper;

  std::vector< unsigned int > m_instanceIDs;

  size_t m_lastSelectedTrack;
  
};

inline int TrackLayer::round( double val ) const
{
  return (val < 0.0) ? (int)(val - 0.5) : (int)(val + 0.5);
}

#endif // TRACKLAYER_H
