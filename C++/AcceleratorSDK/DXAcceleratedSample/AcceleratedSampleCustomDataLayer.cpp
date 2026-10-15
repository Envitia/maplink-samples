
#include "stdafx.h"
#include "acceleratedsamplecustomdatalayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




struct CUSTOMVERTEX
{
  D3DXVECTOR3 position ; // The position.
  D3DCOLOR color;        // The color.
} ;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)



bool AcceleratedSampleCustomDataLayer::drawLayer (const TSLAcceleratorEnvelope *extent, TSLAcceleratedRenderingInterface* renderingInterface)
{
  ////////////////////////////////////////////////////////////////////////////
  // This sample method draws a Red rectangle at Latitude and Longitude 55, 5.
  ////////////////////////////////////////////////////////////////////////////

  // We call unlockSurface() because we are going to use DirectX rendering.
  //
  // If we were going to use GDI we would call lockSurface().
  //
  // We do not need to pair the calls to unlock and lock as internally the calls are combined
  renderingInterface->unlockSurface();
  // Notes:
  // The extent is the bounding box for the map tile areas being drawn, so will be larger then the
  // actual area being displayed.
  //
  // The extent is defined in DirectX coordinates (the mapping is the one chossen by us).
  //
  // If the surface is rotated then the extent will be the expanded bounding box extent.
  //
  // We are only drawing 2D items so there is no depth buffer.
  //
  // Polygons are also flat shaded.

  //
  // Get the DirectX 9 device.
  //
  LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)renderingInterface->handleToDrawable();

  // if we need access to the full setup use the following:
  //
  //D3DPRESENT_PARAMETERS presentParameters;
  //LPDIRECT3DDEVICE9 device;
  //LPDIRECT3D9 lpD3D;
  //renderingInterface->handleToDirectXDrawingParameters(&lpD3D, &device, &presentParameters);

  // The world matrix already has the drawing surface rotation applied.
  //
  // We need to ensure that we put it back the same way for the next layer.
  D3DXMATRIX matrix;
  device->GetTransform( D3DTS_WORLD, &matrix );

  // Obtain the DirectX Device units for drawing the rectangle.
  // We draw around 0,0 to make translating and rotating simpler.
  //
  // Notes:
  //  1. The points returned by latLonToADU are un-rotated points, they
  //     are also Device Units for DirectX.
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
  double inverseScaleX, inverseScaleY;
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
  D3DXMATRIX world;
  if (m_rotate)
  {
    // Undo surface rotation.
    double viewRotation = renderingInterface->viewRotation();
    D3DXMATRIX rotate;
    D3DXMatrixRotationZ(&rotate, -D3DXToRadian(viewRotation));

    // final draw position.
    D3DXMATRIX translateToMiddle;
    D3DXMatrixTranslation(&translateToMiddle, middleX, middleY, 0.0 );

    // Undo the scaling to obtain an undistorted square.
    D3DXMATRIX scale;
    if (scaled)
    {
      D3DXMatrixScaling(&scale, inverseScaleX, inverseScaleY, 1.0);
    }
    else
    {
      D3DXMatrixIdentity( &scale );
    }

    // If you are coming from OpenGL compare the order with the OpenGL
    // sample.
    world = rotate * scale * translateToMiddle * matrix;
  }
  else
  {
    // Translate to final drawing position.
    D3DXMATRIX translateToMiddle;
    D3DXMatrixTranslation(&translateToMiddle,  middleX, middleY, 0.0 );

    // Undo the scaling to obtain an undistorted square.
    D3DXMATRIX scale;
    if (scaled)
    {
      D3DXMatrixScaling(&scale, inverseScaleX, inverseScaleY, 1.0);
    }
    else
    {
      D3DXMatrixIdentity( &scale );
    }
    world = scale * translateToMiddle * matrix;
  }
  // Set the world transform.
  device->SetTransform( D3DTS_WORLD, &world );

  if (!m_vertexBuffer)
  {
    // Create the DX resources.
    HRESULT result = device->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
      D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX,
      D3DPOOL_DEFAULT, &m_vertexBuffer, NULL );  
    if (FAILED(result))
    {
      // problem occured handle it
      return false;
    }
  }

  // The ADU values can change when we zoom or are using a Dynamic Arc map
  // so we need to update the corners of the rectangle.
  CUSTOMVERTEX* pVertices = NULL;
  // lock the vertex buffer 
  HRESULT result = m_vertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
  if (FAILED(result))
  {
    // problem occured handle it
    return false;
  }
  //0.0, 1.0     1.0, 1.0
  //0.0, 0.0     1.0, 0.0
  // Triangle strip
  //2      4
  //  |\  |
  //  | \ |
  //  |  \|
  //1       3

  pVertices[0].position = D3DXVECTOR3( x1, y1, 0.0 ) ;
  pVertices[0].color = D3DCOLOR_RGBA(255, 0, 0, 255 );

  pVertices[1].position = D3DXVECTOR3( x1, y2, 0.0 ) ;
  pVertices[1].color = D3DCOLOR_RGBA(255, 0, 0, 255 );

  pVertices[2].position = D3DXVECTOR3( x2, y1, 0.0 ) ;
  pVertices[2].color = D3DCOLOR_RGBA(255, 0, 0, 255 );

  pVertices[3].position = D3DXVECTOR3( x2, y2, 0.0 ) ;
  pVertices[3].color = D3DCOLOR_RGBA(255, 0, 0, 255 );

  // finished updating the vertex buffer
  m_vertexBuffer->Unlock();

  // Clear the texture.
  device->SetTexture( 0, 0 ) ;

  // Draw the polygon.
  device->SetStreamSource( 0, m_vertexBuffer, 0, sizeof(CUSTOMVERTEX) );
  device->SetFVF( D3DFVF_CUSTOMVERTEX );
  device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

  // Restore the matrix to original.
  device->SetTransform( D3DTS_WORLD, &matrix );
  return true;
}


void AcceleratedSampleCustomDataLayer::releaseResources (void *device)
{
  // If the device is lost (screensaver or other app)
  // we need to release the resources and start again.
  //LPDIRECT3DDEVICE9 direct3DDevice = (LPDIRECT3DDEVICE9)device;

  if (m_vertexBuffer)
  {
    m_vertexBuffer->Release();
    m_vertexBuffer = NULL;
  }
}
