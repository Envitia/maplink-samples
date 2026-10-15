/****************************************************************************
    Copyright (c) 2000-2017 by Envitia Group PLC
=============================================================================
MODULE          : 
PACKAGE         : MapLink
SCCS            : 
AUTHOR(s)       : D.Dixon
=============================================================================
DESCRIPTION     : 
****************************************************************************/

#include "stdafx.h"
#include "APP6ATrack.h"
#include "DOAPP6ATrack.h"
#include "TrackManager.h"


// position in APP6A config file
#define OWNSHIP_SYMBOL          594
#define AIRCRAFT_CARRIER_SYMBOL 594
#define BOMBER_SYMBOL           9
#define DESTROYER_SYMBOL        597
#define FRIGATE_SYMBOL          598
#define GENERAL_AIR_SYMBOL      7 
#define HELICOPTER_SYMBOL       39
#define INTERCEPTOR_SYMBOL      11
#define SUBMARINE_SYMBOL        658

// pixels size
#define SYMBOL_HEIGHT   50

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APP6ATrack::APP6ATrack(TSLAPP6AHelper* helper, HOSTILE_TYPE hostility, int type, int app6aType)
 : m_type(type)
 , m_app6aType(app6aType)
 , m_symbolAsEntSet(0)
 , m_entitySetDirty(true)
 , m_symbolHelper(helper)
{
  m_hostility = hostility;
  double vel = 0.0, alt = 0.0;
  CTrackManager::initialiseTrack( (TRACK_TYPE)m_type, vel, alt ) ;
  m_velocity = vel ;
  m_altitude = alt ;

  updateCourse() ;

  m_symbol.textColour(216); //white
  m_symbol.key(typeToAPP6ASymbol(type));
  m_symbol.height(SYMBOL_HEIGHT);
  m_symbol.heightType(TSLDimensionUnitsPixels);
  m_symbol.hostility((TSLAPP6ASymbol::HostilityEnum) hostilityToAPP6AHostility(hostility));
  m_symbol.isFramed(true);
  m_symbol.type((TSLAPP6ASymbol::TypeEnum)m_app6aType);

  setAnnotationLevel(CTrackManager::getAnnotationLevel());

  getAsEntitySet();
}

APP6ATrack::~APP6ATrack()
{
  }

APP6ATrack::APP6ATrack( const APP6ATrack & other ) :
  CTrack( (const CTrack &)other )
{
  m_symbol = other.m_symbol;
  m_type = other.m_type;
  m_app6aType = other.m_app6aType;
  m_symbolHelper = other.m_symbolHelper;
  m_symbolAsEntSet = m_symbolHelper->getSymbolAsEntitySet(&m_symbol);
  m_symbolAsEntSet->translate(other.position().x(), other.position().y());
  setEntity((TSLEntity *)m_symbolAsEntSet, true);
}

TSLDisplayObject * APP6ATrack::instantiateDO( TSLDisplayType key, int dsID ) const
{
  return new DOAPP6ATrack( ) ;
}

TRACK_TYPE APP6ATrack::trackType() const
{
  return (TRACK_TYPE)m_type ;
}

void APP6ATrack::hostility( HOSTILE_TYPE hostility )
{
  m_hostility = hostility ;
  m_symbol.hostility((TSLAPP6ASymbol::HostilityEnum) hostilityToAPP6AHostility(m_hostility));
  m_entitySetDirty = true;
}

void APP6ATrack::setAnnotationLevel ( AnnotationLevelEnum level )
{
  m_symbol.quantity(0);
  m_symbol.unitSize(TSLAPP6ASymbol::UnitSizeNone);
  m_symbol.higherFormation(0);
  m_symbol.name(0);
  m_symbol.speed(0);
  m_symbol.staffComments(0);
  m_symbol.combatEffectiveness(0);
  m_symbol.designation(0);
  m_symbol.C2HQName(0);
  m_symbol.additionalInformation(0);
  m_symbol.altitudeOrDepth(0);


  switch ( level )
  {
  case AnnotationLevelHigh:
    m_symbol.altitudeOrDepth("AltDepth");
    m_symbol.additionalInformation("AdditionalInfo");
    m_symbol.C2HQName("C2HQName");
    m_symbol.designation("Designation");
  case AnnotationLevelMedium:
    m_symbol.staffComments("Comments");
    m_symbol.speed("Speed");
    m_symbol.name("Name");
    m_symbol.higherFormation("Formation");
  case AnnotationLevelLow:
    m_symbol.combatEffectiveness("Effectiveness");
    m_symbol.unitSize(TSLAPP6ASymbol::UnitSizeArmy);
    m_symbol.quantity("Quality");
  case AnnotationLevelNone:
    break;
  } 

  m_entitySetDirty = true;
}

TSLEntitySet* APP6ATrack::getAsEntitySet ()
{
  if ( m_entitySetDirty )
  {
    m_symbolAsEntSet = m_symbolHelper->getSymbolAsEntitySet(&m_symbol);
    m_symbolAsEntSet->translate(position().x(), position().y());
    const int size = m_symbolAsEntSet->size();
    for (int i = 0; i < size; ++i)
    {
      TSLEntity * ent = m_symbolAsEntSet->operator[](i);
      if ( TSLText::isText(ent ) )
      {
        ent->setRendering(TSLRenderingAttributeTextRotatable, false);
      }
      if ( TSLSymbol::isSymbol(ent ) )
      {
        ent->setRendering(TSLRenderingAttributeSymbolRotatable, (int) TSLSymbolRotationDisabled );
      }
    }
    setEntity((TSLEntity *)m_symbolAsEntSet, true);
    m_entitySetDirty = false;
  }

  return m_symbolAsEntSet;
}

int APP6ATrack::typeToAPP6ASymbol ( int type )
{
  switch( type )
  {
    case AIRCRAFT_CARRIER :
      return AIRCRAFT_CARRIER_SYMBOL;
    case BOMBER:
      return BOMBER_SYMBOL;
    case DESTROYER:
      return DESTROYER_SYMBOL;
    case FRIGATE:
      return FRIGATE_SYMBOL;
    case GENERAL_AIR:
      return GENERAL_AIR_SYMBOL;
    case HELICOPTER:
      return HELICOPTER_SYMBOL;
    case INTERCEPTOR:
      return INTERCEPTOR_SYMBOL;
    case SUBMARINE:
      return SUBMARINE_SYMBOL;
    default:
      return 0;
  }
}

int APP6ATrack::hostilityToAPP6AHostility ( HOSTILE_TYPE hostility )
{
  switch ( hostility )
  {
  case HOSTILE:
    return (int)TSLAPP6ASymbol::HostilityHostile;
  case FRIENDLY:
    return (int)TSLAPP6ASymbol::HostilityFriend;
  case NEUTRAL:
    return (int)TSLAPP6ASymbol::HostilityNeutral;
  case PENDING:
    return (int)TSLAPP6ASymbol::HostilityPending;
  case ASSUMED_FRIEND:
    return (int)TSLAPP6ASymbol::HostilityAssumedFriend;
  case SUSPECT:
    return (int)TSLAPP6ASymbol::HostilitySuspect;
  case JOKER:
    return (int)TSLAPP6ASymbol::HostilityJoker;
  case FAKER:
    return (int)TSLAPP6ASymbol::HostilityFaker;
  case UNKNOWN:
  default:
    return (int)TSLAPP6ASymbol::HostilityUnknown;
  }
}

bool APP6ATrack::getAsSymbol(HBITMAP bitmap)
{
  static const bool clearTheBitmap = false;

  return m_symbolHelper->getSymbolAsBitmap( &m_symbol, (TSLBitmapHandle)bitmap, clearTheBitmap );
}
