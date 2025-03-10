using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MovingFeaturesSample
{
  public class DockManager
  {
    public System.Windows.Controls.DockPanel DockPanel { get; set; }
    public enum DockLocation
    {
      Left,
      Right,
      Top,
      Bottom
    };
    public DockLocation Location { get; set; }

    public class GridLocation
    {
      public System.Windows.Controls.Grid Grid { get; set; }
      public int X { get; set; }
      public int Y { get; set; }
    }

    public GridLocation MaximiseGridLocation { get; set; }
    public GridLocation FloatGridLocation { get; set; }

    public System.Windows.Window MainWindow { get => System.Windows.Application.Current.MainWindow; }

    public DockableWindow Floating { get; private set; }
    public DockableWindow Maximised { get; private set; }
    private List<DockableWindow> Windows { get; } = new List<DockableWindow>();

    public void AddWindow(DockableWindow window)
    {
      Windows.Add(window);

      window.DockEvent += (o, e) =>
      {
        Dock((DockableWindow)o);
      };
      window.MaximiseEvent += (o, e) =>
      {
        Maximise((DockableWindow)o);
      };
      window.MinimiseEvent += (o, e) =>
      {
        Minimise((DockableWindow)o);
      };
      window.RestoreEvent += (o, e) =>
      {
        Float((DockableWindow)o);
      };
    }

    private static void RemoveChild(DockableWindow window, System.Windows.Controls.UIElementCollection children)
    {
      if (window.ContentBackup == null)
      {
        return;
      }

      int index = children.IndexOf(window.ContentBackup);
      if (index < 0)
      {
        return;
      }

      children.RemoveAt(index);

      window.Window.Content = window.ContentBackup;
    }

    private void Undock(DockableWindow window)
    {
      RemoveChild(window, DockPanel.Children);
    }

    private void RemoveFromGrid(DockableWindow window, System.Windows.Controls.Grid grid)
    {
      RemoveChild(window, grid.Children);
    }

    private void FillGridCell(DockableWindow window, GridLocation cell)
    {
      if (cell == null)
      {
        throw new NullReferenceException("No grid cell");
      }

      window.Minimised.Visibility = System.Windows.Visibility.Hidden;
      window.Window.Hide();

      if (window.ContentBackup == null)
      {
        window.ContentBackup = (System.Windows.UIElement)window.Window.Content;
      }

      window.Window.Content = null;
      System.Windows.Controls.Grid.SetColumn(window.ContentBackup, cell.X);
      System.Windows.Controls.Grid.SetRow(window.ContentBackup, cell.Y);
      cell.Grid.Children.Add(window.ContentBackup);
    }

    public void Float(DockableWindow window)
    {
      Undock(window);

      if (Floating != null)
      {
        Minimise(Floating);
      }
      Floating = null;

      if (Maximised != null)
      {
        Minimise(Maximised);
      }
      Maximised = null;

      if (FloatGridLocation == null)
      {
        throw new NullReferenceException("No float grid");
      }

      Floating = window;
      FillGridCell(Floating, FloatGridLocation);
    }

    public void Maximise(DockableWindow window)
    {
      Undock(window);

      if (Floating != null)
      {
        Minimise(Floating);
      }
      Floating = null;

      if (Maximised != null)
      {
        Minimise(Maximised);
      }

      if (MaximiseGridLocation == null)
      {
        throw new NullReferenceException("No maximise grid");
      }

      Maximised = window;
      FillGridCell(Maximised, MaximiseGridLocation);
    }

    public void Minimise(DockableWindow window)
    {
      Undock(window);
      RemoveFromGrid(window, MaximiseGridLocation.Grid);
      RemoveFromGrid(window, FloatGridLocation.Grid);

      if (window == Maximised)
      {
        Maximised = null;
      }
      if (window == Floating)
      {
        Floating = null;
      }

      window.Window.Hide();
      window.Minimised.Visibility = System.Windows.Visibility.Visible;
    }

    public void Dock(DockableWindow window)
    {
      Undock(window);
      RemoveFromGrid(window, MaximiseGridLocation.Grid);
      RemoveFromGrid(window, FloatGridLocation.Grid);

      if (DockPanel == null)
      {
        throw new NullReferenceException("No dock panel");
      }

      if (window == Maximised)
      {
        Maximised = null;
      }
      if (window == Floating)
      {
        Floating = null;
      }

      window.Minimised.Visibility = System.Windows.Visibility.Hidden;
      window.Window.Hide();

      if (window.ContentBackup == null)
      {
        window.ContentBackup = (System.Windows.UIElement)window.Window.Content;
      }
      window.Window.Content = null;

      DockPanel.Children.Add(window.ContentBackup);
    }
  }
}
