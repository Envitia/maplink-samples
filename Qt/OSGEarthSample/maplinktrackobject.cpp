/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "maplinktrackobject.h"

#include <osgEarth/Units>
#include <osgEarth/GeoMath>
#include <osgEarthAnnotation/AnnotationData>

using namespace osgEarth;


static const MaplinkTrackObjectProperties possibleMaplinkTrackObjects[] = 
{
  {"1.x.2.1.1.1"   , 15000 , 250},    // Bomber
  {"1.x.2.1.1.6"   , 12000 , 200},    // Tanker
  {"1.x.2.1.1.10.2", 20000 , 300},    // Surveillance
  {"1.x.2.1.1.16"  , 5000  , 150},    // Uav
  {"1.x.2.1.1.17"  , 2500  , 100},    // Anti-submarine
  {"1.x.2.1.2.1"   , 10000 , 700},    // Attack-air
  {"1.x.2.1.2.7"   , 2000  , 100},    // Uav (rotary)
  {"1.x.2.1.3"     , 1000  , 50 },    // Military balloon
  {"1.x.2.3.1"     , 7500  , 200},    // Fixed-wing civillian
  {"1.x.2.3.2"     , 4000  , 100},    // Rotary-wing civillian
};
static const size_t possibleMaplinkTrackObjectsSize = sizeof(possibleMaplinkTrackObjects)/sizeof(possibleMaplinkTrackObjects[0]);



const MaplinkTrackObjectProperties& MaplinkTrackObject::getRandomPossibleObject()
{
  return possibleMaplinkTrackObjects[rand() % possibleMaplinkTrackObjectsSize];
}

const TSLAPP6ASymbol::HostilityEnum MaplinkTrackObject::getRandomPossibleHostility()
{
  switch(rand() % 7)
  {
    case 0:
      return TSLAPP6ASymbol::HostilityAssumedFriend;
    case 1:
      return TSLAPP6ASymbol::HostilityFriend;
    case 2:
      return TSLAPP6ASymbol::HostilityNeutral;
    case 3:
      return TSLAPP6ASymbol::HostilitySuspect;
    case 4:
      return TSLAPP6ASymbol::HostilityHostile;
    case 5:
      return TSLAPP6ASymbol::HostilityJoker;
    case 6:
      return TSLAPP6ASymbol::HostilityFaker;
    default:
      return TSLAPP6ASymbol::HostilityFriend;
  }
}

const double MaplinkTrackObject::random(double min, double max)
{
  return ((max - min) * ( (double)rand() / (double)RAND_MAX ) + min);
}

MaplinkTrackObject::MaplinkTrackObject(MapNode* mapNode, osg::Group* parent, int trackIndex, osgEarth::Annotation::TrackNodeFieldSchema& schema, envitia::MapLink::MilitarySymbols& maplinkSymbols)
  : m_startTime(0.0)
  , m_greatCircleDistance(0.0)
  , m_parent(parent)
{
  // Setup the maplink APP6a symbol
  m_app6aSymbol.textColour(216); //white
  m_app6aSymbol.height(ICONSIZE);
  m_app6aSymbol.heightType(TSLDimensionUnitsPixels);
  m_app6aSymbol.isFramed(true);
  m_app6aSymbol.quantity("");
  m_app6aSymbol.unitSize(TSLAPP6ASymbol::UnitSizeNone);
  m_app6aSymbol.higherFormation("");
  m_app6aSymbol.name("");
  m_app6aSymbol.speed("");
  m_app6aSymbol.staffComments("");
  m_app6aSymbol.combatEffectiveness("");
  m_app6aSymbol.designation("");
  m_app6aSymbol.C2HQName("");
  m_app6aSymbol.additionalInformation("");
  m_app6aSymbol.altitudeOrDepth("");

  //Generate a symbol using one of the possible ids, and a random hostility value
  MaplinkTrackObjectProperties properties = getRandomPossibleObject();
  m_app6aSymbol.id( properties.id );
  m_app6aSymbol.hostility( getRandomPossibleHostility() );

  // set lat/lon to random values
  m_startLatitude  = random(-90.0, 90.0);
  m_endLatitude    = random(-90.0, 90.0);
  m_startLongitude = random(-180.0, 180.0);
  m_endLongitude   = random(-180.0, 180.0);
  const SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();
  GeoPoint pos(geoSRS, m_startLongitude.as(Units::DEGREES), m_startLatitude.as(Units::DEGREES));

  // Use preset speed and positions to calculate time to target point
  m_speed = properties.speed;
  m_altitude = properties.altitude;
  m_greatCircleDistance = TSLCoordinateConverter::greatCircleDistance(m_startLatitude.as(Units::DEGREES), 
                                                                m_startLongitude.as(Units::DEGREES), 
                                                                m_endLatitude.as(Units::DEGREES), 
                                                                m_endLongitude.as(Units::DEGREES));

  m_eta = m_greatCircleDistance / m_speed;


  // Convert the Maplink APP6A Symbol to an osgEarth track
  osg::ref_ptr<osg::Image> image = maplinkSymbols.draw(m_app6aSymbol, ICONSIZE);

  m_trackNode = new Annotation::TrackNode(mapNode, pos, image, schema );

  m_trackNode->setFieldValue( SCHEMAFIELD_NAME, Stringify() << "Track" << trackIndex );
  m_trackNode->setFieldValue( SCHEMAFIELD_POSITION, Stringify() << "" );


  m_trackNode->setPriority( (float)m_altitude );

  m_parent->addChild( m_trackNode );
}

MaplinkTrackObject::~MaplinkTrackObject()
{
  m_parent->removeChild((osg::Node*)m_trackNode);
}

void MaplinkTrackObject::update(const double& time, const int& simulationSpeed, const PositionFormat& positionFormat)
{
  // Calculate how far along the path the track should be
  double routeProgress = ((time - m_startTime)*simulationSpeed) / m_eta;

  osg::Vec3d pos;
  GeoMath::interpolate( m_startLatitude.as(Units::RADIANS),
                        m_startLongitude.as(Units::RADIANS),
                        m_endLatitude.as(Units::RADIANS),
                        m_endLongitude.as(Units::RADIANS),
                        routeProgress,
                        pos.y(), pos.x() );

  double x(osg::RadiansToDegrees(pos.x()));
  double y(osg::RadiansToDegrees(pos.y()));

  GeoPoint geo( m_trackNode->getMapNode()->getMapSRS(),
                x,
                y,
                m_altitude,
                ALTMODE_ABSOLUTE);

  m_trackNode->setPosition(geo);


  switch(positionFormat)
  {
    case FORMAT_GARS:
      {
        const char* garsStr = TSLCoordinateConverter::latLongToGARS(y, x);
        if( garsStr )
        {
          m_trackNode->setFieldValue( SCHEMAFIELD_POSITION, garsStr);
        }
      }
      break;
    case FORMAT_MGRS:
      {
        const char* mgrsStr = TSLCoordinateConverter::latLongToMGRS(y, x);
        if( mgrsStr )
        {
          m_trackNode->setFieldValue( SCHEMAFIELD_POSITION, mgrsStr);
        }
        break;
      }
    case FORMAT_LATLON:
      m_trackNode->setFieldValue( SCHEMAFIELD_POSITION, Stringify() << y << " : " << x );
      break;
    case FORMAT_NONE:
    default:
      m_trackNode->setFieldValue( SCHEMAFIELD_POSITION, "" );
      break;
  }
  
  // If the end point has been reached, set a new target
  if( routeProgress >= 1.0 )
  {
    m_startLatitude = geo.y();
    m_startLongitude = geo.x();
    m_endLatitude = random(-90.0, 90.0);
    m_endLongitude = random(-180.0, 180.0);

    double distance = TSLCoordinateConverter::greatCircleDistance(m_startLatitude.as(Units::DEGREES), 
                                                                  m_startLongitude.as(Units::DEGREES), 
                                                                  m_endLatitude.as(Units::DEGREES), 
                                                                  m_endLongitude.as(Units::DEGREES));

    m_eta = distance / m_speed;
    m_startTime = time;
  }
}
