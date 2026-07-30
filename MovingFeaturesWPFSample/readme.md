# The MapLink Pro Moving Features WPF Sample
[![MapLink Pro Developer Site](https://img.shields.io/badge/MapLink%20Pro%20Developer%20Site-84bd00)]([https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html](https://envitia.github.io/maplink-docs/))

## Introduction

This sample demonstrates 3 concepts:
- Creation of moving features using the Envitia.MapLink.Tracks .NET SDK. See [MovingFeaturesWPFSample/Tracks](MovingFeaturesWPFSample/Tracks/TracksLayer.cs)
- Creation of moving features using the Envitia.MapLink.DDO .NET SDK. See [MovingFeaturesWPFSample/ddo](MovingFeaturesWPFSample/ddo/DdoLayer.cs)
- Use of the MapLink Pro WPF control to display the map and features moving around it. See [MovingFeaturesWPFSample/MainWindow.xaml](MovingFeaturesWPFSample/MainWindow.xaml)

## Building and Running the Sample
1. You will need to have the MapLink Pro SDK installed on your machine. [Get an evaluation copy of MapLink Pro](https://forms.office.com/e/6ydUswfjEe)
1. Ensure your MAPL_PATH environment variable is set to the location of your MapLink Pro SDK installation's bin64 folder. This should have been set up by the MapLink Pro SDK installer.
1. Clone the repository to your local machine.
1. In Visual Studio 2026, open the solution file `MovingFeaturesWPFSample.sln`.
1. Build and Run the application.

## Application Instructions
The WPF application starts with a map and a countdown. When the countdown reaches zero, the application will start moving features on the map. The moving features are represented by tracks and DDOs (Dynamic Data Objects). The legend at the bottom left of the map indicates which colour corresponds to tracks and which colour corresponds to DDOs.

## Using MapLink Pro in a WPF Application
The MapLink Pro WPF control is a powerful tool for displaying maps and moving features in a WPF application. The control provides a rich set of features, including support for multiple layers, zooming, panning, and more. The control can be easily integrated into a WPF application by adding it to the XAML file and configuring its properties.
[Learn more about the MapLink Pro WPF control](https://envitia.github.io/maplink-docs/pages/features/cross-platform-surface#wpf-integration)

## Envitia MapLink Pro
Envitia’s software technology for mission system developers, enabling them to create high performance geospatial intelligence, situational awareness and map-based systems. Feature rich and proven in demanding operational systems, Envitia’s MapLink Pro provides system integrators and OEMs with the application control and flexibility they need while minimising delivery time and cost.

## Resources
[Envitia MapLink Pro Developer Site](https://envitia.github.io/maplink-docs/)
## Contributing
Everyone is welcome to contribute to this repository.
## Try Envitia MapLink Pro
[Get an evaluation copy of MapLink Pro](https://forms.office.com/e/6ydUswfjEe)
## Licensing
Copyright 2026 Envitia Ltd

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

A copy of the license is available in the repository's [LICENSE](LICENSE) file.
##
[![Envitia MapLink Pro](https://envitia.github.io/maplink-docs/img/MapLink%20Pro%2011%20Logo%20New%20V1.1.png)](https://www.envitia.com/for-developers/maplink-pro/)
