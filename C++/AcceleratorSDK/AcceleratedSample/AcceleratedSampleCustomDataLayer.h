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
  }

  //
  // drawLayer method.
  //
  // This method is called on every frame for the user to draw their own graphics on top
  // of the map.
  virtual bool drawLayer (const TSLAcceleratorEnvelope *extent, TSLAcceleratedRenderingInterface* renderingInterface);


  void unrotateRectangle()
  {
    m_rotate = !m_rotate;
  }



  double m_lat, m_lon;
  bool m_rotate;
};
