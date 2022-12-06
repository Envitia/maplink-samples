This sample has been built and tested against the following Qt versions:

 - 5.6
 - 5.8

On CentOS 7.3 using gcc 4.7 & 4.8.

The sample code should work with Qt version 5.0 and newer with at most minor changes to the code.



Known Issues
------------
The binary NVIDIA drivers work with no issues.
Mesa 12.1 using the i965 driver is know to crash on exit while deleting a OpenGL context created by Qt. This is an issue with Mesa.

The code for this sample to work on Windows is present but as of writing this readme the sample has not been built on Windows.
The TSLWGLSurface constructor being used within the code is known to not be the correct one.

The drawing surface option TSLOptionTileBufferedLayers may not work at present. The behaviour required for this to work is driver specific. As such I have commented out the capability until we can address the problems.

Damian Dixon, 6 April 2017
