using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.InteractionModes;
using Envitia.MapLink.DDO;

namespace CSharpDDOSample
{
  // A MapLink interaction mode that performs a simple pick operation.
  // The following is required for this to function correctly:
  // - The drawing surface should contain a ddo layer
  // - The ddo layer should contain display objects
  // - The display object should override the pick method from TSLNDisplayObject
  class PickInteractionMode : TSLNInteractionMode
  {
    public PickInteractionMode(int id, bool middleButtonPansToPoint)
      : base(id, middleButtonPansToPoint)
    {

    }

    public override bool onLButtonDown(int x, int y, bool shift, bool control)
    {
      if (this.display != null && this.display.drawingSurface != null)
      {
        TSLNDrawingSurface ds = this.display.drawingSurface;

        // Pick
        //
        // The x and y coordinates are in display units i.e the pixel coordinate on the screen that was clicked.
        //
        // The aperture size defines how close to an object you need to click.
        // If it's too big this method will have to do a lot of filtering on the results.
        // This will need to be tailored for the specific use case.
        //
        // The depth parameter is set to -1 as we want to pick individual entities, 
        // if it's not then a TSLEntitySet or group of objects may be returned in the 
        // pick results.
        TSLNPickResultSet results = ds.pick(x, y, 50, -1);

        // Convert the pick position to tmc for comparisons
        int tmcX;
        int tmcY;
        if (!ds.DUToTMC(x, y, out tmcX, out tmcY))
        {
          return false;
        }

        AircraftDO closest = null;

        for( int i = 0; i < results.numResults; i++ )
        {
          TSLNPickResult res = results[i];
          // Filter results to just Display Objects
          // Optionally could use the data layer name or DO-specific parameters
          if (res.GetType() == typeof(TSLNPickResultDisplayObject) )
          {
            TSLNPickResultDisplayObject doRes = (TSLNPickResultDisplayObject)res;
            AircraftDO DO = (AircraftDO)doRes.getDisplayObject();
            if (closest == null)
            {
              closest = DO;
            }
            else
            {
              // In this case we only want to consider visible objects
              if( DO.visibility == true )
              {
                double xDelta = DO.position.x - tmcX;
                double yDelta = DO.position.y - tmcY;
                double distance = Math.Sqrt((xDelta * xDelta) + (yDelta * yDelta));

                double closestXDelta = closest.position.x - tmcX;
                double closestYDelta = closest.position.y - tmcY;
                double closestDistance = Math.Sqrt((closestXDelta * closestXDelta) + (closestYDelta * closestYDelta));

                if (distance <= closestDistance)
                {
                  closest = DO;
                }
              }
            }
          }
        }

        // Do something to the picked object
        if (closest != null)
        {
          closest.visibility = false;
          return true;
        }
      }
      return false;
    }
  }
}
