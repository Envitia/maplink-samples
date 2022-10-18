# Envitia MapLink QPixmap Sample
[![API reference](https://img.shields.io/badge/MapLink%20Pro%20API%20Documentation-84bd00)](https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html) [![Blog](https://img.shields.io/badge/Envitia%20Blog-1F2A44)](https://www.envitia.com/category/the-envitia-blog/) 

## Introduction

This sample code demonstrates how to draw MapLink maps into off-screen QPixmaps before drawing the pixmap to screen.

## The Important Bit of Code
The important lines of code can be found in the DrawToPixmap method in [main.cpp](https://github.com/Envitia/maplink-samples/blob/main/Qt/qpixmapexample/main.cpp).

    // Create a bitmap in memory - this will be dependent on your operating system
    ...
    // Tell the drawing surface to draw to the bitmap.    
    drawingSurface->drawToHDC((TSLDeviceContext)displayDc, bottomLeft.first, bottomLeft.second, topRight.first, topRight.second, false);
    // Get the bits of the bitmap - this will be dependent on your operating system
    ...
    // Load the bitmap bits into a QImage...
    QImage image(lpBits, cr.width(), cr.height(), QImage::Format_RGB32);
    // ... which we then load into the pixmap.
    pixmap = QPixmap::fromImage(image);

## Try Envitia MapLink Pro
[Get an evaluation copy of MapLink Pro](mailto:info@envitia.com?subject=I%20want%20to%20evaluate%20MapLink%20Pro%20please)

## Resources
- [Envitia MapLink Pro API Documentation](https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html)
- [Qt](https://www.qt.io/)

## Build Instructions
Requires Qmake.
### Windows

 1. Update your local copy of [maplinkqtdefs.pri](../maplinkqtdefs.pri ) to point at your installed MapLink root folder (e.g. MAPLINK_ROOT_DIR="C:/my_installs/Envitia/MapLink Pro/11.0").
 2. Launch a Visual Studio x64 Native Tools Command Line.
 3. Navigate to the folder containing this readme.
 4. `qmake`

### Linux

 1. In the command line, navigate to your MapLink distribution directory.
 2. `. mapl_init.bash`
 3. Navigate to the folder containing this readme.
 4. `qmake`
 5. `make`

## Contributing
Everyone is welcome to contribute to this repository.

## Licensing
Copyright 2022 Envitia Ltd

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

A copy of the license is available in the repository's [LICENSE](../../LICENSE) file.
##
[![Envitia MapLink Pro](https://user-images.githubusercontent.com/60386764/159908069-b33f1ba7-6ad9-45d0-a872-dfd38dc40c91.png)](http://maplinkpro.com/)
