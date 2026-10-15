/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
*/
#pragma once

#include "MapLink.h"
using namespace std;

namespace envitia { namespace MapLink { namespace TacticalGraphics {


      // Default length of Arrowhead, defined as a percentage of length of line formed by the 2 specified points
      // Default length of the legs of the graphics base. A percentage of the distance between the control points that define the base.
      // Default percentage is 25%.
      const int DEFAULT_LEG_LENGTH_PERCENTAGE = 25;


      /*
       * Create an TSLEntitySet containing the Attack By Fire Position Tactical Graphic.
       * 
       * Fires employed to destroy the enemy from a distance, normally used when the mission does not
       * dictate or support occupation of the objective. This task is usually given to the supporting
       * element during the offensive and as a counterattack option for the reserve during defensive operations.
       *
       * 2.x.2.1.5.3.3
       *
       * Symbol Identification Code (SIDC)
       * G*C*MOOP--*****
       *
       * The Attack By Fire Position Tactical Graphic is defined by 
       *
       * /code
       * 
       *                          PT1
       *                          /|\
       *                           |
       *                PT2        |      PT3
       *                 --------------------
       *                /         PTm        \
       *               /                      \
       *              /                        \
       *            PTlb                      PTrb 
       *
       * /endcode
       * 
       * PTm is the midpoint of the line between PT2 and PT3 (calculated).
       * PTlb and PTrb are the left and right base points of the lines (legs) that encompass the 
       * firing position (calculated).
       *
       * Anchor Points
       * The graphic requires three anchor points.
       * PT1 is the tip of the arrowhead. PT2 and PT3 define
       * the endpoints of the straight line on the backside of the graphic.
       *
       * Size/Shape
       * PT2 and PT3 determine the length of the straight line on the backside of the graphic.
       * The rear of the arrow should connect to the midpoint of the line between PT2 and PT3.
       *
       * Orientation
       * Orientation is determined by the anchor points. The back side of the graphic 
       * encompasses the firing position, while the arrowhead typically points at the target.
       *
       * Arrowhead
       * The arrowhead is in the plane defined by the two points, PT1 and PTm.
       * The arrowhead length is by default a percentage of the length of the line described by these two points.
       * Length defaults to 20%. Length can be specified as a percentage.
       * The angle (between the arrow arm and stem) is defaulted to 20 degrees.
       * Arrowhead angle can be specified in degrees (range 10 - 80).
       *
       * Legs
       * The leg length defaults to a percentage of the distance between the control points that define the base.
       * Default percentage is 25%.
       *
       * @param positions Controls points that define the orientation of the graphic. Two points
       * need to be specified.
      */
      class attackByFirePosition
      {
        public:

          attackByFirePosition(){}

          virtual ~attackByFirePosition(){}
          
          /*
          * create
          *
          * @param positions A coordinate set containing the three anchor points PT1, PT2 and PT3.
          * @param lengthArrowhead Percentage of the length of the line described by PT1 and PT2 to 
          * define the arrowhead length. If omitted or zero, defaults to 20%.
          * @param angleArrowhead The angle defined by the arrow head, specified in degrees.
          * Permitted range is 10 -170 degrees. If omitted or zero, defaults to 70 degrees.
          * @param lengthLegs Percentage of the length of the base line described by PT2 and PT3 to 
          * define the leg length. If omitted or zero, defaults to 25%.
          * @return A TSLEntitySet containing the polylines that define the graphic. 
          * Returns NULL on error.
          */
          virtual TSLEntitySet* create( const TSLCoordSet &positions, int lengthArrowhead = 0, int angleArrowhead = 0, int lengthLegs = 0 );


        protected:

          virtual TSLPolyline* calculateFiringPositionLine( const TSLCoordSet &positions, int lengthLegs );

          virtual bool checkParameters( const TSLCoordSet &positions, int& length, int& angle, int& lengthLegs );

          virtual TSLEntitySet* createArrowheadSet( const TSLCoordSet& coords, const int length, const int angle );


        private:

      };



    } // end TacticalGraphics namespace
  } // end MapLink namespace
} // end Envitia namespace