using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Configuration;
using System.Xml;

namespace MapLinkProApp
{
  /// <summary>
  /// Map management.
  /// </summary>
  class Maps
  {
    public List<MapLayers.MapLayer> MapLayers { get; set; } = new List<MapLayers.MapLayer>();

    /// <summary>
    /// Get the foundation (AKA base, background), layer
    /// </summary>
    public MapLayers.MapLayer FoundationLayer { get; private set; }

    public Maps()
    {
      // Load all map layers from a configuration XML file.
      var doc = new XmlDocument();
      var mapConfig = ConfigurationManager.AppSettings["mapLayersConfig"];
      doc.Load(mapConfig);

      // Load the MapLink map layers.
      var mapLayersNode = doc.SelectSingleNode("//MapLayers");
      var mapLayerNodes = mapLayersNode.SelectNodes("MapLinkMapLayer");
      foreach (XmlNode mapLayerNode in mapLayerNodes)
      {
        var mapLayer = new MapLayers.MapLinkMapLayer()
        {
          Depth = mapLayerNode.SelectSingleNode("Depth") == null ? MapLinkProApp.MapLayers.MapLayer.ALL_DEPTHS : Convert.ToDouble(mapLayerNode.SelectSingleNode("Depth").InnerText),
          Property = mapLayerNode.SelectSingleNode("Property").InnerText,
          FeatureType = mapLayerNode.SelectSingleNode("FeatureType").InnerText,
          DataLocation = Environment.ExpandEnvironmentVariables(mapLayerNode.SelectSingleNode("DataLocation").InnerText),
          IsFoundationLayer = mapLayerNode.SelectSingleNode("FoundationLayer") != null,
          Z = mapLayerNode.SelectSingleNode("Z") == null ? 0 : int.Parse(mapLayerNode.SelectSingleNode("Z").InnerText),
        };

        if (mapLayer.IsFoundationLayer)
        {
          // Foundation layer goes at the bottom of the Z-order
          FoundationLayer = mapLayer;
          FoundationLayer.Z = -100;
        }
        else
        {
          MapLayers.Add(mapLayer);
        }
      }

      // Load all native (Direct Import) map layers.
      var nativeLayerNodes = mapLayersNode.SelectNodes("NativeMapLayer");

      foreach (XmlNode nativeLayerNode in nativeLayerNodes)
      {
        var mapLayer = new MapLayers.NativeMapLayer()
        {
          Depth = nativeLayerNode.SelectSingleNode("Depth") == null ? MapLinkProApp.MapLayers.MapLayer.ALL_DEPTHS : Convert.ToDouble(nativeLayerNode.SelectSingleNode("Depth").InnerText),
          Property = nativeLayerNode.SelectSingleNode("Property").InnerText,
          FeatureType = nativeLayerNode.SelectSingleNode("FeatureType").InnerText,
          DataLocation = Environment.ExpandEnvironmentVariables(nativeLayerNode.SelectSingleNode("DataLocation").InnerText),
          Z = nativeLayerNode.SelectSingleNode("Z") == null ? 10 : int.Parse(nativeLayerNode.SelectSingleNode("Z").InnerText),
        };

        if (mapLayer.IsFoundationLayer)
        {
          FoundationLayer = mapLayer;
          FoundationLayer.Z = -100;
        }
        else
        {
          MapLayers.Add(mapLayer);
        }
      }

      // Sort by Z-order.
      Sort();
    }

    /// <summary>
    /// Sort the layers by Z-order.
    /// </summary>
    public void Sort()
    {
      MapLayers.Sort(delegate (MapLayers.MapLayer first, MapLayers.MapLayer second)
      {
        if (first.Z == second.Z)
        {
          if (first.Depth == second.Depth)
          {
            return String.Compare(first.Property, second.Property);
          }
          return first.Depth < second.Depth ? -1 : 1;
        }          
        return first.Z < second.Z ? -1 : 1;
      });
    }

    /// <summary>
    /// Get the layer for the specified depth, property and feature type.
    /// </summary>
    /// <param name="depth"></param>
    /// <param name="property"></param>
    /// <param name="featureType"></param>
    /// <returns>Map layer</returns>
    public MapLayers.MapLayer GetMapLayer(double depth, string property, string featureType)
    {
      var found = MapLayers.Find(delegate (MapLayers.MapLayer layer)
      {
        return layer.Depth == depth && layer.Property == property && layer.FeatureType == featureType;
      });

      return found;
    }

    public List<string> AllProperties
    {
      get
      {
        var properties = new List<string>();
        foreach (var layer in MapLayers)
        {
          properties.Add(layer.Property);
        }
        return properties.Distinct().ToList();
      }
    }

    public List<string> AllFeatureTypes
    {
      get
      {
        var properties = new List<string>();
        foreach (var layer in MapLayers)
        {
          properties.Add(layer.FeatureType);
        }
        return properties.Distinct().ToList();
      }
    }

    /// <summary>
    /// Shows the map layers identified by the set of properties and feature types, at the given depth.
    /// </summary>
    /// <param name="surface"></param>
    /// <param name="properties"></param>
    /// <param name="featureTypes"></param>
    /// <param name="depth"></param>
    /// <param name="transparentOverlays">If true, set the layer opacities to less than 255.</param>
    public void ShowLayers(Envitia.MapLink.TSLNDrawingSurface surface, string[] properties, string[] featureTypes, double depth, bool transparentOverlays)
    {
      foreach (var layer in MapLayers)
      {
        bool visible = properties.Contains(layer.Property)
            && (layer.FeatureType.Length == 0 || featureTypes.Contains(layer.FeatureType))
            && (layer.Depth == MapLinkProApp.MapLayers.MapLayer.ALL_DEPTHS || layer.Depth == depth);

        if (visible)
        {
          var dataLayer = surface.getDataLayer(layer.Identifier());
          if (dataLayer == null)
          {
            surface.addDataLayer(layer.GetDataLayer(), layer.Identifier());
          }
        }

        layer.Opacity = transparentOverlays ? 120 : 255;
        layer.ConfigureMapLayer(surface, visible, depth);    
      }
    }
  }
}
