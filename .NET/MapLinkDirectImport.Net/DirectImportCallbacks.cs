using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using Envitia.MapLink;
using Envitia.MapLink.DirectImport;

namespace CSharpDirectImportSample
{
  // An example implementation of TSLNDirectImportDataLayerCallbacks
  //
  // This implementation provides a simple (but not optimal) implementation
  // of each callback method, allowing the sample to load a wide range
  // of geospatial data types.
  //
  // Data without an extent or coordinate system is handled, however the selection
  // made by the callbacks may be incorrect.
  //
  // This callback class is also responsible for triggering a redraw of the MapLink
  // drawing surface, as required by the TSLNDirectImportDataLayer
  class DirectImportCallbacks : TSLNDirectImportDataLayerCallbacks
  {
    private TSLNDrawingSurface m_surface = null;
    private TSLNDirectImportDataLayer m_dataLayer = null;
    private TSLNCoordinateSystem m_epsg3395 = null;
    private TSLNCoordinateSystem m_epsg4326 = null;
    private MapViewerPanel m_panel;
    private MapViewerForm m_mainForm = null;

    public DirectImportCallbacks(TSLNDrawingSurface surface, TSLNDirectImportDataLayer layer, MapViewerPanel panel, MapViewerForm mainForm)
    {
      m_surface = surface;
      m_dataLayer = layer;
      m_epsg3395 = TSLNCoordinateSystem.findByEPSG(3395);
      m_epsg4326 = TSLNCoordinateSystem.findByEPSG(4326);
      m_panel = panel;
      m_mainForm = mainForm;
    }

    public override uint onChoiceOfDrivers(string data, TSLNDirectImportDriverNameList drivers)
    {
      // Choose the first driver
      return 0;
    }

    public override void onDataSetLoadCancelled(TSLNDirectImportDataSet dataSet, uint numProcessingTotal)
    {
    }

    public override void onDataSetLoadComplete(TSLNDirectImportDataSet dataSet, uint numProcessingTotal)
    {
      // A data set has been added to the direct import layer
      // Trigger a reset of the drawing surface
      if (m_surface != null ||
          m_panel != null)
      {
        m_surface.reset(false);
        m_panel.Invalidate();
      }
    }

    public override void onDataSetLoadScheduled(TSLNDirectImportDataSet dataSet, uint numProcessingTotal)
    {
    }

    public override void onDeviceCapabilitiesRequired(TSLNDeviceCapabilities capabilities)
    {
      // Forward the call to the drawing surface
      if (m_surface != null)
      {
        m_surface.getDeviceCapabilities(capabilities);
      }
    }
    public override TSLNCoordinateSystem onNoCoordinateSystem(TSLNDirectImportDataSet dataSet, TSLNDirectImportDriver driver)
    {
      return m_mainForm.InputCS == null ? TSLNCoordinateSystem.findByEPSG(4326) : m_mainForm.InputCS;
    }
    public override TSLNMUExtent onNoExtent(TSLNDirectImportDataSet dataSet, TSLNDirectImportDriver driver)
    {
      // The data cannot be loaded without a valid extent
      // For raster data we can create one using the raster dimensions.
      // Note that if this raster is displayed on top of a map it will
      // appear at 0,0 using this callback.
      //
      // The application should read the extent of the data from any available metadata,
      // or provide a GUI for the user to geo-locate the data.
      if (dataSet.dataType() == TSLNDirectImportDriver.DataType.DataTypeRaster)
      {
        TSLNDirectImportDataSetRaster rasterDataSet = (TSLNDirectImportDataSetRaster)dataSet;
        TSLNDirectImportRasterSettings rasterSettings = rasterDataSet.rasterSettings();

        uint width = rasterSettings.width();
        uint height = rasterSettings.height();
        double x = (double)width / 2.0;
        double y = (double)height / 2.0;

        if (width != 0 && height != 0)
        {
          return new TSLNMUExtent(-x, -y, x, y);
        }
      }
      // Return an empty extent. The call to TSLNDirectImportDataLayer.createDataSets will fail.
      return new TSLNMUExtent();
    }
    public override void onTileLoadCancelled(TSLNDirectImportDataSet dataSet, uint numProcessing, uint numProcessingTotal)
    {
    }
    public override void onTileLoadComplete(TSLNDirectImportDataSet dataSet, uint numProcessing, uint numProcessingTotal)
    {
      // Refresh the display after every tile

      if (m_panel != null &&
          m_dataLayer != null)
      {
        m_dataLayer.notifyChanged();
        m_panel.Invalidate();
      }
    }
    public override void onTileLoadFailed(TSLNDirectImportDataSet dataSet, uint numProcessing, uint numProcessingTotal)
    {
    }
    public override void onTileLoadScheduled(TSLNDirectImportDataSet dataSet, uint numScheduled, uint numProcessing, uint numProcessingTotal)
    {
    }
    public override void requestRedraw()
    {
      if (m_panel != null)
      {
        m_panel.Invalidate();
      }
    }
  }
}