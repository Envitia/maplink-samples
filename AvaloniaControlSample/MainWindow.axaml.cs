/****************************************************************************
Copyright (c) 1991-2026 Envitia Group PLC
=============================================================================
DESCRIPTION     : This contains a simple example of using the MapLink WPF control.
****************************************************************************/

using Avalonia.Controls;
using Avalonia.Interactivity;

namespace AvaloniaControlSample;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        Title = "Avalonia Map Demo";
    }

    private void ResetButton_Click(object? sender, RoutedEventArgs e)
    {
        MapControl.ResetView();
        MapControl.RequestRender();
    }

    private void ZoomInButton_Click(object? sender, RoutedEventArgs e)
    {
        MapControl.Zoom(30, true);
        MapControl.RequestRender();
    }

    private void ZoomOutButton_Click(object? sender, RoutedEventArgs e)
    {
        MapControl.Zoom(30, false);
        MapControl.RequestRender();
    }
}