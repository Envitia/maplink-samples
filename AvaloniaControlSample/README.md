## Avalonia UI Integration

`TSLNSkiaDrawingSurfaceWPFControl`

MapLink Pro 11.3.1 also introduces an **Avalonia UI drawing control**.

The control is available from a new `Envitia.MapLink.TSLNSkiaDrawingSurfaceAvaloniaControl` .NET library included in the MapLink installation.

### Purpose

- Enables MapLink rendering directly within Avalonia AXAML applications
- Provides another drop-in UI component for .NET developers
- **Demonstrates MapLink's intention to provide a code-once-deploy-anywhere capability, leveraging the flexibility of [Avalonia UI](https://avaloniaui.net/).**

### Usage Model

The control can be declared directly in AXAML, similar to standard WPF controls:

```
<maplink:TSLNSkiaDrawingSurfaceAvaloniaControl x:Name="MapControl"
    HorizontalAlignment="Stretch"
    VerticalAlignment="Stretch"
    BaseMap="C:\Program Files\Envitia\MapLink Pro\11.3\Maps\NaturalEarthBasic\NaturalEarthBasic.map" />
```
