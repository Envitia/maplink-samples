#include "pluginstdafx.h"
#include "MyDynamicRenderer.h"
#include <vector>

// Constructor - the default one.
MyDynamicRenderer::MyDynamicRenderer(void)
: m_queriedColours( false ), m_red( 0 ), 
  m_green( 0 ), m_blue( 0 ), m_white( 0 )
{
  OutputDebugString( _T("Default constructor for my dynamic renderer\n") ) ;
}

// Copy Constructor - used by the clone method.
MyDynamicRenderer::MyDynamicRenderer( const MyDynamicRenderer & other )
: m_queriedColours( other.m_queriedColours ), m_red( other.m_red ), 
  m_green( other.m_green ), m_blue( other.m_blue ), m_white( other.m_white )
{
  OutputDebugString( _T("Copy constructor for my dynamic renderer\n") ) ;
}

// Destructor - ensure virtual so that it can be triggered
// when the renderer is destroyed through the base class pointer.
MyDynamicRenderer::~MyDynamicRenderer(void)
{
  OutputDebugString( _T("Destructor for my dynamic renderer\n") ) ;
}

// clone method.
// =============
// This will be called by the Dynamic Renderer Factory if this
// renderer is registered with it and the application calls 'create'
TSLClientCustomDynamicRenderer * MyDynamicRenderer::clone () const
{
  OutputDebugString( _T("Clone for my dynamic renderer\n") ) ;
  return new MyDynamicRenderer( *this ) ;
}

// setConfig method.
// =================
// This may be called by an application in order to allow for persistence
// of configuration values.  The renderer should read from the provided string
// and populate any relevant configuration.  Care should be taken to include
// some level of versioning information so that it can be updated if necessary
// Future releases of MapLink may use this functionality
bool MyDynamicRenderer::setConfig (const TSLSimpleString &src)
{
  OutputDebugString( _T("setConfig \n") ) ;
  return true ;
}

// getConfig method.
// =================
// This may be called by an application in order to allow for persistence
// of configuration values.  The renderer should write any relevant configuration
// to the the provided string.  Care should be taken to include
// some level of versioning information so that it can be updated if necessary
// Future releases of MapLink may use this functionality
bool MyDynamicRenderer::getConfig (TSLSimpleString &dst) const
{
  OutputDebugString( _T("getConfig in my dynamic renderer\n") ) ;
  return true ;
}

// render method.
// ==============
// This is called by MapLink when a matching entity is required to be drawn.
// By default, it will not be called for Entity Sets.  This is configurable
// when the TSLDynamicRendererCustom object is created.
//
// The renderer can perform various actions:
//
//   Decide that the entity should not be drawn, for example the scale factor
//   of the current display may be inappropriate.  In this case, the renderer
//   should return TSLDynamicRendererActionIgnore.  If this is returned for 
//   an Entity Set, then the contents of the Entity Set will not be drawn.
//
//   Draw the entity itself, typically by querying geometric properties and
//   perhaps dataset attributes and then using the renderingInterface
//   directly.  In this case, the renderer should make the relevant rendering
//   calls and then return TSLDynamicRendererActionNone
//   
//   Decide that normal MapLink renderering should take place.  In this case, 
//   the renderer should return TSLDynamicRendererActionDefault.
TSLDynamicRendererAction MyDynamicRenderer::render (const TSLEntity *entity, TSLRenderingInterface *renderingInterface, const TSLDataHandler *dataHandler, int pass, bool *morePassesRequired, TSLFeatureID *featureID)
{
  // If we haven't queried the colours before, do so now
  if ( !m_queriedColours )
  {
    // Note that these could actually change if a new palette gets created, but 
    // just handle the simple case for now.
    m_queriedColours = true ;
    m_red   = renderingInterface->getIDofNearestColour( 255, 0, 0 ) ;
    m_green = renderingInterface->getIDofNearestColour( 0, 255, 0 ) ;
    m_blue  = renderingInterface->getIDofNearestColour( 0, 0, 255 ) ;
    m_black = renderingInterface->getIDofNearestColour( 0, 0, 0 ) ;
    m_white = renderingInterface->getIDofNearestColour( 255, 255, 255 ) ;
  }

  // Now render the primitive if we know how to
  if ( TSLPolyline::isPolyline( entity ) )
  {
    renderPolyline( TSLPolyline::isPolyline( entity ), renderingInterface ) ;
  }
  else if ( TSLPolygon::isPolygon( entity ) )
  {
    renderPolygon( TSLPolygon::isPolygon( entity ), renderingInterface ) ;
  }
  else if ( TSLArc::isArc( entity ) )
  {
    renderArc( TSLArc::isArc( entity ), renderingInterface ) ;
  }
  else if ( TSLEllipse::isEllipse( entity ) )
  {
    renderEllipse( TSLEllipse::isEllipse( entity ), renderingInterface ) ;
  }
  else if ( TSLText::isText( entity ) )
  {
    renderText( TSLText::isText( entity ), renderingInterface ) ;
  }
  else if ( TSLSymbol::isSymbol( entity ) )
  {
    renderSymbol( TSLSymbol::isSymbol( entity ), renderingInterface ) ;
  }
  else if ( TSLMultiPolygon::isMultiPolygon( entity ) )
  {
    renderMultiPolygon( TSLMultiPolygon::isMultiPolygon( entity ), renderingInterface ) ;
  }
  else if ( TSLMultiPolyline::isMultiPolyline( entity ) )
  {
    renderMultiPolyline( TSLMultiPolyline::isMultiPolyline( entity ), renderingInterface ) ;
  }
  else
  { 
    // Don't know how to draw this primitive, so do the default
    return TSLDynamicRendererActionDefault ;
  }

  // I've drawn it, so don't do the default
  return TSLDynamicRendererActionNone ;
}

bool MyDynamicRenderer::delayedRender( TSLRenderingInterface *renderingInterface, const TSLDataHandler *dataHandler)
{
  OutputDebugString( _T("delayedRender in my dynamic renderer\n") ) ;
  return false ; // return false to say we've not drawn anything.
}

// type method.
// ============
// This method should return a renderer specific indentifier that can be used to
// let the application know what type this is.
// This example will merely return 'typeValue' as defined above.
int MyDynamicRenderer::type ( ) const
{
  return typeValue ;
}

// Shows how to render a polyline
void MyDynamicRenderer::renderPolyline(const TSLPolyline * entity, TSLRenderingInterface * renderingInterface)
{
  // Draw all polylines as a single pixel thick red line
  if ( renderingInterface->setupEdgeAttributes( 1, m_red, 1, TSLDimensionUnitsPixels ) )
    renderingInterface->drawPolyline( &entity->points(), entity->closed() ) ;
}

// Shows how to render a polygon
void MyDynamicRenderer::renderPolygon(const TSLPolygon * entity, TSLRenderingInterface * renderingInterface)
{
  // Draw all polygons as a one pixel blue line, with blue translucent fill
  if (   renderingInterface->setupEdgeAttributes( 1, m_blue, 1, TSLDimensionUnitsPixels )
      && renderingInterface->setupAreaAttributes( 500, m_blue ) )
  {
    const TSLCoordSet * outer = &entity->outer() ;
    std::vector< const TSLCoordSet * > inners ;

    for ( int i = 0 ; i < entity->numInners() ; ++i )
      inners.push_back( entity->inner( i ) ) ;

    renderingInterface->drawPolygon( outer, inners.size(), inners.size() ? &inners[0] : 0 ) ;
  }
}

// Shows how to render an ellipse
void MyDynamicRenderer::renderEllipse(const TSLEllipse * entity, TSLRenderingInterface * renderingInterface)
{
  // Draw all ellipses as a two pixel red line, with red cross-hatched fill
  if (   renderingInterface->setupEdgeAttributes( 1, m_red, 2, TSLDimensionUnitsPixels )
      && renderingInterface->setupAreaAttributes( 4, m_red ) )
  {
    renderingInterface->drawEllipse( entity->centre(), entity->xRadius(), entity->yRadius(), entity->rotation() ) ;
  }
}

// Shows how to render an arc
void MyDynamicRenderer::renderArc(const TSLArc * entity, TSLRenderingInterface * renderingInterface)
{
  // Draw all arcs as a three pixel thick red line
  // Note that this may be slower than normal rendering, due to some internal optimisations within MapLink
  if ( renderingInterface->setupEdgeAttributes( 1, m_red, 3, TSLDimensionUnitsPixels ) )
    renderingInterface->drawArc( entity->centre(), entity->xRadius(), entity->yRadius(), entity->startAngle(), entity->endAngle(), entity->rotation() ) ;
}

// Shows how to render a text object
void MyDynamicRenderer::renderText(const TSLText * entity, TSLRenderingInterface * renderingInterface)
{
  // Draw all text as green with a halo, 15 pixels high
  if ( renderingInterface->setupTextAttributes( 1, m_green, 15.0, TSLDimensionUnitsPixels,
    1, 2000, 0.0, 0.0, TSLDimensionUnitsUndefined, entity->rotation(), 
    entity->horizontalAlignment(), entity->verticalAlignment(), TSLTextBackgroundModeHalo, 0, m_black ) )
  {
    renderingInterface->drawText( entity->position(), entity->value() ) ;
  }
}

// Shows how to render a symbol
void MyDynamicRenderer::renderSymbol(const TSLSymbol * entity, TSLRenderingInterface * renderingInterface)
{
  // Draw symbols as a big red star, ignoring rotation.
  if ( renderingInterface->setupSymbolAttributes( 14, m_red, 30.0, TSLDimensionUnitsPixels ) )
  {
    renderingInterface->drawSymbol( entity->position() ) ;
  }
}

// Shows how to render a multi-polygon
void MyDynamicRenderer::renderMultiPolygon( const TSLMultiPolygon * entity, TSLRenderingInterface * renderingInterface )
{
  // Draw all polygons as a one pixel blue line, with blue translucent fill
  if (   renderingInterface->setupEdgeAttributes( 1, m_blue, 1, TSLDimensionUnitsPixels )
      && renderingInterface->setupAreaAttributes( 500, m_blue ) )
  {
    // For performance, just pass the envelope of the entire entity
    TSLEnvelope extent = entity->envelope() ;
    int numParts = entity->numberOfParts() ;

    for ( int i = 0 ; i < numParts ; ++i )
    {
      const TSLMultiPolygonPart * part = entity->partAt( i ) ;

      int numOuterCoords ;
      const TSLCoord * outer = part->outer( numOuterCoords ) ;

      std::vector< const TSLCoord * > inners ;
      std::vector< int > innerCounts ;

      int numInners = part->numInners() ;

      for ( int j = 0 ; j < numInners ; ++j )
      {
	int numInnerCoords ;
	const TSLCoord * inner = part->inner( j, numInnerCoords ) ;
	inners.push_back( inner ) ;
	innerCounts.push_back( numInnerCoords ) ;
      }

      if ( numInners ) 
	renderingInterface->drawPolygon( outer, numOuterCoords, numInners, &inners[0], &innerCounts[0], &extent ) ;
      else 
	renderingInterface->drawPolygon( outer, numOuterCoords, 0, 0, 0, &extent ) ;
    }
  }
}

// Shows how to render a multi-polyline
void MyDynamicRenderer::renderMultiPolyline( const TSLMultiPolyline * entity, TSLRenderingInterface * renderingInterface )
{
  // Draw all polylines as a single pixel thick red line
  if (  renderingInterface->setupEdgeAttributes( 1, m_red, 1, TSLDimensionUnitsPixels )  )
  {
    // For performance, just pass the envelope of the entire entity
    TSLEnvelope extent = entity->envelope() ;
    int numParts = entity->numberOfParts() ;

    for ( int i = 0 ; i < numParts ; ++i )
    {
      const TSLMultiPolylinePart * part = entity->partAt( i ) ;

      int numCoords ;
      const TSLCoord * coords = part->coords( numCoords ) ;

      renderingInterface->drawPolyline( coords, numCoords, false, &extent ) ;  // These are never closed
    }
  }
}
