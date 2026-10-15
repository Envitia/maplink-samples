/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
*/
#pragma once

#include "attackByFirePosition.h"
using namespace std;

namespace envitia { namespace MapLink { namespace TacticalGraphics {




      /*
       * Create an TSLEntitySet containing the Support By Fire Position Tactical Graphic.
       *
       * A tactical task by which a manoeuvre element moves to a position on the battlefield
       * where it can engage the enemy by direct fire. The manoeuvre element does not attempt
       * to manoeuvre to capture enemy forces or terrain.
       *
       * 2.x.2.1.5.3.4
       *
       * Symbol Identification Code (SIDC)
       * G*C*MOOS--*****
       *
       * The Support By Fire Position Tactical Graphic is defined by 
       *
       * /code
       * 
       *           PT3 _                    PT4_
       *             |\                        /|
       *               \                      /
       *                \                    /
       *             PT1 -------------------- PT2
       *                /                    \
       *               /                      \
       *              /                        \
       *            PTlb                      PTrb 
       *
       * /endcode
       * 
       * PTm is the midpoint of the line between PT1 and PT2 (calculated).
       * PTlb and PTrb are the left and right base points of the lines (legs) that encompass the 
       * firing position (calculated).
       *
       * Anchor Points
       * The graphic requires four anchor points.
       * PT1 and PT2 define the endpoints of the straight line on the backside of the graphic.
       * PT3 and PT4 define the tips of the arrowheads.
       *
       * Size/Shape
       * PT1 and PT2 determine the length of the straight line on the backside of the graphic.
       * The rear of the arrows should connect to PT1 and PT2.
       *
       * Orientation
       * Orientation is determined by the anchor points. The back side of the graphic 
       * encompasses the firing position, while the arrowheads typically indicate the arc of coverage
       * that the firing position is meant to support.
       *
       * Arrowheads
       * The arrowhead is in the plane defined by the two points, PT3 and PT1, PT4 and PT2.
       * The arrowhead length is by default a percentage of the length of the line described by these two points.
       * Length defaults to 20%. Length can be specified as a percentage.
       * The angle (between arrow arm and stem) defaulted to 20 degrees.
       * Arrowhead angle can be specified in degrees (range 10 - 80).
       *
       * Legs
       * The leg length defaults to a percentage of the distance between the control points that define the base.
       * Default percentage is 25%.
       *
       * @param positions Controls points that define the orientation of the graphic. Two points
       * need to be specified.
      */
      class supportByFirePosition : public attackByFirePosition
      {
        public:

          supportByFirePosition() : attackByFirePosition(){}

          virtual ~supportByFirePosition(){}
          
          /*
          * create
          *
          * @param positions A coordinate set containing the four anchor points PT1, PT2, PT3 and PT4.
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

        private:

      };



    } // end TacticalGraphics namespace
  } // end MapLink namespace
} // end Envitia namespace