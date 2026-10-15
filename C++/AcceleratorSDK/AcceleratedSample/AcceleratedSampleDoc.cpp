// AcceleratedSampleDoc.cpp : implementation of the AcceleratedSampleDoc class
//

#include "stdafx.h"
#include "AcceleratedSample.h"
#include <direct.h>
#include "AcceleratedSampleDoc.h"
#include "AcceleratedSampleCustomDataLayer.h"
#include "AcceleratedSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleDoc

const char * AcceleratedSampleDoc::m_mapLayerName = "map" ;
const char * AcceleratedSampleDoc::m_customLayerName = "custom" ;
const char * AcceleratedSampleDoc::m_stdLayerName1 = "std1" ;
const char * AcceleratedSampleDoc::m_stdLayerName2 = "std2" ;
const char * AcceleratedSampleDoc::m_rasterLayerName = "raster" ;

IMPLEMENT_DYNCREATE(AcceleratedSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(AcceleratedSampleDoc, CDocument)
//{{AFX_MSG_MAP(AcceleratedSampleDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
  ON_COMMAND(ID_TOOLS_UN, OnToolsUnrotateRectangle)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_UN, OnUpdateToolsAction)
  ON_COMMAND(ID_FILE_LOADRASTER, &AcceleratedSampleDoc::OnFileLoadraster)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleDoc construction/destruction

AcceleratedSampleDoc::AcceleratedSampleDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
  m_customDataLayer = 0;
  m_customClient = 0;
  m_stdDataLayer1 = 0;
  m_stdDataLayer2 = 0;
  m_rasterDataLayer = 0;

  m_rasterBuffering = true;
  m_backLayerVisibility = true;
  m_frontLayerVisibility = true;
}

AcceleratedSampleDoc::~AcceleratedSampleDoc()
{
  if (m_mapDataLayer)
  {
    m_mapDataLayer->destroy();
  }
  if (m_customDataLayer)
  {
    m_customDataLayer->destroy();
  }
  if (m_customClient)
  {
    delete m_customClient;
  }
  if (m_stdDataLayer1)
  {
    m_stdDataLayer1->destroy();
  }
  if (m_stdDataLayer2)
  {
    m_stdDataLayer2->destroy();
  }
  if (m_rasterDataLayer)
  {
    m_rasterDataLayer->destroy();
  }
  m_mapDataLayer = 0;
  m_customDataLayer = 0;
  m_customClient = 0;
  m_stdDataLayer1 = 0;
  m_stdDataLayer2 = 0;
  m_rasterDataLayer = 0;
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleDoc diagnostics

#ifdef _DEBUG
void AcceleratedSampleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void AcceleratedSampleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// AcceleratedSampleDoc commands

BOOL AcceleratedSampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  // NOTE:
  // If you add a normal TSLCustomDataLayer or TSLObjectDataLayer to an Accelerated Surface
  // you will force the map texture tiles to be regenerated every frame.
  //
  // This will have a significant impact on performance.
  //
  // In addition 3D layers can not be added to the accelerated surface.
  //
  // Text and Symbols with rotation flags set will not be updated as the underlying surface
  // is never rotated.
  //
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  AcceleratedSampleView *view = AcceleratedSampleView::GetView();
  if (view)
  {
    // Due to an issue in MapLink 5.3 you need to remove a layer before
    // you destroy it.
     view->detachLayers();
    //view->stopBackgroundThread();
  }

 
  // clean up.
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy();
  }
  if (m_customDataLayer)
  {
    m_customDataLayer->destroy();
  }
  if (m_customClient)
  {
    delete m_customClient;
  }
  if (m_stdDataLayer1)
  {
    m_stdDataLayer1->destroy();
  }
  if (m_stdDataLayer2)
  {
    m_stdDataLayer2->destroy();
  }
  if (m_rasterDataLayer)
  {
    m_rasterDataLayer->destroy();
  }

  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();

  // Clear the error stack, load the map then check for errors.
  TSLThreadedErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Create a custom datalayer and client.
  m_customDataLayer = new TSLAcceleratedCustomDataLayer;
  m_customClient = new AcceleratedSampleCustomDataLayer;
  m_customDataLayer->setClientCustomDataLayer((TSLAcceleratedClientCustomDataLayer*)m_customClient);

  // create a standard data layer
  m_stdDataLayer1 = new TSLStandardDataLayer();
  m_stdDataLayer2 = new TSLStandardDataLayer();

  // create a raster datalayer
  m_rasterDataLayer = new TSLRasterDataLayer();

  // Display any errors that have occurred
  TSLSimpleString msg ( "" );
  bool anyErrors = TSLThreadedErrorStack::errorString( msg, "Cannot load map\n" ) ;
  if ( anyErrors )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
    m_customDataLayer->destroy();
    delete m_customClient;
    m_customClient = 0;
    m_customDataLayer = 0;
    m_stdDataLayer1->destroy() ;
    m_stdDataLayer1 = 0 ;
    m_stdDataLayer2->destroy() ;
    m_stdDataLayer2 = 0 ;
    m_rasterDataLayer->destroy() ;
    m_rasterDataLayer = 0 ;
  }

  return TRUE;
}

bool AcceleratedSampleDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  || !m_customDataLayer || !m_stdDataLayer1 || !m_stdDataLayer2 || !m_rasterDataLayer)
    return false;

  // Add a standard datalayer behind the maplayer
  surface->addDataLayer( m_stdDataLayer1, m_stdLayerName1 ) ;
  surface->setDataLayerProps( m_stdLayerName1, TSLPropertyVisible, m_backLayerVisibility);

  // Add the map to the specified drawing surface.
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;

  // Tell the surface to render this data layer into the background tiles.
  //
  // Note:
  //  1. Layers will only be added to the background renderer if
  //     the TSLPropertyBuffered flag is set to true.
  //
  //  2. Dynamic Data Object layers and Custom Data layers not designed for this
  //     surface will defeat the object of the Accelerated Surface by forcing
  //     a redraw every frame. These layers will need to be reimplemented for
  //     this surface.
  //
  //     Please see the documentation for class TSLAcceleratedSurface for
  //     additional information.
  //
  surface->setDataLayerProps(m_mapLayerName, TSLPropertyBuffered, true);

  // Add the custom layer to the specified drawing surface.
  //
  // This type of layer will never be added to the background renderer.
  //
  surface->addDataLayer( m_customDataLayer, m_customLayerName ) ;
  

  // Add the Standard DataLayer to the surface.
  //
  // If we double buffer the layer it will added to the background renderer.
  //
  // If we do not double buffer layer it will be rendered in the foreground
  // surface.
  //
  // All double buffered layers (except custom accelerated) will be drawn at
  // one time.
  //
  // Single buffered layers are drawin in order they were added (or subsequently
  // re-ordered). If a single buffered layer appears between double buffered layers
  // the single buffered layer is drawn after the double buffered layers.
  //
  // Single buffered layers before double buffered layers are drawn before. Normal
  // drawing surfaces (X11 & NT) draw double buffered layers first.
  //
  // The buffered layers are drawn as texture tiles using DirectX.
  // 
  // Custom Accelerated layers can be drawn using either GDI (TMF Geometry) or OpenGL.
  //
  // Try to avoid too many switches between GDI and OpenGL as this will impact performance.
  //
  surface->addDataLayer( m_stdDataLayer2, m_stdLayerName2 ) ;
  surface->setDataLayerProps( m_stdLayerName2, TSLPropertyVisible, m_frontLayerVisibility);

  // to rotate a raster layer we need to put it into the background tiles
  surface->addDataLayer( m_rasterDataLayer, m_rasterLayerName ) ;
  surface->setDataLayerProps(m_rasterLayerName, TSLPropertyBuffered, m_rasterBuffering);

  /////////////////////////////////////////////////////////
  //
  createGeometry(m_stdDataLayer1);
  createGeometry(m_stdDataLayer2);

  return true ;
}



bool AcceleratedSampleDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer || !m_customDataLayer  || !m_stdDataLayer1 || !m_stdDataLayer2 || !m_rasterDataLayer)
    return false;

  // Remove the map from the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  // Remove the custom layer from the specified drawing surface.
  surface->removeDataLayer( m_customLayerName ) ;

  // Remove the standard datalayer from the specified drawing surface.
  surface->removeDataLayer( m_stdLayerName1 ) ;

  surface->removeDataLayer( m_stdLayerName2 ) ;

  // remove raster
  surface->removeDataLayer( m_rasterLayerName ) ;

  return true ;
}

void AcceleratedSampleDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  AcceleratedSampleView *view = AcceleratedSampleView::GetView();
  if (view)
  {
    // Due to an issue in MapLink 5.3 you need to remove a layer before
    // you destroy it.
    view->detachLayers();
    //view->stopBackgroundThread();
  }

  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if ( m_stdDataLayer1 )
  {
    m_stdDataLayer1->destroy() ;
    m_stdDataLayer1 = 0 ;
  }
  if ( m_stdDataLayer2 )
  {
    m_stdDataLayer2->destroy() ;
    m_stdDataLayer2 = 0 ;
  }
  if ( m_rasterDataLayer )
  {
    m_rasterDataLayer->destroy() ;
    m_rasterDataLayer = 0 ;
  }
  if ( m_customDataLayer )
  {
    m_customDataLayer->destroy() ;
    m_customDataLayer = 0 ;
  }
  if (m_customClient)
  {
    delete m_customClient;
    m_customClient = 0 ;
  }

  CDocument::DeleteContents();
}

void AcceleratedSampleDoc::getMapExtent(TSLEnvelope &envelope)
{
  TSLTMC x1, y1, x2, y2;
  m_mapDataLayer->getTMCExtent( &x1, &y1, &x2, &y2, -1 );
  envelope = TSLEnvelope(x1, y1, x2, y2);
}

void AcceleratedSampleDoc::OnToolsUnrotateRectangle()
{
  if (m_customClient)
  {
    m_customClient->unrotateRectangle();
  }
}

void AcceleratedSampleDoc::OnUpdateToolsAction(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_customClient ? true : false ) ;
}


void AcceleratedSampleDoc::createGeometry(TSLStandardDataLayer *layer)
{
  TSLTMC x1, y1, x2, y2;
  m_mapDataLayer->getTMCExtent( &x1, &y1, &x2, &y2 );
  TSLEnvelope mapExtent( x1, y1, x2, y2); // required by Green Hills V3.5 compiler

  TSLEntitySet* parent = layer->entitySet();

  // We are going to create 10 different primitives in a strip across
  // the middle of the map, so work out the extents now....
  TSLEnvelope   extents[10];

  // The top and bottom are 10% of the map height, centred around the middle of the map
  long  top = mapExtent.centre().y() + (long)( (double)mapExtent.height() * 0.05 ); 
  long  bottom = mapExtent.centre().y() - (long)( (double)mapExtent.height() * 0.05 ); 

  long  x = mapExtent.xMin();
  long  inc = mapExtent.width() / 11;

  for ( int i = 0; i < 10; i++ )
  {
    extents[i].corners( x, bottom, x + inc, top );
    x += inc;
  }
#define ARCID 1
#define BPID  2
#define ELLID 3
#define POLYID 4
#define RECTID 5
#define SCALESYMID 6
#define NONSCALESYMID 7
#define SCALETEXTID 8
#define NONSCALETEXTID 9
#define POLYLINEID 10

  // Now start creating the objects and setting thier drawing attributes
  // ALSO see GeometryCreation sample for NT.

  // Arc

  TSLArc * arc;
  arc = parent->createArc( ARCID, 0.1 * M_PI/180.0 , 359.1 * M_PI/180.0, extents[0].centre().x(), extents[0].centre().y(), extents[0].width() );

  arc->setRendering( TSLRenderingAttributeEdgeColour, 1 );
  arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
  arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

  arc = parent->createArc( ARCID, 359.8 * M_PI/180.0, 359.999 * M_PI/180.0, extents[0].centre().x()+300, extents[0].centre().y(), extents[0].width() );

  arc->setRendering( TSLRenderingAttributeEdgeColour, 1 );
  arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
  arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

  double startAngle = 10;
  double endAngle = 55;
  for (int i = 0; i < 10; ++i)
  {
    arc = parent->createArc( ARCID, startAngle * M_PI/180.0, endAngle * M_PI/180.0, 
               extents[0].centre().x(),
               (extents[0].centre().y() / 2) + (extents[0].width() / 4) * i, 
               extents[0].width(), extents[0].width() );

    arc->setRendering( TSLRenderingAttributeEdgeColour, 1);
    arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
    arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

    startAngle += 10;
    endAngle += 15;
  }


  startAngle = 0;
  endAngle = 45;
  for (int i = 0; i < 10; ++i)
  {
    arc = parent->createArc( ARCID, startAngle * M_PI/180.0, endAngle * M_PI/180.0, 
               extents[0].centre().x(),
               (extents[0].centre().y() / 2) - (extents[0].width() / 4) * i, 
               extents[0].width(), extents[0].width() / 2. );

    arc->setRendering( TSLRenderingAttributeEdgeColour, 1 * i);
    arc->setRendering( TSLRenderingAttributeEdgeStyle , 1 );
    arc->setRendering( TSLRenderingAttributeEdgeThickness, 2 );

    startAngle += 10;
    endAngle += 15;
  }

  // Bordered Polygon
  TSLCoordSet*  coords = new TSLCoordSet;
  coords->add( extents[1].bottomLeft().x(), extents[1].bottomLeft().y() );
  coords->add( extents[1].bottomLeft().x(), extents[1].topRight().y() );
  coords->add( extents[1].topRight().x(), extents[1].topRight().y() );
  coords->add( extents[1].topRight().x(), extents[1].bottomLeft().y() );
  TSLBorderedPolygon *borderedPolygon = parent->createBorderedPolygon( BPID, coords, extents[1].width() / 10, true );

  borderedPolygon->setRendering( TSLRenderingAttributeFillColour, 3 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeFillStyle,  35 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;
  borderedPolygon->setRendering( TSLRenderingAttributeBorderColour, 2 ) ;

  // Ellipse
  TSLEllipse *ellipse = parent->createEllipse( ELLID, extents[2].centre().x(), extents[2].centre().y(), extents[2].width() / 2, extents[2].width() / 4 );

  ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
  ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
  ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
  ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
  ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;


  for (int i = 0; i < 10; ++i)
  {
    ellipse = parent->createEllipse( ELLID, extents[2].centre().x(), 
                  extents[2].centre().y() + (extents[2].width() / 4) * i,
                  extents[2].width() / 8, extents[2].width() / 8, (10.0 * i) / M_PI * 180.0  );

    ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  }

  for (int i = 0; i < 10; ++i)
  {
    ellipse = parent->createEllipse( ELLID, extents[2].centre().x(), 
                  extents[2].centre().y() - (extents[2].width() / 4) * i,
                  extents[2].width() / 7, extents[2].width() / 8, (10.0 * i) / M_PI * 180.0  );

    ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  }

  for (int i = 0; i < 10; ++i)
  {
    ellipse = parent->createEllipse( ELLID, extents[2].centre().x() + (extents[2].width() / 4), 
                  extents[2].centre().y() + (extents[2].width() / 4) * i,
                  extents[2].width() / 8, extents[2].width() / 8 );

    ellipse->setRendering( TSLRenderingAttributeEdgeColour, 1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeStyle,  1 ) ;
    ellipse->setRendering( TSLRenderingAttributeEdgeThickness, 2 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillColour, 106 ) ;
    ellipse->setRendering( TSLRenderingAttributeFillStyle,  1 ) ;
  }


  // Polygon
  coords = new TSLCoordSet;
  coords->add( extents[3].bottomLeft().x(), extents[3].bottomLeft().y() );
  coords->add( extents[3].bottomLeft().x(), extents[3].topRight().y() );
  coords->add( extents[3].topRight().x(), extents[3].topRight().y() );
  coords->add( extents[3].topRight().x(), extents[3].bottomLeft().y() );
  TSLPolygon * poly = parent->createPolygon( POLYID, coords, true );
  poly->setRendering( TSLRenderingAttributeFillColour, 181 ) ;
  poly->setRendering( TSLRenderingAttributeFillStyle,  36 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  poly->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;

  // Rectangle
  TSLRectangle *rect = parent->createRectangle( RECTID, extents[4].bottomLeft(), extents[4].topRight() );
  rect->setRendering( TSLRenderingAttributeFillColour, 181 ) ;
  rect->setRendering( TSLRenderingAttributeFillStyle,  503 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeStyle, 1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeColour, 1 ) ;
  rect->setRendering( TSLRenderingAttributeExteriorEdgeThickness, 2 ) ;


  // Symbols
  TSLSymbol * symbol = parent->createSymbol( SCALESYMID, extents[5].centre().x(), extents[5].centre().y(), extents[5].height() / 5 );
  symbol->setRendering( TSLRenderingAttributeSymbolColour, 66 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle,  6001 ) ;

  symbol = parent->createSymbol( NONSCALESYMID, extents[6].centre().x(), extents[6].centre().y(), 20, 45.0 / M_PI * 180.0 );
  symbol->setRendering( TSLRenderingAttributeSymbolColour, 66 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle,  6001 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels ) ; 
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, 40 ) ; // height is in pixels
  symbol->setRendering( TSLRenderingAttributeSymbolRotatable,  TSLSymbolRotationEnabled ) ; 
  

  // Text
  // Note: The effect of scaling non-vector text is highly dependent on the 
  //       font's installed and the font specification in tslfonts.dat.
  //
  //       Only vector font's can be rotated on X11.
  //
  TSLText * text = parent->createText( SCALETEXTID, extents[7].centre().x(), extents[7].centre().y(), "Scale-Text", extents[7].height() / 5 );
  text->setRendering( TSLRenderingAttributeTextColour, 1 ) ;
  text->setRendering( TSLRenderingAttributeTextFont, 1 ) ;

  //
  // A non scalable text is defined as index 57 in the file tslfonts.dat.
  //
  // In this case it is user11x19, but could be a fully specified
  // font (for example -adobe-courier-bold-o-normal--12-120-75-75-m-70-hp-roman8)
  //
  // Another method for setting a non scaleable font is to
  // set Text units to Pixels and set a size factor as shown below.
  //
  // Note: Font selection in X is very dependent on the available
  //       fonts. If a particular font size is not available X
  //       tends to provide a substitute.
  //
  //       Additionally not all fonts are available on all systems.
  //       You may need to substitute fonts in tslfonts.dat for
  //       ones available to you or add additional fonts to your
  //       system.
  //
  text = parent->createText( NONSCALETEXTID, extents[8].centre().x(), extents[8].centre().y(), "Fixed-Text", 19 );
  text->setRendering( TSLRenderingAttributeTextColour, 1 ) ;
  text->setRendering( TSLRenderingAttributeTextFont, 2 ) ;
  text->setRendering( TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPixels ) ;
  text->setRendering( TSLRenderingAttributeTextSizeFactor, 19 ) ;

  // PolyLine

  for (int i = 0; i < 13; ++i)
  {
    coords = new TSLCoordSet;
    coords->add( extents[9].bottomLeft().x(), (extents[9].bottomLeft().y()) + (extents[9].width() / 7) * i );
    coords->add( extents[9].topRight().x(), extents[9].topRight().y() + (extents[9].width() / 7) * i);
    coords->add( extents[9].bottomLeft().x(), extents[9].topRight().y() + (extents[9].width() / 7) * i);

    TSLPolyline * polyline = parent->createPolyline( POLYLINEID, coords, true );
    polyline->setRendering( TSLRenderingAttributeEdgeStyle, i ) ;
    polyline->setRendering( TSLRenderingAttributeEdgeColour, i * 8 ) ;
    polyline->setRendering( TSLRenderingAttributeEdgeThickness, 1 ) ;
  }
}

void AcceleratedSampleDoc::OnFileLoadraster()
{
  if (m_rasterDataLayer)
  {
    static  std::string  lastDir;

    // Clear the error stack, load the map then check for errors.
    TSLThreadedErrorStack::clear() ;
    
    CFileDialog dlg( TRUE, NULL, 0, 
      OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, 
      _T("Raster Files (*.*)|*.*||") );

    TCHAR	files[TSL_MAX_WPATH];
    dlg.m_ofn.lpstrFile = files;
    dlg.m_ofn.nMaxFile = TSL_MAX_WPATH;
    TSLUTF8Decoder lastDirDecoded(lastDir);
    dlg.m_ofn.lpstrInitialDir = lastDir.empty() ? NULL : (LPCTSTR)lastDirDecoded;
    files[0] = '\0';
    if ( dlg.DoModal() != IDOK )
    {
      return;
    }

    POSITION	pos;
    pos = dlg.GetStartPosition();
    CString file = dlg.GetNextPathName( pos );

    TSLTMC x1, y1, x2, y2;
    m_mapDataLayer->getTMCExtent( &x1, &y1, &x2, &y2 );
    long middleX = x1 + labs(x2 - x1) / 2.0;
    long middleY = y1 + labs(y2 - y1) / 2.0;

    long factorX = labs(x2 - x1)/4.0;
    x1 = middleX - factorX;
    x2 = middleX + factorX;
    long factorY= labs(y2 - y1)/4.0;
    y1 = middleY - factorY;
    y2 = middleY + factorY;

    m_rasterDataLayer->addRaster(TSLUTF8Encoder(file), x1, y1, x2, y2);
    m_rasterDataLayer->notifyChanged(true);

    AcceleratedSampleView *view = AcceleratedSampleView::GetView();
    if (view)
    {
      view->redraw();
    }

    TSLSimpleString msg( "" );
    bool anyErrors = TSLThreadedErrorStack::errorString( msg, "Cannot load raster\n" ) ;
    if ( anyErrors )
    {
      // The map load failed, so delete the data layer and path list
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    }
  }
}


void AcceleratedSampleDoc::rasterBuffering(TSLDrawingSurface * surface)
{
  m_rasterBuffering = !m_rasterBuffering;
  surface->setDataLayerProps(m_rasterLayerName, TSLPropertyBuffered, m_rasterBuffering);
  
  m_rasterDataLayer->notifyChanged(true);
}


void AcceleratedSampleDoc::changeFrontLayerVisibility(TSLDrawingSurface * surface)
{
  m_frontLayerVisibility = !m_frontLayerVisibility;
  surface->setDataLayerProps(m_stdLayerName2, TSLPropertyVisible, m_frontLayerVisibility);
  
  m_stdDataLayer2->notifyChanged(true);
}

void AcceleratedSampleDoc::changeBackLayerVisibility(TSLDrawingSurface * surface)
{
  m_backLayerVisibility = !m_backLayerVisibility;
  surface->setDataLayerProps(m_stdLayerName1, TSLPropertyVisible, m_backLayerVisibility);
  
  m_stdDataLayer1->notifyChanged(true);
}