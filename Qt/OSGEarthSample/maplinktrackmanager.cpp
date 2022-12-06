/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include "simulationoptionsdialog.h"
#include "maplinktrackmanager.h"

#include <QProgressDialog>

#include <osg/OperationThread>

#include <osgEarth/MapNode>
#include <osgEarthAnnotation/TrackNode>
#include <osgEarth/ScreenSpaceLayout>

using namespace osgEarth;

MaplinkTrackManager::MaplinkTrackManager(int numTracks, osg::Group* rootNode, osgEarth::MapNode* mapNode, bool declutter, QWidget* mainWindow)
  : osg::Operation( "trackmanager", true ) // Set this operations name, and set it to repeat
  , m_simulationSpeed(1)
  , m_mapNode( mapNode )
  , m_positionFormat( MaplinkTrackObject::FORMAT_GARS )
  , m_mainWindow( mainWindow )
  , m_skipUpdate( false )
{
  // Setup the track schema
  // draw the track name above the icon:
  TextSymbol* nameSymbol = new TextSymbol();
  nameSymbol->pixelOffset()->set( 0, 2+ICONSIZE/2 );
  nameSymbol->alignment() = TextSymbol::ALIGN_CENTER_BOTTOM;
  nameSymbol->halo()->color() = Color::Black;
  nameSymbol->size() = nameSymbol->size()->eval() + 2.0f;
  m_trackNodeSchema[SCHEMAFIELD_NAME] = Annotation::TrackNodeField(nameSymbol, false);

  // draw the track coordinates below the icon:
  TextSymbol* posSymbol = new TextSymbol();
  posSymbol->pixelOffset()->set( 0, -2-ICONSIZE/2 );
  posSymbol->alignment() = TextSymbol::ALIGN_CENTER_TOP;
  posSymbol->fill()->color() = Color::Yellow;
  posSymbol->size() = posSymbol->size()->eval() - 2.0f;
  m_trackNodeSchema[SCHEMAFIELD_POSITION] = Annotation::TrackNodeField(posSymbol, true);

  srand(time(NULL));

  // Initialise the tracks
  m_tracksGroup = new osg::Group();
  addOrRemoveTracks(numTracks);

  rootNode->addChild( m_tracksGroup );

  osg::StateSet* declutterStateSet = m_tracksGroup->getOrCreateStateSet();

  osgEarth::ScreenSpaceLayout::activate(declutterStateSet);
  
  osgEarth::ScreenSpaceLayoutOptions declutterOptions( osgEarth::ScreenSpaceLayout::getOptions() );
  declutterOptions.inAnimationTime() = 1.0f;
  declutterOptions.outAnimationTime() = 1.0f;
  declutterOptions.sortByPriority() = true;
  osgEarth::ScreenSpaceLayout::setOptions( declutterOptions );

  osgEarth::ScreenSpaceLayout::setDeclutteringEnabled(declutter);
}

MaplinkTrackManager::~MaplinkTrackManager()
{
  m_tracks.clear();
}

void MaplinkTrackManager::operator()(osg::Object* obj)
{
  if( m_skipUpdate )
  {
    return;
  }
  osg::View* view = dynamic_cast<osg::View*>(obj);
  // Time in seconds since the start of the simulation
  double time = view->getFrameStamp()->getSimulationTime();
  MaplinkTracks::iterator it( m_tracks.begin() );
  MaplinkTracks::iterator end( m_tracks.end() );
  while( it != end )
  {
    it->get()->update(time, m_simulationSpeed, m_positionFormat);
    ++it;
  }
}

void MaplinkTrackManager::addOrRemoveTracks(int targetNumber)
{
  int numberOfOperations = 0;
  if( m_tracks.size() < targetNumber )
  {
    numberOfOperations = targetNumber - m_tracks.size();
    m_tracks.reserve(numberOfOperations);
  }
  else if( m_tracks.size() > targetNumber )
  {
    numberOfOperations = m_tracks.size() - targetNumber;
  }

  QProgressDialog progress("Modifying Tracks...", "Cancel",m_tracks.size(), numberOfOperations, m_mainWindow);
  progress.setWindowModality(Qt::WindowModal);

  while(m_tracks.size() < targetNumber)
  {
    m_tracks.push_back( new MaplinkTrackObject(m_mapNode, m_tracksGroup, m_tracks.size(), m_trackNodeSchema, m_maplinkSymbols) );
    progress.setValue(m_tracks.size());
    if( progress.wasCanceled() )
    {
      return;
    }
  }
  while(m_tracks.size() > targetNumber)
  {
    m_tracks.pop_back();

    progress.setValue(m_tracks.size());
    if( progress.wasCanceled() )
    {
      return;
    }
  }
}

void MaplinkTrackManager::decluttering(bool enabled)
{
  osgEarth::ScreenSpaceLayout::setDeclutteringEnabled(enabled);
}

void MaplinkTrackManager::showSimulationOptions()
{
  // Create the options dialog, and make it modal to the main window
  SimulationOptionsDialog options(m_mainWindow);
  options.simulationSpeed(m_simulationSpeed);
  options.numTracks(m_tracks.size());
  options.positionFormat(m_positionFormat);
  options.exec();

  // While tracks are being added, don't perform any update operations
  // This isn't necesarry, but makes the addition of tracks faster
  m_skipUpdate = true;
  addOrRemoveTracks(options.numTracks());
  m_skipUpdate = false;

  m_simulationSpeed = options.simulationSpeed();
  m_positionFormat = options.positionFormat();
}
