/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
*/
#pragma once

#include "../utilities/simpleArrow.h"
using namespace std;

namespace envitia { namespace MapLink { namespace TacticalGraphics {

      /*
       * Create an TSLEntitySet containing the solid arrow head.
       *
       * The arrow head is defined by 
       *
       * /code
       *
       *                                      ptH1
       *                                      |\
       *                                      | \
       *                                      |  \
       *   PT2___________________________ptHm_|_ _\  PT1
       *                                      |   /
       *                                      |  /
       *                                      | /
       *                                      |/
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
       * The angle defined by the arrow head is defaulted to 70 degrees.
       * Arrowhead angle can be specified in degrees (range 10 - 170).
       * 
       * @param positions Controls points that define the orientation of the graphic. Two points
       * need to be specified.
      */
      class solidSimpleArrow : public simpleArrow
      {
        public:

          solidSimpleArrow() : simpleArrow(){}

          virtual ~solidSimpleArrow(){}

        protected:

          virtual bool addPoints( TSLCoordSet &arrowPoints );
          virtual bool createArrowEntity( TSLEntitySet* group, TSLCoordSet& positions );

        private:

      };



    } // end TacticalGraphics namespace
  } // end MapLink namespace
} // end Envitia namespace