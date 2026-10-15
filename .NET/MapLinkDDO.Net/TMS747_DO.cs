using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace CSharpDDOSample
{
  class TMS747_DO : AircraftDO
  {

#if GDI_PLUS_DRAWING

    protected override Color foregroundColour
    {
      get 
      {
        return Color.FromArgb(240, 240, 240);
      }
    }

    protected override Color backgroundColour
    {
      get 
      {
        return Color.FromArgb(51, 153, 255);
      }
    }

    protected override Point[] points
    {
      get 
      {
        return (Point[])pts.Clone();
      }
    }


    private static Point [] pts = { new Point( -10,  10 ),
                                    new Point(  10,  10 ),
                                    new Point(   9,  -1 ),
                                    new Point(   8,  -4 ),
                                    new Point(   6,  -7 ),
                                    new Point(   4,  -9 ),
                                    new Point(   1, -10 ),
                                    new Point(  -1, -10 ),
                                    new Point(  -4,  -9 ),
                                    new Point(  -6,  -7 ),
                                    new Point(  -8,  -4 ),
                                    new Point(  -9,  -1 )
                                  };
#else

    protected override int foregroundColour
    {
      get
      {
        return 0x00F0F0F0;
      }
    }

    protected override int backgroundColour
    {
      get
      {
        return 0x00FF9933;
      }
    }

    protected override int[] points
    {
      get
      {
        return (int[])pts.Clone();
      }
    }


    private static int[] pts = { -10,  10,
				                          10,  10,
				                           9,  -1,
				                           8,  -4,
				                           6,  -7,
 				                           4,  -9,
				                           1, -10,
				                          -1, -10,
				                          -4,  -9,
				                          -6,  -7,
				                          -8,  -4,
				                          -9,  -1
				                       };
 #endif
  }
}
