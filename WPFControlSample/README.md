## WPF Integration

`TSLNSkiaDrawingSurfaceWPFControl`

To support modern .NET desktop applications, MapLink Pro 11.3.1 introduces a **WPF-native drawing control**.

The control is available from a new `Envitia.MapLink.TSLNSkiaDrawingSurfaceWPFControl` .NET library included in the MapLink installation.

### Purpose

- Enables MapLink rendering directly within WPF XAML applications
- Provides a drop-in UI component for .NET developers
- Eliminates the need for custom interop layers

### Usage Model

The control can be declared directly in XAML, similar to standard WPF controls:

```
<maplink:TSLNSkiaDrawingSurfaceWPFControl x:Name="MapControl" 
    MinWidth="100"
    MinHeight="100"
    BaseMap ="C:\Program Files\Envitia\MapLink Pro\11.3\Maps\NaturalEarthBasic\NaturalEarthBasic.map" />
```
