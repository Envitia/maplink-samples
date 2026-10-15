using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DrawingSurfacePanel
{
  public interface IPanel
  {
    void SafeInvalidate();
    Envitia.MapLink.TSLN2DDrawingSurface GetDrawingSurface();
  }
}
