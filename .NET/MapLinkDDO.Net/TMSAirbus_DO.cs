using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace CSharpDDOSample
{
  class TMSAirbus_DO : AircraftDO
  {
#if GDI_PLUS_DRAWING

    protected override Color foregroundColour
    {
      get
      {
        return Color.Black;
      }
    }

    protected override Color backgroundColour
    {
      get
      {
        return Color.Red;
      }
    }

    protected override Point[] points
    {
      get
      {
        return (Point[]) pts.Clone();
      }
    }


    private static Point[] pts = { new Point(   0, -10 ),
                                   new Point(   2,  -8 ),
                                   new Point(   3,  -4 ),
                                   new Point(   8,  -1 ),
                                   new Point(   9,   1 ),
                                   new Point(   2,   2 ),
                                   new Point(   2,   7 ),
                                   new Point(   5,   8 ),
                                   new Point(   5,   9 ),
                                   new Point(   0,  10 ),
                                   new Point(  -2,   7 ),
                                   new Point(  -2,   2 ),
                                   new Point(  -9,   1 ),
                                   new Point(  -8,  -1 ),
                                   new Point(  -3,  -4 ),
                                   new Point(  -2,  -8 )
                                  };
#else

    protected override int foregroundColour
    {
      get
      {
        return 0x00000000;
      }
    }

    protected override int backgroundColour
    {
      get
      {
        return 0x000000FF;
      }
    }

    protected override int[] points
    {
      get
      {
        return (int[])pts.Clone();
      }
    }


    private static int[] pts = {    0, -10,
				                            2,  -8,
				                            3,  -4,
				                            8,  -1,
				                            9,   1,
				                            2,   2,
				                            2,   7,
				                            5,   8,
				                            5,   9,
				                            0,  10,
				                           -2,   7,
				                           -2,   2,
				                           -9,   1,
				                           -8,  -1,
				                           -3,  -4,
				                           -2,  -8
				                          };
#endif
  }
}
