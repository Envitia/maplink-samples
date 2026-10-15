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
      const int DEFAULT_ARROWHEAD_LENGTH_PERCENTAGE = 20;

      // Default angle of Arrowhead, defined in degrees, (range 10 - 80).
      const int DEFAULT_ARROWHEAD_ANGLE_DEG = 20;
      const int MIN_ARROWHEAD_ANGLE_DEG = 10;
      const int MAX_ARROWHEAD_ANGLE_DEG = 80;

      /*
       * Create an TSLEntitySet containing the arrow head.
       *
       * The arrow head is defined by 
       *
       * /code
       *
       *                                      ptH1
       *                                       \
       *                                        \
       *                                         \
       *   PT2____________________________ptHm_|__\  PT1
       *                                          /
       *                                         /
       *                                        /
       *                                       /
       *                                      ptH2
       *
       * /endcode
       * 
       * ptH1, ptH2 and ptHm will be calculated.
       *
       *
       * The arrowhead is in the plane defined by the two points, PT1 and PT2.
       * The arrowhead length is by default a percentage of the length of the line described by the two points.
       * Length defaults to 20%. Length can be specified as a percentage.
       * The angle (between the arrow arm and stem) is defaulted to 20 degrees.
       * Arrowhead angle can be specified in degrees (range 10 - 80).
       * 
       * @param positions Controls points that define the orientation of the graphic. Two points
       * need to be specified.
      */
      class simpleArrow
      {
        public:

          simpleArrow(){}

          virtual ~simpleArrow(){}


          /*
          * Create Arrowhead
          *
          * @param positions A coordinate set containing the two points PT1 and PT2.
          * @param length Percentage of the length of the line described by PT1 and PT2 to 
          * define the arrowhead length. If omitted, defaults to 20%.
          * @param angle The angle defined by the arrow head, specified in degrees.
          * Permitted range is 10 -170 degrees. If omitted, defaults to 70 degrees.
          * @return A TSLEntitySet containing the polyline that defines the arrowhead. 
          * Returns NULL on error.
          */
          virtual TSLEntitySet* createArrow( const TSLCoordSet &positions, int length = 0, int angle = 0 );


        protected:

          virtual bool checkParameters( const TSLCoordSet &positions, int& length, int& angle );

          virtual bool calculatePoints( const TSLCoordSet &positions, int length = 0, int angle = 0 );

          virtual bool addPoints( TSLCoordSet &arrowPoints );

          virtual bool createArrowEntity( TSLEntitySet* group, TSLCoordSet& positions );

        protected:
          // Member variables 
   
          /*
           *                                      ptH1
           *                                       \
           *                                        \
           *                                         \
           *   PT2____________________________ptHm_|__\  PT1
           *                                          /
           *                                         /
           *                                        /
           *                                       /
           *                                      ptH2
           */  
          TSLCoord m_ptH1;
          TSLCoord m_ptHm;
          TSLCoord m_ptH2;
          TSLCoord m_pt2;
          TSLCoord m_pt1;


        private:

      };



    } // end TacticalGraphics namespace
  } // end MapLink namespace
} // end Envitia namespace