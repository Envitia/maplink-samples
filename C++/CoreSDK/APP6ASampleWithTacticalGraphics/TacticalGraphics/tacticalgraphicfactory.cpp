#define _USE_MATH_DEFINES
#include <cmath>

#include "MapLink.h"

#include "lines/axisarrow.h"
#include "areas/attackByFirePosition.h"
#include "areas/supportByFirePosition.h"
#include "areas/contain.h"
#include "areas/ambush.h"
#include "tacticalgraphicfactory.h"
#include "utilities/simpleArrow.h"
#include "utilities/solidSimpleArrow.h"

#include <cstring>

#ifndef WIN32
# define strnicmp strncasecmp
#endif


namespace envitia { namespace MapLink { namespace TacticalGraphics {


  TSLEntitySet *APP6CTacticalGraphicsFactory::createArea(const char *sidc, const TSLCoordSet &positions, Modifiers *modifiers, size_t numberOfModifiers, int lineWidth)
  {
    if (!sidc && strlen(sidc) >= 10)
    {
      return NULL;
    }

    TSLEntitySet* eSet = NULL;

    /*
    SIDC: GHGAGLP----AUSX
    G  	    Code scheme
    H 	    hostility (colour)
    G 	    Battle dimension
    A 	    Status  (line style)
    GLP--- 	Type of graphic to draw.
    -A    	Size/mobility
    SX	    Country code
    */
    char codeScheme = sidc[0];
    char hostility = sidc[1];
    char status = sidc[3];
    char battle = sidc[2];
    char graphicType[7];
    strncpy(graphicType, &sidc[4], 6);
    graphicType[6] = '\0';

    if (strnicmp("MOOP--", graphicType, 6) == 0)
    {
      // Attack By Fire Position: APP-6A(C) May 2011, Table 7-7 page 7-61
      // SIDC: G*C*MOOP--*****
      TacticalGraphics::attackByFirePosition attackByFirePosition;
      eSet = attackByFirePosition.create( positions );
    }

    if (strnicmp("MOOS--", graphicType, 6) == 0)
    {
      // Support By Fire Position: APP-6A(C) May 2011, Table 7-7 page 7-62
      // SIDC: G*C*MOOS--*****
      TacticalGraphics::supportByFirePosition supportByFirePosition;
      eSet = supportByFirePosition.create( positions );
    }

    if (strnicmp("GJ----", graphicType, 6) == 0)
    {
      // Contain: APP-6A(C) May 2011, Table 7-A-1 page 7-A-10
      // SIDC: G*T*GJ----*****
      TacticalGraphics::contain contain;
      eSet = contain.create( positions );
    }

    if (strnicmp("MSGA--", graphicType, 6) == 0)
    {
      // Ambush: APP-6A(C) May 2011, Table 7-7 page 7-64
      // SIDC: G*C*MSGA--*****
      TacticalGraphics::ambush ambush;
      eSet = ambush.create( positions );
    }

    // TBD Temporary Rendering Setup
    if ( eSet )
    {
      TSLRenderingAttributes atts;
      // Polylines
	  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_edgeThickness = 1 ;
      atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      atts.m_edgeStyle = 1 ;

      eSet->setRendering( atts );
    }

    return eSet;
  }

  TSLEntitySet *APP6CTacticalGraphicsFactory::createLine(const char *sidc, const TSLCoordSet &positions, Modifiers *modifiers, size_t numberOfModifiers, bool spline, int lineWidth)
  {
    if (!sidc && strlen(sidc) >= 10)
    {
      return NULL;
    }
    /*
    SIDC: GHGAGLP----AUSX
    G  	    Code scheme
    H 	    hostility (colour)
    G 	    Battle dimension
    A 	    Status  (line style)
    GLP--- 	Type of graphic to draw.
    -A    	Size/mobility
    SX	    Country code
    */
    char codeScheme = sidc[0];
    char hostility = sidc[1];
    char status = sidc[3];
    char battle = sidc[2];
    char graphicType[7];
    strncpy(graphicType, &sidc[4], 6);
    graphicType[6] = '\0';


    if (strnicmp("MOLAM-", graphicType, 6) == 0)
    {
      // Friendly Main Attack
      // Main Attack: APP-6A(C) May 2011, Table 7-7 page 7-47
      // SIDC: GFC*MOLAM-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.innerArrow(true);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      return arrow;
    }

    if (strnicmp("MOLAS-", graphicType, 6) == 0)
    {
      // Supporting Attack
      // Supporting Attack: APP-6A(C) May 2011, Table 7-7 page 7-48
      // SIDC: GFC*MOLAS-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      return arrow;
    }

    if (strnicmp("MOLAO-", graphicType, 6) == 0)
    {
      // Friendly Planned Attack
      // Supporting Attack Planned or On Order: APP-6A(C) May 2011, Table 7-7 page 7-49
      //Friendly Ground Axis On Order with Date and Time (if known) Effective
      // SIDC: GFC*MOLAO-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.dashLines(true);
      axisArrow.lineWidth(lineWidth);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      // TBD modifiers
      return arrow;
    }

    if (strnicmp("MOLAE-", graphicType, 6) == 0)
    {
      // Enemy Confirmed
      // SIDC: GHC*MOLAE-*****
      // Enemy Confirmed: APP-6A(C) May 2011, Table 7-7 page 7-51
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.enemy(true); // You can either display as red or back with text 'ENY', see 0702
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      return arrow;
    }

    if (strnicmp("MOLAF-", graphicType, 6) == 0)
    {
      // Friendly Airborne/Aviation: APP-6A(C) May 2011, Table 7-7 page 7-45
      // SIDC: GFC*MOLAF-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.arrowTailCrossOver(true);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      return arrow;
    }


    if (strnicmp("MDA---", graphicType, 6) == 0)
    {
      // Friendly Feint
      // Axis of Advance for a Feint: APP-6A(C) May 2011, Table 7-7 page 7-50
      // SIDC G*C*MDA---*****

      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.outerArrow(true); // slightly wrong PT1 is slightly out.
      axisArrow.lineWidth(lineWidth);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      return arrow;
    }

    if (strnicmp("MOLAT-", graphicType, 6) == 0)
    {
      // Enemy Suspect
      // Enemy Templated or Suspected: APP-6A(C) May 2011, Table 7-7 page 7-52
      // SIDC: GHC*MOLAT-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.enemy(true); // You can either display as red or back with text 'ENY', see 0702
      axisArrow.dashLines(true);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);
      return arrow;
    }

    // Counterattack By Fire
    // TBD
    // SIDC: G*T*GKF---*****

    // Advance To Contact (MTV): APP-6A(C) May 2011, Table 7-A-7 page 7-A-1
    // TBD

    // Counterattack by Fire (MT): APP-6A(C) May 2011, Table 7-A-7 page 7-A-11
    // TBD

    /////////////////////////////////////////////////////////
    // Convoys
    if (strnicmp("SLCM--", graphicType, 6) == 0)
    {
      //Moving convoy
      // SIDC: G*C*SLCM--******
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.closeTail(true);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false);
      return arrow;
    }

    if (strnicmp("SLCH--", graphicType, 6) == 0)
    {
      // Halted Convoy
      // SIDC: G*C*SLCH--******
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.closeTail(true);
      axisArrow.closeHead(true);
      axisArrow.invertArrowHead(true);
      // TBD head is touching tail
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false);
      return arrow;
    }

    //////////////////////////////////////////////////
    // Direction of Attack 7-54

    if (strnicmp("MOLDM-", graphicType, 6) == 0)
    {
      // friendly direction of main attack
      // GFC*MOLDM-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.innerArrow(true, false);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      return arrow;
    }

    if (strnicmp("MOLDS-", graphicType, 6) == 0)
    {
      // friendly direction of supporting attack
      // GFC*MOLDS-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.innerArrow(false, false);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      return arrow;
    }

    if (strnicmp("MOLDO-", graphicType, 6) == 0)
    {
      // friendly ground axis planned or On order with effective date and time (if known)
      //GFC*MOLDO-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.dashLines(true);
      axisArrow.innerArrow(false, false);
      // TBD not sure if arrow head is a solid line.
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      return arrow;
    }

    if (strnicmp("MDF---", graphicType, 6) == 0)
    {
      // Direction of attack for a feint
      // G*C*MDF---*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.outerArrowDash(true);
      axisArrow.outerArrow(true);
      axisArrow.innerArrow(false, false);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      return arrow;
    }

    if (strnicmp("MOLDG-", graphicType, 6) == 0)
    {
      // Enemy Confirmed
      // GHC*MOLDG-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.enemy(true);
      axisArrow.innerArrow(false, false);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      return arrow;
    }

    if (strnicmp("MOLDR-", graphicType, 6) == 0)
    {
      // Enemy Templated or Suspected
      // GHC*MOLDR-*****
      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.dashLines(true);
      axisArrow.enemy(true);
      axisArrow.innerArrow(false, false);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      return arrow;
    }

    //////////////////////////////////////////////////
    // Task Graphics
    if (strnicmp("GK----", graphicType, 6) == 0)
    {
      // Counterattack (MT/MTV): APP-6A(C) May 2011, Table 7-A-7 page 7-A-11
      // G*T*GK----*****

      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.dashLines(true);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, spline);

      if (arrow)
      {
        // Add the text
        const TSLCoord &point = axisArrow.midPoint();
        double height = axisArrow.routeWidth();
        double rotation = axisArrow.rotation();

        TSLText *text = arrow->createText(0, point.m_x, point.m_y, "CATK", 
          height, TSLHorizontalAlignmentCentre, TSLVerticalAlignmentMiddle, rotation);

        TSLRenderingAttributes atts;
        atts.m_textColour = TSLComposeRGB( 0, 0, 0 );
        atts.m_textSizeFactor = height;
        atts.m_textSizeFactorUnits = TSLDimensionUnitsTMCs;
        atts.m_textFont = 1 ;
        atts.m_textRotatable = TSLTextRotationEnabled ;
        text->setRendering( atts ) ;
      }
      return arrow;
    }
    if (strnicmp("GA----", graphicType, 6) == 0)
    {
      // Follow and Assume, 7-A-22
      // G*T*GA----*****

      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.dashLines(true);
      axisArrow.innerArrow(true);
      axisArrow.solidArrowHead(true);

      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);
      if (arrow)
      {
        ///////////////////////////////////////////////
        // trailing graphics
        double width = axisArrow.routeWidth();

        math::Vector3D pt1(positions[0]);
        math::Vector3D pt2(positions[1]);
        // calculate vector from pt1 to pt2
        math::Vector3D vector12 = (pt1 - pt2);
        double length = vector12.length();

        math::Vector3D ptN1(positions[positions.size()-2]);
        math::Vector3D ptN2(positions[positions.size()-1]);

        // calculate vector from last two points
        math::Vector3D vectorN12 = (ptN1 - ptN2);
        vectorN12.normalise();

        /*
        *    leftA = endPt + offset      
        *                        leftB = ptA + offset
        *    +--------------------+
        *    |                     \
        *    |                      \ pt2 (ptN2)
        *    + endPt              +  \+-------------------------pt1 (ptN1)
        *    |                 ptA   /
        *    |                      /
        *    +--------------------+/
        *    rightA = endPt - offset
        *                         rightB = ptA - offset
        */

        math::Vector3D endPt = ptN2 - (vectorN12 * length);

        const math::Vector3D normal(0, 0, 1); // z- axis
        math::Vector3D p = normal.crossProduct(vectorN12);  
        p.normalise();

        math::Vector3D offset = p * width;

        math::Vector3D leftA = endPt + offset;
        math::Vector3D rightA = endPt - offset;

        //
        math::Vector3D ptA = ptN2 - (vectorN12 * width);

        math::Vector3D leftB = ptA + offset;
        math::Vector3D rightB = ptA - offset;

        TSLCoordSet *points = new TSLCoordSet;

        points->add(TSLCoord(ptN2.m_x, ptN2.m_y));
        points->add(TSLCoord(leftB.m_x, leftB.m_y));
        points->add(TSLCoord(leftA.m_x, leftA.m_y));
        points->add(TSLCoord(rightA.m_x, rightA.m_y));
        points->add(TSLCoord(rightB.m_x, rightB.m_y));

        TSLPolyline *line = arrow->createPolyline(0, points, true);
        line->closed(true);

        TSLRenderingAttributes atts;
        atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
        atts.m_edgeThickness = lineWidth ;
        atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
        atts.m_edgeStyle = 1;
        line->setRendering( atts ) ;
      }
      return arrow;
    }

    if (strnicmp("GAS---", graphicType, 6) == 0)
    {
      // Follow and Support 7-A-22
      // G*T*GAS---*****

      TacticalGraphics::AxisArrow axisArrow;
      axisArrow.lineWidth(lineWidth);
      axisArrow.dashLines(false);
      axisArrow.innerArrow(false, false);
      TSLEntitySet *arrow = axisArrow.createArrow(positions, false, false);

      if (arrow)
      {
        ///////////////////////////////////////////////
        // trailing graphics
        double width = axisArrow.routeWidth();

        math::Vector3D pt1(positions[0]);
        math::Vector3D pt2(positions[1]);
        // calculate vector from pt1 to pt2
        math::Vector3D vector12 = (pt1 - pt2);
        double length = vector12.length();

        math::Vector3D ptN1(positions[positions.size()-2]);
        math::Vector3D ptN2(positions[positions.size()-1]);

        // calculate vector from last two points
        math::Vector3D vectorN12 = (ptN1 - ptN2);
        vectorN12.normalise();

        /*
        *    leftA = endPt + offset      
        *                        leftB = ptA + offset
        *    +--------------------+
        *    \                     \
        *endPt\                     \ pt2 (ptN2)
        *   +  + ptB              +  \+-------------------------pt1 (ptN1)
        *     /                 ptA  /
        *    /                      /
        *    +--------------------+/
        *    rightA = endPt - offset
        *                         rightB = ptA - offset
        */

        math::Vector3D endPt = ptN2 - (vectorN12 * length);

        math::Vector3D ptB = ptN2 - (vectorN12 * (length - width));

        const math::Vector3D normal(0, 0, 1); // z- axis
        math::Vector3D p = normal.crossProduct(vectorN12);  
        p.normalise();

        math::Vector3D offset = p * width;

        math::Vector3D leftA = endPt + offset;
        math::Vector3D rightA = endPt - offset;

        //
        math::Vector3D ptA = ptN2 - (vectorN12 * width);

        math::Vector3D leftB = ptA + offset;
        math::Vector3D rightB = ptA - offset;

        TSLCoordSet *points = new TSLCoordSet;

        points->add(TSLCoord(ptN2.m_x, ptN2.m_y));
        points->add(TSLCoord(leftB.m_x, leftB.m_y));
        points->add(TSLCoord(leftA.m_x, leftA.m_y));
        points->add(TSLCoord(ptB.m_x, ptB.m_y));
        points->add(TSLCoord(rightA.m_x, rightA.m_y));
        points->add(TSLCoord(rightB.m_x, rightB.m_y));

        TSLPolyline *line = arrow->createPolyline(0, points, true);
        line->closed(true);

        TSLRenderingAttributes atts;
        atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
        atts.m_edgeThickness = lineWidth ;
        atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
        atts.m_edgeStyle = 1;
        line->setRendering( atts ) ;
      }
      return arrow;
    }

    ///////////////////////////////////////////////////////
    // Obstacle Control Measure

    if (strnicmp("BOAEB-", graphicType, 6) == 0)
    {
      /*
      * - block p 7-165
      *  - SIDC: G*C*BOAEB-*****
      */
      if (positions.size() != 3)
        return NULL;

      math::Vector3D pt1(positions[0]);
      math::Vector3D pt2(positions[1]);
      math::Vector3D pt3(positions[2]);

      // calculate vector from pt1 to pt2
      math::Vector3D vector12 = (pt1 - pt2);
      double length = vector12.length();
      vector12.normalise();

      math::Vector3D midPoint = pt1 - vector12 * (length/2.);

      TSLCoordSet *points = new TSLCoordSet;
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));

      TSLEntitySet *group = TSLEntitySet::create();
      TSLPolyline *line = group->createPolyline(0, points, true);

      TSLRenderingAttributes atts;
      atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_edgeThickness = lineWidth ;
      atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      atts.m_edgeStyle = 1;
      line->setRendering( atts ) ;

      points = new TSLCoordSet;
      points->add(TSLCoord(pt3.m_x, pt3.m_y));
      points->add(TSLCoord(midPoint.m_x, midPoint.m_y));

      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      return group;
    }
    if (strnicmp("BOAED-", graphicType, 6) == 0)
    {
      /*
      * - disrupt p 7-166
      *  - SIDC: G*C*BOAED-*****
      */
      if (positions.size() != 3)
        return NULL;

      math::Vector3D pt1(positions[0]);
      math::Vector3D pt2(positions[1]);
      math::Vector3D pt3(positions[2]);

      // calculate vector from pt1 to pt2
      math::Vector3D vector12 = (pt1 - pt2);
      double length = vector12.length();
      vector12.normalise();

      math::Vector3D midPoint = pt1 - vector12 * (length/2.);

      TSLCoordSet *points = new TSLCoordSet;
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));

      TSLEntitySet *group = TSLEntitySet::create();
      TSLPolyline *line = group->createPolyline(0, points, true);

      TSLRenderingAttributes atts;
      atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_edgeThickness = lineWidth ;
      atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      atts.m_edgeStyle = 1;
      atts.m_fillColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_fillStyle= 1;
      line->setRendering( atts ) ;

      // TBD arrow sizes are not quite correct.
      // TBD orientation needs to be double checked.

      // top arrow
      points = new TSLCoordSet;
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      points->add(TSLCoord(pt3.m_x, pt3.m_y));

      TacticalGraphics::solidSimpleArrow soldArrow;
      TSLEntitySet *arrow = soldArrow.createArrow(*points, 20.0 * 1./3.0);

      group->insert(arrow);
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      math::Vector3D vector13 = (pt1 - pt3);
      double length13 = vector13.length();
      vector13.normalise();

      // middle arrow
      math::Vector3D ptA = midPoint + (vector13 * length13 * 2.0/3.0);
      math::Vector3D ptB = midPoint - (vector13 * length13 * 2.0/3.0);

      points = new TSLCoordSet;
      points->add(TSLCoord(ptA.m_x, ptA.m_y));
      points->add(TSLCoord(ptB.m_x, ptB.m_y));

      arrow = soldArrow.createArrow(*points, 20.0 * (0.5 * 2/3.0));
      arrow->setRendering( atts ) ;
      group->insert(arrow);

      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      // bottom arrow
      math::Vector3D ptC = pt2 - (vector13 * length13 * 1.0/3.0);

      points = new TSLCoordSet;
      points->add(TSLCoord(pt2.m_x, pt2.m_y));
      points->add(TSLCoord(ptC.m_x, ptC.m_y));
      arrow = soldArrow.createArrow(*points);
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;
	  group->setRendering( atts );

      return group;
    }

    if (strnicmp("BYOE--", graphicType, 6) == 0)
    {
      /*
      * -  Obstacle Bypass Easy p7-171
      *  - SIDC: G*C*BYOE--*****
      */
      if (positions.size() != 3)
        return NULL;

      math::Vector3D pt1(positions[0]);
      math::Vector3D pt2(positions[1]);
      math::Vector3D pt3(positions[2]);

      // calculate vector from pt1 to pt2
      math::Vector3D vector12 = (pt1 - pt2);
      double length = vector12.length();
      vector12.normalise();

      math::Vector3D midPoint = pt1 - vector12 * (length/2.);

      math::Vector3D vectorM3 = pt3 - midPoint;
      //double lengthM3 = vectorM3.length();

      math::Vector3D pt3A = pt1 + vectorM3;
      math::Vector3D pt3B = pt2 + vectorM3;


      TSLCoordSet *points = new TSLCoordSet;
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));

      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));

      TSLEntitySet *group = TSLEntitySet::create();
      TSLPolyline *line = group->createPolyline(0, points, true);

      TSLRenderingAttributes atts;
      atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_edgeThickness = lineWidth ;
      atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      atts.m_edgeStyle = 1;
      atts.m_fillColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_fillStyle= 1;
      line->setRendering( atts ) ;

      // TBD arrow sizes are not quite correct.
      // TBD orientation needs to be double checked.

      // top arrow
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      TacticalGraphics::solidSimpleArrow soldArrow;
      TSLEntitySet *arrow = soldArrow.createArrow(*points, 20.0 * 2./3.0);
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      points->clear();

      // bottom arrow
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));

      arrow = soldArrow.createArrow(*points, 20.0 * ( 2./3.0));
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      points->destroy();

      return group;
    }
    if (strnicmp("BYOD--", graphicType, 6) == 0)
    {
      /*
      * -  Obstacle Bypass Difficult p7-172
      *  - SIDC: G*C*BYOD--*****
      */
      if (positions.size() != 3)
        return NULL;
      math::Vector3D pt1(positions[0]);
      math::Vector3D pt2(positions[1]);
      math::Vector3D pt3(positions[2]);

      // calculate vector from pt1 to pt2
      math::Vector3D vector12 = (pt1 - pt2);
      double length = vector12.length();
      vector12.normalise();

      math::Vector3D midPoint = pt1 - vector12 * (length/2.);

      math::Vector3D vectorM3 = pt3 - midPoint;
      //double lengthM3 = vectorM3.length();

      math::Vector3D pt3A = pt1 + vectorM3;
      math::Vector3D pt3B = pt2 + vectorM3;

      // left
      TSLCoordSet *points = new TSLCoordSet;
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));

      TSLEntitySet *group = TSLEntitySet::create();
      TSLPolyline *line = group->createPolyline(0, points, true);

      TSLRenderingAttributes atts;
      atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_edgeThickness = lineWidth ;
      atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      atts.m_edgeStyle = 1;
      atts.m_fillColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_fillStyle= 1;
      line->setRendering( atts ) ;

      // right
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      // bottom

      points = new TSLCoordSet;
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));
      line = group->createPolyline(0, points, true);
      atts.m_edgeStyle = 16042;  // TBD need to create a linestyle that more closely matches the specified one.
      // We may need to actually create a zig-zag line though.
      line->setRendering( atts ) ;

      // TBD arrow sizes are not quite correct.
      // TBD orientation needs to be double checked.

      // top arrow
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      TacticalGraphics::solidSimpleArrow soldArrow;
      TSLEntitySet *arrow = soldArrow.createArrow(*points, 20.0 * 2./3.0);
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      points->clear();

      // bottom arrow
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));

      arrow = soldArrow.createArrow(*points, 20.0 * ( 2./3.0));
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      points->destroy();

      return group;
    }
    if (strnicmp("BYOI--", graphicType, 6) == 0)
    {
      /*
      * -  Obstacle Bypass Impossible p7-173
      *  - SIDC: G*C*BYOI--*****
      */
      if (positions.size() != 3)
        return NULL;
      math::Vector3D pt1(positions[0]);
      math::Vector3D pt2(positions[1]);
      math::Vector3D pt3(positions[2]);

      // calculate vector from pt1 to pt2
      math::Vector3D vector12 = (pt1 - pt2);
      double length = vector12.length();
      vector12.normalise();

      math::Vector3D midPoint = pt1 - vector12 * (length/2.);

      math::Vector3D vectorM3 = pt3 - midPoint;


      math::Vector3D pt3A = pt1 + vectorM3;
      math::Vector3D pt3B = pt2 + vectorM3;

      // left
      TSLCoordSet *points = new TSLCoordSet;
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));

      TSLEntitySet *group = TSLEntitySet::create();
      TSLPolyline *line = group->createPolyline(0, points, true);

      TSLRenderingAttributes atts;
      atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_edgeThickness = lineWidth ;
      atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels ;
      atts.m_edgeStyle = 1;
      atts.m_fillColour = TSLComposeRGB( 0, 0, 0 );
      atts.m_fillStyle= 1;
      line->setRendering( atts ) ;

      // right
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      math::Vector3D pt3A1 = pt3A - (vector12 * (length * 1/3.));
      math::Vector3D pt3B1 = pt3B + (vector12 * (length * 1/3.));


      // bottom
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt3B1.m_x, pt3B1.m_y));
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      points = new TSLCoordSet;
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));
      points->add(TSLCoord(pt3A1.m_x, pt3A1.m_y));
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      // marks
      double lengthM3 = vectorM3.length();
      vectorM3.normalise();
      math::Vector3D pt3A1a = pt3A1 - (vectorM3 * (lengthM3 * .1));
      math::Vector3D pt3A1b = pt3A1 + (vectorM3 * (lengthM3 * .1));
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3A1a.m_x, pt3A1a.m_y));
      points->add(TSLCoord(pt3A1b.m_x, pt3A1b.m_y));
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      math::Vector3D pt3B1a = pt3B1 - (vectorM3 * (lengthM3 * .1));
      math::Vector3D pt3B1b = pt3B1 + (vectorM3 * (lengthM3 * .1));
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3B1a.m_x, pt3B1a.m_y));
      points->add(TSLCoord(pt3B1b.m_x, pt3B1b.m_y));
      line = group->createPolyline(0, points, true);
      line->setRendering( atts ) ;

      // TBD arrow sizes are not quite correct.
      // TBD orientation needs to be double checked.

      // top arrow
      points = new TSLCoordSet;
      points->add(TSLCoord(pt3A.m_x, pt3A.m_y));
      points->add(TSLCoord(pt1.m_x, pt1.m_y));
      TacticalGraphics::solidSimpleArrow soldArrow;
      TSLEntitySet *arrow = soldArrow.createArrow(*points, 20.0 * 2./3.0);
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      points->clear();

      // bottom arrow
      points->add(TSLCoord(pt3B.m_x, pt3B.m_y));
      points->add(TSLCoord(pt2.m_x, pt2.m_y));

      arrow = soldArrow.createArrow(*points, 20.0 * ( 2./3.0));
      arrow->setRendering( atts ) ;
      group->insert(arrow);
      points->destroy();

      return group;
    }
    return NULL;
  }


}; }; };