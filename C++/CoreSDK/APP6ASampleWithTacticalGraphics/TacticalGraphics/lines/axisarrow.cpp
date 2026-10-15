/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#define _USE_MATH_DEFINES
#include <cmath>

#include "MapLink.h"
#include "axisarrow.h"
#include "../utilities/vector3d.h"
#include "../utilities/spline.h"

#include <list>


namespace envitia { namespace MapLink { namespace TacticalGraphics {

  const double AxisArrow::m_tailExpansion = 1.25;

  math::Vector3D innerArrowHeadTipPosition(const math::Vector3D &pt1, const math::Vector3D &posN, const math::Vector3D &posLeft, const math::Vector3D &vector12)
  {
    /*
    * /code
    *   
    *      |\
    *      | \
    *  ---- \ \
    *        \ \
    *         \ \ 
    *         / /
    *        / /
    *       / /
    *  ----| /
    *      |/
    *
    * /endcode
    */

    math::Vector3D pLN = posN - posLeft;
    double length = pLN.length();

    const math::Vector3D xaxis(1, 0, 0);

    // work out the angle between the vector vector12 and x-axis.
    double angle = vector12.angleBetween(xaxis);

    math::Vector3D vector12N(vector12);
    vector12N.normalise();
    vector12N *= (length ); // TBD length needs to be adjusted.

    // find the end point of the arrow head that is inside the 
    // main arrow head.
    return pt1 - vector12N;
  }

  void AxisArrow::calcPTN(const TSLCoord &cPT1, const TSLCoord &cPT2, TSLCoord &cPTN) const
  {
    math::Vector3D pt1(cPT1);  // pointy bit of arrow
    math::Vector3D pt2(cPT2);  // back of arrow

    // calculate vector from pt1 to pt2
    math::Vector3D vector12 = (pt1 - pt2);

    // 1/5 of the length
    double length = (vector12.length() / 5.0);
    vector12.normalise();

    // move along the vector a 1/5 of the way.
    math::Vector3D PTN = pt1 - (vector12 * (length));

    // vector perpendicular to the vector12
    const math::Vector3D normal(0, 0, 1); // z- axis
    math::Vector3D p = normal.crossProduct(vector12);  
    p.normalise();

    // move PTN out by length
    math::Vector3D offset = p * length;
    PTN = PTN - offset;

    cPTN= TSLCoord(PTN.m_x, PTN.m_y);
  }

  TSLEntitySet *AxisArrow::createArrow(const TSLCoord &cPT1, const TSLCoord &cPT2, bool buffered)
  {
    TSLCoordSet *s = new TSLCoordSet;

    // add in end point - line buffering needs at least one point.
    s->add(cPT2);

    TSLCoord cPTN;
    calcPTN(cPT1, cPT2, cPTN);

    TSLEntitySet *r = createArrow(cPT1, cPT2, cPTN, *s, false, buffered);

    s->destroy();
    return r;
  }


  TSLEntitySet *AxisArrow::createArrow(const TSLCoordSet &positions, bool spline, bool buffered)
  {
    int size = positions.size();
    if (size == 2)
    {
      // Convoy Arrows.
      const TSLCoord &cPT1 = positions[0];
      const TSLCoord &cPT2 = positions[1];
      return createArrow(cPT1, cPT2, true);
    }
    if (size < 3)
    {
      // not supported... insufficient points.
      return NULL;
    }

    TSLCoordSet *s = new TSLCoordSet;

    s->reserve(positions.size());
    const TSLCoord &cPT1 = positions[0];
    const TSLCoord &cPT2 = positions[1];
    TSLCoord cPTN;
    // TBD - needs to be reworked
    if (!buffered)
    {
      -- size;
      calcPTN(cPT1, cPT2, cPTN);
    }
    else
    {
      size -= 2;
      cPTN = positions.back();
    }

    for (int i = 1, j = 0; j < size; ++i, ++j)
    {
      s->add(positions[i]);
    }

    TSLEntitySet *r = createArrow(cPT1, cPT2, cPTN, *s, spline, buffered);

    s->destroy();
    return r;
  }



  TSLEntitySet *AxisArrow::createArrow(const TSLCoord &cPT1, const TSLCoord &cPT2, const TSLCoord &cPTN, const TSLCoordSet &positions, bool spline, bool buffered)
  {
    const int size = positions.size(); 
    if (size < 1)
    {
      return NULL;
    }

    math::Vector3D pt1(cPT1);  // pointy bit of arrow
    math::Vector3D pt2(cPT2);  // back of arrow
    math::Vector3D ptN(cPTN);  // width of arrow head

    // calculate vector from pt1 to pt2
    math::Vector3D vector12 = (pt1 - pt2);
    vector12.normalise();

    /*
    * /code
    *                                    ptN
    *                                     |\
    *                                     | \
    *   ----------------------------------|  \
    *                                         \
    *                     pt2          pt1P    \  pt1
    *                                          /
    *                                         /
    *   ----------------------------------|  /
    *                                     | /
    *                                     |/
    *                                     ptNP
    * /endcode
    */
    // Calculate where the opposite point to ptN is
    math::Vector3D ptT = (ptN - pt1);
    double mult = ptT.dotProduct(vector12);

    math::Vector3D pt1P = pt1 + (vector12 * mult);
    math::Vector3D ptNP = pt1P - (ptN - pt1P);

    // store back of arrow head
    m_arrowHeadBack = TSLCoord(pt1P.m_x, pt1P.m_y);

    /*
    * /code
    *                                    ptN    ptA
    *                                     |\
    *                                     | \
    *   ----------------------------------|  \
    *                                         \
    *                     pt2          pt1P    \  pt1
    *                                          /
    *                                         /
    *   ----------------------------------|  /
    *                                     | /
    *                                     |/     ptB
    *                                     ptNP
    *
    *  ptA, pt1 and ptB line on a line parallel to ptN, pt1P and ptNP.
    *
    * /endcode
    */
    // vector from pt1P to pt1
    math::Vector3D vector1P1 = pt1P - pt1;
    double lengthV1P1 = vector1P1.length();
    vector1P1.normalise();
    vector1P1 *= lengthV1P1;

    math::Vector3D ptA = ptN - vector1P1;
    math::Vector3D ptB = ptNP - vector1P1;

    ////////////////////////////////////////////////////////////////
    // Calculate width of route
    m_routeWidth = math::round((ptN - pt1P).length() / 4.0) * 3.0;

    // Workout offset from center line given width of route and middle vector v12
    //   The cross product of two three-dimensional vectors is a three-dimensional
    //   vector perpendicular to both.
    //
    //   NOTE: we assume that the z-axis is out of the screen.
    const math::Vector3D normal(0, 0, 1); // z- axis
    math::Vector3D p = normal.crossProduct(vector12);  
    p.normalise();

    math::Vector3D offset = p * m_routeWidth;

    // Left side
    math::Vector3D left = pt1P + offset;
    TSLCoord posLeft(left.m_x, left.m_y);

    // Right size
    math::Vector3D right = pt1P - offset;
    TSLCoord posRight(right.m_x, right.m_y);

    // Need to figure out which side the offset points too.
    //
    // Using Scalar Triple Product gives us a signed volume calculation.
    //
    // The sign is dependent on the orientation of the frame which means that we can figure out left or right
    // given we know up is out of the screen (z-axis) and the other axis is down the center line (x-axis).
    //
    // Ref: http://en.wikipedia.org/wiki/Triple_product
    //
    // tripleProduct = (a x b).c
    //
    //  a == normal (z-axis)
    //  b == v12
    //
    // offset = normal.crossProduct(v12) * halfWidth
    //
    //  c == ptN - pt1P      vector towards ptN... our left/right vector
    //
    double t = offset.dotProduct(ptN - pt1P);

    TSLCoord posN;
    TSLCoord pos1;
    TSLCoord posNP;
    if (t < 0.0)
    {
      posN = TSLCoord(ptNP.m_x, ptNP.m_y);
      posNP = TSLCoord(ptN.m_x, ptN.m_y);
    }
    else
    {
      posN = TSLCoord(ptN.m_x, ptN.m_y);
      posNP = TSLCoord(ptNP.m_x, ptNP.m_y);
    }
    pos1 = cPT1;  // point of arrow.

    /* So what we have at this point is:
    *
    *              +  posN
    *              |\
    *              | \
    *      posLeft +  \
    *                  \
    *                   \ pos1
    *                   /
    *                  /
    *     posRight +  /
    *              | /
    *              |/
    *              +  posNP
    *
    */

    // Offset lines. This is simple line buffering.
    // 
    // 0 = arrow tip
    // 1 = arrow back
    // N = next point

    std::list<TSLCoord> leftCS;
    std::list<TSLCoord> rightCS;

    BufferLine::expandTail(m_expandTail, m_tailExpansion);

    if (buffered)
    {
      simpleBuffer(cPT1, positions, m_routeWidth, leftCS, rightCS);
    }

    /////////////////////////////////////////////////
    // Angle between xaxis and vector12 - used for rotation of text behind arrow head
    const math::Vector3D xaxis(1, 0, 0);

    // work out the angle between the vector vector12 and x-axis.
    m_rotation = vector12.angleBetween(xaxis);
    if(vector12.m_y < 0 )
      m_rotation *= -1;

    // adjust the rotation so that the text is more readable.
    if (m_rotation > M_PI_2)
      m_rotation -= M_PI;
    else if (m_rotation < -M_PI_2)
      m_rotation += M_PI;

    //////////////////////////////////////////////////
    // pt2 required for text location.
    m_point2 = TSLCoord(pt2.m_x, pt2.m_y);

    //////////////////////////////////////////////////
    // midway between pt2 and pt1P for placement of text behind arrow head.
    math::Vector3D vector21P = pt1P - pt2;
    double lengthV21P = vector21P.length();
    vector21P.normalise();
    math::Vector3D pt2_1 = pt2 + (vector21P * lengthV21P/2.0);

    m_midPoint = TSLCoord(pt2_1.m_x, pt2_1.m_y);

    ///////////////////////////////////////////////////
    bool shiftPt1 = m_innerArrow && !buffered;
    math::Vector3D arrowPt;
    if (shiftPt1)
    {
      arrowPt = innerArrowHeadTipPosition(pt1, posN, posLeft, vector12);
    }

    ////////////////////////////////////////////////////////
    // Create geometry

    TSLEntitySet *group = TSLEntitySet::create();

    //
    // Create the points that make up the Arrow head and tail
    TSLCoordSet *points = new TSLCoordSet();

    if (spline)
    {
      leftCS.push_back(posLeft);

      Geometry::Spline sp(leftCS, false);
      TSLCoordSet *curve = sp.curve(false);

      if (curve)
        points->concatenate(*curve);
    }
    else
    {
      // left side
      std::list<TSLCoord>::iterator it(leftCS.begin());
      std::list<TSLCoord>::iterator endIt(leftCS.end());  
      while (it != endIt)
        points->add(*it ++);
    }

    if (!m_invertArrowHead)
    {
      // arrow head
      if (m_arrowTailCrossOver)
      {
        if (buffered || m_innerArrowCloseTriangles)
          points->add(posRight);
        points->add(posNP);
        points->add(pos1);
        points->add(posN);
        if (buffered || m_innerArrowCloseTriangles)
          points->add(posLeft);
      }
      else
      {
        if (buffered || m_innerArrowCloseTriangles)
          points->add(posLeft);
        points->add(posN);
        points->add(pos1);
        points->add(posNP);
        if (buffered || m_innerArrowCloseTriangles)
          points->add(posRight);
      }
    }
    else
    {
      // just close the top of the tail.
      points->add(posLeft);
      points->add(posRight);
    }
    // right side
    if (spline)
    {
      rightCS.push_front(posRight);

      Geometry::Spline sp(rightCS, false);
      TSLCoordSet *curve = sp.curve(false);

      if (curve)
        points->concatenate(*curve);
    }
    else
    {
      std::list<TSLCoord>::iterator it(rightCS.begin());
      std::list<TSLCoord>::iterator endIt(rightCS.end()); 

      while (it != endIt)
        points->add(*it ++);
    }

    // Create the outline
    TSLPolyline *line = group->createPolyline(0, points, true);

    if (m_closeTail)
    {
      line->closed(true);
    }

    TSLRenderingAttributes atts;
    atts.m_edgeColour = m_enemy ? TSLComposeRGB( 255, 0, 0 ) : TSLComposeRGB( 0, 0, 0 );
    atts.m_edgeThickness = m_lineWidth ;
    atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;

    if (!buffered)
    {
      atts.m_edgeStyle = m_solidArrowHead ? 1 : m_dashLines ? m_dashType : 1;
    }
    else
    {
      atts.m_edgeStyle = m_dashLines ? m_dashType : 1 ;
    }
    
    line->setRendering( atts ) ;
    atts.m_edgeStyle = m_dashLines ? m_dashType : 1 ; // reset

    if (!buffered)
    {
      TSLCoordSet *points = new TSLCoordSet();
      if (positions.size() == 1)
      {
        points->add(shiftPt1 ? TSLCoord(arrowPt.m_x, arrowPt.m_y) : pos1);
        points->add(cPT2);
      }
      else
      {
        points->add(shiftPt1 ? TSLCoord(arrowPt.m_x, arrowPt.m_y) : pos1);
        points->concatenate(positions);
      }

      TSLPolyline *line = group->createPolyline(0, points, true);
      atts.m_edgeStyle = m_dashLines ? m_dashType : 1 ; 
      line->setRendering( atts ) ;
    }

    if (m_invertArrowHead)
    {
      /*
      * /code
      *   
      *      
      *          /|
      *  ----   / |
      *     |  /  |
      *     | /   | 
      *     |/    |
      *     |\    |
      *     | \   |
      *  ----  \  |
      *         \ |
      *          \|
      *
      * /endcode
      */

      TSLCoordSet *points = new TSLCoordSet();
      points->add(TSLCoord(ptA.m_x, ptA.m_y));
      points->add(TSLCoord(ptB.m_x, ptB.m_y));
      points->add(TSLCoord(pt1P.m_x, pt1P.m_y));
      points->add(TSLCoord(ptA.m_x, ptA.m_y));

      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;
    }
    else
    {
      // 
      if (m_innerArrow)
      {
        math::Vector3D arrowPt = innerArrowHeadTipPosition(pt1, posN, posLeft, vector12);

        TSLCoordSet *points = new TSLCoordSet();
        points->add(posLeft);
        points->add(TSLCoord(arrowPt.m_x, arrowPt.m_y));
        points->add(posRight);
        line = group->createPolyline(0, points, true);

        if (!buffered)
        {
          atts.m_edgeStyle = m_solidArrowHead ? 1 : m_dashLines ? m_dashType : 1;
        }
        line->setRendering( atts ) ;
        atts.m_edgeStyle = m_dashLines ? m_dashType : 1 ; // reset
      }

      if (m_outerArrow)
      {
        /*
        * /code
        *   
        *      |\ \
        *      | \ \
        *  ----   \ \
        *          \ \
        *           \ \ 
        *           / /
        *          / /
        *         / /
        *  ----| / /
        *      |/ /
        *
        * /endcode
        */

        math::Vector3D pLN = posN - posLeft;
        double length = pLN.length();

        math::Vector3D v12N(vector12);
        v12N.normalise();
        v12N *= length;

        // Shift the triangle that makes the arrow head outwards
        math::Vector3D arrowPt = pt1 + v12N;
        math::Vector3D newLeft = ptN + v12N;
        math::Vector3D newRight = ptNP + v12N;

        TSLCoordSet *points = new TSLCoordSet();

        points->add(TSLCoord(newLeft.m_x, newLeft.m_y));
        points->add(TSLCoord(arrowPt.m_x, arrowPt.m_y));
        points->add(TSLCoord(newRight.m_x, newRight.m_y));
        line = group->createPolyline(0, points, true);

        atts.m_edgeStyle = m_outerArrowDash ? m_dashType : 1 ; 
        line->setRendering( atts ) ;
      }
    }

    return group;
  }




}; }; };