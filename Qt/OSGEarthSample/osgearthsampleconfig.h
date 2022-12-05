#ifndef OSGEARTHSAMPLECONFIG_H
#define OSGEARTHSAMPLECONFIG_H
/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#define SUN_YEAR                           2014
#define SUN_MONTH                          7
#define SUN_DAY                            6

#define TERRAIN_VERTICALSCALE              5.0

// This is the resolution of the tile in pixels
#define TERRAIN_TILESIZE                   1024

// This is the number of levels that OSGEarth goes down requesting smaller tile extents
// So the higher the number the smaller the tile extents but the more often it will request tiles and terrain data
// So this is a tradeoff between resolution and performance
#define TERRAIN_LAYER_MAXLEVEL             16

#define TRACKS_INITIALNUMBER               10

#define IMAGERY_TILESIZE                   256
#define IMAGERY_TILESIZE_STR              "256"
#define MAPLINKIMAGERY_MINIMUMZOOMFACTOR   0.4

#endif
