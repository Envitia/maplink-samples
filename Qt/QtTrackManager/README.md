

# Envitia MapLink Pro Tracks SDK Sample
[![API reference](https://img.shields.io/badge/MapLink%20Pro%20API%20Documentation-84bd00)](https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html) [![Blog](https://img.shields.io/badge/Envitia%20Blog-1F2A44)](https://www.envitia.com/category/the-envitia-blog/) 

## Introduction

This sample code demonstrates usage of Envitia's MapLink Pro Tracks SDK. The sample is built using Qt.

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
## Try Envitia MapLink Pro
[Get an evaluation copy of MapLink Pro](mailto:info@envitia.com?subject=I%20want%20to%20evaluate%20MapLink%20Pro%20please)
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
[![Envitia MapLink Pro](https://www.envitia.com/technologies/products/maplink-pro/userguide/maplinkpro.png)](http://maplinkpro.com/)