

# Envitia MapLink Pro Tracks SDK Sample
[![API reference](https://img.shields.io/badge/MapLink%20Pro%20API%20Documentation-84bd00)](https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html) [![Blog](https://img.shields.io/badge/Envitia%20Blog-1F2A44)](https://www.envitia.com/category/the-envitia-blog/) 

## Introduction

This sample code demonstrates usage of Envitia's MapLink Pro Tracks SDK. The sample is built using Qt.
[Read our blog post that describes how to code using the Tracks SDK.](https://www.envitia.com/2022/03/24/using-maplink-pro-to-track-dynamic-objects-in-real-time/)

## The Envitia MapLink Pro Tracks SDK
The challenge often encountered by teams developing embedded mapping applications is to have the ability to track many 1000’s of dynamic objects from various sensors in real-time.

This is a requirement common to most types of operational type applications whether being C4ISR, GPS tracking of emergency vehicles being used by first responders, displaying the location of UAVs and IoT applications using input from many types of dynamic and static sensors.

Envitia's MapLink Pro Tracks SDK provides the ability to quickly and easily visualise your moving objects or sensor feeds, with real-time location and attribute update. You can display breadcrumbs to show where your tracked objects have been or are projected to go in the future, with a playback feature so that the tracks can be recorded and replayed. You can configure the map to follow your tracked object and even re-project your map in real-time to ensure that the mapping information is as accurate as possible, wherever your objects take you in the world.

The Tracks SDK is available for C++ and C# developers and is available on Windows and Linux operating systems.

## Try Envitia MapLink Pro
[Get an evaluation copy of MapLink Pro](mailto:info@envitia.com?subject=I%20want%20to%20evaluate%20MapLink%20Pro%20please)

## Resources
- [Envitia MapLink Pro API Documentation](https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html)
- [Tracks SDK Developer Guide](https://www.envitia.com/technologies/products/maplink-pro/userguide/trackmanagerdeveloperguide_page.html)
- [Tracks SDK API Documentation](https://www.envitia.com/technologies/products/maplink-pro/userguide/group__apigroup__track__manager.html)
- [Qt](https://www.qt.io/)

## Build Instructions
Requires Qmake.
### Windows

 1. Update your local copy of [maplinkqtdefs.pri](../maplinkqtdefs.pri ) to point at your installed MapLink root folder (e.g. MAPLINK_ROOT_DIR="C:/my_installs/Envitia/MapLink Pro/11.0").
 2. Launch a Visual Studio x64 Native Tools Command Line.
 3. Navigate to the folder containing this readme.
 4. `qmake -t app qttrackmanagerexample.pro`
 5. `nmake` 

### Linux

 1. In the command line, navigate to your MapLink distribution directory.
 2. `. mapl_init.bash`
 3. Navigate to the folder containing this readme.
 4. `qmake qttrackmanagerexample.pro`
 5. `make`

## Contributing
Everyone is welcome to contribute to this repository.

## Licensing
Copyright 2022 Envitia Ltd

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free 
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more 
details.

You should have received a copy of the GNU Lesser General Public License 
along with this program. If not, see <https://www.gnu.org/licenses/>.

A copy of the license is available in the repository's [LICENSE](../../LICENSE) file.
##
[![Envitia MapLink Pro](https://user-images.githubusercontent.com/60386764/159908069-b33f1ba7-6ad9-45d0-a872-dfd38dc40c91.png)](http://maplinkpro.com/)
