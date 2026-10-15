using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Envitia.MapLink;

namespace MapLinkProApp.MapLayers
{
  /// <summary>
  /// Visualisation of geospatial data loaded in its native format.
  /// Works for vector data (SHP etc.) and raster.
  /// Uses Direct Import so only the vector formats listed here [https://www.envitia.com/technologies/products/maplink-pro/userguide/fileformats_page.html#Vector_Formats]
  /// and marked as supported by the Direct Import SDK, are supported.
  /// </summary>
  public class NativeMapLayer : MapLayer
  {
    // Applies a default rendering configuration for vectors. Can be overriden.
    public DirectImport.VectorRendering VectorRendering { get; set; } = new DirectImport.VectorRendering();

    private Envitia.MapLink.DirectImport.TSLNDirectImportDataLayer DirectImportDataLayer { get; set; } = null;

    private DirectImport.DirectImportCallbacks DirectImportCallbacks { get; } = new DirectImport.DirectImportCallbacks();

    // Load data in it's native format into a direct import data layer.
    private Envitia.MapLink.DirectImport.TSLNDirectImportDataLayer CreateDirectImportDataLayer(string mapPath)
    {
      // One we made earlier?
      if (DirectImportDataLayer != null)
      {
        return DirectImportDataLayer;
      }

      // Create a new layer.
      DirectImportDataLayer = new Envitia.MapLink.DirectImport.TSLNDirectImportDataLayer();

      // Tell the layer to project the data into the same coordinate system as the drawing surface is already using.
      DirectImportDataLayer.coordinateSystem = Panel.GetDrawingSurface().getCoordinateProvidingLayer().coordinateSystem;

      // Direct import data is loaded asynchronously in tiles. Set up an object
      // that will be notifed when the tiles are loaded, and that will then update the drawing surface.
      DirectImportCallbacks.DrawingPanel = Panel;
      DirectImportCallbacks.DataLayer = DirectImportDataLayer;
      DirectImportDataLayer.setCallbacks(DirectImportCallbacks);

      string errors = "";

      // Data loading first pass: create a dataset object for each dataset in the source.
      var datasets = DirectImportDataLayer.createDataSets(DataLocation);
      if (datasets == null)
      {
        // Failed, report error and clean up
        string error = TSLNErrorStack.errorString("\nErrors:\n");
        errors += ("Failed to create datasets: " + DataLocation + error + "\n");
        DirectImportDataLayer = null;
      }
      else
      {
        // Apply the rendering configuration to the datasets.
        VectorRendering.applyRendering(datasets, DirectImportDataLayer);

        // Add a default scale band. Setting the scale band to 0 effectively makes all the data available at all zoom levels.
        DirectImportDataLayer.addScaleBand(0.0, "Default");

        foreach (var dataset in datasets)
        {
          // Load the dataset into the datalayer
          if (!DirectImportDataLayer.loadData(dataset, VectorRendering.FeatureClassConfig))
          {
            string error = TSLNErrorStack.errorString("\nErrors:\n");
            errors += ("Failed to load data: " + dataset.name() + error + "\n");
          }
        }
      }

      if (errors.Length > 0)
      {
        System.Windows.MessageBox.Show(errors);
      }

      return DirectImportDataLayer;
    }

    public void SetPanel(DrawingSurfacePanel.IPanel panel)
    {
      DirectImportCallbacks.DrawingPanel = panel;
    }

    public void SetDrawingSurface(Envitia.MapLink.TSLN2DDrawingSurface surface)
    {
      DirectImportCallbacks.Surface = surface;
    }

    public override void ConfigureMapLayer(Envitia.MapLink.TSLN2DDrawingSurface surface, bool visible, double depth)
    {
      surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyProgressiveDisplay, 1);

      surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, visible ? 1 : 0);
      surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyDetect, visible ? 1 : 0);
      surface.setDataLayerProps(Identifier(), Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyTransparency, Opacity);

      if (DirectImportDataLayer != null)
      {
        DirectImportDataLayer.notifyChanged();
      }

      SetDrawingSurface(surface);
    }

    public override TSLNDataLayer GetDataLayer()
    {
      if (DirectImportDataLayer == null)
      {
        // Try to load using direct import
        return CreateDirectImportDataLayer(DataLocation);
      }
      return DirectImportDataLayer;
    }
  }
}
