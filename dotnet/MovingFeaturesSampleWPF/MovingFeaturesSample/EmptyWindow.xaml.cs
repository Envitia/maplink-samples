using System.Windows;

namespace ContourApp
{
  /// <summary>
  /// Interaction logic for Window1.xaml
  /// </summary>
  public partial class EmptyWindow : Window
  {
    public DockableWindow Dockable { get; private set; }

    public EmptyWindow(DockableWindow dockable)
    {
      InitializeComponent();

      Dockable = dockable;
      Dockable.Window = this;
    }

    private void Minimise_Click(object sender, RoutedEventArgs e)
    {
      Dockable.RaiseMinimiseEvent();
    }

    private void Maximise_Click(object sender, RoutedEventArgs e)
    {
      Dockable.RaiseMaximiseEvent();
    }
  }
}
