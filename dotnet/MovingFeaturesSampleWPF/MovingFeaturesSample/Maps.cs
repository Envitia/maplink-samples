using System;
using System.Linq;

namespace ContourApp
{
  /// <summary>
  /// Map management.
  /// </summary>
  public class Maps
  {
    private System.Collections.Generic.List<string> SelectedProperties { get; } = new System.Collections.Generic.List<string>();
    public void SetPropertySelection(string propertyName, bool selected)
    {
      if (selected)
      {
        SelectedProperties.Add(propertyName);
      }
      else
      {
        SelectedProperties.Remove(propertyName);
      }
    }

    public System.Collections.Generic.List<MapLayers.MapLayer> MapLayers { get; set; } = new System.Collections.Generic.List<MapLayers.MapLayer>();

    /// <summary>
    /// Get the foundation (AKA base, background), layer
    /// </summary>
    public MapLayers.MapLayer FoundationLayer { get; private set; }

    public Maps()
    {
      // Load all map layers from a configuration XML file.
      var doc = new System.Xml.XmlDocument();
      var mapConfig = System.Configuration.ConfigurationManager.AppSettings["mapLayersConfig"];
      doc.Load(mapConfig);

      // Load the MapLink map layers.
      var mapLayersNode = doc.SelectSingleNode("//MapLayers");

      var mapLayerNodes = mapLayersNode.SelectNodes("MapLinkMapLayer");
      foreach (System.Xml.XmlNode mapLayerNode in mapLayerNodes)
      {
        var mapLayer = new MapLayers.MapLinkMapLayer()
        {
          Property = mapLayerNode.SelectSingleNode("Property").InnerText,
          DataLocation = Environment.ExpandEnvironmentVariables(mapLayerNode.SelectSingleNode("DataLocation").InnerText),
          IsFoundationLayer = mapLayerNode.SelectSingleNode("FoundationLayer") != null,
          
        };
        // There could be multiple layers with the same properties, but for different layers
        // We are creating a unique layer name here
        mapLayer.Name = mapLayer.Property;

        if (mapLayer.IsFoundationLayer)
        {
          // Foundation layer goes at the bottom of the Z-order
          FoundationLayer = mapLayer;
        }
        else
        {
          MapLayers.Add(mapLayer);
        }
      }

      var ddoNodes = mapLayersNode.SelectNodes("DdoLayer");
      foreach (System.Xml.XmlNode node in ddoNodes)
      {
        var mapLayer = new MapLayers.DdoLayer()
        {
          Property = node.SelectSingleNode("Property").InnerText,
        };
        mapLayer.FoundationLayer = FoundationLayer.GetDataLayer();
        mapLayer.Name = mapLayer.Property;

        MapLayers.Add(mapLayer);
      }

      var trackNodes = mapLayersNode.SelectNodes("TracksLayer");
      foreach (System.Xml.XmlNode node in trackNodes)
      {
        var mapLayer = new MapLayers.TracksLayer
        {
          Property = node.SelectSingleNode("Property").InnerText,
        };
        mapLayer.FoundationLayer = FoundationLayer.GetDataLayer();
        mapLayer.Name = mapLayer.Property;

        MapLayers.Add(mapLayer);
      }

      // Foundation layer is always selected
      SetPropertySelection(FoundationLayer.Property, true);
    }

    /// <summary>
    /// Get the layer for the specified depth, property and feature type.
    /// </summary>
    /// <param name="depth"></param>
    /// <param name="property"></param>
    /// <param name="featureType"></param>
    /// <returns>Map layer</returns>
    public MapLayers.MapLayer GetMapLayer(string property, string featureType, bool includeFoundation)
    {
      var found = MapLayers.Find(delegate (MapLayers.MapLayer layer)
        {
          return layer.Property == property && (featureType == string.Empty);
        });

      if (found is null && includeFoundation && FoundationLayer.Property == property)
      {
        found = FoundationLayer;
      }

      return found;
    }

    /// <summary>
    /// Get the layer for the specified property
    /// This would match the first layer of a given property
    /// </summary>
    /// <param name="property"></param>
    /// <returns>Map layer</returns>
    public MapLayers.MapLayer GetMapLayer(string property)
    {
      return MapLayers.Find(delegate (MapLayers.MapLayer layer)
      {
        return (layer.Property.StartsWith(property));
      });
    }


    public System.Collections.Generic.List<string> AllProperties
    {
      get
      {
        var properties = new System.Collections.Generic.List<string>();
        foreach (var layer in MapLayers)
        {
          properties.Add(layer.Property);
        }

        return properties.Distinct().ToList();
      }
    }

    /// Shows the map layers identified by the set of properties and feature types.
    /// </summary>
    /// <param name="surface"></param>
    /// <param name="properties"></param>
    /// <param name="featureTypes"></param>
    /// <param name="transparentOverlays">If true, set the layer opacities to less than 255.</param>
    public void ShowLayers(Envitia.MapLink.TSLNDrawingSurface surface, string[] properties, string[] featureTypes, bool transparentOverlays)
    {
      foreach (var layer in MapLayers)
      {
        bool visible = properties.Contains(layer.Property);

        if (visible)
        {
          var dataLayer = surface.getDataLayer(layer.Identifier());
          if (dataLayer == null)
          {
            surface.addDataLayer(layer.GetDataLayer(), layer.Identifier());
          }
        }

        layer.Opacity = transparentOverlays ? 120 : 255;
        layer.ConfigureMapLayer(surface, visible);
      }
    }

    public void ReorderLayers(Envitia.MapLink.TSLNDrawingSurface surface)
    {
      foreach (var layer in MapLayers)
      {
        surface.bringToFront(layer.Identifier());
      }
    }
  }
}
