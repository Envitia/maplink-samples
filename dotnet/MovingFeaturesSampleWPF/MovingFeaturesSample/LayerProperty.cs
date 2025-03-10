using System.Windows;

namespace MovingFeaturesSample
{
  internal class LayerProperty
  {
    public static readonly DependencyProperty LayerNameProperty = DependencyProperty.RegisterAttached(
      "SetLayerName", typeof(string), typeof(LayerProperty), new PropertyMetadata() );

    public static string GetLayerNameProperty(DependencyObject obj)
    {
      return (string)obj.GetValue(LayerNameProperty);
    }

    public static void SetLayerNameProperty(DependencyObject obj, string value)
    {
      obj.SetValue(LayerNameProperty, value);
    }
  }
}
