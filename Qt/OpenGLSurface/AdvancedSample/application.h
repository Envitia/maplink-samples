/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QGLWidget>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Include MapLink Pro Headers...
//
// Define some required Macros and include X11 and Win32 headers as
// necessary.
//
// Define: TTLDLL & WIN32 within the project make settings.
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif
# include <windows.h>
#endif

#include <qtreewidget.h>
#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkOpenGLSurface.h"

#include "ui/attributetreewidget/attributetreewidget.h"
#include "ui/layertreeview/treemodel.h"
#include "ui/layertreeview/treeitem.h"

class FramerateLayer;
class TSLInteractionModeManagerGeneric;
class AddWaypointInteractionMode;
class LayerManager;
class TrackSelectionMode;

#ifdef HAVE_DIRECT_IMPORT_SDK
class DirectImportDataSetTreeItem;
class DirectImportScaleBandTreeItem;
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Main Application class.
//
// Contains the calls to MapLink and the simple application
// code.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Application : public TSLInteractionModeRequest
{

public:
  Application( QWidget *parent );
  virtual ~Application();

  // Creates the MapLink drawing surface and associated map data layer
  void create();

  // Called when the size of the window has changed
  void resize( int width, int height );

  // Called to redraw the map
  void redraw();

  // Mouse and Keyboard events - if the method returns true it indicates that the widget needs to redraw
  bool mouseMoveEvent( unsigned int button, bool shiftPressed, bool controlPressed, int x, int y );
  bool onLButtonDown( bool shiftPressed, bool controlPressed, int x, int y, std::string entity );
  bool onMButtonDown( bool shiftPressed, bool controlPressed, int x, int y );
  bool onRButtonDown( bool shiftPressed, bool controlPressed, int x, int y );
  bool onLButtonUp( bool shiftPressed, bool controlPressed, int x, int y );
  bool onMButtonUp( bool shiftPressed, bool controlPressed, int x, int y );
  bool onRButtonUp( bool shiftPressed, bool controlPressed, int x, int y );
  bool onKeyPress( bool shiftPressed, bool controlPressed, int keySym );
  bool onMouseWheel( bool shiftPressed, bool controlPressed, short zDelta, int x, int y );

  // Interaction Mode control - if the method returns true it indicates that the widget needs to redraw
  bool zoomIn();
  bool zoomOut();
  void resetView();
  void activatePanMode();
  void activateZoomMode();
  void activateGrabMode();
  void activateWaypointMode();
  void activateTrackSelectMode();

  // Interaction Mode request implementations.
  virtual void resetMode( TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU );
  virtual void viewChanged( TSLDrawingSurface* drawingSurface );

  TreeModel* getTreeModel();
  void setAttributeTree( AttributeTreeWidget* atw );

  AddWaypointInteractionMode* getWaypointMode() const;

  void moveLayerToIndex( const char* moveLayer, const char* targetLayer, int row );
#ifdef HAVE_DIRECT_IMPORT_SDK
  void moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo );
  void removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
#endif
  void editTreeModel( const QModelIndex& index, long value );
  void removeTreeLayer( const std::string& name, const std::string& treeAttribute );
  void zoomToLayerExtent( const std::string& layerName );
#ifdef HAVE_DIRECT_IMPORT_SDK
  void zoomToLayerDataSetExtent( DirectImportDataSetTreeItem* );
  void zoomToLayerScaleBandExtent( DirectImportScaleBandTreeItem* );
#endif
  void zoomToExtent( TSLDataLayer*, TSLEnvelope );
  void selectedAttributeData( const std::string& layerName, const std::string& layerType );
  void reloadAttributeTree();
  bool checkDuplicateLayer( const std::string& layerName ) const;
  LayerManager* layerManager();

  bool createPolyline( double lat, double lon );
  bool createPolygon( double lat, double lon );
  bool createText( double lat, double lon );
  bool createVectorSymbol( double lat, double lon );
  bool createRasterSymbol( double lat, double lon );
  bool createArc( double lat, double lon );
  bool createEllipse( double lat, double lon );

  bool createGeoPolyline( double lat, double lon );
  bool createGeoPolygon( double lat, double lon );
  bool createGeoText( double lat, double lon );
  bool createGeoVectorSymbol( double lat, double lon );
  bool createGeoRasterSymbol( double lat, double lon );
  bool createGeoArc( double lat, double lon );
  bool createGeoEllipse( double lat, double lon );

  void saveToTMF( const char* filename );
  void loadFromTMF( const char* filename );

  // Information to enable the drawing surface to draw.
#ifdef _MSC_VER
  void drawingInfo( WId window );
  TSLWGLSurface* getDrawingSurface();
#else
  void drawingInfo( Display *display, Screen *screen );
  TSLGLXSurface* getDrawingSurface();
#endif

private:

  // Drawing surface setup.
#ifdef _MSC_VER
  // The MapLink OpenGL drawing surface
  TSLWGLSurface* m_drawingSurface;

  // The window to draw to
  WId m_window;
#else
  // The MapLink OpenGL drawing surface
  TSLGLXSurface *m_drawingSurface;

  // The display connection and screen to use
  Display *m_display;
  Screen *m_screen;
#endif

  // Interaction manager - this handles panning and zooming around the map
  // based on the active interaction mode
  TSLInteractionModeManagerGeneric* m_modeManager;

  // The size of the window the drawing surface is attached to
  TSLDeviceUnits m_widgetWidth;
  TSLDeviceUnits m_widgetHeight;

  // Rotation of the drawing surface in radians
  double m_surfaceRotation;

  QWidget* m_parentWidget;

  AttributeTreeWidget* m_attributeTree;
  TreeModel* m_treeModel;
  LayerManager* m_layerManager;

  std::string m_selectedAttributeLayer;
  std::string m_selectedAttributeType;

  AddWaypointInteractionMode* m_waypointMode;

  // Retrieve the data layer, entity set of that data layer and map coordinate system 
  // (not the runtime coordinate system) from the entity layer of the application.
  bool getDataAndMapLayers(TSLStandardDataLayer*& dataLayer, TSLEntitySet*& entitySet, const TSLCoordinateSystem*& coordSys);
};

#endif
