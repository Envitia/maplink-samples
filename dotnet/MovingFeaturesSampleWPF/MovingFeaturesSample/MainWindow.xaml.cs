using MovingFeaturesSample.MapLayers;
using System;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace MovingFeaturesSample
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public DockableWindow Button1Dockable { get; set; } = new DockableWindow();
    public DockableWindow Button2Dockable { get; set; } = new DockableWindow();

    public EmptyWindow Button1Window { get; set; }
    public EmptyWindow Button2Window { get; set; }

    public DockManager BottomDockManager { get; } = new DockManager { Location = DockManager.DockLocation.Bottom };
    
    private DrawingSurfacePanel.MapViewerPanel MapPanel { get; set; }

    private Maps Maps { get; set; } = new Maps();

    private LayerSelector LayerSelector { get; set; }
    
    public MainWindow()
    {
      InitializeComponent();

      BottomDockManager.DockPanel = BottomDockPanel;
      BottomDockManager.MaximiseGridLocation = new DockManager.GridLocation { Grid = this.MaximiseGrid, X = 0, Y = 0 };
      BottomDockManager.FloatGridLocation = new DockManager.GridLocation { Grid = this.HorzFloatingWindowGrid, X = 1, Y = 0 };
      
      Button1Window = new EmptyWindow(Button1Dockable);
      Button1Dockable.Minimised = Button1;
      BottomDockManager.AddWindow(Button1Dockable);

      Button2Window = new EmptyWindow(Button2Dockable);
      Button2Dockable.Minimised = Button2;
      BottomDockManager.AddWindow(Button2Dockable);
      
      Envitia.MapLink.TSLNCoordinateSystem.loadCoordinateSystems();

      MapPanel = (MainMap.Child as DrawingSurfacePanel.MapViewerPanel);

      LayerSelector = new LayerSelector(this);
      LayerSelector.MapPanel = MapPanel;
      LayerSelector.LayerChanged += OnLayerSelectionChanged;
      LayerSelector.LayerPropertyChanged += OnLayerPropertyChanged;

      ShowFoundationLayer();

      Maps.MapLayers.ForEach(layer =>
      {
        layer.Panel = MapPanel;
      });

      // The layers defined in the config are hidden initially, but added to the layer selection panel so that
      // they can be turned on later
      var layersPanel = (StackPanel)this.FindName("MainLayersPanel");
      Maps.AllProperties.ForEach(layer =>
      {
        LayerSelector.AddLayerToSelectionPanel(layersPanel, [new LayerSelector.LayerDetails { Name = layer, Label = layer, Index = -1 }]);
      });

      MapPanel.TooltipTextProvider = new MapPropertiesSummaryProvider { DrawingSurface = MapPanel.DrawingSurface };
    }

    /// <summary>
    /// Applies properties on the MapPanel overlay
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e">Property details</param>
    public void OnLayerPropertyChanged(object sender, LayerChangedEventArgs e)
    {
      switch (e.LayerProperty)
      {
        case MapLayerProperty.Transparency:
          SetTransparency(e.Selected, e.LayerName);
          break;
      }
    }

    /// <summary>
    /// Set the transparency property of a given layer
    /// </summary>
    /// <param name="visible">Whether we want the layer transparent or not</param>
    /// <param name="layerName">Layer name</param>
    private void SetTransparency(bool? visible, string property)
    {
      var layer = Maps.GetMapLayer(property, String.Empty, false);
      MapPanel.DrawingSurface.setDataLayerProps(layer.Name, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyTransparency, (bool)visible ? 220 : 255);
      MapPanel.DrawingSurface.redraw();
    }

    /// <summary>
    /// Reloads the map when the selected overlays change
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e">Contains layer name that is selected/deselected</param>
    public void OnLayerSelectionChanged(object sender, LayerChangedEventArgs e)
    {
      LayerSelector layerSelector = (LayerSelector)sender;
      string property = e.LayerName;
      bool visible = e.Selected;

      ToggleLayerOnMap(visible, property);
    }

    /// <summary>
    /// Toggles an overlay on Map Panel
    /// </summary>
    /// <param name="visible"></param>
    /// <param name="property"></param>
    private bool ToggleLayerOnMap(bool visible, string property)
    {
      var layer = Maps.GetMapLayer(property, String.Empty, false);
      if (layer is null) { return false; }

      Maps.SetPropertySelection(layer.Property, visible);
      return ToggleLayerOnMap(layer, visible);
    }

    /// <summary>
    /// Toggles an overlay on Map Panel
    /// </summary>
    /// <param name="visible"></param>
    /// <param name="property"></param>
    private bool ToggleLayerOnMap(MapLayer layer, bool visible)
    {
      if (visible)
      {
        MapPanel.AddLayer(layer, layer.Name);
      }
      else
      {
        MapPanel.HideLayer(layer, layer.Name);
      }

      layer.ConfigureMapLayer(MapPanel.DrawingSurface, visible);
      MapPanel.DrawingSurface.setDataLayerProps(layer.Name, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, visible ? 1 : 0);

      Maps.ReorderLayers(MapPanel.DrawingSurface);
      MapPanel.RedrawMap();

      return true;
    }

    /// <summary>
    /// Make sure the foundation layer is visible on the Map
    /// </summary>
    /// <exception cref="ArgumentOutOfRangeException"></exception>
    private void ShowFoundationLayer()
    {
      if (Maps.FoundationLayer == null)
      {
        throw new ArgumentOutOfRangeException("Maps configured incorrectly");
      }

      Maps.FoundationLayer.Panel = MapPanel;
      MapPanel.SetFoundationLayer(Maps.FoundationLayer);
    }

    private void Button1_Click(object sender, RoutedEventArgs e)
    {
      Button1Dockable.Window.Owner = this;
      Button1Dockable.RaiseDockEvent();
    }

    private void Button2_Click(object sender, RoutedEventArgs e)
    {
      Button2Dockable.Window.Owner = this;
      Button2Dockable.RaiseDockEvent();
    }

    /// <summary>
    /// Show or hides the layer selection panel
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void LayersButton_Click(object sender, RoutedEventArgs e)
    {
      StackPanel element = (StackPanel)this.FindName("MainLayersPanel");

      Visibility visibility = element.Visibility == Visibility.Visible ? Visibility.Collapsed : Visibility.Visible;
      element.Visibility = visibility;
    }

/// <summary>
    /// Window Closed Event Handler
    /// </summary>
    /// <param name="e"></param>
    protected override async void OnClosed(EventArgs e)
    {
      base.OnClosed(e);
      // Shut things down 2 seconds from now
      Timer t = new Timer(
          (state) => { App.Current.Shutdown(); },
          null, 4000, -1);
    }
  }
}
