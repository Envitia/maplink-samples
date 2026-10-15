#include "StdAfx.h"

#include "TerrainViewerOptions.h"


void TerrainViewerOptions::load()
{
  // Load the options from the config file.
  TSLSimpleString section;
  TSLProfileHelper::getDefaultSection (section);

  TSLProfileHelper::setDefaultSection( "options" ) ;

  double d ;
  int i;

  // Terrain profile view options
  TSLProfileHelper::lookupProfile( "manual_scale",  &d, 5000.0 ) ;
  m_manual_scale = d ;
  TSLProfileHelper::lookupProfile( "auto_scale",  &i, 0 ) ;
  m_terrainAutoscale = i ? true : false ;
  TSLProfileHelper::lookupProfile( "terrain_scale",  &d, 1.0 ) ;
  m_terr_scale =  d ;
  TSLProfileHelper::lookupProfile( "bathymetry_scale",  &d, 1.0 ) ;
  m_bathy_scale =  d ;
  TSLProfileHelper::lookupProfile( "bathymetry",  &i, 1 ) ;
  m_bathy = i  ? true : false ;


  // Single LOS options
  TSLProfileHelper::lookupProfile( "start_height",  &d, 1.0 ) ;
  m_losStartHeight = d ;
  TSLProfileHelper::lookupProfile( "end_height",  &d, 0.0 ) ;
  m_losEndHeight = d ;
  TSLProfileHelper::lookupProfile( "los_curve_of_earth",  &i, 0 ) ;
  m_losEarthCurvature = i ? true : false ;
  TSLProfileHelper::lookupProfile( "los_sea_level_reference",  &i, 0 ) ;
  m_losFromSeaLevel = i ? true : false ;

  // Area LOS options
  TSLProfileHelper::lookupProfile( "los_area_maximum_radius", &d, 35.0 );
  m_viewShedMaximumRadius = d;
  TSLProfileHelper::lookupProfile( "los_area_colour_visible", &i, 0x0000FF00 ); //ARGB
  m_viewShedColourVisible = i;
  TSLProfileHelper::lookupProfile( "los_area_colour_not_visible", &i, 0x00FF0000 ); //ARGB
  m_viewShedColourNotVisible = i;
  TSLProfileHelper::lookupProfile( "los_area_colour_no_data", &i, 0x000000FF ); //ARGB
  m_viewShedColourNoData = i;

  TSLProfileHelper::lookupProfile( "los_area_colour_no_data_transparent", &i, 0 );
  m_viewShedColourNoDataTransparent = i ? true : false ;
  TSLProfileHelper::lookupProfile( "los_area_colour_visible_transparent", &i, 0 );
  m_viewShedColourVisibleTransparent = i ? true : false ;
  TSLProfileHelper::lookupProfile( "los_area_colour_not_visible_transparent", &i, 0 );
  m_viewShedColourNotVisibleTransparent = i ? true : false ;

  TSLProfileHelper::lookupProfile( "los_area_start_height", &d, 10.0 );
  m_viewShedStartHeight = d;
  TSLProfileHelper::lookupProfile( "los_area_end_height", &d, 0.0 );
  m_viewShedEndHeight = d;
  TSLProfileHelper::lookupProfile( "los_area_start_height_relative_to_ground", &i, 1 );
  m_viewShedStartRelativeToGround = i ? true : false ;\
  TSLProfileHelper::lookupProfile( "los_area_end_height_relative_to_ground", &i, 1 );
  m_viewShedEndRelativeToGround = i ? true : false ;
  TSLProfileHelper::lookupProfile( "los_area_earth_curvature_box_index", &i, 0 );
  m_viewShedCurvatureBoxIndex = i;

  // Reset to default. 
  TSLProfileHelper::setDefaultSection( section.c_str() ) ;
}


void TerrainViewerOptions::save()
{
  // Write the new values to the ini. 
  TSLSimpleString ini;
  TSLProfileHelper::getDefaultFilename (ini);
 
  std::string section = "options" ;

  // Terrain profile view options
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "manual_scale", m_manual_scale);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "auto_scale", m_terrainAutoscale);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "terrain_scale", m_terr_scale);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "bathymetry_scale", m_bathy_scale);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "bathymetry", m_bathy);

  // Single LOS options
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "start_height", m_losStartHeight);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "end_height", m_losEndHeight);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_curve_of_earth", (int)m_losEarthCurvature);
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_sea_level_reference", (int)m_losFromSeaLevel);

  // Area LOS options
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_maximum_radius", m_viewShedMaximumRadius );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_colour_visible", (int)m_viewShedColourVisible );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_colour_not_visible", (int)m_viewShedColourNotVisible );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_colour_no_data", (int)m_viewShedColourNoData );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_start_height", m_viewShedStartHeight );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_end_height", m_viewShedEndHeight );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_start_height_relative_to_ground", m_viewShedStartRelativeToGround );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_end_height_relative_to_ground", m_viewShedEndRelativeToGround );
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_earth_curvature_box_index", m_viewShedCurvatureBoxIndex );

  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_colour_no_data_transparent", (int)m_viewShedColourNoDataTransparent ) ;
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_colour_visible_transparent", (int)m_viewShedColourVisibleTransparent ) ;
  TSLProfileHelper::writeProfile( ini.c_str(), section.c_str(), "los_area_colour_not_visible_transparent", (int)m_viewShedColourNotVisibleTransparent ) ;
}