#pragma once

namespace MaplinkWinformsGLOverlay
{
  /// <summary>
  /// Interface class to pass your application opengl code [optional].
  /// </summary>
  public ref class ApplicationGLInterface abstract
  {
  public:
    /// <summary>
    /// Draw your native opengl [Do your magic here...].
    /// </summary>
    virtual void draweNativeGL() = 0;
  };
}