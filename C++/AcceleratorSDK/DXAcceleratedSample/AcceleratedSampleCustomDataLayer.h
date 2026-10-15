#pragma once



class AcceleratedSampleCustomDataLayer : TSLAcceleratedClientCustomDataLayer
{
public:
  // Constructor.
  AcceleratedSampleCustomDataLayer()  : TSLAcceleratedClientCustomDataLayer()
  {
    m_lat = 55.0;
    m_lon = 5.0;
    m_rotate = false;
    m_vertexBuffer = NULL;
  }

  ~AcceleratedSampleCustomDataLayer()
  {
    if(m_vertexBuffer)
    {
      m_vertexBuffer->Release();
      m_vertexBuffer = NULL;
    }
  }

  //
  // drawLayer method.
  //
  // This method is called on every frame for the user to draw their own graphics on top
  // of the map.
  virtual bool drawLayer (const TSLAcceleratorEnvelope *extent, TSLAcceleratedRenderingInterface* renderingInterface);

  //
  // This method is called when we have to release resources
  //
  virtual void releaseResources (void *device);

  void unrotateRectangle()
  {
    m_rotate = !m_rotate;
  }

private:
  double m_lat, m_lon;
  bool m_rotate;

      LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;

};
