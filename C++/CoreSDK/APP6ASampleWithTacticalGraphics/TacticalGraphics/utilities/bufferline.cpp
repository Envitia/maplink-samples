/* -*-c++-*- */
/* BufferLine
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#define _USE_MATH_DEFINES
#include <cmath>

#include "MapLink.h"
#include "bufferline.h"
#include "vector3d.h"

#include <list>


namespace envitia { namespace MapLink { namespace GeometryOperations {
  
  
  bool BufferLine::simpleBuffer(const TSLCoord &pt1, const TSLCoordSet &positions, double width, std::list<TSLCoord> &leftCS, std::list<TSLCoord> &rightCS)
  {
    const int size = positions.size(); 
    if (size < 1)
    {
      return false;
    }
    math::Vector3D a = positions[0]; // next
    math::Vector3D b = pt1; // current
    math::Vector3D c; // previous
    int index = 1;    // start point... we have next and current
    math::Vector3D offset;

    const math::Vector3D normal(0, 0, 1); // z- axis

    /*
                       + b



                                      + c
        +
        a

        a = next     = 2  (arrow tail 1)
        b = current  = 1  (arrow tail 0)
        c = previous = 0  (arrow head)
    */

    double expandTailMult = 1;
    while (index < (size+1))
    {
      c = b;
      b = a;

      if (index < size)
      {
        a = positions[index];  // next
      }
      else
      {
        // at the end... I need 3 points so we repeat the last point.
        a = positions[size-1]; // repeat last point.
        if (m_expandTail)
        {
          expandTailMult = m_tailExpansion;
        }
      }

      /*
      
                                       perpendicular
                                     +                                
                       + b      
                               
                               +
                               
                                      + c
          
      */

      math::Vector3D vectorBC = c - b;  // vector between c and b
      math::Vector3D perpendicular = vectorBC.crossProduct(normal); // vector perpendicular to the line c to b

      // If the points a, b, and c form a line which is collinear with vector vBC
      // then the points are on a straight line so in that case we just use
      // the perpendicular vector for the offset vector.
      //
      // If the points are not collinear we need to calculate the 
      // vector between a and b (vBA).
      //
      //   When we add these two vectors we get a vector which is half-way between 
      //   the vBA and vBC. This is the direction vector that the offset lies upon.
      //
      //   Depending on which directions the vectors vBA and vBC point we 
      //   need to figure out if the offset is to the left of the arrow head
      //   or to the right of the arrow head.
      //
      // If we are at the end of the arrow tail then we just use the 
      // perpendicular vector for the offset direction.
      //
      if (index != size && !vectorBC.colinear(a, b, c))
      {
        // last point or the three points are not collinear between c and b.
        math::Vector3D vectorBA = a - b;  // vector between a and b.
        vectorBA.normalise();
        vectorBC.normalise();

        offset = vectorBA + vectorBC;
        offset.normalise();
        // Work out left and right side
        //
        // tripleProduct = (a x b).c
        //
        //  a == vBA
        //  b == vBC
        //  c == perpendicular
        if (perpendicular.dotProduct(offset) < 0)
          offset = offset * -1;
      }
      else
      {
        // the three points are collinear to the vector between c and b so
        // the offset is perpendicular to the line.
        offset = perpendicular;
        offset.normalise();
      }

      // work out the angle between the vector vBC and the offset vector.
      double angle = vectorBC.angleBetween(offset);

      // Basic trig' to get the actual distance along the unit offset vector.
      double len = (width * expandTailMult) / sin(angle);

      // extend the offset vector by the actual length calculated.
      offset *= len;

      // position the offset vector to the left and right of the current point.
      math::Vector3D right = b + offset;
      math::Vector3D left = b - offset;

      leftCS.push_front(TSLCoord(left.m_x, left.m_y));
      rightCS.push_back(TSLCoord(right.m_x, right.m_y));

      ++index ;
    }
    return true;

  }


}; }; };