/****************************************************************************
Copyright (c) 1991-2026 Envitia Group PLC
=============================================================================
DESCRIPTION     : A demonstration of loading Military Symbology using the MapLink WPF control.
****************************************************************************/

using Envitia.MapLink;
using Microsoft.Win32;
using System.IO;
using System.Windows;

namespace WPFControlSample;

public partial class MainWindow : Window
{
    private bool _isMapLoaded;
    private TSLNStandardDataLayer? _mapSymbolsLayer;

    public MainWindow()
    {
        InitializeComponent();
        MapControl.SizeChanged += MapControl_SizeChanged;
    }

    private void MapControl_SizeChanged(object sender, SizeChangedEventArgs e)
    {
        if (!_isMapLoaded || e.NewSize.Width <= 0 || e.NewSize.Height <= 0)
            return;

        MapControl.ResetView();
        MapControl.RequestRender();
    }

    private void ZoomIn_Click(object sender, RoutedEventArgs e)
    {
        MapControl.Zoom(20, true);
        MapControl.RequestRender();
    }

    private void ZoomOut_Click(object sender, RoutedEventArgs e)
    {
        MapControl.Zoom(20, false);
        MapControl.RequestRender();
    }

    private void Reset_Click(object sender, RoutedEventArgs e)
    {
        MapControl.ResetView();
        MapControl.RequestRender();
    }

    private void LoadMap_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new OpenFileDialog
        {
            Title = "Load Map",
            Filter = "Map files (*.map)|*.map",
            DefaultExt = ".map",
            CheckFileExists = true,
            Multiselect = false
        };

        if (dialog.ShowDialog(this) == true)
        {
            var drawingSurface = MapControl.DrawingSurface;
            if (drawingSurface != null)
            {
                while (drawingSurface.numDataLayers > 0)
                {
                    if (!drawingSurface.getDataLayerInfo(0, out _, out string layerName) ||
                        !drawingSurface.removeDataLayer(layerName))
                        throw new InvalidOperationException("Cannot remove the previous layers from the drawing surface.");
                }
            }

            _mapSymbolsLayer = null;
            _isMapLoaded = false;
            _isMapLoaded = MapControl.LoadMap(dialog.FileName);
            drawingSurface = MapControl.DrawingSurface;
            if (drawingSurface != null && drawingSurface.backgroundColour.IsEmpty)
                drawingSurface.backgroundColour = System.Drawing.Color.White;

            MapControl.RequestRender();
        }
    }

    private void MilitarySymbology_Click(object sender, RoutedEventArgs e)
    {
        PutSymbols();
        MapControl.RequestRender();
    }

    private TSLNMilitarySymbologyGenerator getTSLNMilitarySymbologyGenerator()
    {
        TSLNMilitarySymbologyGenerator symbols_generator = new TSLNMilitarySymbologyGenerator();
        DirectoryInfo repositoryRoot = new DirectoryInfo(AppContext.BaseDirectory);
        while (repositoryRoot != null &&
               !Directory.Exists(Path.Combine(repositoryRoot.FullName, ".git")) &&
               !File.Exists(Path.Combine(repositoryRoot.FullName, ".git")))
        {
            repositoryRoot = repositoryRoot.Parent;
        }

        if (repositoryRoot == null)
        {
            throw new DirectoryNotFoundException("Cannot find the Git repository above " + AppContext.BaseDirectory);
        }

        string repository = Path.Combine("C:\\Program Files\\Envitia\\MapLink Pro\\11.4\\", "config", "symbols", "MILSTD_2525D");
        if (!Directory.Exists(repository))
        {
            throw new DirectoryNotFoundException("Cannot find the military symbols directory: " + repository);
        }

        symbols_generator.loadSymbols(repository);
        return symbols_generator;
    }
    private void PutSymbols()
    {
        var drawingSurface = MapControl.DrawingSurface;
        if (drawingSurface == null)
        {
            return;
        }

        TSLNMilitarySymbologyGenerator symbols_generator = getTSLNMilitarySymbologyGenerator();

        string[] symbolIds = {
                "10031000001211000000",
                "10061000001211000000",
                "10031000001211010000",
                "10031000001630000000",
                "10030100001101000000",
                "10030100001201000000",
                "10060100001101000000",
                "10060100001201000000",
                "10034000001101070000",
                "10032000001208010000",
                "10131000001211000000",
                "10231000001630000000",
                "10031010001630000000",
                "10031020001630000000",
                "10031030001630000000",
                "10031040001630000000",
                "10031050001630000000",
                "10031007001630000000",
                "10031000001417000000",
                "10031000001417010000",
                "10031000001417020000",
                "10031000001417950000",
                "10031000001417960000",
                "10031000001417970000",
            };

        TSLNMilitarySymbologyOptions options = new TSLNMilitarySymbologyOptions();
        options.Scale = 0.3F;

        Rect bounds = new Rect(0, 0, (int)MapControl.ActualWidth, (int)MapControl.ActualHeight);
        if (bounds.Width <= 0 || bounds.Height <= 0 || symbolIds.Length == 0)
        {
            return;
        }

        if (_mapSymbolsLayer == null)
        {
            var symbolsLayer = new TSLNStandardDataLayer();
            if (!drawingSurface.addDataLayer(symbolsLayer, "symbols"))
                throw new InvalidOperationException("Cannot attach the symbols layer to the drawing surface.");

            _mapSymbolsLayer = symbolsLayer;
        }
        drawingSurface.bringToFront("symbols");

        int columns = Math.Min(symbolIds.Length, Math.Max(1,
            (int)Math.Ceiling(Math.Sqrt(symbolIds.Length * (double)bounds.Width / bounds.Height))));
        int rows = (symbolIds.Length + columns - 1) / columns;

        for (int i = 0; i < symbolIds.Length; ++i)
        {
            int x = (int)(bounds.Left + (i % columns + 0.5) * bounds.Width / columns);
            int y = (int)(bounds.Top + (i / columns + 0.5) * bounds.Height / rows);

            drawingSurface.DUToUU(x, y, out double uuX, out double uuY);
            drawingSurface.UUToTMC(uuX, uuY, out int tmcX, out int tmcY);

            TSLNSidc2525d sidc = new TSLNSidc2525d(symbolIds[i]);
            TSLNSymbol symbol = symbols_generator.getSymbol(sidc, TSLNSidc2525dSymbolSet.SeaSurface, options);
            if (symbol == null)
            {
                continue;
            }

            symbol.move(new TSLNCoord(tmcX, tmcY), new TSLNCoord(0, 0));
            _mapSymbolsLayer.entitySet.insert(symbol);
        }

        _mapSymbolsLayer.notifyChanged(true);
        MapControl.RequestRender();
    }
}
