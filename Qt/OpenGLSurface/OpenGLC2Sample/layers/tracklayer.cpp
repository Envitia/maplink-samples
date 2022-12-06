/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "tracklayer.h"
#include "textureatlas.h"
#include "tracks/trackmanager.h"
#include "tracks/track.h"
#include "shaders.h"
#include "MapLinkDrawing.h"
#include "MapLinkOpenGLSurface.h"
#include <cmath>
#include <cassert>
#include <QMessageBox>

using std::make_pair;

TrackLayer::TrackLayer()
  : m_atlas( NULL )
  , m_trackDisplayVBO( 0 )
  , m_trackDisplayIBO( 0 )
  , m_trackDisplayVAO( 0 )
  , m_trackHeadingVBO( 0 )
  , m_trackHeadingVAO( 0 )
  , m_trackHistoryVBO( 0 )
  , m_trackHistoryVAO( 0 )
  , m_bufferTrackLimit( 0 )
  , m_trackBodyShader( NULL )
  , m_trackHeadingShader( NULL )
  , m_trackHistoryShader( NULL )
  , m_trackBodyMVPMatrix( 0 )
  , m_trackHeadingMVPMatrix( 0 )
  , m_trackHistoryMVPMatrix( 0 )
  , m_fbo( 0 )
  // Feature IDs used for decluttering of tracks by hostility type
  , m_friendFeatureID( 1 )
  , m_hostileFeatureID( 2 )
  , m_neutralFeatureID( 3 )
  , m_unknownFeatureID( 4 )
  , m_suspectFeatureID( 5 )
  , m_assumedFriendFeatureID( 6 )
  , m_pendingFeatureID( 7 )
  , m_jokerFeatureID( 8 )
  , m_fakerFeatureID( 9 )
  , m_headingIndicatorFeatureID( 10 )
  , m_historyPointsFeatureID( 11 )
  , m_lastAnnotationLevel( AnnotationNone )
{
}

TrackLayer::~TrackLayer()
{
  // This layer is no longer associated with a drawing surface, delete all OpenGL resources we used.
  // We are only used in one drawing surface, so we don't need to use the surface ID to identify which
  // resources to delete.
  // We also do not store any MapLink entities that are used with the TSLRenderingInterface, so we don't
  // need to forward this call to anything.
  delete m_atlas;
  m_atlas = NULL;

  glDeleteBuffers(1, &m_trackDisplayVBO);
  glDeleteBuffers(1, &m_trackDisplayIBO);
  glDeleteBuffers(1, &m_trackHeadingVBO);
  glDeleteBuffers(1, &m_trackHistoryVBO);
  glDeleteVertexArrays(1, &m_trackDisplayVAO);
  glDeleteVertexArrays(1, &m_trackHeadingVAO);
  glDeleteVertexArrays(1, &m_trackHistoryVAO);
  m_trackDisplayVBO = 0;
  m_trackDisplayIBO = 0;
  m_trackHeadingVBO = 0;
  m_trackHistoryVBO = 0;
  m_trackDisplayVAO = 0;
  m_trackHeadingVAO = 0;
  m_trackHistoryVAO = 0;

  glDeleteFramebuffers(1, &m_fbo);

  delete m_trackBodyShader;
  m_trackBodyShader = NULL;
  delete m_trackHeadingShader;
  m_trackHeadingShader = NULL;
  delete m_trackHistoryShader;
  m_trackHistoryShader = NULL;
}


bool TrackLayer::initialise( TSLOpenGLSurface *surface )
{
  initializeOpenGLFunctions();

  // This layer is written to require OpenGL 3.0, so if we didn't get a 3.0 or better context then
  // report an error and exit
  GLint glMajorVersion = 0;
  glGetIntegerv( GL_MAJOR_VERSION, &glMajorVersion );
  if( glMajorVersion < 3 )
  {
    QMessageBox::critical( NULL, "Insufficient OpenGL Version", "This sample requires OpenGL 3.0 support in order to display tracks" );
    return false;
  }

  m_atlas = new TextureAtlas( surface );

  // Create OpenGL resources that will be used to render the various parts of the tracks
  glGenFramebuffers( 1, &m_fbo );

  glGenBuffers( 1, &m_trackDisplayVBO );
  glGenBuffers( 1, &m_trackDisplayIBO );
  glGenBuffers( 1, &m_trackHeadingVBO );
  glGenBuffers( 1, &m_trackHistoryVBO );
  glGenVertexArrays( 1, &m_trackDisplayVAO );
  glGenVertexArrays( 1, &m_trackHeadingVAO );
  glGenVertexArrays( 1, &m_trackHistoryVAO );

  vector< pair< string, GLuint > > trackBodyShaderAttributeLocations;
  trackBodyShaderAttributeLocations.push_back( make_pair( "vertexPosition", 0 ) );
  trackBodyShaderAttributeLocations.push_back( make_pair( "clipShift", 1 ) );
  trackBodyShaderAttributeLocations.push_back( make_pair( "texCoords", 2 ) );
  m_trackBodyShader = GLHelpers::compileShader( g_trackBodyVertexShaderSource, g_trackBodyFragmentShaderSource, trackBodyShaderAttributeLocations );
  if( m_trackBodyShader )
  {
    m_trackBodyMVPMatrix = glGetUniformLocation( m_trackBodyShader->m_program, "mvpMatrix" );

    GLint textureUniform = glGetUniformLocation( m_trackBodyShader->m_program, "tex0" );
    surface->stateTracker()->useProgram( m_trackBodyShader->m_program );
    glUniform1i( textureUniform, 0 );
  }
  else
  {
    return false;
  }

  vector< pair< string, GLuint > > trackVertexShaderAttributeLocations;
  trackVertexShaderAttributeLocations.push_back( make_pair( "vertexPosition", 0 ) );
  trackVertexShaderAttributeLocations.push_back( make_pair( "colour", 1 ) );
  m_trackHeadingShader = GLHelpers::compileShader( g_trackHeadingVertexShaderSource, g_trackHeadingFragmentShaderSource, trackVertexShaderAttributeLocations );
  if( m_trackHeadingShader )
  {
    m_trackHeadingMVPMatrix = glGetUniformLocation( m_trackHeadingShader->m_program, "mvpMatrix" );
  }
  else
  {
    return false;
  }

  m_trackHistoryShader = GLHelpers::compileShader( g_trackHistoryVertexShaderSource, g_trackHeadingFragmentShaderSource, trackVertexShaderAttributeLocations );
  if( m_trackHistoryShader )
  {
    m_trackHistoryMVPMatrix = glGetUniformLocation( m_trackHistoryShader->m_program, "mvpMatrix" );

    GLuint historyPointSizeUniform = glGetUniformLocation( m_trackHistoryShader->m_program, "size" );
    surface->stateTracker()->useProgram( m_trackHistoryShader->m_program );
    glUniform1f( historyPointSizeUniform, 5.0f );
  }
  else
  {
    return false;
  }

  // Create some features that we can use to declutter tracks by hostility type.
  TSLDataLayer *customLayer = dataLayer();
  customLayer->addFeatureRendering( "Friend", m_friendFeatureID);
  customLayer->addFeatureRendering( "Hostile", m_hostileFeatureID );
  customLayer->addFeatureRendering( "Neutral", m_neutralFeatureID );
  customLayer->addFeatureRendering( "Unknown", m_unknownFeatureID );
  customLayer->addFeatureRendering( "Suspect", m_suspectFeatureID );
  customLayer->addFeatureRendering( "Assumed Friend", m_assumedFriendFeatureID );
  customLayer->addFeatureRendering( "Pending", m_pendingFeatureID );
  customLayer->addFeatureRendering( "Joker", m_jokerFeatureID );
  customLayer->addFeatureRendering( "Faker", m_fakerFeatureID );
  customLayer->addFeatureRendering( "Heading Indicators", m_headingIndicatorFeatureID );
  customLayer->addFeatureRendering( "History Points", m_historyPointsFeatureID );

  return true;
}

bool TrackLayer::drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler)
{
  // Use the drawing surface's state tracker to ensure the OpenGL state remains consistent between the application and the drawing surface.
  const TSLOpenGLSurface *glSurface = reinterpret_cast<const TSLOpenGLSurface*>( layerHandler.drawingSurface() );
   // It is legitimate to remove the constness from the drawing surface for the purposes of accessing the state tracker.
  TSLOpenGLSurface *nonConstGLSurface = const_cast<TSLOpenGLSurface*>(glSurface);
  TSLOpenGLStateTracker *stateTracker = nonConstGLSurface->stateTracker();

  if (!m_atlas)
  {
    // releaseResources has been called so we need to re-create.
    initialise(nonConstGLSurface);
  }

  const TrackManager::DisplayInfo *displayInfo = TrackManager::instance().displayInformation();
  if( !displayInfo || displayInfo->m_tracks.empty() )
  {
    // No tracks to display
    return false;
  }

  if( displayInfo->m_tracks.size() > m_bufferTrackLimit )
  {
    // We have more tracks than we can display with our current set of OpenGL buffers, we need to resize them to handle this many tracks
    stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackDisplayVBO );
    // Each track requires 4 vertices to display
    glBufferData( GL_ARRAY_BUFFER, displayInfo->m_tracks.size() * 4 * sizeof(TrackTextureVertex), NULL, GL_STREAM_DRAW );

    // Set up the VAO for drawing the track bodies
    GLuint originalVAO = stateTracker->bindVertexArrayObject( m_trackDisplayVAO ); // Record the original VAO so that we can restore it when done

    // Since we are now modifying our own VAO state we should not use the state tracker to change any OpenGL state included in the VAO
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(TrackTextureVertex), NULL );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(TrackTextureVertex), (const GLvoid*)(3 * sizeof(GLfloat)) );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(TrackTextureVertex), (const GLvoid*)(5 * sizeof(GLfloat)) );

    // Since the structure of what we generate each time will not change (a sequence of squares), we can pregenerate the
    // index buffer so that we don't need to rebuilt it each draw.
    GLuint *indices = new GLuint[displayInfo->m_tracks.size() * 6]; // 6 indices per track
    GLuint *currentIndexGroup = indices;
    for( GLuint i = 0; i < displayInfo->m_tracks.size(); ++i, currentIndexGroup += 6 )
    {
      // For each track the indices define a square made from two triangles, like so:
      //  
      //   ----------
      //   |       /|
      //   |      / |
      //   |     /  |
      //   |    /   |
      //   |   /    |
      //   |  /     |
      //   | /      |
      //   |/       |
      //   ----------
      //
      // This square will be textured with the pre-rasterised track from the texture atlas during drawing.
      currentIndexGroup[0] = 0 + (i*4);
      currentIndexGroup[1] = 1 + (i*4);
      currentIndexGroup[2] = 2 + (i*4);
      currentIndexGroup[3] = 1 + (i*4);
      currentIndexGroup[4] = 3 + (i*4);
      currentIndexGroup[5] = 2 + (i*4);
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_trackDisplayIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, displayInfo->m_tracks.size() * sizeof(GLuint) * 6, indices, GL_STATIC_DRAW );
    delete[] indices;

    // Now set up the buffer and VAO for the track headings. Each heading is a two-point line.
    stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackHeadingVBO );
    // Each track heading indicator requires 2 vertices to display. Also reserve space for four additional lines
    // as we can use this same data to display a box showing the currently selected track.
    glBufferData( GL_ARRAY_BUFFER, (displayInfo->m_tracks.size()+4) * 2 * sizeof(TrackVertex), NULL, GL_STREAM_DRAW );

    stateTracker->bindVertexArrayObject( m_trackHeadingVAO );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(TrackVertex), NULL );
    glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TrackVertex), (const GLvoid*)(3 * sizeof(GLfloat)) );

    // And finally set up the buffer and VAO for the track history points. These will be displayed using point sprites.
    stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackHistoryVBO );
    glBufferData( GL_ARRAY_BUFFER, displayInfo->m_tracks.size() * Track::maxHistoryPoints() * sizeof(TrackVertex), NULL, GL_STREAM_DRAW );

    stateTracker->bindVertexArrayObject( m_trackHistoryVAO );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(TrackVertex), NULL );
    glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TrackVertex), (const GLvoid*)(3 * sizeof(GLfloat)) );

    m_bufferTrackLimit = displayInfo->m_tracks.size();

    // Restore the original VAO binding
    stateTracker->bindVertexArrayObject( originalVAO );
  }

  if( m_lastAnnotationLevel != displayInfo->m_annotationLevel )
  {
    // As we put some of the APP6A/2525B annotations into the texture atlas along with the symbol, changing
    // the amount of annotations displayed requires us to regenerate the contents of the texture atlas.
    reset( nonConstGLSurface );
  }
  m_lastAnnotationLevel = displayInfo->m_annotationLevel;

  // Fill in the vertex data with each of the tracks at the correct position
  stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackDisplayVBO );
  TrackTextureVertex *trackData = (TrackTextureVertex*)glMapBufferRange( GL_ARRAY_BUFFER, 0, displayInfo->m_tracks.size() * 4 * sizeof(TrackTextureVertex),
                                                                         GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

  stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackHeadingVBO );
  TrackVertex *trackHeadingData = (TrackVertex*)glMapBufferRange( GL_ARRAY_BUFFER, 0, (displayInfo->m_tracks.size()+4) * 2 * sizeof(TrackVertex),
                                                                  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

  stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackHistoryVBO );
  TrackVertex *trackHistoryData = (TrackVertex*)glMapBufferRange( GL_ARRAY_BUFFER, 0, displayInfo->m_tracks.size() * Track::maxHistoryPoints() * sizeof(TrackVertex),
                                                                  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

  double surfaceCoordinateCentreX = glSurface->coordinateCentreX();
  double surfaceCoordinateCentreY = glSurface->coordinateCentreY();

  double screenResX, screenResY;
  renderingInterface->screenResolution( screenResX, screenResY );
  TSLDeviceUnits duX1, duY1, duX2, duY2;
  glSurface->getDUExtent( &duX1, &duY1, &duX2, &duY2 );

  GLfloat pixelClipSizeX = 2.0f / (duX2 - duX1);
  GLfloat pixelClipSizeY = 2.0f / (duY2 - duY1);

  GLfloat glCoordysHalfWidth = extent->width() / 2.0f;
  GLfloat glCoordSysHalfHeight = extent->height() / 2.0f;

  uint32_t numVisibleTracks = 0;
  uint32_t numHistoryPoints = 0;
  uint32_t numDisplayLines = 0;

  for( size_t i = 0; i < displayInfo->m_tracks.size(); ++i )
  {
    const Track::DisplayInfo &currentTrack = displayInfo->m_tracks[i];

    // See if this track is decluttered
    TSLFeatureID declutterID = 0;
    switch( currentTrack.m_hostility )
    {
      case TSLAPP6ASymbol::HostilityUnknown:
        declutterID = m_unknownFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityAssumedFriend:
        declutterID = m_assumedFriendFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityFriend:
        declutterID = m_friendFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityNeutral:
        declutterID = m_neutralFeatureID;
        break;

      case TSLAPP6ASymbol::HostilitySuspect:
        declutterID = m_suspectFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityHostile:
        declutterID = m_hostileFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityPending:
        declutterID = m_pendingFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityJoker:
        declutterID = m_jokerFeatureID;
        break;

      case TSLAPP6ASymbol::HostilityFaker:
        declutterID = m_fakerFeatureID;
        break;

      default:
        assert( false );
        break;
    }

    if( renderingInterface->isDecluttered( NULL, declutterID ) )
    {
      // Tracks of this hostility are decluttered, don't add it to the list to draw
      continue;
    }

    // Get the entry in the texture atlas for this type of track visualisation. Multiple different tracks might share the
    // same texture atlas entry if they have the same visualisation.
    map< pair< int, TSLAPP6ASymbol::HostilityEnum >, RasterisedTrack >::iterator trackTexCoords(
                                                      m_rasterisedTracks.find( make_pair(currentTrack.m_symbolKey, currentTrack.m_hostility) ) );
    if( trackTexCoords == m_rasterisedTracks.end() )
    {
      // We don't have a rasterisation for this type of track yet - create it now
      rasteriseTrack( currentTrack, (TSLOpenGLSurface*)glSurface );
      trackTexCoords = m_rasterisedTracks.find( make_pair(currentTrack.m_symbolKey, currentTrack.m_hostility) );
    }

    RasterisedTrack &atlasCoords = trackTexCoords->second;

    // Calculate where in the current OpenGL drawing surface coordinate system this track is located - 
    // See the 'OpenGL Drawing Surface - The Drawing System and Custom Data Layers' section from the
    // MapLink developer's guide for more information.
    GLfloat trackCentreX = (GLfloat)(currentTrack.m_x - surfaceCoordinateCentreX);
    GLfloat trackCentreY = (GLfloat)(currentTrack.m_y - surfaceCoordinateCentreY);

    GLfloat halfTrackWidth = atlasCoords.width * 0.5f;
    GLfloat halfTrackHeight = atlasCoords.height * 0.5f;
    GLfloat halfTrackSizeX = halfTrackWidth * screenResX;
    GLfloat halfTrackSizeY = halfTrackHeight * screenResY;

    // Simple bounding box intersection test to see if a track is visible before we try and draw it
    if( trackCentreX - halfTrackSizeX <= glCoordysHalfWidth &&
        trackCentreY - halfTrackSizeY <= glCoordSysHalfHeight &&
        trackCentreX + halfTrackSizeX >= -glCoordysHalfWidth &&
        trackCentreY + halfTrackSizeY >= -glCoordSysHalfHeight )
    {
      // Assign depths to each part of the track so that it will appear ordered correctly
      // with text labels rendered by MapLink.
      GLfloat histoyPointDepth = nonConstGLSurface->acquireDepthSlice();
      GLfloat headingDepth = nonConstGLSurface->acquireDepthSlice();
      GLfloat trackDepth = nonConstGLSurface->acquireDepthSlice();

      // Fill in the vertex information for the track. We want the position of the track to
      // move with any drawing surface rotation that has been applied, but we don't want the track itself to appear
      // rotated with it. To achieve this we send the four vertices as the track's base position, and provide an additional
      // attribute that tells the vertex shader how far to translate the vertex in clip space to make the track appear the correct
      // size.
      GLfloat tmcOffsetX = atlasCoords.offsetX * screenResX;
      GLfloat tmcOffsetY = atlasCoords.offsetY * screenResY;
      trackData[0].x = trackCentreX + tmcOffsetX;
      trackData[0].y = trackCentreY + tmcOffsetY;
      trackData[0].depth = trackDepth;
      trackData[0].clipShiftX = -halfTrackWidth * pixelClipSizeX;
      trackData[0].clipShiftY = -halfTrackHeight * pixelClipSizeY;
      trackData[0].textureX = atlasCoords.blX;
      trackData[0].textureY = atlasCoords.blY;
      trackData[0].textureLevel = atlasCoords.level;
      trackData[1].x = trackCentreX + tmcOffsetX;
      trackData[1].y = trackCentreY + tmcOffsetY;
      trackData[1].depth = trackDepth;
      trackData[1].clipShiftX = halfTrackWidth * pixelClipSizeX;
      trackData[1].clipShiftY = -halfTrackHeight * pixelClipSizeY;
      trackData[1].textureX = atlasCoords.trX;
      trackData[1].textureY = atlasCoords.blY;
      trackData[1].textureLevel = atlasCoords.level;
      trackData[2].x = trackCentreX + tmcOffsetX;
      trackData[2].y = trackCentreY + tmcOffsetY;
      trackData[2].depth = trackDepth;
      trackData[2].clipShiftX = -halfTrackWidth * pixelClipSizeX;
      trackData[2].clipShiftY = halfTrackHeight * pixelClipSizeY;
      trackData[2].textureX = atlasCoords.blX;
      trackData[2].textureY = atlasCoords.trY;
      trackData[2].textureLevel = atlasCoords.level;
      trackData[3].x = trackCentreX + tmcOffsetX;
      trackData[3].y = trackCentreY + tmcOffsetY;
      trackData[3].depth = trackDepth;
      trackData[3].clipShiftX = halfTrackWidth * pixelClipSizeX;
      trackData[3].clipShiftY = halfTrackHeight * pixelClipSizeY;
      trackData[3].textureX = atlasCoords.trX;
      trackData[3].textureY = atlasCoords.trY;
      trackData[3].textureLevel = atlasCoords.level;

      // Determine the colour to make heading indicators and history points based on this track's hostility
      GLuint hostilityColour;
      switch( currentTrack.m_hostility )
      {
      case TSLAPP6ASymbol::HostilityPending:
      case TSLAPP6ASymbol::HostilityUnknown:
        hostilityColour = 0xFF00FFFF;
        break;

      case TSLAPP6ASymbol::HostilityAssumedFriend:
      case TSLAPP6ASymbol::HostilityFriend:
         hostilityColour = 0xFFFFFF00;
        break;

      case TSLAPP6ASymbol::HostilityNeutral:
        hostilityColour = 0xFF00FF00;
        break;

      case TSLAPP6ASymbol::HostilitySuspect:
      case TSLAPP6ASymbol::HostilityHostile:
      case TSLAPP6ASymbol::HostilityJoker:
      case TSLAPP6ASymbol::HostilityFaker:
        hostilityColour = 0xFF0000FF;
        break;

      case TSLAPP6ASymbol::HostilityNone:
      default:
        hostilityColour = 0xFF000000;
        break;
      }

      // Display each of the track's history points in the hostility colour
      if( !renderingInterface->isDecluttered( NULL, m_historyPointsFeatureID ) )
      {
        for( size_t historyPoint = 0; historyPoint < currentTrack.m_historyPoints.size(); ++historyPoint, ++trackHistoryData, ++numHistoryPoints )
        {
          trackHistoryData->x = (GLfloat)(currentTrack.m_historyPoints[historyPoint].m_x - surfaceCoordinateCentreX);
          trackHistoryData->y = (GLfloat)(currentTrack.m_historyPoints[historyPoint].m_y - surfaceCoordinateCentreY);
          trackHistoryData->depth = histoyPointDepth;
          trackHistoryData->colour = hostilityColour;
        }
      }

      if( !renderingInterface->isDecluttered( NULL, m_headingIndicatorFeatureID ) )
      {
        // Fill in the heading indicator based on the track's position and orientation
        trackHeadingData[0].x = trackCentreX;
        trackHeadingData[0].y = trackCentreY;
        trackHeadingData[0].depth = headingDepth;
        trackHeadingData[1].x = trackCentreX + currentTrack.m_sinDisplayHeading * 50.0 * screenResX;
        trackHeadingData[1].y = trackCentreY + currentTrack.m_cosDisplayHeading * 50.0 * screenResY;
        trackHeadingData[1].depth = headingDepth;

        // Make the heading indicator's colour the same as the track's hostility colour
        trackHeadingData[0].colour = hostilityColour;
        trackHeadingData[1].colour = hostilityColour;
        trackHeadingData += 2;
        ++numDisplayLines;
      }

      // If there are additional labels beyond those baked into the texture atlas for the track, display them now.
      // These labels change over time for every track, so putting them into the texture atlas is not useful.
      if( currentTrack.m_speedLabel )
      {
        renderingInterface->drawEntity( currentTrack.m_speedLabel );
      }
      if( currentTrack.m_positionLabel )
      {
        renderingInterface->drawEntity( currentTrack.m_positionLabel );
      }

      ++numVisibleTracks;
      trackData += 4;
    }
  }

  GLsizeiptr lineDataSize = numDisplayLines * 2 * sizeof(TrackVertex);
  if( displayInfo->m_selectedTrack < displayInfo->m_tracks.size() )
  {
    // A track is currently selected. Display a box around it to show which one it is.
    const Track::DisplayInfo &selectedTrack = displayInfo->m_tracks[displayInfo->m_selectedTrack];
    GLfloat trackCentreX = (GLfloat)(selectedTrack.m_x - surfaceCoordinateCentreX);
    GLfloat trackCentreY = (GLfloat)(selectedTrack.m_y - surfaceCoordinateCentreY);

    GLfloat halfBoxHeight = selectedTrack.m_size / 2.0f;
    GLfloat halfBoxSizeY = halfBoxHeight * screenResY;
    GLfloat halfBoxSizeX = halfBoxHeight * screenResX;

    // Apply the modelview matrix to the centre of the selection box in order to give us
    // the correct position around which to draw the box. We then expand this in the drawing
    // surface's coordinate system in order to give us a box that doesn't rotate with the
    // drawing surface.
    const GLfloat *modelViewMatrix = glSurface->modelViewMatrix();

    GLfloat transformedPosX = modelViewMatrix[0] * trackCentreX +
                              modelViewMatrix[4] * trackCentreY +
                              modelViewMatrix[12];

    GLfloat transformedPosY = modelViewMatrix[1] * trackCentreX +
                              modelViewMatrix[5] * trackCentreY +
                              modelViewMatrix[13];

    GLfloat selectionBoxDepth = nonConstGLSurface->acquireDepthSlice();

    trackHeadingData[0].x = transformedPosX - halfBoxSizeX;
    trackHeadingData[0].y = transformedPosY - halfBoxSizeY;
    trackHeadingData[0].depth = selectionBoxDepth;
    trackHeadingData[0].colour = 0xFFFFFFFF;
    trackHeadingData[1].x = transformedPosX + halfBoxSizeX;
    trackHeadingData[1].y = transformedPosY - halfBoxSizeY;
    trackHeadingData[1].depth = selectionBoxDepth;
    trackHeadingData[1].colour = 0xFFFFFFFF;

    trackHeadingData[2].x = transformedPosX + halfBoxSizeX;
    trackHeadingData[2].y = transformedPosY - halfBoxSizeY;
    trackHeadingData[2].depth = selectionBoxDepth;
    trackHeadingData[2].colour = 0xFFFFFFFF;
    trackHeadingData[3].x = transformedPosX + halfBoxSizeX;
    trackHeadingData[3].y = transformedPosY + halfBoxSizeY;
    trackHeadingData[3].depth = selectionBoxDepth;
    trackHeadingData[3].colour = 0xFFFFFFFF;

    trackHeadingData[4].x = transformedPosX + halfBoxSizeX;
    trackHeadingData[4].y = transformedPosY + halfBoxSizeY;
    trackHeadingData[4].depth = selectionBoxDepth;
    trackHeadingData[4].colour = 0xFFFFFFFF;
    trackHeadingData[5].x = transformedPosX - halfBoxSizeX;
    trackHeadingData[5].y = transformedPosY + halfBoxSizeY;
    trackHeadingData[5].depth = selectionBoxDepth;
    trackHeadingData[5].colour = 0xFFFFFFFF;

    trackHeadingData[6].x = transformedPosX - halfBoxSizeX;
    trackHeadingData[6].y = transformedPosY + halfBoxSizeY;
    trackHeadingData[6].depth = selectionBoxDepth;
    trackHeadingData[6].colour = 0xFFFFFFFF;
    trackHeadingData[7].x = transformedPosX - halfBoxSizeX;
    trackHeadingData[7].y = transformedPosY - halfBoxSizeY;
    trackHeadingData[7].depth = selectionBoxDepth;
    trackHeadingData[7].colour = 0xFFFFFFFF;

    lineDataSize += 8 * sizeof(TrackVertex);
  }

  // Upload the finished set of drawing data to the GPU
  stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackHeadingVBO );
  glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0, lineDataSize );
  glUnmapBuffer( GL_ARRAY_BUFFER );

  stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackDisplayVBO );
  glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0, numVisibleTracks * 4 * sizeof(TrackTextureVertex) );
  glUnmapBuffer( GL_ARRAY_BUFFER );

  stateTracker->bindBuffer( GL_ARRAY_BUFFER, m_trackHistoryVBO );
  glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0, numHistoryPoints * sizeof(TrackVertex) );
  glUnmapBuffer( GL_ARRAY_BUFFER );

  if( numVisibleTracks > 0 )
  {
    // Calculate the ModelViewProjection matrix for items that should be rotated with the drawing surface rotation
    GLfloat mvpMatrix[16];
    GLHelpers::matrixMultiply( glSurface->projectionMatrix(), glSurface->modelViewMatrix(), mvpMatrix );

    stateTracker->disableBlending();
    stateTracker->enableDepthTest();
    stateTracker->depthFunction( GL_LESS );
    stateTracker->enableMultisample();

    // We are not using primitive restart, however the drawing surface might be. Ensure that this is disabled
    // so that it doesn't affect our track rendering.
    stateTracker->disablePrimitiveRestart();

    // First draw the track heading indicators
    GLuint originalVAO = stateTracker->bindVertexArrayObject( m_trackHeadingVAO );
    if( numDisplayLines > 0 )
    {
      stateTracker->useProgram( m_trackHeadingShader->m_program );

      glUniformMatrix4fv( m_trackHeadingMVPMatrix, 1, GL_FALSE, mvpMatrix );
      
      // Draw all the headings in one go for best performance
      glDrawArrays( GL_LINES, 0, numDisplayLines * 2 );
    }

    // Then draw the track history points
    if( numHistoryPoints > 0 )
    {
      stateTracker->useProgram( m_trackHistoryShader->m_program );

      glUniformMatrix4fv( m_trackHistoryMVPMatrix, 1, GL_FALSE, mvpMatrix );

      stateTracker->bindVertexArrayObject( m_trackHistoryVAO );
      stateTracker->enablePointSprites();

      // Draw all the history points in one go for best performance
      glDrawArrays( GL_POINTS, 0, numHistoryPoints );
    }

    // Then draw the tracks themselves
    stateTracker->useProgram( m_trackBodyShader->m_program );

    // Use the matrix that removes drawing surface rotation so the track bodies all appear unrotated
    glUniformMatrix4fv( m_trackBodyMVPMatrix, 1, GL_FALSE, mvpMatrix );

    stateTracker->bindVertexArrayObject( m_trackDisplayVAO );
    stateTracker->bindTexture( GL_TEXTURE0, GL_TEXTURE_2D_ARRAY, m_atlas->textureID() );
    stateTracker->enableBlending(); // The track textures require blending so that the unfilled areas of the atlas appear transparent
    stateTracker->disableMultisample();

    // Draw all the tracks in one go for best performance
    glDrawElements( GL_TRIANGLES, numVisibleTracks * 6, GL_UNSIGNED_INT, NULL );

    if( displayInfo->m_selectedTrack < displayInfo->m_tracks.size() )
    {
      // If we have a selected track, draw the selection box last so that it appears on top
      stateTracker->bindVertexArrayObject( m_trackHeadingVAO );
      stateTracker->useProgram( m_trackHeadingShader->m_program );

      // We have already applied the modelview matrix to the selection box, so only send the projection matrix
      glUniformMatrix4fv( m_trackHeadingMVPMatrix, 1, GL_FALSE, glSurface->projectionMatrix() );
      
      glDrawArrays( GL_LINES, numDisplayLines * 2, 8 );
    }

    // Finally restore the original VAO binding
    stateTracker->bindVertexArrayObject( originalVAO );
  }

  return true;
}

void TrackLayer::releaseResources(int /*surfaceID*/)
{
  // This layer performs drawing independently of MapLink. So it does not need any implementation of releaseResources.
}

void TrackLayer::rasteriseTrack( const Track::DisplayInfo &track, TSLOpenGLSurface *surface )
{
  // Ensure there are no outstanding draws that might affect what we're about to do
  surface->flushPendingDraws();

  // In order to rasterise the symbol, we first need to get it in a form that can be drawn using a drawing surface.
  // In our case, this is the TSLEntitySet representation. We can draw this using a TSLStandardDataLayer in conjunction
  // with the MapLink drawing surface and a framebuffer object to render directly to our texture atlas.
  TSLAPP6ASymbol symbol;
  TrackManager::instance().symbolHelper()->getSymbol( track.m_symbolKey, symbol );
  symbol.hostility( track.m_hostility );
  symbol.height( track.m_size );
  symbol.heightType( TSLDimensionUnitsPixels );

  // Add any required fixed annotations to the symbol. We will bake these into the texture atlas as they
  // don't change over time.
  // For static labels we don't have specific strings to use, so just fill in a basic identifier to show where
  // the label is in relation to the symbol.
  switch( m_lastAnnotationLevel )
  {
  case AnnotationHigh:
    symbol.additionalInformation("AdditionalInfo");
    symbol.C2HQName("C2HQName");
    symbol.designation("Designation");
    // Fall through

  case AnnotationMedium:
    symbol.staffComments("Comments");
    symbol.higherFormation("Formation");
    symbol.quantity( "Quality" );
    symbol.combatEffectiveness( "Effectiveness" );
    // Fall through

  case AnnotationLow:
    // Position and speed annotations are enabled, but as they vary on a per-track basis they aren't
    // stored in the texture atlas and are instead rendered seperately.
    symbol.unitSize( TSLAPP6ASymbol::UnitSizeArmy );
    break;

  case AnnotationNone:
  default:
    break;
  }
  symbol.textColour( TSLComposeRGB( 255, 255, 255 ) );

  TSLEntitySet *es = TrackManager::instance().symbolHelper()->getSymbolAsEntitySet( &symbol );
  if( !es )
  {
    // This symbol is not valid
    return;
  }
  // Apply a black halo to the annotation text so it is more visible.
  applyHaloTextStyle( es, TSLComposeRGB( 0, 0, 0 ) );

  // Rather than alter the settings of the main drawing surface to render our symbol, we will instead use
  // a 'child' drawing surface. This is a drawing surface that uses the same OpenGL context as the main drawing surface (and thus
  // is bound to the same thread) as well as some of the internal OpenGL resources of the surface it was created from. The child
  // surface does not have any of the same layers as the parent surface and maintains its own view settings, so we can use it to
  // render different views without altering the settings of the main drawing surface.
  TSLOpenGLSurface *childSurface = surface->createChildSurface();

  // As we are drawing to the texture atlas we need to tell the child drawing surface the size of the
  // render target, which is the size of the texture atlas
  childSurface->wndResize( 0, 0, m_atlas->atlasDimensions(), m_atlas->atlasDimensions(), false );

  // Choose an arbitrary TMC per DU to use when rendering to our texture atlas
  double tmcPerDUX = 1000.0;
  double tmcPerDUY = 1000.0;

  // Set up the drawing surface to view the current layer of the texture atlas with our arbitrary coordinate
  // system
  double viewSizeX = m_atlas->atlasDimensions() * tmcPerDUX;
  double viewSizeY = m_atlas->atlasDimensions() * tmcPerDUY;

  double uuX1, uuY1, uuX2, uuY2;
  childSurface->TMCToUU( 0, 0, &uuX1, &uuY1 );
  childSurface->TMCToUU( viewSizeX, viewSizeY, &uuX2, &uuY2 );
  childSurface->resize( uuX1, uuY1, uuX2, uuY2, false );

  // Set up the drawing surface to write to this section of the atlas by using a framebuffer object.
  // Both the parent and child drawing surfaces will have the same state tracker object, so changes made to one
  // are automatically reflected in the other.
  TSLOpenGLStateTracker *stateTracker = childSurface->stateTracker();
  stateTracker->bindFramebuffer( GL_DRAW_FRAMEBUFFER, m_fbo );
  glFramebufferTextureLayer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_atlas->textureID(), 0,
                             m_atlas->currentTextureLevel() );

  GLenum framebufferStatus = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );
  switch( framebufferStatus )
  {
    case GL_FRAMEBUFFER_COMPLETE:
      break; // All is OK

    default:
    {
      assert( false );
    }
  }

  // Perform a dummy draw with the drawing surface to initialise it for the subsequent extent calculations
  // we need to do to determine the symbol size. This doesn't actually draw anything as the surface is empty
  // and we ask it not to perform a clear.
  childSurface->drawDU( 0, 0, m_atlas->atlasDimensions(), m_atlas->atlasDimensions(), false );

  // Create a temporary data layer to draw the symbol and add it to the chid drawing surface
  TSLStandardDataLayer *tempLayer = new TSLStandardDataLayer();
  tempLayer->entitySet()->insert( es );

  childSurface->addDataLayer( tempLayer, "symbol" );
  // As we're not attaching a depth buffer to our texture atlas, ensure that no draw ordering takes place
  // when drawing the symbol. As the symbol will only be drawn to the atlas once the performance impact from
  // doing this is small.
  childSurface->setLayerTransparencyHint( "symbol", TSLOpenGLTransparencyHintAlways );

  // Determine the pixel size of the symbol.
  childSurface->updateEntityExtent( es, "symbol" );
  TSLEnvelope symbolExtent = es->envelope( childSurface->id() );

  // Adjust the extent so that it is central
  TSLTMC maxX = std::max( abs( symbolExtent.xMax() ), abs( symbolExtent.xMin() ) ) ;
  TSLTMC maxY = std::max( abs( symbolExtent.yMax() ), abs( symbolExtent.yMin() ) ) ;

  symbolExtent.corners( -maxX, -maxY, maxX, maxY ) ;

  // The extent of the symbol combined with our arbitrary coordinate system tells us how big the symbol is
  uint32_t rasterisedWidth = ceil( symbolExtent.width() / tmcPerDUX );
  uint32_t rasterisedHeight = ceil( symbolExtent.height() / tmcPerDUY );

  TextureAtlas::AtlasLocation atlasLocation = m_atlas->allocateSpace( surface, rasterisedWidth, rasterisedHeight );

  // Reconfigure our framebuffer in case the level of the texture atlas we need to use changed since our original binding.
  // This can happen if there wasn't enough space in the current level for the symbol we're rasterising.
  glFramebufferTextureLayer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_atlas->textureID(), 0,
                             m_atlas->currentTextureLevel() );

  framebufferStatus = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );
  switch( framebufferStatus )
  {
    case GL_FRAMEBUFFER_COMPLETE:
      break; // All is OK

    default:
    {
      assert( false );
    }
  }

  // We now need to position the drawing surface so that our symbol appears in the position we reserved
  // in the atlas. This means that the bottom left corner of the symbol's envelope needs to be positioned
  // at the reserved position in the atlas.
  es->move( TSLCoord(atlasLocation.blX * tmcPerDUX, atlasLocation.blY * tmcPerDUY), symbolExtent.bottomLeft() );

  // Now draw the symbol to the atlas. Enable the 'updateExtentOnly' flag to enable OpenGL scissoring around
  // the given extent. This prevents accidentally overwriting other parts of the atlas.
  childSurface->drawDU( atlasLocation.blX, m_atlas->atlasDimensions() - atlasLocation.trY, atlasLocation.trX,
                        m_atlas->atlasDimensions() - atlasLocation.blY, false, true );

  // Record where in the texture atlas this type of track is so that we can look it up when the track needs to be drawn
  RasterisedTrack textureCoords;
  GLfloat pixelSize = 1.0f / m_atlas->atlasDimensions();
  textureCoords.blX = pixelSize * atlasLocation.blX;
  textureCoords.blY = pixelSize * atlasLocation.blY;
  textureCoords.trX = pixelSize * atlasLocation.trX;
  textureCoords.trY = pixelSize * atlasLocation.trY;
  textureCoords.level = atlasLocation.level;
  textureCoords.width = rasterisedWidth;
  textureCoords.height = rasterisedHeight;

  // Store the origin of the symbol so that at draw time we will correctly position symbols whose origin isn't 0,0
  textureCoords.offsetX = symbolExtent.centre().x() / tmcPerDUX;
  textureCoords.offsetY = symbolExtent.centre().y() / tmcPerDUY;
  m_rasterisedTracks[make_pair( track.m_symbolKey, track.m_hostility )] = textureCoords;

  // Deleting the data layer automatically removes it from the drawing surface
  tempLayer->destroy();

  // We no longer need the child drawing surface, so delete it
  delete childSurface;

  // Make the window the render target again
  stateTracker->bindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

  // Calling drawDU with updateExtentOnly set to true enables scissor testing, turn this off again as we don't want it to affect
  // the rest of our drawing
  glDisable( GL_SCISSOR_TEST );
}

void TrackLayer::reset( TSLOpenGLSurface *surface )
{
  if( m_atlas )
  {
    m_atlas->clear( surface );
    m_rasterisedTracks.clear();
  }
}

void TrackLayer::applyHaloTextStyle( TSLEntitySet *set, TSLStyleID colour )
{
  int numEntities = set->size();
  for( int i = 0; i < numEntities; ++i )
  {
    TSLEntity *entity = (*set)[i];
    if( entity->type() == TSLGeometryTypeEntitySet )
    {
      applyHaloTextStyle( reinterpret_cast<TSLEntitySet*>(entity), colour );
    }
    else if( entity->type() == TSLGeometryTypeText )
    {
      entity->setRendering( TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo );
      entity->setRendering( TSLRenderingAttributeTextBackgroundColour, colour );
    }
  }
}
