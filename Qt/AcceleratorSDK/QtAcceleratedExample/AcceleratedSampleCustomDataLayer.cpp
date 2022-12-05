/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/
/////////////////////////////////////////////////////////////////////
// Include MapLink Pro Headers...
//
// Define some required Macro's and include X11 and Win32 headers as
// necessary.
//
// Define: TTLDLL & WIN32 within the project make settings.
//
/////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif
# include <windows.h>
#else
# include <GL/glx.h>
# ifndef X11
#  define X11
# endif
#endif

#define tslwin32printcontext_h
#include "MapLink.h"
#include "tslvariant.h"
#include "maplinkaccelerator.h"
//////////////////////////////////////////////////////////////////////

#include "AcceleratedSampleCustomDataLayer.h"

#include <GL/gl.h>
#include <GL/glu.h>

#if defined( WIN32 ) && defined( _DEBUG )
# define new DEBUG_NEW
#endif

bool AcceleratedSampleCustomDataLayer::drawLayer (const TSLAcceleratorEnvelope *extent, TSLAcceleratedRenderingInterface* renderingInterface)
{
  ////////////////////////////////////////////////////////////////////////////
  // This sample method draws a Red rectangle at Latitude and Longitude 5, 5.
  ////////////////////////////////////////////////////////////////////////////

  // Notes:
  // The extent is the bounding box for the map tile areas being drawn, so will be larger then the
  // actual area being displayed.
  //
  // The extent is defined in OpenGL coordinates (the mapping is the one chossen by us).
  //
  // If the surface is rotated then the extent will be the expanded bounding box extent.
  //
  // We are only drawing 2D items so depth buffer is not cleared and depth test is turned off.
  //
  // Polygons are also flat shaded.

  // We will be in model matrix mode.
  // The view rotation will already have been applied.
  glPushMatrix();

  // Disable depth test as we are drawing on top of the map tiles
  // at same depth.
  // This is the default for the Accelerator SDK.
  glDisable( GL_DEPTH_TEST );

  // Disable textures.
  // This is the default for the Accelerator SDK.
  glDisable( GL_TEXTURE_2D );

  // Set the colour.
  glColor4f( 1.0, 0.0, 0.0, 1.0 );

  // Set the polygon mode.
  // This is the default for the Accelerator SDK.
  glPolygonMode( GL_FRONT, GL_FILL );

  // Obtain the OpenGL Device units for drawing the rectangle.
  // We draw around 0,0 to make translating and rotating simpler.
  //
  // Notes:
  //  1. The points returned by latLonToADU are un-rotated points, they
  //     are also Device Units for OpenGL.
  //
  //     We don't store the ADU's as they may change on each redraw.
  //
  //     Scaling may be required (see code below) to ensure that the object
  //     being drawn is not distorted by for example; Dynamic Arc.
  //
  //  2. If we want to maintain the same size (in pixels) for the square
  //     we would use the following method call:
  //
  //       double xUnits, yUnits;
  //       renderingInterface->ADUsPerDU(xUnits, yUnits);
  //
  //     In this case scaling of the points would not be required.
  //
  //  3. It is possible to draw MapLink symbols or APP6A symbology into
  //     a bitmap using TSLNTSurface or TSLMotifSurface.
  //
  //     This bitmap can then be converted into a texture with the
  //     background colour set up with an alpha of 0, with other colours
  //     set to an alpha of 255.
  //
  //     The texture can then be applied to a polygon, using Alpha blending.
  //
  //     Please contact support@envitia.com if you require additional help
  //     on this topic.
  //
  double x1, y1, x2, y2;
  renderingInterface->latLonToADU(m_lat, m_lon, x1, y1);
  renderingInterface->latLonToADU(m_lat + 1.0, m_lon, x2, y2);

  // The distance for 1 degree of longitude changes with latitude.
  double halfSpan = fabs(y2 - y1) / 2.0;  // constant distance.

  // Set up the square.
  x1 = -halfSpan;
  x2 = halfSpan;
  y1 = -halfSpan;
  y2 = halfSpan;

  // Center point of square.
  double middleX, middleY;
  renderingInterface->latLonToADU(m_lat, m_lon, middleX, middleY);

  // Display scaling (Dynamic Arc).
  double scaleX, scaleY;
  double inverseScaleX = 1.0, inverseScaleY = 1.0;
  bool scaled = renderingInterface->displayScale(scaleX, scaleY);

  if (scaled)
  {
    // Calculate inverse scaling.
    inverseScaleX = 1.0/scaleX;
    inverseScaleY = 1.0/scaleY;
  }

  // We can either draw the square on the surface and by default have it
  // displayed with the surface rotation applied or we can undo the
  // surface rotation.
  if (m_rotate)
  {
    double viewRotation = renderingInterface->viewRotation();

    glTranslatef( middleX, middleY, 0.0);
    if (scaled)
    {
      // Undo the scaling to obtain an undistorted square.
      glScaled(inverseScaleX, inverseScaleY, 1.0);
    }
    glRotatef( -(float)viewRotation, 0.0, 0.0, 1.0 );
  }
  else
  {
    // Translate to final drawing position.
    glTranslatef( middleX, middleY, 0.0);

    if (scaled)
    {
      // Undo the scaling to obtain an undistorted square.
      glScaled(inverseScaleX, inverseScaleY, 1.0);
    }
  }

  // Draw the polygon.
  glBegin( GL_POLYGON );
  {
    // CCW order for points.
    glVertex2d( x1, y1 );
    glVertex2d( x2, y1 );
    glVertex2d( x2, y2 );
    glVertex2d( x1, y2 );
  }
  glEnd();

  // Restore the matrix to original top matrix.
  glPopMatrix();
  return true;
}
