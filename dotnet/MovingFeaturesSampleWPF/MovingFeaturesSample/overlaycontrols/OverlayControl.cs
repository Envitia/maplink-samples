using System;
using System.Windows.Interop;
using System.Windows.Controls;

namespace MovingFeaturesSample
{
  /// <summary>
  /// MapLink does not yet have a WPF-native drawing surface, so we need to wrap the Windows.Forms drawing surface in a WindowsFormHost.
  /// This approach, however, means that we cannot simply overlay WPF controls on the drawing surface, due to the "airspace issue":
  /// https://learn.microsoft.com/en-us/dotnet/desktop/wpf/advanced/technology-regions-overview?view=netframeworkdesktop-4.8
  /// We solve this with the insertion of the OverlayControl, a solution devised by Saurabh Singh (https://www.codeproject.com/Tips/5326355/Airspace-Solution).
  /// OverlayControl is a wrapper control where you can provide content to render on top of a WinformHost.
  /// This container injects another WPF window (OverlayWindow) into the region supplying its own content.
  /// Extended HwndHost is used to glue WPF window into OverlayControl.
  /// </summary>
  internal class OverlayControl : ContentControl, IDisposable
  {
    OverlayWindow wpfInjection;
    HwndHostEx windowsFormsHost;

    public OverlayControl()
    {
      Loaded += OverlayControl_Loaded;
    }

    private void OverlayControl_Loaded(object sender, System.Windows.RoutedEventArgs e)
    {
      var content = Content;

      // Inject the WPF window
      wpfInjection = new OverlayWindow();
      wpfInjection.Content = content;
      wpfInjection.Show();

      // 
      IntPtr windowHandle = new WindowInteropHelper(wpfInjection).Handle;

      windowsFormsHost = new HwndHostEx(windowHandle);
      Content = windowsFormsHost;
    }

    public void Dispose()
    {
      windowsFormsHost?.Dispose();
      wpfInjection?.Close();
    }
  }
}
