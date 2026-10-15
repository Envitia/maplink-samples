using System;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace MapLinkProApp
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private const String BUTTON_PREFIX = "Button_";
    private const String LABEL_PREFIX = "Label_";
    private const String BORDER_PREFIX = "Border_";
    private const String LAYER_OPTIONS_PREFIX = "LayerOption_";
    private const int TRANSPARENT_INDEX = 1;

    private DrawingSurfacePanel.MapViewerPanel MapPanel { get; set; }

    private Maps Maps { get; set; } = new Maps();

    public MainWindow()
    {
      InitializeComponent();

      Envitia.MapLink.TSLNCoordinateSystem.loadCoordinateSystems();

      MapPanel = (MainMap.Child as DrawingSurfacePanel.MapViewerPanel);

      ShowFoundationLayer();

      StackPanel layersPanel = (StackPanel)this.FindName("LayersPanel");
      Maps.MapLayers.ForEach(layer =>
      {
        layer.Panel = MapPanel;
        MapPanel.DrawingSurface.addDataLayer(layer.GetDataLayer(), layer.Property);
        MapPanel.DrawingSurface.setDataLayerProps(layer.Property, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 0);
        // The layers defined in the config are hidden initially, but added to the layer selection panel so that
        // we can turn them on later
        AddLayerToSelectionPanel(layersPanel, layer.Property);
      });
    }

    private void ShowFoundationLayer()
    {
      if (Maps.FoundationLayer == null)
      {
        throw new ArgumentOutOfRangeException("Maps configured incorrectly");
      }

      Maps.FoundationLayer.Panel = MapPanel;
      MapPanel.SetFoundationLayer(Maps.FoundationLayer);
    }

    /// <summary>
    /// Show or hides the layer selection panel
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void LayersButton_Click(object sender, RoutedEventArgs e)
    {
      StackPanel element = (StackPanel)this.FindName("LayersPanel");

      Visibility visibility = element.Visibility == Visibility.Visible ? Visibility.Collapsed : Visibility.Visible;
      element.Visibility = visibility;
    }

    /// <summary>
    /// This creates the layer dropdown and layer options controls and adds them to the layer selection panel
    /// </summary>
    /// <param name="layersPanel"></param>
    /// <param name="layerName"></param>
    private void AddLayerToSelectionPanel(StackPanel layersPanel, string layerName)
    {
      Button layerButton = createLayerButton(layerName);
      // Add click event to it so that it can show layer options panel
      layerButton.Name = BUTTON_PREFIX + layerName;
      layerButton.Click += new RoutedEventHandler(LayerButton_Click);

      StackPanel layerOptionsPanel = new StackPanel();
      layerOptionsPanel.Visibility = Visibility.Collapsed;
      layerOptionsPanel.Orientation = Orientation.Vertical;
      layerOptionsPanel.Name = LAYER_OPTIONS_PREFIX + layerName;

      AddItemToLayerOption(layerOptionsPanel, "Transparent", layerName, TRANSPARENT_INDEX);
      
      layerOptionsPanel.Margin = new Thickness(5);

      Border layerOptionsBorder = new Border();
      layerOptionsBorder.Name = BORDER_PREFIX + layerName;
      Style borderStyle = this.FindResource("StackBorder") as Style;
      layerOptionsBorder.Style = borderStyle;
      layerOptionsBorder.Child = layerOptionsPanel ;

      StackPanel layerPanel = new StackPanel();
      layerPanel.Orientation = Orientation.Vertical;
      layerPanel.Margin = new Thickness(15, 0, 15, 10);
      layerPanel.Children.Add(layerButton);
      layerPanel.Children.Add(layerOptionsBorder);
      
      layersPanel.Children.Add(layerPanel);
    }

    /// <summary>
    /// This creates the layer dropdown control. The control consists of a checkbox, label and dropdown image added to a button
    /// </summary>
    /// <param name="layerName">This is used to name the control so that we can derive the layer name from control name at a later stage</param>
    /// <returns>layer dropdown control</returns>
    private Button createLayerButton(string layerName)
    {
      CheckBox checkbox = new CheckBox();
      checkbox.VerticalAlignment = VerticalAlignment.Center;
      checkbox.Margin = new Thickness(5);
      checkbox.Name = layerName;
      checkbox.AddHandler(CheckBox.ClickEvent, new RoutedEventHandler(LayerCheckBoxChanged));

      Image icon = new Image
      {
        Height = 20,
        Width = 20,
        Source = new BitmapImage(new Uri("../img/drop-down.png", UriKind.Relative)),
        VerticalAlignment = VerticalAlignment.Center
      };
      Label label = new Label();
      label.Width = 150;
      label.Name = LABEL_PREFIX + layerName;
      label.Content = layerName;

      StackPanel layerButtonPanel = new StackPanel();
      layerButtonPanel.Orientation = Orientation.Horizontal;
      layerButtonPanel.VerticalAlignment = VerticalAlignment.Center;
      layerButtonPanel.Children.Add(checkbox);
      layerButtonPanel.Children.Add(label);
      layerButtonPanel.Children.Add(icon);

      Button layerButton = new Button();
      Style style = this.FindResource("overlayButton") as Style;
      layerButton.Style = style;
      layerButton.Content = layerButtonPanel;

      return layerButton;
    }

    /// <summary>
    /// Even handler for layer checkbox - shows or hides an overlay
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void LayerCheckBoxChanged(object sender, RoutedEventArgs e)
    {
      CheckBox checkBox = (CheckBox)sender;
      StackPanel stackPanel = (StackPanel)checkBox.Parent;

      string layerName = checkBox.Name;
      string labelName = LABEL_PREFIX + layerName;
      Label label = FindChild<Label>(stackPanel, labelName);

      setCheckBoxColor(checkBox, label);

      if((bool)checkBox.IsChecked == true)
      {
        MapPanel.DrawingSurface.setDataLayerProps(layerName, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 1);
      }
      else
      {
        MapPanel.DrawingSurface.setDataLayerProps(layerName, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyVisible, 0);
      }
      MapPanel.DrawingSurface.redraw();
    }

    /// <summary>
    /// Styles checkbox and associated text (different styles depending on if it is selected or not)
    /// </summary>
    /// <param name="checkBox"></param>
    /// <param name="checkBoxLabel"></param>
    private void setCheckBoxColor(CheckBox checkBox, Label checkBoxLabel)
    {
      Color textColor = (Color)this.FindResource("CheckboxSelectedTextColor");
      Brush labelBrushSelected = new SolidColorBrush(textColor);

      if ((bool)checkBox.IsChecked == true)
      {
        Color checkBoxColor = (Color)this.FindResource("CheckboxCheckedColor");
        Brush checkBoxBrush = new SolidColorBrush(checkBoxColor);
        checkBox.Background = checkBoxBrush;
        checkBoxLabel.Foreground = labelBrushSelected;
      }
      else
      {
        Color checkBoxColor = (Color)this.FindResource("CheckboxDefaultColor");
        Brush checkBoxBrush = new SolidColorBrush(checkBoxColor);
        checkBox.Background = checkBoxBrush;
        checkBoxLabel.Foreground = Brushes.Black;
      }
    }

    /// <summary>
    /// Event handler for layer dropdown, to expand it so that layer options are visible
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void LayerButton_Click(object sender, RoutedEventArgs e)
    {
      Button button = (Button) sender;
      StackPanel stackPanel = (StackPanel)button.Parent;

      string layerName = getLayerNameFromId(button.Name, BUTTON_PREFIX);
      string layerOptionsName = LAYER_OPTIONS_PREFIX + layerName;

      StackPanel layerOptionsPanel = FindChild<StackPanel>(stackPanel, layerOptionsName);
      Visibility visibility = layerOptionsPanel.Visibility == Visibility.Visible ? Visibility.Collapsed : Visibility.Visible;
      layerOptionsPanel.Visibility = visibility;
    }

    /// 
    /// Finds a Child of a given item in the visual tree. (This is a utility method. So this can be moved to a utility class 
    /// and made static so that it is accessible from other classes as well in the future)
    /// </summary>
    /// <param name="parent">A direct parent of the queried item.</param>
    /// <typeparam name="T">The type of the queried item.</typeparam>
    /// <param name="childName">x:Name or Name of child. </param>
    /// <returns>The first parent item that matches the submitted type parameter. 
    /// If not matching item can be found, 
    /// a null parent is being returned.</returns>
    private T FindChild<T>(DependencyObject parent, string childName)
       where T : DependencyObject
    {
      // Confirm parent and childName are valid. 
      if (parent == null) return null;

      T foundChild = null;

      int childrenCount = VisualTreeHelper.GetChildrenCount(parent);
      for (int i = 0; i < childrenCount; i++)
      {
        var child = VisualTreeHelper.GetChild(parent, i);
        // If the child is not of the request child type child
        T childType = child as T;
        if (childType == null)
        {
          // recursively drill down the tree
          foundChild = FindChild<T>(child, childName);

          // If the child is found, break so we do not overwrite the found child. 
          if (foundChild != null) break;
        }
        else if (!string.IsNullOrEmpty(childName))
        {
          var frameworkElement = child as FrameworkElement;
          // If the child's name is set for search
          if (frameworkElement != null && frameworkElement.Name == childName)
          {
            // if the child's name is of the request name
            foundChild = (T)child;
            break;
          }
        }
        else
        {
          // child element found.
          foundChild = (T)child;
          break;
        }
      }

      return foundChild;
    }

    /// <summary>
    /// This would extract layername from element id. It expects the element name to be "prefix+layername"
    /// </summary>
    /// <param name="elementName"></param>
    /// <param name="prefix"></param>
    /// <returns></returns>
    private String getLayerNameFromId(string elementName, string prefix)
    {
      int startIndex = elementName.Contains(prefix) ? elementName.IndexOf(prefix) + prefix.Length : 0;
      return elementName.Substring(startIndex);
    }

    /// <summary>
    /// Adds the various checkboxes to the layer options panel
    /// </summary>
    /// <param name="parentElement">Parent element to which the checkbox would be added</param>
    /// <param name="labelContent"></param>
    /// <param name="layerName">Layer name is used to name the label so that we can derive the layername from element name at later stage</param>
    /// <param name="index">this is used to create distinct names for each checkbox</param>
    private void AddItemToLayerOption(StackPanel parentElement, string labelContent, string layerName, int index) 
    {
      StackPanel stackPanel = new StackPanel();
      stackPanel.Orientation = Orientation.Horizontal;
      stackPanel.Name = LAYER_OPTIONS_PREFIX + index + layerName;

      CheckBox checkbox = new CheckBox();
      Label label = new Label();
      label.Name = LAYER_OPTIONS_PREFIX + index;
      label.Content = labelContent;

      if(index == 1) 
      {
        checkbox.AddHandler(CheckBox.ClickEvent, new RoutedEventHandler(TransparentCheckBoxChanged));
      }

      stackPanel.Children.Add(checkbox);
      stackPanel.Children.Add(label);

      parentElement.Children.Add(stackPanel);
    }

    /// <summary>
    /// Checkbox event handler function for the transparency functionality
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void TransparentCheckBoxChanged(object sender, RoutedEventArgs e)
    {
      CheckBox checkBox = (CheckBox)sender;
      StackPanel parent = (StackPanel)checkBox.Parent;

      string layerOptionsName = parent.Name;
      string layerName = getLayerNameFromId(layerOptionsName, LAYER_OPTIONS_PREFIX + TRANSPARENT_INDEX);

      string labelName = LAYER_OPTIONS_PREFIX + TRANSPARENT_INDEX;
      Label label = FindChild<Label>(parent, labelName);

      // This would apply styling to checkbox (different styles depending on whether it is checked or not
      setCheckBoxColor(checkBox, label);

      if(checkBox.IsChecked == true)
      {
        MapPanel.DrawingSurface.setDataLayerProps(layerName, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyTransparency, 100);
      }
      else
      {
        MapPanel.DrawingSurface.setDataLayerProps(layerName, Envitia.MapLink.TSLNPropertyEnum.TSLNPropertyTransparency, 255);
      }

      MapPanel.DrawingSurface.redraw();
    }

    /// <summary>
    /// Window Closed Event Handler
    /// </summary>
    /// <param name="e"></param>
    protected override void OnClosed(EventArgs e)
    {
      base.OnClosed(e);
      // Shut things down 2 seconds from now
      Timer t = new Timer(
          (state) => { App.Current.Shutdown(); },
          null, 2000, -1);
    }
  }
}
