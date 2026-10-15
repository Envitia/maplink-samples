/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#ifndef _AXISARROW_H_
#define _AXISARROW_H_

#include "../utilities/bufferline.h"

namespace envitia { namespace MapLink { namespace TacticalGraphics {

  /***
   * Axis of Advance Arrow base class.
   *
   * @Note: certain combinations do not make sense.
   */
  class AxisArrow : protected GeometryOperations::BufferLine {
  public:
    AxisArrow()
    : m_expandTail(false)
    , m_enemy(false)
    , m_arrowTailCrossOver(false)
    , m_innerArrow(false)
    , m_outerArrow(false)
    , m_closeTail(false)
    , m_invertArrowHead(false)
    , m_closeHead(false)
    , m_dashType(20)
    , m_dashLines(false)
    , m_routeWidth(0.0)
    , m_rotation(0.0)
    , m_outerArrowDash(false)
    , m_solidHead(true)
    , m_lineWidth(1)
    , m_innerArrowCloseTriangles(true)
    , m_solidArrowHead(true)
    {
    }
    virtual ~AxisArrow() {}



    /**
     * Create an TSLEntitySet containing the arrow head and tail.
     *
     * The arrow head is defined by 
     *
     * /code
     *                                    PTN (last point)
     *                                     |\
     *                                     | \
     *   ----------------------------------|  \
     *                                         \
     *   PTn+1             PT2          pt1P    \  PT1
     *                                          /
     *                                         /
     *   ----------------------------------|  /
     *                                     | /
     *                                     |/
     *                                     ptNP
     * /endcode
     *
     * @param positions Controls points that define the orientation of the graphic. At least three points
     * need to be specified. First point is the arrow head point, second point is arrow tail position. Last point is the
     * width of the arrow head.
     * @param spline Convert the tail into a spline curve.
     *
     * @note No point should be duplicated.
     * @note Setting spline to true will cause the tail to be converted to a spline curve.
     * The curve may not always appear sensibly if the points go back on themselves or cross over.
     * This is a side effect of converting a polyline into a spline.
     */
    TSLEntitySet *createArrow(const TSLCoordSet &positions, bool spline, bool buffered = true);

    /**
     * Create an TSLEntitySet containing the arrow head and tail.
     *
     * The arrow head is defined by (for un-buffered)
     *
     * /code
     *                                       \
     *                                        \
     *                                         \
     *   PT2 ___________________________________\  PT1
     *                                          /
     *                                         /
     *                                        /
     *                                       /
     * /endcode
     *
     * for buffered
     * /code
     *                                     |\
     *                                     | \
     *   ----------------------------------|  \
     *                                         \
     *   PT2                                    \  PT1
     *                                          /
     *                                         /
     *   ----------------------------------|  /
     *                                     | /
     *                                     |/
     * /endcode
     *
     *
     * @param pt1
     * @param pt2
     * @param buffered
     *
     */
    TSLEntitySet *createArrow(const TSLCoord &pt1, const TSLCoord &pt2, bool buffered = false);

    /**
     * Query method to return if the arrow is hostile.
     *
     * The arrow is coloured red if hostile. It is coloured black otherwise.
     */
    bool enemy()
    {
      return m_enemy;
    }
    /**
     * Setter method to set if the arrow is hostile.
     */
    void enemy(bool s)
    {
      m_enemy = s;
    }

    /**
    * Expand out the last two points of the tail.
    */
    void expandTail(bool s)
    {
      m_expandTail = s;
    }
    /**
     * Returns true if the last two points are expanded out slightly.
     */
    bool expandTail()
    {
      return m_expandTail;
    }

    /**
     * Returns true if we cross over the left and right join points of the
     * arrow head
     */
    bool arrowTailCrossOver()
    {
      return m_arrowTailCrossOver;
    }
    /**
     * Cross over the left and right join points of the arrow head.
     *
     * /code
     *                                    PTN (last point)
     *                                     |\
     *                                     | \
     *   --------------------+ a         b +  \
     *                                         \
     *                                          \
     *                    pt2+      + e          \  PT1
     *                                          /
     *                                         /
     *   --------------------+ d         c +  /
     *                                     | /
     *                                     |/
     *
     *
     * a is joined to c
     * b is joined to d
     * lines cross at e
     *
     * /endcode
     *
    */
    void arrowTailCrossOver(bool s)
    {
      m_arrowTailCrossOver = s;
    }

    /**
     * If passed true draws an arrow head like Main Attack.
     *
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
     *
     */
    void innerArrow(bool s, bool closeTriangles = true)
    {
      m_innerArrow = s;
      m_innerArrowCloseTriangles = closeTriangles;
    }

    bool innerArrow()
    {
      return m_innerArrow;
    }

    /**
     * If passed true draws an arrow head like Axis of Advance for Faint.
     *
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
     *
     */
    void outerArrow(bool s)
    {
      m_outerArrow = s;
    }

    bool outerArrow()
    {
      return m_outerArrow;
    }

    bool closeTail()
    {
      return m_closeTail;
    }

    void closeTail(bool s)
    {
      m_closeTail = s;
    }

    bool invertArrowHead()
    {
      return m_invertArrowHead;
    }

    /**
     * Invert the arrow head and close it and 
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
    void invertArrowHead(bool s)
    {
      m_invertArrowHead = s;
    }

    bool closeHead()
    {
      return m_closeHead;
    }

    /**
     * Close the bottom of the arrow head
     */
    void closeHead(bool s)
    {
      m_closeHead = s;
    }

    // line styling
    void dashLines(bool s)
    {
      m_dashLines = s;
    }

    void longDash(bool s)
    {
      if (s)
        m_dashType = 21;
      else
        m_dashType = 20;
    }
    bool longDash()
    {
      return m_dashType == 21;
    }

    /**
     * Width of the route in TMCs.
     */
    double routeWidth()
    {
      return m_routeWidth;
    }

    const TSLCoord &backOfArrowHead() const
    {
      return m_arrowHeadBack;
    }

    /**
     * Returns the angle of rotation, in radians, between the vector from pt1 to pt2 from the x-axis.
     *
     * The rotation is adjusted so that the text is readable when the drawing surface is un-rotated.
     */
    double rotation() const
    {
      return m_rotation;
    }

    const TSLCoord &point2() const
    {
      return m_point2;
    }

    /**
    * The midPoint between the back of the arrow head and PT2.
    */
    const TSLCoord &midPoint() const
    {
      return m_midPoint;
    }

    void outerArrowDash(bool s)
    {
      m_outerArrowDash = s;
    }

    void lineWidth(int width)
    {
      m_lineWidth = width;
    }

    void solidArrowHead(bool s)
    {
      m_solidArrowHead = s;
    }

     void calcPTN(const TSLCoord &cPT1, const TSLCoord &cPT2, TSLCoord &cPTN) const;
  private:

    /**
     * Create an TSLEntitySet containing the arrow head and tail.
     *
     * The arrow head is defined by 
     *
     * /code
     *                                    PTN (last point)
     *                                     |\
     *                                     | \
     *   ----------------------------------|  \
     *                                         \
     *   PTn+1             PT2          pt1P    \  PT1
     *                                          /
     *                                         /
     *   ----------------------------------|  /
     *                                     | /
     *                                     |/
     *                                     ptNP
     * /endcode
     *
     * @param cPT1 First point of arrow head.
     * @param cPT2 Second point.
     * @param cPTN Last Point, which defines the arrow head width.
     * @param positions Tail points (not including the PT1 and PTN).
     * @param spline Convert the tail into a spline curve.
     *
     * @note No point should be duplicated.
     * @note Setting spline to true will cause the tail to be converted to a spline curve.
     * The curve may not always appear sensibly if the points go back on themselves or cross over.
     * This is a side effect of converting a polyline into a spline.
     */
    TSLEntitySet *createArrow(const TSLCoord &cPT1, const TSLCoord &cPT2, const TSLCoord &cPTN, const TSLCoordSet &positions, bool spline, bool buffered = true);

    bool m_expandTail;
    bool m_enemy;
    bool m_arrowTailCrossOver;
    bool m_innerArrow;
    bool m_outerArrow;
    bool m_closeTail;
    bool m_invertArrowHead;
    bool m_closeHead;
    bool m_dashLines;
    int m_dashType;
    double m_routeWidth;
    TSLCoord m_arrowHeadBack;
    double m_rotation;
    TSLCoord m_point2;
    TSLCoord m_midPoint;
    bool m_outerArrowDash;
    bool m_solidHead;
    int m_lineWidth;
    bool m_innerArrowCloseTriangles;
    bool m_solidArrowHead;

    static const double m_tailExpansion;
  };



}; }; };

#endif
