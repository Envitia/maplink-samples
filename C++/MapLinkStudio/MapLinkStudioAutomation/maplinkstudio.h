// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CMapLinkStudio wrapper class

class IMapLinkStudio : public COleDispatchDriver
{
public:
  IMapLinkStudio(){} // Calls COleDispatchDriver default constructor
  IMapLinkStudio(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IMapLinkStudio(const IMapLinkStudio& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IMapLinkStudio methods
public:
  BOOL OpenProject(LPCTSTR projectFile)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x10, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, projectFile);
    return result;
  }
  BOOL SaveProject()
  {
    BOOL result;
    InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL SaveProjectAs(LPCTSTR projectFile)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, projectFile);
    return result;
  }
  void Quit()
  {
    InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  BOOL CreateMap(LPCTSTR mapName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapName);
    return result;
  }
  BOOL SelectLayersForMap(LPCTSTR layers)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x15, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layers);
    return result;
  }
  BOOL SelectAllLayersForMap()
  {
    BOOL result;
    InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL MergeLayersIntoMap(LPCTSTR mapName, LPCTSTR layers)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapName, layers);
    return result;
  }
  BOOL MergeSelectedLayersIntoMap(LPCTSTR mapName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapName);
    return result;
  }
  BOOL SelectLayer(LPCTSTR layer)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x19, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer);
    return result;
  }
  BOOL SelectDataset(LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset);
    return result;
  }
  BOOL SelectData(LPCTSTR layer, LPCTSTR dataset, LPCTSTR data)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, data);
    return result;
  }
  BOOL SetDatasetInputClipping(LPCTSTR layer, LPCTSTR dataset, double minX, double maxX, double minY, double maxY)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
    InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, minX, maxX, minY, maxY);
    return result;
  }
  BOOL DisableDatasetInputClipping(LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset);
    return result;
  }
  BOOL SetAllDatasetsInputClipping(double minX, double maxX, double minY, double maxY)
  {
    BOOL result;
    static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
    InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, minX, maxX, minY, maxY);
    return result;
  }
  BOOL AddDataToDataset(LPCTSTR datafile, LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, datafile, layer, dataset);
    return result;
  }
  BOOL AddDataToAllDatasets(LPCTSTR datafile)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x20, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, datafile);
    return result;
  }
  BOOL RemoveData(LPCTSTR datafile, LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, datafile, layer, dataset);
    return result;
  }
  BOOL DatasetInputExtent(LPCTSTR layer, LPCTSTR dataset, double * x1, double * y1, double * x2, double * y2)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
    InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, x1, y1, x2, y2);
    return result;
  }
  BOOL DataInputExtent(LPCTSTR layer, LPCTSTR dataset, LPCTSTR datafile, double * x1, double * y1, double * x2, double * y2)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
    InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, datafile, x1, y1, x2, y2);
    return result;
  }
  BOOL SetMapCoordinateSystem(LPCTSTR mapCoordinateSystemName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapCoordinateSystemName);
    return result;
  }
  long LayerNames(VARIANT * LayerNames)
  {
    long result;
    static BYTE parms[] = VTS_PVARIANT ;
    InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, LayerNames);
    return result;
  }
  long DatasetNames(LPCTSTR layer, VARIANT * DatasetNames)
  {
    long result;
    static BYTE parms[] = VTS_BSTR VTS_PVARIANT ;
    InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms, layer, DatasetNames);
    return result;
  }
  long DatafileNames(LPCTSTR layer, LPCTSTR dataset, VARIANT * DatafileNames)
  {
    long result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_PVARIANT ;
    InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, layer, dataset, DatafileNames);
    return result;
  }
  CString DatasetFilterName(LPCTSTR layer, LPCTSTR dataset)
  {
    CString result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x28, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, layer, dataset);
    return result;
  }
  BOOL CreateTerrainDatabase(LPCTSTR terrainDatabaseName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, terrainDatabaseName);
    return result;
  }
  BOOL MapTMCperMU(short tmcPerMU)
  {
    BOOL result;
    static BYTE parms[] = VTS_I2 ;
    InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, tmcPerMU);
    return result;
  }

  // IMapLinkStudio properties
public:
  BOOL GetBatchProcessing()
  {
    BOOL result;
    GetProperty(0x1, VT_BOOL, (void*)&result);
    return result;
  }
  void SetBatchProcessing(BOOL propVal)
  {
    SetProperty(0x1, VT_BOOL, propVal);
  }
  CString GetLogFile()
  {
    CString result;
    GetProperty(0x2, VT_BSTR, (void*)&result);
    return result;
  }
  void SetLogFile(CString propVal)
  {
    SetProperty(0x2, VT_BSTR, propVal);
  }
  CString GetMapDirectory()
  {
    CString result;
    GetProperty(0x3, VT_BSTR, (void*)&result);
    return result;
  }
  void SetMapDirectory(CString propVal)
  {
    SetProperty(0x3, VT_BSTR, propVal);
  }
  CString GetTerrainDatabaseDirectory()
  {
    CString result;
    GetProperty(0x4, VT_BSTR, (void*)&result);
    return result;
  }
  void SetTerrainDatabaseDirectory(CString propVal)
  {
    SetProperty(0x4, VT_BSTR, propVal);
  }
  CString GetDataDirectory()
  {
    CString result;
    GetProperty(0x5, VT_BSTR, (void*)&result);
    return result;
  }
  void SetDataDirectory(CString propVal)
  {
    SetProperty(0x5, VT_BSTR, propVal);
  }
  BOOL GetAutoloadFiles()
  {
    BOOL result;
    GetProperty(0x6, VT_BOOL, (void*)&result);
    return result;
  }
  void SetAutoloadFiles(BOOL propVal)
  {
    SetProperty(0x6, VT_BOOL, propVal);
  }
  BOOL GetSubDirLayers()
  {
    BOOL result;
    GetProperty(0x7, VT_BOOL, (void*)&result);
    return result;
  }
  void SetSubDirLayers(BOOL propVal)
  {
    SetProperty(0x7, VT_BOOL, propVal);
  }
  BOOL GetVisible()
  {
    BOOL result;
    GetProperty(0x8, VT_BOOL, (void*)&result);
    return result;
  }
  void SetVisible(BOOL propVal)
  {
    SetProperty(0x8, VT_BOOL, propVal);
  }
  CString GetRasterPyramidFormat()
  {
    CString result;
    GetProperty(0x9, VT_BSTR, (void*)&result);
    return result;
  }
  void SetRasterPyramidFormat(CString propVal)
  {
    SetProperty(0x9, VT_BSTR, propVal);
  }
  short GetJPEGCompressionFactor()
  {
    short result;
    GetProperty(0xa, VT_I2, (void*)&result);
    return result;
  }
  void SetJPEGCompressionFactor(short propVal)
  {
    SetProperty(0xa, VT_I2, propVal);
  }
  LPDISPATCH GetVPFPanel()
  {
    LPDISPATCH result;
    GetProperty(0xb, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetVPFPanel(LPDISPATCH propVal)
  {
    SetProperty(0xb, VT_DISPATCH, propVal);
  }
  LPDISPATCH GetDTMPanelOcean()
  {
    LPDISPATCH result;
    GetProperty(0xc, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetDTMPanelOcean(LPDISPATCH propVal)
  {
    SetProperty(0xc, VT_DISPATCH, propVal);
  }
  LPDISPATCH GetDTMRasterPanelOcean()
  {
    LPDISPATCH result;
    GetProperty(0xd, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetDTMRasterPanelOcean(LPDISPATCH propVal)
  {
    SetProperty(0xd, VT_DISPATCH, propVal);
  }

};

// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CMapLinkStudio2 wrapper class

class IMapLinkStudio2 : public COleDispatchDriver
{
public:
  IMapLinkStudio2(){} // Calls COleDispatchDriver default constructor
  IMapLinkStudio2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IMapLinkStudio2(const IMapLinkStudio2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IMapLinkStudio2 methods
public:
  BOOL OpenProject(LPCTSTR projectFile)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x10, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, projectFile);
    return result;
  }
  BOOL SaveProject()
  {
    BOOL result;
    InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL SaveProjectAs(LPCTSTR projectFile)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, projectFile);
    return result;
  }
  void Quit()
  {
    InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  BOOL CreateMap(LPCTSTR mapName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapName);
    return result;
  }
  BOOL SelectLayersForMap(LPCTSTR layers)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x15, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layers);
    return result;
  }
  BOOL SelectAllLayersForMap()
  {
    BOOL result;
    InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL MergeLayersIntoMap(LPCTSTR mapName, LPCTSTR layers)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapName, layers);
    return result;
  }
  BOOL MergeSelectedLayersIntoMap(LPCTSTR mapName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapName);
    return result;
  }
  BOOL SelectLayer(LPCTSTR layer)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x19, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer);
    return result;
  }
  BOOL SelectDataset(LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset);
    return result;
  }
  BOOL SelectData(LPCTSTR layer, LPCTSTR dataset, LPCTSTR data)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, data);
    return result;
  }
  BOOL SetDatasetInputClipping(LPCTSTR layer, LPCTSTR dataset, double minX, double maxX, double minY, double maxY)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
    InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, minX, maxX, minY, maxY);
    return result;
  }
  BOOL DisableDatasetInputClipping(LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset);
    return result;
  }
  BOOL SetAllDatasetsInputClipping(double minX, double maxX, double minY, double maxY)
  {
    BOOL result;
    static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
    InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, minX, maxX, minY, maxY);
    return result;
  }
  BOOL AddDataToDataset(LPCTSTR datafile, LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, datafile, layer, dataset);
    return result;
  }
  BOOL AddDataToAllDatasets(LPCTSTR datafile)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x20, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, datafile);
    return result;
  }
  BOOL RemoveData(LPCTSTR datafile, LPCTSTR layer, LPCTSTR dataset)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, datafile, layer, dataset);
    return result;
  }
  BOOL DatasetInputExtent(LPCTSTR layer, LPCTSTR dataset, double * x1, double * y1, double * x2, double * y2)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
    InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, x1, y1, x2, y2);
    return result;
  }
  BOOL DataInputExtent(LPCTSTR layer, LPCTSTR dataset, LPCTSTR datafile, double * x1, double * y1, double * x2, double * y2)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
    InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, dataset, datafile, x1, y1, x2, y2);
    return result;
  }
  BOOL SetMapCoordinateSystem(LPCTSTR mapCoordinateSystemName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mapCoordinateSystemName);
    return result;
  }
  long LayerNames(VARIANT * LayerNames)
  {
    long result;
    static BYTE parms[] = VTS_PVARIANT ;
    InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, LayerNames);
    return result;
  }
  long DatasetNames(LPCTSTR layer, VARIANT * DatasetNames)
  {
    long result;
    static BYTE parms[] = VTS_BSTR VTS_PVARIANT ;
    InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms, layer, DatasetNames);
    return result;
  }
  long DatafileNames(LPCTSTR layer, LPCTSTR dataset, VARIANT * DatafileNames)
  {
    long result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_PVARIANT ;
    InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, layer, dataset, DatafileNames);
    return result;
  }
  CString DatasetFilterName(LPCTSTR layer, LPCTSTR dataset)
  {
    CString result;
    static BYTE parms[] = VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x28, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, layer, dataset);
    return result;
  }
  BOOL CreateTerrainDatabase(LPCTSTR terrainDatabaseName)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, terrainDatabaseName);
    return result;
  }
  BOOL MapTMCperMU(short tmcPerMU)
  {
    BOOL result;
    static BYTE parms[] = VTS_I2 ;
    InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, tmcPerMU);
    return result;
  }
  BOOL CreateDTED(LPCTSTR dtedName, short dtedLevel)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_I2 ;
    InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dtedName, dtedLevel);
    return result;
  }
  BOOL LayerExtent(LPCTSTR layer, double * x1, double * y1, double * x2, double * y2)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
    InvokeHelper(0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, x1, y1, x2, y2);
    return result;
  }
  BOOL LayerTilingFixedNumber(LPCTSTR layer, short numTilesAcross, short numTilesDown)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I2 ;
    InvokeHelper(0x2d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, numTilesAcross, numTilesDown);
    return result;
  }
  BOOL LayerTilingFixedSize(LPCTSTR layer, double tileWidth, double tileHeight)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_R8 VTS_R8 ;
    InvokeHelper(0x2e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, tileWidth, tileHeight);
    return result;
  }
  BOOL DisableLayerTiling(LPCTSTR layer)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer);
    return result;
  }
  BOOL IsLayerTilingEnabled(LPCTSTR layer, BOOL * isTilingFixedNumber)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_PBOOL ;
    InvokeHelper(0x30, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, isTilingFixedNumber);
    return result;
  }
  BOOL QueryLayerTilingFixedNumber(LPCTSTR layer, short * numTilesAcross, short * numTilesDown)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_PI2 VTS_PI2 ;
    InvokeHelper(0x31, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, numTilesAcross, numTilesDown);
    return result;
  }
  BOOL QueryLayerTilingFixedSize(LPCTSTR layer, double * tileWidth, double * tileHeight)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_PR8 VTS_PR8 ;
    InvokeHelper(0x32, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layer, tileWidth, tileHeight);
    return result;
  }

  // IMapLinkStudio2 properties
public:
  BOOL GetBatchProcessing()
  {
    BOOL result;
    GetProperty(0x1, VT_BOOL, (void*)&result);
    return result;
  }
  void SetBatchProcessing(BOOL propVal)
  {
    SetProperty(0x1, VT_BOOL, propVal);
  }
  CString GetLogFile()
  {
    CString result;
    GetProperty(0x2, VT_BSTR, (void*)&result);
    return result;
  }
  void SetLogFile(CString propVal)
  {
    SetProperty(0x2, VT_BSTR, propVal);
  }
  CString GetMapDirectory()
  {
    CString result;
    GetProperty(0x3, VT_BSTR, (void*)&result);
    return result;
  }
  void SetMapDirectory(CString propVal)
  {
    SetProperty(0x3, VT_BSTR, propVal);
  }
  CString GetTerrainDatabaseDirectory()
  {
    CString result;
    GetProperty(0x4, VT_BSTR, (void*)&result);
    return result;
  }
  void SetTerrainDatabaseDirectory(CString propVal)
  {
    SetProperty(0x4, VT_BSTR, propVal);
  }
  CString GetDataDirectory()
  {
    CString result;
    GetProperty(0x5, VT_BSTR, (void*)&result);
    return result;
  }
  void SetDataDirectory(CString propVal)
  {
    SetProperty(0x5, VT_BSTR, propVal);
  }
  BOOL GetAutoloadFiles()
  {
    BOOL result;
    GetProperty(0x6, VT_BOOL, (void*)&result);
    return result;
  }
  void SetAutoloadFiles(BOOL propVal)
  {
    SetProperty(0x6, VT_BOOL, propVal);
  }
  BOOL GetSubDirLayers()
  {
    BOOL result;
    GetProperty(0x7, VT_BOOL, (void*)&result);
    return result;
  }
  void SetSubDirLayers(BOOL propVal)
  {
    SetProperty(0x7, VT_BOOL, propVal);
  }
  BOOL GetVisible()
  {
    BOOL result;
    GetProperty(0x8, VT_BOOL, (void*)&result);
    return result;
  }
  void SetVisible(BOOL propVal)
  {
    SetProperty(0x8, VT_BOOL, propVal);
  }
  CString GetRasterPyramidFormat()
  {
    CString result;
    GetProperty(0x9, VT_BSTR, (void*)&result);
    return result;
  }
  void SetRasterPyramidFormat(CString propVal)
  {
    SetProperty(0x9, VT_BSTR, propVal);
  }
  short GetJPEGCompressionFactor()
  {
    short result;
    GetProperty(0xa, VT_I2, (void*)&result);
    return result;
  }
  void SetJPEGCompressionFactor(short propVal)
  {
    SetProperty(0xa, VT_I2, propVal);
  }
  LPDISPATCH GetVPFPanel()
  {
    LPDISPATCH result;
    GetProperty(0xb, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetVPFPanel(LPDISPATCH propVal)
  {
    SetProperty(0xb, VT_DISPATCH, propVal);
  }
  LPDISPATCH GetDTMPanelOcean()
  {
    LPDISPATCH result;
    GetProperty(0xc, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetDTMPanelOcean(LPDISPATCH propVal)
  {
    SetProperty(0xc, VT_DISPATCH, propVal);
  }
  LPDISPATCH GetDTMRasterPanelOcean()
  {
    LPDISPATCH result;
    GetProperty(0xd, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetDTMRasterPanelOcean(LPDISPATCH propVal)
  {
    SetProperty(0xd, VT_DISPATCH, propVal);
  }
  LPDISPATCH GetCIBPanel()
  {
    LPDISPATCH result;
    GetProperty(0xe, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetCIBPanel(LPDISPATCH propVal)
  {
    SetProperty(0xe, VT_DISPATCH, propVal);
  }
  LPDISPATCH GetASRPPanel()
  {
    LPDISPATCH result;
    GetProperty(0xf, VT_DISPATCH, (void*)&result);
    return result;
  }
  void SetASRPPanel(LPDISPATCH propVal)
  {
    SetProperty(0xf, VT_DISPATCH, propVal);
  }
  BOOL GetConvertColourIndexes()
  {
    BOOL result;
    GetProperty(0x33, VT_BOOL, (void*)&result);
    return result;
  }
  void SetConvertColourIndexes(BOOL propVal)
  {
    SetProperty(0x33, VT_BOOL, propVal);
  }

};

// CEventProgressNotify wrapper class

class IEventProgressNotify : public COleDispatchDriver
{
public:
  IEventProgressNotify(){} // Calls COleDispatchDriver default constructor
  IEventProgressNotify(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IEventProgressNotify(const IEventProgressNotify& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IEventProgressNotify methods
public:
  BOOL NotifyProcessingProgress(LPCTSTR file, long filePercent, LPCTSTR dataset, long datasetPercent, LPCTSTR layer, long layerPercent)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 ;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, file, filePercent, dataset, datasetPercent, layer, layerPercent);
    return result;
  }
  BOOL NotifyMapProgress(LPCTSTR file, long filePercent, LPCTSTR dataset, long datasetPercent, LPCTSTR layer, long layerPercent)
  {
    BOOL result;
    static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 ;
    InvokeHelper(0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, file, filePercent, dataset, datasetPercent, layer, layerPercent);
    return result;
  }

  // IEventProgressNotify properties
public:

};

// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CVPFPanel wrapper class

class IVPFPanel : public COleDispatchDriver
{
public:
  IVPFPanel(){} // Calls COleDispatchDriver default constructor
  IVPFPanel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IVPFPanel(const IVPFPanel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IVPFPanel methods
public:
  BOOL Init()
  {
    BOOL result;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void Update()
  {
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  long NumberOfLibraries()
  {
    long result;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  void LibraryNames(VARIANT * LibraryNames)
  {
    static BYTE parms[] = VTS_PVARIANT ;
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LibraryNames);
  }
  void SelectLibrary(LPCTSTR libraryName)
  {
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, libraryName);
  }

  // IVPFPanel properties
public:

};

// IDTMPanelOcean wrapper class

class IDTMPanelOcean : public COleDispatchDriver
{
public:
  IDTMPanelOcean(){} // Calls COleDispatchDriver default constructor
  IDTMPanelOcean(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IDTMPanelOcean(const IDTMPanelOcean& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IDTMPanelOcean methods
public:
  BOOL Init()
  {
    BOOL result;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void Update()
  {
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  long NumberOfResolutions()
  {
    long result;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  void ResolutionNames(VARIANT * ResolutionNames)
  {
    static BYTE parms[] = VTS_PVARIANT ;
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ResolutionNames);
  }
  void SelectResolution(LPCTSTR resolutionName)
  {
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, resolutionName);
  }

  // IDTMPanelOcean properties
public:

};

// IDTMRasterPanelOcean wrapper class

class IDTMRasterPanelOcean : public COleDispatchDriver
{
public:
  IDTMRasterPanelOcean(){} // Calls COleDispatchDriver default constructor
  IDTMRasterPanelOcean(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IDTMRasterPanelOcean(const IDTMRasterPanelOcean& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IDTMRasterPanelOcean methods
public:
  BOOL Init()
  {
    BOOL result;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void Update()
  {
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  long NumberOfResolutions()
  {
    long result;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  void ResolutionNames(VARIANT * ResolutionNames)
  {
    static BYTE parms[] = VTS_PVARIANT ;
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ResolutionNames);
  }
  void SelectResolution(LPCTSTR resolutionName)
  {
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, resolutionName);
  }

  // IDTMRasterPanelOcean properties
public:

};

// ICIBPanel wrapper class

class ICIBPanel : public COleDispatchDriver
{
public:
  ICIBPanel(){} // Calls COleDispatchDriver default constructor
  ICIBPanel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  ICIBPanel(const ICIBPanel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // ICIBPanel methods
public:
  BOOL Init()
  {
    BOOL result;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void Update()
  {
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  long NumberOfResolutions()
  {
    long result;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  void ResolutionNames(VARIANT * ResolutionNames)
  {
    static BYTE parms[] = VTS_PVARIANT ;
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ResolutionNames);
  }
  void SelectResolution(LPCTSTR resolutionName)
  {
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, resolutionName);
  }

  // ICIBPanel properties
public:

};

// CASRPPanel wrapper class

class IASRPPanel : public COleDispatchDriver
{
public:
  IASRPPanel(){} // Calls COleDispatchDriver default constructor
  IASRPPanel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  IASRPPanel(const IASRPPanel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IASRPPanel methods
public:
  BOOL Init()
  {
    BOOL result;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void Update()
  {
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  long NumberOfRasters()
  {
    long result;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  void SelectRaster(long rasterNo)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, rasterNo);
  }
  void SelectAllRasters()
  {
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }

  // IASRPPanel properties
public:

};
