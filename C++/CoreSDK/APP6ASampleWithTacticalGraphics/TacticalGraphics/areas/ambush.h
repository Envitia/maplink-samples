/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright 2014 Envitia PLC
* http://www.envitia.com
*
*/
#pragma once

#include "attackByFirePosition.h"
using namespace std;

namespace envitia { namespace MapLink { namespace TacticalGraphics {




      /*
       * Create an TSLEntitySet containing the Ambush Tactical Graphic.
       *
       * A surprise attack by fire from concealed positions on a moving or 
       * temporarily halted enemy.
       *
       * 2.X.2.1.6.1.2
       *
       * Symbol Identification Code (SIDC)
       * G*C*MSGA--*****
       *
       * The Ambush Tactical Graphic is defined by 
       *
       * /code
       * 
       *           PT2
       *           ---\                          
       *            ---\                       
       *             ---\                    
       *             --- ------------------->PT1
       *             ---/                    
       *            ---/                       
       *           ---/                         
       *            PT3
       *
       * /endcode
       * 
       * where PT2 to PT3 is an ARC rather the straight lines shown.
       *
       * Anchor Points
       * The graphic requires three anchor points.
       * PT1 defines the tip of the arrowhead.
       * PT2 and PT3 define the endpoints of the curved line (Arc) on the backside of the graphic.
       *
       * Size/Shape
       * PT2 and PT3 determine the length of the curved line on the backside of the graphic.
       * The rear of the arrow should connect to the midpoint of the line between points PT2 and PT3.
       *
       * Orientation
       * Orientation is determined by the anchor points. The back side of the graphic 
       * encompasses the ambush position with the arrowhead shaft positioned at the centre of mass, while
       * the arrowhead points in the direction of fire.
       *
       * Arrowheads
       * The arrowhead is in the plane defined by PT1 and the midpoint of the curved line PT2 and PT3.
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
      class ambush : public attackByFirePosition
      {
        public:

          ambush() : attackByFirePosition(){}

          virtual ~ambush(){}
          
          /*
          * create
          *
          * @param positions A coordinate set containing the three anchor points PT1, PT2 and PT3.
          * @param lengthArrowhead Percentage of the length of the line described by PT1 and the midpoint of the curved line PT2 - PT3 to 
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


        private:

          bool findPointOnLine( const TSLCoord& startPt, const TSLCoord& endPoint, TSLCoord& outPoint, int distance );
          int findLengthOfLine( const TSLCoord& pt1, const TSLCoord& pt2 ) const;

//          TSLArc* createArc( const TSLCoordSet &anchorPoints, const TSLCoord& midPointAnchorLine );
          TSLEntitySet* createArc( const TSLCoordSet &anchorPoints, const TSLCoord& midPointAnchorLine, TSLCoordSet& arrowPts);

      };



    } // end TacticalGraphics namespace
  } // end MapLink namespace
} // end Envitia namespace