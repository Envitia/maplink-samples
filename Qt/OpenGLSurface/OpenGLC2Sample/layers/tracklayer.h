/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TRACKLAYER_H
#define TRACKLAYER_H

// This custom data layer is responsible for drawing all of the tracks and their associated
// annotations. This class demonstrates the use of texture atlases (http://en.wikipedia.org/wiki/Texture_atlas)
// to provide extremely high performance track rendering.

#include <QWidget>
#include "textureatlas.h"
#include "tracks/trackmanager.h"
#include "glhelpers.h"
#include <map>
#include <string>
#include <vector>

class TSLOpenGLSurface;

using std::map;
using std::pair;
using std::string;
using std::vector;

class TrackLayer : public TSLClientCustomDataLayer, protected QOpenGLFunctions_3_0
{
public:
  TrackLayer();
  virtual ~TrackLayer();

  // Called to create rendering resources that will be used for drawing
  bool initialise( TSLOpenGLSurface *surface );

  // Called when the basic track visualisation changes enough to invalidate the texture atlas
  void reset( TSLOpenGLSurface *surface );

  virtual bool drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler);
  virtual void releaseResources (int surfaceID);

private:
  void applyHaloTextStyle( TSLEntitySet *set, TSLStyleID colour );

  // Vertex definition used to draw a set of track symbols from the texture atlas
  struct TrackTextureVertex
  {
    GLfloat x;
    GLfloat y;
    GLfloat depth;
    GLfloat clipShiftX;
    GLfloat clipShiftY;
    GLfloat textureX;
    GLfloat textureY;
    GLfloat textureLevel;
  };

  // Vertex definition used to draw track heading and history points
  struct TrackVertex
  {
    GLfloat x;
    GLfloat y;
    GLfloat depth;
    GLuint colour; // RGBA format
  };

  // Information about a specific type of track stored in the texture atlas.
  // Each unique track type being used has an entry.
  struct RasterisedTrack
  {
    // Texture coordinates in the texture atlas for the track
    GLfloat blX;
    GLfloat blY;
    GLfloat trX;
    GLfloat trY;
    GLfloat level;

    // Track's pixel size
    uint32_t width;
    uint32_t height;

    // Track's centre
    GLfloat offsetX;
    GLfloat offsetY;
  };

  // Creates an entry in the texture atlas for the given track
  void rasteriseTrack( const Track::DisplayInfo &track, TSLOpenGLSurface *surface );
  int round( double val ) const;

  // The texture atlas that holds the rasterised track visualisations
  TextureAtlas *m_atlas;

  // Vertex and index buffers and VAO for storing and displaying the position of rasterised tracks
  GLuint m_trackDisplayVBO;
  GLuint m_trackDisplayIBO;
  GLuint m_trackDisplayVAO;

  // Vertex buffer and VAO for track heading indicators
  GLuint m_trackHeadingVBO;
  GLuint m_trackHeadingVAO;

  // Vertex buffer and VAO for track history points
  GLuint m_trackHistoryVBO;
  GLuint m_trackHistoryVAO;

  // How many tracks can be stored for display in the current sized vertex buffers
  size_t m_bufferTrackLimit;

  // Shaders for drawing the various parts of the tracks
  GLHelpers::GLShader *m_trackBodyShader;
  GLHelpers::GLShader *m_trackHeadingShader;
  GLHelpers::GLShader *m_trackHistoryShader;
  GLuint m_trackBodyMVPMatrix;
  GLuint m_trackHeadingMVPMatrix;
  GLuint m_trackHistoryMVPMatrix;

  // Framebuffer object used for rendering to the texture atlas
  GLuint m_fbo;

  // Stores the mapping between a track visualisation and the corresponding entry
  // in the the texure atlas. For the purposes of this sample, a unique rasterisation of a
  // track is defined by the type and hostility of the track.
  map< pair< int, TSLAPP6ASymbol::HostilityEnum >, RasterisedTrack > m_rasterisedTracks;

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
};

inline int TrackLayer::round( double val ) const
{
  return (val < 0.0) ? (int)(val - 0.5) : (int)(val + 0.5);
}

#endif // TRACKLAYER_H
