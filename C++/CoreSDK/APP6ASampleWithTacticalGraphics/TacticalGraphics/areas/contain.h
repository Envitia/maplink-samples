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

      // Default length of spokes, defined as a percentage of length of line formed by the 2 specified points (radius)
      // Default percentage is 30%.
      const int DEFAULT_SPOKE_PERCENTAGE = 30;

      // Number of Spokes
      // If this changes code will break as angle increment is hardcoded as 30 degrees PI/6 to give 7 spokes
      const int NUMBER_OF_SPOKES = 7;

      /*
       * Create an TSLEntitySet containing the Contain Tactical Graphic.
       * 
       * To stop, hold or surround the forces of the enemy or to cause the enemy to centre activity on
       * a given front and to prevent his withdrawing any part of his forces for use elsewhere.      
       * (Army)--A tactical task to restrict enemy movement.                                                    
       *
       * 2.x.1.1.6
       *
       * Symbol Identification Code (SIDC)
       * G*T*GJ----*****
       *
       * Anchor Points
       * The graphic requires two anchor points.
       * PT1 defines the centre of the graphic and PT2 defines
       * the graphics startpoint of the semicircle and radius.
       * The semicircle is drawn clockwise from PT2.
       *
       * Size/Shape
       * The radius will be long enough for the graphic to encompass the area where enemy forces
       * are to be contained.
       *
       * Orientation
       * The opening typically faces the enemy forces.
       *
       * Legs
       * The leg length defaults to a percentage of the radius.
       * Default percentage is 30%.
       *
       * @param positions Controls points that define the orientation of the graphic. Two points
       * need to be specified.
      */
      class contain
      {
        public:

          contain(){}

          virtual ~contain(){}
          
          /*
          * create
          *
          * @param positions A coordinate set containing the two anchor points PT1 and PT2.
          * @param lengthLegs Percentage of the length of the base line described by PT2 and PT3 to 
          * define the leg length. If omitted or zero, defaults to 25%.
          * @return A TSLEntitySet containing the polylines that define the graphic. 
          * Returns NULL on error.
          */
          virtual TSLEntitySet* create( const TSLCoordSet &positions, int lengthLegs = 0 );


        protected:

          virtual bool checkParameters( const TSLCoordSet &positions, int& lengthLegs );

          int determineQuadrant( const TSLCoord& pt1, const TSLCoord& pt2 );


        private:

          TSLEntitySet* createSpokes( const TSLCoord& centre, const TSLCoord& startOfArc, int radius, double startAngle, double endAngle, int lengthLegs );
          bool findPointOnLine( const TSLCoord& startPt, const TSLCoord& endPoint, TSLCoord& outPoint, int percentage );
          bool findPointOnSemicircle( const TSLCoord& startPt, int radius, const double angle, TSLCoord& outPoint );
      };



    } // end TacticalGraphics namespace
  } // end MapLink namespace
} // end Envitia namespace