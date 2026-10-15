// importexportDoc.cpp : implementation of the CImportExportDoc class
//

#include "stdafx.h"
#include "importexport.h"
#include <direct.h>

#include "importexportDoc.h"
#include ".\importexportdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CImportExportApp theApp ;

/////////////////////////////////////////////////////////////////////////////
// CImportExportDoc

const char *CImportExportDoc::m_stdLayerName = "std";

static const char* s57filtername = "S57filter";
static const char* shpfiltername = "shpfilter";
static const char* ascfiltername = "asciifilter";
static const char* miffiltername = "miffilter";
static const char* osmfiltername = "osmastermapfilter";

// We need to convert from the internal units of the file (for example Lat/Long) into TMC.
// So we simply specify a factor to use when importing.  Choose 100000, which will allow an
// accuracy of 1/100000 of a degree for Lat/Long data or 1/100000 of a metre for data in metres.
#define FILE_UNIT_TO_TMC_FACTOR 100.0

IMPLEMENT_DYNCREATE(CImportExportDoc, CDocument)

BEGIN_MESSAGE_MAP(CImportExportDoc, CDocument)
//{{AFX_MSG_MAP(CImportExportDoc)
	ON_COMMAND(ID_FILE_APPEND, OnFileAppend)
	ON_UPDATE_COMMAND_UI(ID_FILE_APPEND, OnUpdateFileAppend)
	ON_COMMAND(ID_FILE_EXPORT_ASCII, OnFileExportAscii)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_ASCII, OnUpdateFileExportAscii)
	ON_COMMAND(ID_FILE_EXPORT_SHAPEFILE, OnFileExportShapefile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_SHAPEFILE, OnUpdateFileExportShapefile)
	ON_COMMAND(ID_FILE_EXPORT_TMF, OnFileExportTmf)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_TMF, OnUpdateFileExportTmf)
	ON_COMMAND(ID_FILE_EXPORT_MIF, OnFileExportMif)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_MIF, OnUpdateFileExportMif)
	ON_COMMAND(ID_FILE_EXPORT_OSM, OnFileExportOSMasterMap)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_OSM, OnUpdateFileExportOSMasterMap)
	//}}AFX_MSG_MAP
        ON_COMMAND_RANGE(ID_TEST_DEFAULT, ID_TEST_ATTRIB_RENAMING, OnSetTestConfig)
        ON_UPDATE_COMMAND_UI_RANGE(ID_TEST_DEFAULT, ID_TEST_ATTRIB_RENAMING, OnUpdateSetTestConfig)


        ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportExportDoc construction/destruction

CImportExportDoc::CImportExportDoc()
{
  // Initialise my variables.
  m_stdDataLayer = 0 ;
  
  const char * maplinkHome = TSLUtilityFunctions::getMapLinkHome() ;
  if ( maplinkHome )
  {
    m_mifFilterConfig = maplinkHome ;      
    m_mifFilterConfig += "\\config\\mifinteroperability.ini" ;

    FILE * fp = fopen( m_mifFilterConfig.c_str(), "rb" ) ;

    if ( fp )
    {
      fclose( fp ) ;
    }
    else
    {
      string msg = "Cannot find mif interop config :\n" ;
      msg += m_mifFilterConfig ;
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg.c_str()), MB_OK | MB_ICONEXCLAMATION   ) ;
      exit( -1 ) ;
    }
  }
  else
  {
    AfxMessageBox( _T("Cannot get MapLink home, bad installation"), MB_OK | MB_ICONEXCLAMATION  ) ;
    exit( -1 ) ;
  }

  // Create new interoperability configuration settings.
  m_interopConfig = new TSLInteropConfig;

  m_testConfigSelection = ID_TEST_DEFAULT;

  //////////////////////////////////////////////////////////////////////
  // Please contact support for the unlock code.
  //////////////////////////////////////////////////////////////////////
  TSLUtilityFunctions::unlockSupport( TSLKeyedS57Import, "unlock code") ;
  TSLUtilityFunctions::unlockSupport( TSLKeyedS57Export, "unlock code") ;

  TSLUtilityFunctions::unlockSupport( TSLKeyedMIFImport, "unlock code") ;
  TSLUtilityFunctions::unlockSupport( TSLKeyedMIFExport, "unlock code") ;

  TSLUtilityFunctions::unlockSupport( TSLKeyedShapefileImport, "unlock code") ;
  TSLUtilityFunctions::unlockSupport( TSLKeyedShapefileExport, "unlock code") ;

  TSLUtilityFunctions::unlockSupport( TSLKeyedMasterMapImport, "unlock code") ;
  TSLUtilityFunctions::unlockSupport( TSLKeyedMasterMapExport, "unlock code") ;

  initialiseFeatureRendering();

}

CImportExportDoc::~CImportExportDoc()
{
  if ( m_stdDataLayer )
  {
    m_stdDataLayer->destroy() ;
    m_stdDataLayer = 0 ;
  }

  if ( m_interopConfig )
  {
    m_interopConfig->destroy() ;
    m_interopConfig = 0 ;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CImportExportDoc diagnostics

#ifdef _DEBUG
void CImportExportDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CImportExportDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImportExportDoc commands

BOOL CImportExportDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // Import data from file.
  return importFile(TSLUTF8Encoder(lpszPathName), false);
}

bool CImportExportDoc::importFile(const char *filepath, bool append)
{
  // Create a standard data layer to import to.
  TSLStandardDataLayer *importLayer = new TSLStandardDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  bool post_process = true;

  // First ensure the filename is uppercase.  This is important, as part of the filename may be used 
  // to form the feature code for some file formats.  If we make the filename uppercase now,then we know
  // that the feature name will also be uppercase.
  char  filename[TSL_MAX_PATH];
  strcpy( filename, filepath );
  _strupr( filename );

  // Load the data. If it is TMF data, we can load it directly.  
  // If it is Shapefile or MIF data, we need to specify a filter.
  // We will simply do it on extension in this example
  char  fname[_MAX_FNAME], ext[_MAX_EXT];
  _splitpath( filename, 0, 0, fname, ext );

  if ( !_stricmp( ext, ".tmf" ) )
  {
    // Since we don't know what features we are using at present, simply create a single feature code for this file....
    addFeatureCodeMapping(fname);

    // Simple TMF file load
    importLayer->loadData( filename );
    post_process = false ;
  }
  else if ( !_stricmp( ext, ".shp" ) )
  {
    // Import the file. Since SHP format is a themed format, there is a single feature code
    // associated with the whole file.  This feature code is uppercase version of the filename (minus
    // extension).  For example, all features in file <coast.shp> have feature code "COAST".
    // We need to map this single feature code to a MapLink Feature ID.  Thus we specify the featureID of 
    // each feature in the file.
    addFeatureCodeMapping(fname);

    TSLUtilityFunctions::importData( importLayer, filename, shpfiltername, &m_featureCodesLoaded[0],
      m_featureCodesLoaded.size(), FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR );
  }
  else if ( !_stricmp( ext, ".000" ) || !_stricmp( ext, ".031" ) )
  {
    // create feature mappings for a range of MasterMap feature codes (this may not cover all possibilites)
    addS57FeatureMapping();

    // This will be in degrees
    TSLUtilityFunctions::importData( importLayer, filename, s57filtername, &m_featureCodesLoaded[0],
      m_featureCodesLoaded.size(), 1000000.0, 1000000.0, 0, 0, 
     "" ); // "c:\\program files\\tenet\\MapLink Pro\\5.2\\config\\AMLS57Catalogue_v10.xml" );
    post_process = false ;
  }
  else if ( !_stricmp( ext, ".mif" ) )
  {
    // Since we don't know what features we are using at present, simply create a single feature code for this file....
    addFeatureCodeMapping(fname);

    TSLUtilityFunctions::importData( importLayer, filename, miffiltername, &m_featureCodesLoaded[0],
      m_featureCodesLoaded.size(), FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR, 0.0, 0.0,
      m_mifFilterConfig.c_str());
  }
  else if ( !_stricmp( ext, ".gml" ) || !_stricmp( ext, ".gz" ) || !_stricmp( ext, ".gzip" ) )
  {
    // create feature mappings for a range of MasterMap feature codes (this may not cover all possibilites)
      
    addOSMFeatureMapping(); 
    _strlwr( filename );
 
    TSLUtilityFunctions::importData( importLayer, filename, osmfiltername, &m_featureCodesLoaded[0], 
      m_featureCodesLoaded.size(), FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR );

    post_process = false;
  }

  
  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load data\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR );
    importLayer->destroy();
    return false;
  }

  TSLStandardDataLayer *processedLayer = NULL;

  bool postProcess;
  if (!post_process)
  {
    postProcess = true;
    processedLayer = importLayer;
  }
  else
  {
    // Use the interoperability manager to post-process the data.
    
    TSLInteropManager *interopManager = new TSLInteropManager;
    TSLInteropImportSet *importSet = new TSLInteropImportSet;
    importSet->addItem(importLayer, fname);
    setupInteropConfig(fname);
    processedLayer = new TSLStandardDataLayer;

    postProcess = interopManager->postImportProcess(m_interopConfig, importSet, false, processedLayer);
    
    interopManager->destroy();
    importSet->destroy();
    importLayer->destroy();
  }

  if (!postProcess)
  {
    AfxMessageBox(_T("Error while post-processing data."), MB_OK | MB_ICONERROR);
    return false;
  }

  // Client owns the processed layer returned by the manager.
  if (!append)
  {
    // "Open" - take ownership of the returned layer. Note that m_stdDataLayer is deleted in
    // DeleteContents() when opening a new file.
    m_stdDataLayer = processedLayer;
  }
  else
  {
    // "Append" - append contents of returned layer into existing m_stdDataLayer.
    // [***]
    TSLSize bufferLen;
    unsigned char *buffer = processedLayer->saveDataToBuffer(&bufferLen);
    if (!buffer)
    {
      processedLayer->destroy();
      return false;
    }

    if ( !m_stdDataLayer->appendDataFromBuffer(buffer, bufferLen) )
    {
      processedLayer->deleteBufferData(buffer);
      processedLayer->destroy();
      return false;
    }

    processedLayer->deleteBufferData(buffer);
  }

  // Setup some basic rendering information so that we can see the objects loaded.
  // This could be extended on a per-format basis as necessary.
  if ( !_stricmp( ext, ".tmf" ) )
  {
    // Since we don't know what features we are using at present, simply create a single feature code for this file....
    setupFeatureRendering(m_stdDataLayer);
  }
  else if ( !_stricmp( ext, ".000" ) || !_stricmp( ext, ".031" ) )
  {
    // Since we don't know what features we are using at present, simply create a single feature code for this file....
    setupFeatureRendering(m_stdDataLayer);
  }
  else if ( !_stricmp( ext, ".shp" ) )
  {
    // Import the file. Since SHP format is a themed format, there is a single feature code
    // associated with the whole file.  This feature code is uppercase version of the filename (minus
    // extension).  For example, all features in file <coast.shp> have feature code "COAST".
    // We need to map this single feature code to a MapLink Feature ID.  Thus we specify the featureID of 
    // each feature in the file.
    setupFeatureRendering(m_stdDataLayer);
  }
  else if ( !_stricmp( ext, ".mif" ) )
  {
    // Since we don't know what features we are using at present, simply create a single feature code for this file....
    setupFeatureRendering(m_stdDataLayer);
  }
  else if ( !_stricmp( ext, ".gml" ) || !_stricmp( ext, ".gz" ) || !_stricmp( ext, ".gzip" ) )
  {
    // setup feature rendering for OS MasterMap features

    setupOSMFeatureRendering(m_stdDataLayer);
  }

  return true;
}

void CImportExportDoc::addOSMFeatureMapping()
{
  // create feature mappings for a range of MasterMap feature codes (this may not cover all possibilites)

  m_nextImportFeatureCode++;
  
  for (int i = 10000; i <= 10500; i++)
  {
    char buffer[10];
    itoa( i, buffer, 10 );
    TSLFeatureCodeMapping featureMap;
    string nativeFeatureCode = string(buffer);
    strcpy( featureMap.m_nativeFeatureCode, nativeFeatureCode.c_str() );
    featureMap.m_maplinkFeatureID = i ;
    featureMap.m_renderLevel = 0;
    m_featureCodesLoaded.push_back( featureMap );
  } 
}

void CImportExportDoc::addS57FeatureMapping()
{
  // create feature mappings for a range of MasterMap feature codes (this may not cover all possibilites)

  m_featureCodesLoaded.clear() ;

  addFeatureCodeMapping( "43" ) ;
  addFeatureCodeMapping( "42" ) ;
}

void CImportExportDoc::setupOSMFeatureRendering(TSLDataLayer *layer)
{
  // setup some basic rendering information so that we can see the objects loaded
  for (int i = 10000; i <= 10500; i++)
  {
    char buffer[10];
    itoa( i, buffer, 10 );
    string nativeFeatureCode = string(buffer);

    layer->addFeatureRendering( nativeFeatureCode.c_str(), i );

    layer->setFeatureRendering( 0, i, TSLRenderingAttributeEdgeColour, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeEdgeStyle, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeEdgeThickness, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeExteriorEdgeStyle, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeExteriorEdgeThickness, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeExteriorEdgeColour, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeSymbolColour, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeSymbolStyle, 99000 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeTextColour, 1 );
    layer->setFeatureRendering( 0, i, TSLRenderingAttributeTextFont, 1 );
  }

}

void CImportExportDoc::addFeatureCodeMapping(const char *nativeFeatureCode)
{
  // The methods addFeatureCodeMapping() and setupFeatureRendering() provide a basic method
  // for generating feature codes and associating rendering information with these codes.
  // This allows us to see the objects loaded.
  ++m_nextImportFeatureCode;
  ++m_nextEdgeColour;
  ++m_nextEdgeColour;
  ++m_nextFillColour;
  ++m_nextSymbolColour;
  m_nextNativeFeatureCode = nativeFeatureCode;

  TSLFeatureCodeMapping featureMapping;
  strcpy(featureMapping.m_nativeFeatureCode, nativeFeatureCode);
  featureMapping.m_maplinkFeatureID = m_nextImportFeatureCode;
  featureMapping.m_renderLevel = 0; // Default value
  m_featureCodesLoaded.push_back( featureMapping );
}

void CImportExportDoc::setupFeatureRendering(TSLDataLayer *layer)
{
  // The methods addFeatureCodeMapping() and setupFeatureRendering() provide a basic method
  // for generating feature codes and associating rendering information with these codes.
  // This allows us to see the objects loaded.
  layer->addFeatureRendering( m_nextNativeFeatureCode.c_str(), m_nextImportFeatureCode );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeEdgeColour, m_nextEdgeColour );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeEdgeStyle, 1 );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeEdgeThickness, 1 );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeExteriorEdgeStyle, 1 );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeExteriorEdgeThickness, 1 );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeExteriorEdgeColour, m_nextEdgeColour );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeFillColour, m_nextFillColour );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeFillStyle, 1 );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeSymbolColour, m_nextSymbolColour );
  layer->setFeatureRendering( 0, m_nextImportFeatureCode, TSLRenderingAttributeSymbolStyle, 99000 );
}

void CImportExportDoc::initialiseFeatureRendering()
{
  m_featureCodesLoaded.clear();
  m_nextImportFeatureCode = 0;
  m_nextNativeFeatureCode = "";
  m_nextEdgeColour = 4;
  m_nextFillColour = 75;
  m_nextSymbolColour = 99;
  m_nextTextColour = 124;

  // Add a feature mapping for entities that we do not assign a feature code to. If the data comes from
  // a TSLStandardDataLayer that was previously saved to a TMF file then the TSLEntities will typically
  // define their own rendering and have feature code 0. However, we still need an entry in the feature
  // code map so that the export filters can process these entities correctly.
  TSLFeatureCodeMapping featureMapping;
  strcpy(featureMapping.m_nativeFeatureCode, "");
  featureMapping.m_maplinkFeatureID = 0;
  featureMapping.m_renderLevel = 0; // Default value
  m_featureCodesLoaded.push_back( featureMapping );
}

void CImportExportDoc::setupInteropConfig(const char *basefilename)
{
#ifdef MIF_TEST_CONFIG
  // Interoperability configuration is currently implemented as a set of fixed cases.
  // Configure based on the user's selection.
  //
  // Settings specific to MIF test data set.
  m_interopConfig->reset();
  m_interopConfig->basefilename(basefilename);
  switch (m_testConfigSelection)
  {
  case ID_TEST_DEFAULT:
  {
    break;
  }

  case ID_TEST_NATIVE_RENDERING:
  {
    m_interopConfig->useNativeRenderingAttributes(false);
    break;
  }

  case ID_TEST_GEOM_GROUPING:
  {
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ATTRIBUTE_GROUPING:
  {
    m_interopConfig->groupingAttribute("Name");
    break;
  }

  case ID_TEST_GEOM_ATTRIB_GROUPING:
  {
    m_interopConfig->groupingAttribute("Name");
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ENTITY_EXCLUSION:
  {
    const char *attribName = "CONTINENT";
    TSLVariant attribValue;
    attribValue.setVal("North America");
    m_interopConfig->addEntityExclusionAttribute(attribName, attribValue);
    break;
  }

  case ID_TEST_ATTRIB_EXCLUSION:
  {
    m_interopConfig->excludeAttribute("CONTINENT");
    m_interopConfig->excludeAttribute("CAPITAL");
    m_interopConfig->excludeAttribute("TSLType");
    m_interopConfig->excludeAttribute("TSLFStyle");
    break;
  }

  case ID_TEST_ATTRIB_TYPE_EXCLUSION:
  {
    m_interopConfig->excludeAttributeType(TSLInteropAttributeTypeDataset, true);
    m_interopConfig->excludeAttributeType(TSLInteropAttributeTypeRendering, true);
    break;
  }

  case ID_TEST_EXTERNAL_ATTRIB:
  {
    const char *attribName = "External1";
    TSLVariant attribValue;
    attribValue.setVal("String value");
    m_interopConfig->addExternalAttribute(attribName, attribValue);
    attribName = "External2";
    attribValue.setVal("-4");
    m_interopConfig->addExternalAttribute(attribName, attribValue);
    break;
  }

  case ID_TEST_ATTRIB_RENAMING:
  {
    m_interopConfig->renameAttribute("TSLFStyle", "NewFStyle");
    m_interopConfig->renameAttribute("TSLName", "NewName");
    break; 
  }

  default:
    AfxMessageBox("Error! Undefined configuration option.", MB_OK | MB_ICONERROR );
  }

#else   // MIF_TEST_CONFIG

  // Interoperability configuration is currently implemented as a set of fixed cases.
  // Configure based on the user's selection.
  //
  // Settings specific to SHP test data set.
  m_interopConfig->reset();
  m_interopConfig->basefilename(basefilename);
  switch (m_testConfigSelection)
  {
  case ID_TEST_DEFAULT:
  {
    break;
  }

  case ID_TEST_NATIVE_RENDERING:
  {
    m_interopConfig->useNativeRenderingAttributes(false);
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_GEOM_GROUPING:
  {
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ATTRIBUTE_GROUPING:
  {
    m_interopConfig->groupingAttribute("CONTINENT");
    break;
  }

  case ID_TEST_GEOM_ATTRIB_GROUPING:
  {
    m_interopConfig->groupingAttribute("CONTINENT");
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ENTITY_EXCLUSION:
  {
    const char *attribName = "CONTINENT";
    TSLVariant attribValue;
    attribValue.setVal("North America");
    m_interopConfig->addEntityExclusionAttribute(attribName, attribValue);
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ATTRIB_EXCLUSION:
  {
    m_interopConfig->excludeAttribute("FIPS_CODE");
    m_interopConfig->excludeAttribute("WB_CNTRY");
    m_interopConfig->excludeAttribute("TSLType");
    m_interopConfig->excludeAttribute("TSLFStyle");
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ATTRIB_TYPE_EXCLUSION:
  {
    m_interopConfig->excludeAttributeType(TSLInteropAttributeTypeDataset, true);
    m_interopConfig->excludeAttributeType(TSLInteropAttributeTypeRendering, true);
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_EXTERNAL_ATTRIB:
  {
    const char *attribName = "External1";
    TSLVariant attribValue;
    attribValue.setVal("String value");
    m_interopConfig->addExternalAttribute(attribName, attribValue);
    attribName = "External2";
    float valueFloat = 12.345678f;
    attribValue.setVal(&valueFloat);
    m_interopConfig->addExternalAttribute(attribName, attribValue);
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break;
  }

  case ID_TEST_ATTRIB_RENAMING:
  {
    m_interopConfig->renameAttribute("TSLEntID", "NewFStyle");
    m_interopConfig->renameAttribute("NAME", "NewName");
    m_interopConfig->splitIntoHomogGeomLayers(true);
    break; 
  }

  default:
    AfxMessageBox(_T("Error! Undefined configuration option."), MB_OK | MB_ICONERROR );
  }

#endif MIF_TEST_CONFIG

}

bool CImportExportDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_stdDataLayer  )
    return false;
  
  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_stdDataLayer, m_stdLayerName ) ;

  // Make sure that it is detectable, else we will not be able to query for objects
  surface->setDataLayerProps( m_stdLayerName, TSLPropertyDetect, 1 );
  
  return true ;
}

bool CImportExportDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_stdDataLayer  )
    return false;
  
  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_stdLayerName ) ;
  
  return true ;
}

void CImportExportDoc::DeleteContents()
{
  // Clean up by destroying the map and pathlist.
  if ( m_stdDataLayer )
  {
    m_stdDataLayer->destroy() ;
    m_stdDataLayer = 0 ;
  }

  initialiseFeatureRendering();

  CDocument::DeleteContents();
}

void CImportExportDoc::OnFileAppend() 
{
  if ( !m_stdDataLayer )
    return;

  // Browse for files to append.
  CString filter;
  filter.LoadString( IDS_FILE_FILTER );
  CFileDialog dlg( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, (LPCTSTR)filter );
  TCHAR	files[8192];
  dlg.m_ofn.lpstrFile = files;
  dlg.m_ofn.nMaxFile = 8192;
  files[0] = _T('\0');
  if ( dlg.DoModal() != IDOK )
    return;

  POSITION pos;
  pos = dlg.GetStartPosition();
  while (pos)
  {
    // Append data from file.
    if (!importFile(TSLUTF8Encoder((LPCTSTR)dlg.GetNextPathName(pos)), true))
    {
      break;
    }
  }

  UpdateAllViews( 0, 0, 0 );
}

void CImportExportDoc::OnUpdateFileAppend(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( (BOOL)m_stdDataLayer );
}

void CImportExportDoc::OnFileExportAscii() 
{
  saveFile( ".ascii" );
}

void CImportExportDoc::OnUpdateFileExportAscii(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( (BOOL)m_stdDataLayer );
}

void CImportExportDoc::OnFileExportShapefile() 
{
  saveFile( ".shp" );
}

void CImportExportDoc::OnUpdateFileExportShapefile(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( (BOOL)m_stdDataLayer );
}

void CImportExportDoc::OnFileExportOSMasterMap() 
{
  saveFile( ".gml" );
}

void CImportExportDoc::OnUpdateFileExportOSMasterMap(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( (BOOL)m_stdDataLayer );
}

void CImportExportDoc::saveFile( const char* defExt )
{
  if ( !m_stdDataLayer )
    return;

  if ( !m_interopConfig )
    return;

  CString filter;
  filter.LoadString( IDS_FILE_FILTER );
  CFileDialog dlg( FALSE, (LPCTSTR)TSLUTF8Decoder(defExt), 0, OFN_HIDEREADONLY, (LPCTSTR)filter );
  if ( dlg.DoModal() != IDOK )
    return;

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // First ensure the filename is uppercase.  This is important, as part of the filename may be used 
  // to form the feature code for some file formats.  If we make the filename uppercase now,then we know
  // that the feature name will also be uppercase.
  TCHAR  filename[TSL_MAX_PATH];
  _tcscpy( filename, dlg.GetPathName() );
  _tcsupr( filename );

  TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
  _tsplitpath( filename, drive, dir, fname, ext );
  
  if (!_stricmp( defExt, ".gml" ))
  {
    // export data layer straight to OSMasterMap

    TSLUtilityFunctions::exportData( m_stdDataLayer, TSLUTF8Encoder(filename), osmfiltername, &m_featureCodesLoaded[0], 
      m_featureCodesLoaded.size(), FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR );  
  }
  else if ( !_stricmp( defExt, ".000" ) )
  {
    TSLUtilityFunctions::exportData( m_stdDataLayer, TSLUTF8Encoder(filename), s57filtername, &m_featureCodesLoaded[0],
      m_featureCodesLoaded.size(), FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR );
  }
  else if ( !_stricmp( defExt, ".ascii" ) )
  {
    // Simply pass in the m_featureCodesLoaded array as we assume that whatever we are exporting
    // in this example has already been imported!
    TSLUtilityFunctions::exportData( m_stdDataLayer, TSLUTF8Encoder(filename), ascfiltername, 
      &m_featureCodesLoaded[0], m_featureCodesLoaded.size(), 
      FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR );
  }
  else  
  {
    // Configure the interoperability manager.
    TSLInteropManager *interopManager = new TSLInteropManager;
    TSLInteropExportSet *exportSet;
    setupInteropConfig(TSLUTF8Encoder(fname));

    if ( !interopManager->preExportProcess(m_interopConfig, m_stdDataLayer, &exportSet) )
    {
      AfxMessageBox(_T("Error while pre-processing data."), MB_OK | MB_ICONERROR);
      interopManager->destroy();
      return;
    }

    // Export each item in the TSLInteropExportSet.
    bool  result;
    int numItems = exportSet->size();
    for (int i = 0; i < numItems; ++i)
    {
      TSLSimpleString basefilename;
      TSLStandardDataLayer *layer;
    
      if ( !exportSet->item(i, &layer, basefilename) )
      {
        AfxMessageBox(_T("Error while pre-processing data."), MB_OK | MB_ICONERROR);
        exportSet->destroy();
        interopManager->destroy();
        return;
      }
    
      // Construct the export file name based on the basefilename given by the manager.
      TCHAR  exportfilename[TSL_MAX_PATH];
      _tmakepath( exportfilename, drive, dir, (LPTSTR)TSLUTF8Decoder(basefilename.c_str()), (LPTSTR)TSLUTF8Decoder(defExt) );

      // Save the data. If it is to be saved as TMF data, we can save it directly.  
      // If it is Shapefile or MIF data, we need to specify a filter.
      if ( !_stricmp( defExt, ".tmf" ) )
      {
        // Simple TMF file export.
        result = layer->saveData( TSLUTF8Encoder(exportfilename) );
        if (!result)
        {
          break;
        }
      }
      else if ( !_stricmp( defExt, ".shp" ) )
      {
        // Export the file. Since SHP format is a themed format, there is a single feature code
        // associated with the whole file.  This feature code is uppercase version of the filename (minus
        // extension).  For example, all features in file <coast.shp> have feature code "COAST".
        // We need to map this single feature code to a MapLink Feature ID.  Thus we specify the featureID of 
        // each feature in the file.
        // So we need to simply pass in the m_featureCodesLoaded array.
        result = TSLUtilityFunctions::exportData( layer, TSLUTF8Encoder(exportfilename), shpfiltername, 
          &m_featureCodesLoaded[0], m_featureCodesLoaded.size(), 
          FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR );
        if (!result)
        {
          break;
        }
      }
      else if ( !_stricmp( defExt, ".mif" ) )
      {
        // Simply pass in the m_featureCodesLoaded array as we assume that whatever we are exporting
        // in this example has already been imported!
        result = TSLUtilityFunctions::exportData( layer, TSLUTF8Encoder(exportfilename), miffiltername, 
          &m_featureCodesLoaded[0], m_featureCodesLoaded.size(), 
          FILE_UNIT_TO_TMC_FACTOR, FILE_UNIT_TO_TMC_FACTOR, 0, 0, m_mifFilterConfig.c_str() );
        if (!result)
        {
          break;
        }
      }
   }

    // Client owns the export set.
    exportSet->destroy();
    interopManager->destroy();
  }
    
  // Display any errors that have occurred

  const char * msg = TSLErrorStack::errorString( "Cannot export data\n" ) ;
  if ( msg )
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;

  UpdateAllViews( 0, 0, 0 );
}

void CImportExportDoc::OnFileExportTmf() 
{
  saveFile( ".tmf" );
}

void CImportExportDoc::OnUpdateFileExportTmf(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( (BOOL)m_stdDataLayer );
}

void CImportExportDoc::OnFileExportMif() 
{
  saveFile( ".mif" );
}

void CImportExportDoc::OnUpdateFileExportMif(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( (BOOL)m_stdDataLayer );
}

void CImportExportDoc::OnSetTestConfig(UINT nID) 
{
  m_testConfigSelection = nID;
}

void CImportExportDoc::OnUpdateSetTestConfig(CCmdUI* pCmdUI)
{
  pCmdUI->Enable( TRUE );
  pCmdUI->SetCheck( m_testConfigSelection == pCmdUI->m_nID );
}

void CImportExportDoc::OnFileOpen()
{
  CString filter;
  filter.LoadString( IDS_FILE_FILTER );

  CFileDialog dlg( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, (LPCTSTR)filter );
  TCHAR	files[8192];
  dlg.m_ofn.lpstrFile = files;
  dlg.m_ofn.nMaxFile = 8192;
  files[0] = '\0';
  if ( dlg.DoModal() != IDOK )
    return;

  POSITION pos;
  pos = dlg.GetStartPosition();
  bool append = false ;

  while (pos)
  {
    if ( !append )
    {
      theApp.OpenDocumentFile( dlg.GetNextPathName( pos ) ) ;
      append = true ;
    }
    else
    {
      // Append data from file.
      if (!importFile(TSLUTF8Encoder(dlg.GetNextPathName(pos)), append))
      {
        break;
      }
    }
  }

  UpdateAllViews( 0, 0, 0 );
}
