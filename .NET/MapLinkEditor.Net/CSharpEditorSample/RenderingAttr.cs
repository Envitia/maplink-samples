using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace CSharpEditorSample
{
    // See Envitia.MapLink.TSLNRenderingAttributes for details of these fields.
    // Not using that class in GUI controls to avoid weird errors in VS's form designer
    // ("Could not load file or assembly 'Envitia.MapLinkd'...").
    public class RenderingAttr
    {
        // Borders (bordered polygon)
        public Color borderColour;
        public int borderWidth;                // TMC

        // Edges (lines, arcs etc.)
        public Color edgeColour;
        public int edgeStyle;
        public double edgeThickness;
        public int edgeThicknessUnits;         // TSLNDimensionUnits

        // Edges (polygons, ellipses etc.)
        public Color exteriorEdgeColour;
        public int exteriorEdgeStyle;
        public double exteriorEdgeThickness;
        public int exteriorEdgeThicknessUnits; // TSLNDimensionUnits

        // Fills (polygons, ellipses, rectangles etc.)
        public Color fillColour;
        public int fillStyle;

        // Symbols (aka markers, points)
        public Color symbolColour;
        public double symbolSizeFactor;
        public int symbolStyle;

        // Text labels
        public Color textBackgroundColour;
        public Color textBackgroundEdgeColour;
        public int textBackgroundMode;         // TSLNTextBackgroundMode
        public int textBackgroundStyle;
        public Color textColour;
        public int textFont;
        public int textHorizontalAlignment;    // TSLNHorizontalAlignment
        public double textSizeFactor;
        public int textSizeFactorUnits;        // TSLNDimensionUnits
        public int textVerticalAlignment;      // TSLNVerticalAlignment
    }
}