/****************************************************************************
Copyright (c) 1991-2026 Envitia Group PLC
=============================================================================
DESCRIPTION     : This contains a simple example of using the MapLink WPF control.
****************************************************************************/

using System.Windows;
using System.Windows.Threading;

namespace MovingFeaturesWPFSample;

public partial class MainWindow : Window
{
  private DispatcherTimer? _countdownTimer;
  private int _countdownValue;

  public MainWindow()
  {
    InitializeComponent();
    StartCountdown();
  }

  private void StartCountdown()
  {
    _countdownValue = 15;
    CountdownText.Text = _countdownValue.ToString();
    CountdownText.Visibility = Visibility.Visible;

    _countdownTimer = new DispatcherTimer
    {
      Interval = TimeSpan.FromSeconds(1)
    };
    _countdownTimer.Tick += CountdownTimer_Tick;
    _countdownTimer.Start();
  }

  private void CountdownTimer_Tick(object? sender, EventArgs e)
  {
    _countdownValue--;

    if (_countdownValue > 0)
    {
      CountdownText.Text = _countdownValue.ToString();
    }
    else
    {
      // Countdown reached zero
      _countdownTimer?.Stop();
      CountdownText.Visibility = Visibility.Collapsed;

      if (MapControl.DrawingSurface == null)
      {
        MessageBox.Show("Drawing surface is not initialized.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        return;
      }

      StartDdos();
      StartTracks();
    }

    MapControl.RequestRender();
  }

  private void StartTracks()
  {
    var tracksLayer = new Tracks.TracksLayer();

    try
    {
      tracksLayer.Start(MapControl.DrawingSurface, () => MapControl.RequestRender());
    }
    catch (Exception ex)
    {
      MessageBox.Show($"Error configuring tracks layer: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
    }    
  }

  private void StartDdos()
  {
    var ddoLayer = new ddo.DdoLayer();

    try
    {
      ddoLayer.Start(MapControl.DrawingSurface);
    }
    catch (Exception ex)
    {
      MessageBox.Show($"Error configuring tracks layer: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
    }
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
