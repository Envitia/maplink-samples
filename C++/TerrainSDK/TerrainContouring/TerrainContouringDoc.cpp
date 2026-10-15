// TerrainContouringDoc.cpp : implementation of the TerrainContouringDoc class
//

#include "stdafx.h"
#include "TerrainContouring.h"

#include "TerrainContouringDoc.h"
#include "TerrainContouringView.h"
#include "TerrainContouringProgress.h"
#include "TerrainContouringOptions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TerrainContouringDoc

string TerrainContouringDoc::m_mapLayerName("map") ;

string TerrainContouringDoc::m_contourLabelPrefix("Height: ");

string TerrainContouringDoc::m_contourLayerName("ContourLayer");


IMPLEMENT_DYNCREATE(TerrainContouringDoc, CDocument)

BEGIN_MESSAGE_MAP(TerrainContouringDoc, CDocument)
//{{AFX_MSG_MAP(TerrainContouringDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
ON_COMMAND(ID_TERRAIN_OPEN, &TerrainContouringDoc::OnTerrainDBOpen)
ON_COMMAND(ID_CONTOURING_RECONTOURVISIBLEAREA, &TerrainContouringDoc::OnRecontourVisibleArea)
ON_COMMAND(ID_CONTOURTYPE_SIMPLE, &TerrainContouringDoc::OnContourTypeSimple)
ON_COMMAND(ID_CONTOURTYPE_STANDARD, &TerrainContouringDoc::OnContourTypeStandard)
ON_COMMAND(ID_CONTOURTYPE_CONREC, &TerrainContouringDoc::OnContourTypeConrec)
ON_COMMAND(ID_CONTOURTYPE_POLYGON, &TerrainContouringDoc::OnContourTypePolygon)
ON_COMMAND(ID_CONTOURING_TOGGLETEXTLABELS, &TerrainContouringDoc::OnContouringToggleLabels)
ON_COMMAND(ID_TEXTLABELALIGNMENT_BELOW, &TerrainContouringDoc::OnTextLabelAlignmentBelow)
ON_COMMAND(ID_TEXTLABELALIGNMENT_MIDDLE, &TerrainContouringDoc::OnTextLabelAlignmentMiddle)
ON_COMMAND(ID_TEXTLABELALIGNMENT_ABOVE, &TerrainContouringDoc::OnTextLabelAlignmentAbove)
ON_COMMAND(ID_CONTOURING_CHANGECONTOURINGOPTIONS, &TerrainContouringDoc::OnChangeContouringOptions)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TerrainContouringDoc construction/destruction

TerrainContouringDoc::TerrainContouringDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
  m_contour = 0 ;
  m_terrainGridWidth = 100;
  m_terrainGridHeight = 100;
  m_numContourLevels = 10;
  m_lowestHeight = 0.0;
  m_highestHeight = 0.0;
  m_contourLineType = TSLTerrainContourLineTypeStandard;
  m_drawContourPolygons = false;
  m_contourLayer = 0 ;
  m_textSizeFactor = 500;
  m_textAlignment = TSLVerticalAlignmentMiddle;
  m_drawTextLabels = false;
  m_progressDialog = 0;
  m_terrainX1 = 0.0;
  m_terrainY1 = 0.0;
  m_terrainX2 = 0.0;
  m_terrainY2 = 0.0;
}

TerrainContouringDoc::~TerrainContouringDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// TerrainContouringDoc diagnostics

#ifdef _DEBUG
void TerrainContouringDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void TerrainContouringDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TerrainContouringDoc commands

BOOL TerrainContouringDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  // Create the layer that will hold the contour lines
  m_contourLayer = new TSLStandardDataLayer();

  return TRUE;
}

bool TerrainContouringDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName.c_str() ) ;

  // Add the contour layer to the specified drawing surface
  surface->addDataLayer( m_contourLayer, m_contourLayerName.c_str() );

  // Attempt to show contour lines for the loaded data
  drawContours();

  return true ;
}

bool TerrainContouringDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer || !m_contourLayer )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName.c_str() ) ;
  surface->removeDataLayer( m_contourLayerName.c_str() );

  return true ;
}

void TerrainContouringDoc::DeleteContents() 
{
  // Clean up
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if( m_contourLayer )
  {
    m_contourLayer->destroy();
    m_contourLayer = 0;
  }

  if( m_contour )
  {
    m_contour->destroy();
    m_contour = 0 ;
  }

  if( m_progressDialog )
  {
    delete m_progressDialog;
    m_progressDialog = 0;
  }

  m_terrainDB.close();
	
  CDocument::DeleteContents();
}

void TerrainContouringDoc::OnTerrainDBOpen()
{
  // Display a file dialog
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
                  _T("All Terrain Formats|*.tdf;dmed|Terrain Control Files (*.tdf)|*.tdf|DMED Files (dmed)|dmed|All Files (*.*)|*.*||") );
  
  if ( dlg.DoModal() == IDOK )
  {   
    m_terrainDatabaseName = dlg.GetPathName();
  
    if( !loadTerrainData( m_terrainDatabaseName, true ) )
      AfxMessageBox( _T("Failed to load terrain database"), MB_OK | MB_ICONEXCLAMATION );

    // Attempt to show contour lines for the loaded data
    drawContours();
  }
}

bool TerrainContouringDoc::loadTerrainData( const CString &terrainDBFile, bool useFullExtent, double x1, double y1, 
                                            double x2, double y2 )
{
  // Process the terrain data into a terrain database
  if( m_terrainDB.open( TSLUTF8Encoder( (LPCTSTR)terrainDBFile ) ) != TSLTerrain_OK )
    return false;

  // Query the extent of the terrain data
  if( useFullExtent )
  {
    if( m_terrainDB.queryExtent( x1, y1, x2, y2 ) != TSLTerrain_OK )
      return false;
  }

  POSITION pos = GetFirstViewPosition();
  if( !pos )
  {
    // No view exists, so we cannot proceed.
    return false;
  }

  // Inform the terrain database of the size of our drawing surface so it can determine a good resolution
  // for the data.
  TerrainContouringView* pView = (TerrainContouringView*)GetNextView( pos );
  TSLNTSurface *surface = pView->getDrawingSurface();

  TSLDeviceUnits duMinX, duMaxX, duMinY, duMaxY;
  surface->getDUExtent( &duMinX, &duMinY, &duMaxX, &duMaxY );
  m_terrainDB.displayExtent( duMaxX - duMinX, duMaxY - duMinY, x1, y1, x2, y2 );

  // Convert the terrain data to contour vertices so we can give them to the contour object
  bool minSet = false;

  TSLTerrainDataItem *dataItems = new TSLTerrainDataItem[ m_terrainGridWidth * m_terrainGridHeight ];
  if( m_terrainDB.queryArea( x1, y1, x2, y2, m_terrainGridWidth, m_terrainGridHeight, dataItems ) != TSLTerrain_OK )
    return false;

  // Record the boundaries of the area used in the query
  m_terrainX1 = x1;
  m_terrainY1 = y1;
  m_terrainX2 = x2;
  m_terrainY2 = y2;

  TSLTerrainContourVertexList *vertices = new TSLTerrainContourVertexList();

  for( int i = 0; i < m_terrainGridHeight; ++i )
  {
    for( int j = 0; j < m_terrainGridWidth; j++ )
    {
      vertices->addVertex( dataItems[(i * m_terrainGridWidth ) + j].m_x, dataItems[ (i * m_terrainGridWidth ) + j ].m_y, 1,
                           &dataItems[(i * m_terrainGridWidth ) + j].m_z );
      if( dataItems[(i * m_terrainGridWidth ) + j].m_z < m_lowestHeight || !minSet )
      {
        m_lowestHeight = dataItems[(i * m_terrainGridWidth ) + j].m_z;
        minSet = true;
      }
      if( dataItems[(i * m_terrainGridWidth ) + j].m_z > m_highestHeight )
        m_highestHeight = dataItems[(i * m_terrainGridWidth ) + j].m_z;
    }
  }

  delete[] dataItems;

  if( !m_contour )
  {
    m_contour = new TSLTerrainContour();

    // Set the handler that will perform the actual rendering of the contourline lines for us
    m_contour->setCallbackHandler( pView );
  }

  // Give our vertex list to the contour object so we can then perform contouring
  m_contour->setVertices( vertices );

  return true;
}

void TerrainContouringDoc::drawContours()
{
  // If we have both a map and terrain data base loaded then we can display contours
  if( m_contour && m_mapDataLayer )
  {
    // First remove any existing contour lines from the display
    POSITION pos = GetFirstViewPosition();
    if( !pos )
    {
      // No view exists, so we cannot proceed.
      return;
    }

    TerrainContouringView* pView = (TerrainContouringView*)GetNextView( pos );
    m_contourLayer->entitySet()->clear();

    // Determine where each level of contours goes based on the number of levels we have been told to draw
    // and the difference in the height in the terrain.
    double step = ( m_highestHeight - m_lowestHeight ) / (double)m_numContourLevels;

    // Show the progress dialog to keep the user informed of how the operation is going
    double width = 0.0;

    if( m_drawTextLabels )
    {
      // Determine the size in the units of our terrain data of the text labels.
      // As we need to know how wide the text will be before we invoke the draw method
      // we need to create a text object of the maximum expected width and add it to our drawing surface.
      // The drawing surface can then calculate the width of this text object.
      stringstream maxLengthLabel;
      maxLengthLabel << m_contourLabelPrefix << m_highestHeight;
      TSLText *textObj = m_contourLayer->entitySet()->createText( 0, 0, 0, maxLengthLabel.str().c_str(), 100 );

      // It is necessary to set up the following attributes on the text object for updateEntityExtent() to work
      textObj->setRendering( TSLRenderingAttributeTextSizeFactor, m_textSizeFactor );
      textObj->setRendering( TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsMapUnits );
      textObj->setRendering( TSLRenderingAttributeTextFont, 2 );

      // Set an entity ID on the temporary text object so we can remove it once we're done
      textObj->entityID( 0 );

      m_contourLayer->notifyChanged();
      TSLNTSurface *surface = pView->getDrawingSurface();

      // Store the currently viewed area of the map. In order to calculate the extent of the text object we 
      // need to change the viewed area so that our temporary text object would be visible.
      double viewedUUX1, viewedUUY1, viewedUUX2, viewedUUY2;
      surface->getUUExtent( &viewedUUX1, &viewedUUY1, &viewedUUX2, &viewedUUY2 );

      // Change the viewed area on the drawing surface to where our temporary text object is, but don't redraw.
      double newUUX1, newUUY1, newUUX2, newUUY2;
      long newSizeArea = 2 * m_textSizeFactor;
      surface->MUToUU( -newSizeArea, -newSizeArea, &newUUX1, &newUUY1 );
      surface->MUToUU( newSizeArea, newSizeArea, &newUUX2, &newUUY2 );
      surface->resize( newUUX1, newUUY1, newUUX2, newUUY2, false, true );

      // Now calculate the size of our text object
      surface->updateEntityExtent( textObj, m_contourLayerName.c_str() );
      TSLEnvelope env = textObj->envelope( surface->id() );

      // Now we have the width of the text object in TMCs we need to convert this to the terrain database units.
      double lat1, lon1, lat2, lon2, x1, y1, x2, y2;
      surface->TMCToLatLong( env.bottomLeft().x(), env.bottomLeft().y(), &lat1, &lon1 );
      surface->TMCToLatLong( env.topRight().x(), env.topRight().y(), &lat2, &lon2 );
      m_terrainDB.latLongToMU( lat1, lon1, &x1, &y1 );
      m_terrainDB.latLongToMU( lat2, lon2, &x2, &y2 );
      width = ( x2 - x1 ) * 1.5;  // This is the width of the text labels in the terrain database units

      // Now we have the width we no longer need our text object
      m_contourLayer->removeEntity( 0 );

      // Finally, reset the viewied area of the map back to what it was originally
      surface->resize( viewedUUX1, viewedUUY1, viewedUUX2, viewedUUY2, false, false );
    }

    // If we haven't created the progress dialog before, do so now
    if( !m_progressDialog )
    {
      m_progressDialog = new TerrainContouringProgress( pView );
      m_progressDialog->Create( TerrainContouringProgress::IDD, pView );
      pView->setProgressDialog( m_progressDialog );
    }

    // Now show the dialog
    m_progressDialog->ShowWindow( SW_SHOW );

    // Inform the dialog of the total amount of work we have to do so it can accurately give a progress count
    m_progressDialog->setNumberOfContourLevels( m_numContourLevels );
    m_progressDialog->setCompletedLevels( 0 );

    // Draw the requested number of contour levels on the map
    int completedCount = 0;
    for( double level = step; level <= m_highestHeight; level += step )
    {
      if( m_drawContourPolygons )
        m_contour->drawContourPolygon( 0, level, level + step );
      else
      {
        if( m_drawTextLabels )
          m_contour->drawContourLine( 0, level, m_contourLineType, m_contourLabelPrefix.c_str(),
                                      m_textAlignment, width );
        else
          m_contour->drawContourLine( 0, level, m_contourLineType );
      }

      // Update the progress dialog's count of how far along we are in total
      ++completedCount;
      m_progressDialog->setCompletedLevels( completedCount );
    }

    // Now that we have finished contouring, hide the progress dialog
    m_progressDialog->ShowWindow( SW_HIDE );

    // Inform the contour layer it's been modified and then redraw our window
    m_contourLayer->notifyChanged();
    pView->updateWindow();
  }
}

void TerrainContouringDoc::OnRecontourVisibleArea()
{
  // First we need to determine the visible area of the map being shown
  POSITION pos = GetFirstViewPosition();
  if( !pos )
  {
    // No view exists, so we cannot proceed.
    return;
  }

  TerrainContouringView* pView = (TerrainContouringView*)GetNextView( pos );
  TSLNTSurface *surface = pView->getDrawingSurface();

  TSLEnvelope tmcMapExtent;
  surface->getTMCExtent( tmcMapExtent );

  // Now we need to convert the TMC extent to an extent in the terrain database
  TSLCoordinateSystem *terrainCS = m_terrainDB.queryCoordinateSystem();
  const TSLCoordinateSystem *mapCS = m_mapDataLayer->queryCoordinateSystem();
  bool needToConvert = false;
  if( terrainCS->id() != mapCS->id() || terrainCS->getTMCperMU() != mapCS->getTMCperMU() )
    needToConvert = true;

  double terrainX1, terrainY1, terrainX2, terrainY2;

  if( !needToConvert )
  {
    // The terrain database and map coordinate systems are the same
    terrainCS->TMCToMU( tmcMapExtent.bottomLeft().x(), tmcMapExtent.bottomLeft().y(), &terrainX1, &terrainY1 );
    terrainCS->TMCToMU( tmcMapExtent.topRight().x(), tmcMapExtent.topRight().y(), &terrainX2, &terrainY2 );
  }
  else
  {
    // We need to convert between the terrain database and map coordinate systems
    double lat1, lon1, lat2, lon2;
    mapCS->TMCToLatLong( tmcMapExtent.bottomLeft().x(), tmcMapExtent.bottomLeft().y(), &lat1, &lon1 );
    mapCS->TMCToLatLong( tmcMapExtent.topRight().x(), tmcMapExtent.topRight().y(), &lat2, &lon2 );
    terrainCS->latLongToMU( lat1, lon1, &terrainX1, &terrainY1 );
    terrainCS->latLongToMU( lat2, lon2, &terrainX2, &terrainY2 );
  }

  loadTerrainData( m_terrainDatabaseName, false, terrainX1, terrainY1, terrainX2, terrainY2 );
  drawContours();
}

void TerrainContouringDoc::OnContourTypeSimple()
{
  // Redraw the contours using the simple contour algoritm
  m_contourLineType = TSLTerrainContourLineTypeSimple;
  m_drawContourPolygons = false;
  updateMenuState();
  drawContours();
}

void TerrainContouringDoc::OnContourTypeStandard()
{
  // Redraw the contours using the standard contour algoritm
  m_contourLineType = TSLTerrainContourLineTypeStandard;
  m_drawContourPolygons = false;
  updateMenuState();
  drawContours();
}

void TerrainContouringDoc::OnContourTypeConrec()
{
  // Redraw the contours using the CONREC contour algoritm
  m_contourLineType = TSLTerrainContourLineTypeCONREC;
  m_drawContourPolygons = false;
  updateMenuState();
  drawContours();
}

void TerrainContouringDoc::OnContourTypePolygon()
{
  // Redraw the contours using polygons
  m_drawContourPolygons = true;
  updateMenuState();
  drawContours();
}

void TerrainContouringDoc::updateMenuState()
{
  CWnd* wnd = AfxGetMainWnd();
  CMenu *theMenu = wnd->GetMenu();
  if( theMenu )
  {
    // Get the contouring menu
    CMenu *contourMenu = theMenu->GetSubMenu(4);
    if ( contourMenu )
    {
      // Update the checked state of the menu to represent the label setting
      if( m_drawTextLabels )
        contourMenu->CheckMenuItem( ID_CONTOURING_TOGGLETEXTLABELS, MF_CHECKED );
      else
        contourMenu->CheckMenuItem( ID_CONTOURING_TOGGLETEXTLABELS, MF_UNCHECKED );

      CMenu *contourTypeMenu = contourMenu->GetSubMenu( 0 );
      if( contourTypeMenu )
      {
        // Update the checked state of the menus to represent the setting for the contours
        if( m_drawContourPolygons )
        {
          contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_POLYGON, MF_CHECKED );
          contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_STANDARD, MF_UNCHECKED );
          contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_CONREC, MF_UNCHECKED );
          contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_SIMPLE, MF_UNCHECKED );
        }
        else
        {
          contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_POLYGON, MF_UNCHECKED );

          switch( m_contourLineType )
          {
          case TSLTerrainContourLineTypeCONREC:
            {
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_STANDARD, MF_UNCHECKED );
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_CONREC, MF_CHECKED );
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_SIMPLE, MF_UNCHECKED );
            }
            break;

          case TSLTerrainContourLineTypeStandard:
            {
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_STANDARD, MF_CHECKED );
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_CONREC, MF_UNCHECKED );
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_SIMPLE, MF_UNCHECKED );
            }
            break;

          case TSLTerrainContourLineTypeSimple:
            {
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_STANDARD, MF_UNCHECKED );
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_CONREC, MF_UNCHECKED );
              contourTypeMenu->CheckMenuItem( ID_CONTOURTYPE_SIMPLE, MF_CHECKED );
            }
            break;

          default:
            break;
          }
        }
      }

      // Update the checked state of the label alignment menu
      CMenu *labelAlignmentMenu = contourMenu->GetSubMenu( 1 );
      if( labelAlignmentMenu )
      {
        switch( m_textAlignment )
        {
        case TSLVerticalAlignmentFullBottom:
          {
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_BELOW, MF_CHECKED );
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_MIDDLE, MF_UNCHECKED );
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_ABOVE, MF_UNCHECKED );
          }
          break;

        case TSLVerticalAlignmentMiddle:
          {
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_BELOW, MF_UNCHECKED );
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_MIDDLE, MF_CHECKED );
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_ABOVE, MF_UNCHECKED );
          }
          break;

        case TSLVerticalAlignmentFullTop:
          {
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_BELOW, MF_UNCHECKED );
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_MIDDLE, MF_UNCHECKED );
            labelAlignmentMenu->CheckMenuItem( ID_TEXTLABELALIGNMENT_ABOVE, MF_CHECKED );
          }
          break;

        default:
          break;
        }
      }
    }
  }
}
void TerrainContouringDoc::OnContouringToggleLabels()
{
  // Toggle the state of the text labels
  m_drawTextLabels = !m_drawTextLabels;

  // Update the menu to reflect the state of the label alignment
  updateMenuState();

  // Now redraw the contours
  drawContours();
}

void TerrainContouringDoc::OnTextLabelAlignmentBelow()
{
  m_textAlignment = TSLVerticalAlignmentFullBottom;

  // Update the menu to reflect the state of the label alignment
  updateMenuState();

  // Redraw the contours if the labels are visible
  if( m_drawTextLabels )
    drawContours();
}

void TerrainContouringDoc::OnTextLabelAlignmentMiddle()
{
  m_textAlignment = TSLVerticalAlignmentMiddle;

  // Update the menu to reflect the state of the abel alignment
  updateMenuState();

  // Redraw the contours if the labels are visible
  if( m_drawTextLabels )
    drawContours();
}

void TerrainContouringDoc::OnTextLabelAlignmentAbove()
{
  m_textAlignment = TSLVerticalAlignmentFullTop;

  // Update the menu to reflect the state of the label alignment
  updateMenuState();

  // Redraw the contours if the labels are visible
  if( m_drawTextLabels )
    drawContours();
}

void TerrainContouringDoc::OnChangeContouringOptions()
{
  // Show the dialog
  TerrainContouringOptions optionsDialog( NULL, m_terrainGridWidth, m_terrainGridHeight, m_numContourLevels );

  if( optionsDialog.DoModal() == IDOK )
  {
    // Options have been modified, thus we need to get the settings and recontour the map
    m_terrainGridWidth = optionsDialog.getGridWidth();
    m_terrainGridHeight = optionsDialog.getGridHeight();
    m_numContourLevels = optionsDialog.getNumberOfContourLevels();

    // Query the database again using the new grid resolution
    loadTerrainData( m_terrainDatabaseName, false, m_terrainX1, m_terrainY1, m_terrainX2, m_terrainY2 );
    drawContours();
  }
}
