//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "s52stateobject.h"
#include <string>


S52StateObject::S52StateObject()
  : m_scale(0)
  , m_simpleSymbols(false)
  , m_simpleAreas(false)
  , m_contourLabels(true)
  , m_centreSymbols(false)
  , m_showText(true)
  , m_showTextHalo(false)
  , m_showSymbol(true)
    // PresLib e3.4 Part I January 2008
    //   12.2.20	Conditional Symbology Procedure 'SEABEDnn'
    //   page I61, 
  , m_deepContours(10)
  , m_shallowContours(2)
  , m_safetyDepth(4)
  , m_safetyContours(5)
  , m_displayMask(TSLS52DisplayCategoryEnum_DisplayBase)
  , m_paletteIndex(0)
  , m_displayNoDataLayer(false)
{
  __time64_t ltime;
  _time64( &ltime );
  
  m_date = ltime;
}

S52StateObject::~S52StateObject()
{

}

bool S52StateObject::CONTOUR_LABELS () const
{
  return m_contourLabels;
}

bool S52StateObject::CENTRE_SYMBOLS () const
{
  return m_centreSymbols;
}

TSLTimeType S52StateObject::CURRENT_DATE () const
{
  return m_date;
}

int S52StateObject::CURRENT_SCALE () const
{
  return m_scale;
}

double S52StateObject::DEEP_CONTOUR () const
{
  return m_deepContours;
}

bool S52StateObject::DISPLAY_AT_ALL_DATES () const
{
  return false;
}

bool S52StateObject::DISPLAY_AT_ALL_SCALES () const
{
  return false;
}

int S52StateObject::DISPLAY_CATEGORY_MASK () const
{
  return m_displayMask;
}

int S52StateObject::DISTANCE_TAGS () const
{
  return 0;
}

bool S52StateObject::FULL_SECTORS () const
{
  return true;
}

bool S52StateObject::LIGHT_DESCRIPTIONS () const
{
  return true;
}

bool S52StateObject::LOW_ACCURACY_SYMBOLS () const
{
  return true;
}

TSLCoord S52StateObject::OWNSHIP () const
{
  return m_ownShip;
}

bool S52StateObject::RADAR_ON () const
{
  return false;
}

double S52StateObject::SAFETY_CONTOUR () const
{
  return m_safetyContours;
}

double S52StateObject::SAFETY_DEPTH () const
{
  return m_safetyDepth;
}

double S52StateObject::SHALLOW_CONTOUR () const
{
  return m_shallowContours;
}

bool S52StateObject::SHALLOW_PATTERN () const
{
  return false;
}

bool S52StateObject::SHIPS_OUTLINE () const
{
  return true;
}

bool S52StateObject::SHOW_TEXT () const
{
  return m_showText;
}

bool S52StateObject::SHOW_TEXT_HALO() const
{
  return m_showTextHalo;
}

bool S52StateObject::SHOW_SHALLOW_DANGERS () const
{
  return false;
}

bool S52StateObject::SHOW_SYMBOLS () const
{
  return m_showSymbol;
}

bool S52StateObject::SIMPLE_AREAS() const
{
  return m_simpleAreas;
}

bool S52StateObject::SIMPLE_SYMBOLS() const
{
  return m_simpleSymbols;
}

bool S52StateObject::SYMBOL_DISPLAY () const
{
  return true;
}

double S52StateObject::TIME_TAGS () const
{
  return 0.0;
}

bool S52StateObject::TWO_SHADES () const
{
  return false;
}

TSLS52DisplayTextEnum S52StateObject::TEXT_DISPLAY () const
{
  return TSLS52DisplayText_All;
}

bool S52StateObject::VALIDATE_HO_DATA (const char* datasetName, const char* datasetAgency) const
{
  if ( !datasetName || !datasetAgency )
  {
    return false;
  }

  return strcmp(datasetAgency, "Hydrographic Office") == 0;
}

bool S52StateObject::VIEWING_GROUP_DISABLED (int viewingGroup) const
{
  switch ( viewingGroup )
  {
  // viewing groups from the S52 standard.
    case 31040: 
    case 27040:
    case 36050:
    case 36020:
    case 31010: // Meta objects
      return true;

    case 27000:
      if (m_displayBuoysBeacons)
        return true;
    default:
      return false;
  }
}


double S52StateObject::ZERO_CONTOUR () const
{
  return 0;
}

void S52StateObject::setScale (int scale)
{
  m_scale = scale;
}

void S52StateObject::setDisplayMask(int displayMask)
{
  m_displayMask = displayMask;
}

void S52StateObject::setSimpleSymbols(bool simpleSymbols)
{
  m_simpleSymbols = simpleSymbols; 
}

void S52StateObject::setSimpleAreas(bool simpleAreas)
{
  m_simpleAreas = simpleAreas; 
}

void S52StateObject::setContourLabels(bool contourLabels)
{
  m_contourLabels = contourLabels;
}

void S52StateObject::setCentreSymbols(bool centreSymbols)
{
  m_centreSymbols = centreSymbols;
}

void S52StateObject::setShowText(bool showText)
{
  m_showText = showText;
}

void S52StateObject::setShowTextHalo(bool showHalo)
{
  m_showTextHalo = showHalo;
}

void S52StateObject::setShowSymbols(bool showSymbol)
{
  m_showSymbol = showSymbol;
}

void S52StateObject::setDeepContours(double deepContours)
{
  m_deepContours = deepContours;
}

void S52StateObject::setShallowContours(double shallowContours)
{
  m_shallowContours = shallowContours;
}

void S52StateObject::setSafetyDepth(double safetyDepth)
{
  m_safetyDepth = safetyDepth;
}

void S52StateObject::setSafetyContours(double safetyContours)
{
  m_safetyContours = safetyContours;
}

void S52StateObject::setOwnShip(TSLCoord *xy)
{
   m_ownShip = *xy;
}

void S52StateObject::writeConfiguration(const char *iniFile)
{
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SCALE", m_scale); 

  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SIMPLE_AREAS", (int)m_simpleAreas); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SIMPLE_SYMBOLS", (int)m_simpleSymbols); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "CONTOUR_LABELS", (int)m_contourLabels); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "CENTRE_SYMBOLS", (int)m_centreSymbols); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SHOW_TEXT", (int)m_showText); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SHOW_SYMBOL", (int)m_showSymbol); 

  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "DISPLAY_MASK", m_displayMask);

  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "DEEP_CONTOURS", m_deepContours); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SHALLOW_CONTOURS", m_shallowContours); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SAFETY_DEPTH", m_safetyDepth); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "SAFETY_CONTOURS", m_safetyContours); 

  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "COLOURS", m_paletteIndex); 
  TSLProfileHelper::writeProfile(iniFile, "S52_STATE", "DISPLAY_NO_DATA_LAYER", (int)m_displayNoDataLayer); 
}

void S52StateObject::readConfiguration(const char *iniFile)
{
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SCALE", &m_scale, m_scale); 

  int val = 0;
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SIMPLE_AREAS", &val, (int)m_simpleAreas); m_simpleAreas = (val ? true : false);
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SIMPLE_SYMBOLS", &val,(int)m_simpleSymbols); m_simpleSymbols = (val ? true : false);
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "CONTOUR_LABELS", &val,(int)m_contourLabels); m_contourLabels = (val ? true : false);
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "CENTRE_SYMBOLS", &val,(int)m_centreSymbols); m_centreSymbols = (val ? true : false);
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SHOW_TEXT", &val,(int)m_showText); m_showText = (val ? true : false);
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SHOW_SYMBOL", &val,(int)m_showSymbol); m_showSymbol = (val ? true : false);

  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "DISPLAY_MASK", &m_displayMask, m_displayMask);

  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "DEEP_CONTOURS", &m_deepContours, m_deepContours); 
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SHALLOW_CONTOURS", &m_shallowContours, m_shallowContours); 
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SAFETY_DEPTH", &m_safetyDepth, m_safetyDepth); 
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "SAFETY_CONTOURS", &m_safetyContours, m_safetyContours); 

  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "COLOURS", &m_paletteIndex, m_paletteIndex); 
  TSLProfileHelper::lookupProfile(iniFile, "S52_STATE", "DISPLAY_NO_DATA_LAYER", &val, (int)m_displayNoDataLayer);  m_displayNoDataLayer = (val ? true : false);
}