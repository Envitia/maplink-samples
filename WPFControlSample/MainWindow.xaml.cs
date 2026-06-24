/****************************************************************************
Copyright (c) 1991-2026 Envitia Group PLC
=============================================================================
DESCRIPTION     : This contains a simple example of using the MapLink WPF control.
****************************************************************************/

using System.Windows;

namespace WPFControlSample;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
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
}
