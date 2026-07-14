# The MapLink Pro Moving Features WPF Sample
[![MapLink Pro Developer Site](https://img.shields.io/badge/MapLink%20Pro%20Developer%20Site-84bd00)]([https://www.envitia.com/technologies/products/maplink-pro/userguide/index.html](https://envitia.github.io/maplink-docs/))

## Introduction

This sample demonstrates 3 concepts:
- Creation of moving features using the Envitia.MapLink.Tracks .NET SDK. See [MovingFeaturesSample/Tracks](MovingFeaturesSample/Tracks/TracksLayer.cs)
- Creation of moving features using the Envitia.MapLink.DDO .NET SDK. See [MovingFeaturesSample/ddo](MovingFeaturesSample/ddo/DdoLayer.cs) 
- Embedding of a Windows.Forms panel inside a WPF application, to allow a MapLink drawing surface to draw into a WPF application. See [MovingFeaturesSample/overlaycontrols/OverlayControl.cs](MovingFeaturesSample/overlaycontrols/OverlayControl.cs)

## Application Instructions
The WPF application starts with a map showing the Western Seaboard of North America.

Pressing the Layers button causes a drop down menu to appear. Enable the DDO layer by selecting the "Orange DDO" option, and select the Tracks layer with the "Blue Tracks" option. These options cause moving tracks to be displayed on the map, using either the Tracks SDK or the DDO SDK.

## Using MapLink Pro in a WPF Application
The MapLink Map is drawn into a Windows Forms panel that is embedded within a WPF app.

MapLink does not yet have a WPF-native drawing surface, so we need to wrap a Windows.Forms drawing surface in a WindowsFormHost.
This approach, however, means that we cannot simply overlay WPF controls on the drawing surface, due to the "airspace issue": https://learn.microsoft.com/en-us/dotnet/desktop/wpf/advanced/technology-regions-overview?view=netframeworkdesktop-4.8
We solve this with the insertion of the OverlayControl, a solution devised by Saurabh Singh (https://www.codeproject.com/Tips/5326355/Airspace-Solution).
OverlayControl is a wrapper control where you can provide content to render on top of a WinformHost.
This container injects another WPF window (OverlayWindow) into the region supplying its own content.
The extended HwndHost is used to glue the WPF window into OverlayControl.
You can see this is working by the buttons that are drawn on top of the map - this would not be possible if the airspace issue had not been worked around.

The drawing surface is coupled with a [Windows Forms panel](DrawingSurfacePanel/MapViewerPanel.cs). This panel is wrapped in a WindowsFormHost to be displayed in the WPF application.



## Envitia MapLink Pro
Envitia’s software technology for mission system developers, enabling them to create high performance geospatial intelligence, situational awareness and map-based systems. Feature rich and proven in demanding operational systems, Envitia’s MapLink Pro provides system integrators and OEMs with the application control and flexibility they need while minimising delivery time and cost.

## Resources
[Envitia MapLink Pro Developer Site](https://envitia.github.io/maplink-docs/)
## Contributing
Everyone is welcome to contribute to this repository.
## Try Envitia MapLink Pro
[Get an evaluation copy of MapLink Pro](https://forms.office.com/e/6ydUswfjEe)
## Licensing
Copyright 2025 Envitia Ltd

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

A copy of the license is available in the repository's [LICENSE](LICENSE) file.
##
[![Envitia MapLink Pro](https://envitia.github.io/maplink-docs/img/MapLink%20Pro%2011%20Logo%20New%20V1.1.png)](https://www.envitia.com/for-developers/maplink-pro/)
