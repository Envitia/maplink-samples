/* -*-c++-*- */
/* Geometry Operations
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#ifndef _BUFFERLINE_H_
#define _BUFFERLINE_H_

#include <list>
#include "vector3d.h"

namespace envitia { namespace MapLink { namespace GeometryOperations {

  /**
   * Simple Line Buffering (ignoring buffering at ends of line).
   * 
   * This class is specifically for Tactical Graphics.
   */
  class BufferLine
  {
  public:
    BufferLine(bool expandTail = false, double tailExpansion = 1.25)
      : m_expandTail(expandTail)
      , m_tailExpansion(tailExpansion)
    {
    }

    /**
     * Simple Line Buffering (ignoring buffering at ends of line)
     *
     * @param pt1 The arrow tip point. This is uses as the first previous point but not buffered.
     * @param positions The list of points to buffer. 
     * @param width The width in TMC to buffer the line by.
     * @param leftCS The TMC points for the left side of the buffered line.
     * @param rightCS The TMC points for the right side of the buffered line.
     */
    bool simpleBuffer(const TSLCoord &pt1, const TSLCoordSet &positions, double width, std::list<TSLCoord> &leftCS, std::list<TSLCoord> &rightCS);

    void expandTail(bool expandTail, bool tailExpansion)
    {
      m_expandTail = expandTail;
      m_tailExpansion = m_tailExpansion;
    }

  protected:
    bool m_expandTail;
    double m_tailExpansion;
  };

}; }; };

#endif