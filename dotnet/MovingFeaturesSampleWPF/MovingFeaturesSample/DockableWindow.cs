using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MovingFeaturesSample
{
  public class DockableWindowEventArgs
  {
    public enum EventType
    {
      Minimise,
      Maximise,
      Restore,
      Dock
    };
    public EventType Type { get; set; }
  }

  public class DockableWindow
  {
    public System.Windows.Controls.Control Minimised { get; set; }
    public System.Windows.Window Window { get; set; }

    public System.Windows.Window OwnerBackup { get; set; }
    public System.Windows.UIElement ContentBackup { get; set; }

    public delegate void DockableWindowEvent(object sender, DockableWindowEventArgs e);

    public event DockableWindowEvent MaximiseEvent;
    public event DockableWindowEvent MinimiseEvent;
    public event DockableWindowEvent RestoreEvent;
    public event DockableWindowEvent DockEvent;

    public void RaiseMaximiseEvent()
    {
      MaximiseEvent?.Invoke(this, new DockableWindowEventArgs { Type = DockableWindowEventArgs.EventType.Maximise });
    }

    public void RaiseMinimiseEvent()
    {
      MinimiseEvent?.Invoke(this, new DockableWindowEventArgs { Type = DockableWindowEventArgs.EventType.Minimise });
    }

    public void RaiseRestoreEvent()
    {
      RestoreEvent?.Invoke(this, new DockableWindowEventArgs { Type = DockableWindowEventArgs.EventType.Restore });
    }

    public void RaiseDockEvent()
    {
      DockEvent?.Invoke(this, new DockableWindowEventArgs { Type = DockableWindowEventArgs.EventType.Dock });
    }
  }
}
