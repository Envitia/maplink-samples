## The Skia-Based Drawing Surface

### Overview

The new core capability introduced in 11.3.1 is a Skia-backed drawing surface:

- **.NET API:** `TSLNSkiaDrawingSurface`  
- **C++ API:** `TSLSkiaSurface`  

The classes are added to the core:

- `Envitia.MapLink.MapLink64` (.NET 10, currently Windows-only)  
- `MapLink64` (C++)

### Cross-Platform Rendering Core

The surface is designed to operate consistently across:

- Windows  
- Linux  

By using Skia’s unified rendering model, drawing commands are translated into a consistent output regardless of the underlying OS.

Core rendering capabilities supported in the release include:
- Vector, raster and terrain 2D rendering.
- Loading of map data layers, standard data layers, and Direct Import data layers.
- All coordinate system transformations, visualisation options and rendering pipelines provided by the Core SDK.

### Off-Screen Rendering Model

The surface renders into a pixel buffer, rather than directly to a native window:

- Rendering target: bitmap / pixel buffer  
- No dependency on platform drawing contexts  

### Foundation for Future Platform Independence

By abstracting away native drawing APIs:

- MapLink rendering becomes UI-framework agnostic  
- Enables future targets such as:
  - WebAssembly  
  - Mobile platforms  
  - Cross-platform .NET UI frameworks  

This forms the foundation for the **code-once / deploy-anywhere** roadmap ambition.
