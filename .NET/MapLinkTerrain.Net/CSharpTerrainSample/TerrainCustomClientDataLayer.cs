using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Envitia.MapLink;
using Envitia.MapLink.Terrain;

namespace CSharpTerrainSample
{
  class TerrainCustomClientDataLayer : TSLNClientCustomDataLayer
  {
    private Bitmap m_bitmap = null;
    private TerrainOptionsSection m_options = null;

    public TerrainCustomClientDataLayer(TSLNTerrainDatabase db,
                                        TSLNDrawingSurface ds,
                                        TSLNMapDataLayer mdl,
                                        MapPanel mainFrm,
                                        TerrainOptionsSection options)
    {
      m_db = db;
      m_mdl = mdl;
      m_vs = new TSLNTerrainViewShed();
      m_ds = ds;
      m_mf = mainFrm;
      m_options = options;
    }

    public override bool drawLayer(TSLNRenderingInterface renderingInterface, TSLNEnvelope extent, TSLNCustomDataLayerHandler layerHandler)
    {
      if (!m_mf.isTerrainViewShedActive)
        return false;

      bool useHighestRes = m_options.UseAccurateLos;
      bool useDarkShaded = m_options.DarkShaded;
      int losAreadGridCells = m_options.LOSAreaGridCells;
      bool onlyFailedCells = true;

      double muBottomLeftX, muBottomLeftY, muTopRightX, muTopRightY;

      m_ds.TMCToMU(extent.bottomLeft.x, extent.bottomLeft.y, out muBottomLeftX, out muBottomLeftY);
      m_ds.TMCToMU(extent.topRight.x, extent.topRight.y, out muTopRightX, out muTopRightY);

      if ((m_vs.MUBottomLeftX != muBottomLeftX) || (m_vs.MUTopRightX != muTopRightX)
        || (m_vs.MUBottomLeftY != muBottomLeftY) || (m_vs.MUTopRightY != muTopRightY))
      {

        Rectangle rect = m_mf.DisplayRectangle;
        //the view extents have changed or we need new data
        int sizex = rect.Right - rect.Left;
        int sizey = rect.Bottom - rect.Top;
        sizex = sizex / losAreadGridCells;
        sizey = sizey / losAreadGridCells;

        TSLNTerrainDataItem[] data = null;
        int size = (sizex * sizey);

        //check the coordinates are ok for the terrain database
        TSLNCoordinateSystem csTerrain = m_db.coordinateSystem;
        TSLNCoordinateSystem csMap = m_mdl.coordinateSystem;
        double terrainBottomLeftX, terrainBottomLeftY, terrainTopRightX, terrainTopRightY;

        if ( csTerrain != null && csMap.id == csTerrain.id )
        {    
          terrainBottomLeftX=muBottomLeftX;
          terrainBottomLeftY=muBottomLeftY;
          terrainTopRightX=muTopRightX;
          terrainTopRightY=muTopRightY;
        }
        else
        {
          //using different coordinate systems      
          //convert the coordinates to the terrain database's system

          double latBottomLeftX,lonBottomLeftY,latTopRightX,lonTopRightY;

          //map coords to lat long
          csMap.MUToLatLong(muBottomLeftX, muBottomLeftY, out latBottomLeftX, out lonBottomLeftY, false, true);
          csMap.MUToLatLong(muTopRightX, muTopRightY, out latTopRightX, out lonTopRightY, false, true);

          //lat long to terrain coords
          m_db.latLongToMU(latBottomLeftX, lonBottomLeftY, out terrainBottomLeftX, out terrainBottomLeftY, false);
          m_db.latLongToMU(latTopRightX, lonTopRightY, out terrainTopRightX, out terrainTopRightY, false);
        }

        m_db.queryArea(terrainBottomLeftX, terrainBottomLeftY, terrainTopRightX, terrainTopRightY, sizex, sizey, out data, TSLNTerrainData_LEVEL.TSLNTerrainData_Min, useHighestRes, TSLNTerrainInterpolate.TSLNTerrainInterpolate_NONE);
        double[] dblData = new double[size];
        for (int i = 0; i < size; i++)
        {
          if (data[i].m_isNull)
          {
            dblData[i] = TSLNTerrainViewShed.nullDataValue;
          }
          else
          {
            dblData[i] = data[i].m_z;
            if ( dblData[i] > 5000.0 )
              dblData[i] = 0.0;
          }
        }

        double dynamicArcCorrection = 1.0; // No dymanic Arc so needs to be set to unity
        string csName = csMap.name.ToLower();
        if ( csName.Contains("dynamic arc") )
        {
          double centrelat, centrelon;
          double centrex = (muBottomLeftX + muTopRightX) / 2.0;
          double centrey = (muBottomLeftY + muTopRightY) / 2.0;
          csMap.MUToLatLong(centrex, centrey, out centrelat, out centrelon, false, true );
          dynamicArcCorrection = Math.Abs(Math.Cos(centrelat * Math.PI / 180.0)); // RCB
        }

        double gridSizeXInMU = Math.Abs(muTopRightX - muBottomLeftX) * dynamicArcCorrection / (double)sizex; /// RCB
        double gridSizeYInMU = Math.Abs(muTopRightY - muBottomLeftY) / (double)sizey;

        m_vs.setData(dblData, sizey, sizex, gridSizeXInMU, gridSizeYInMU, muBottomLeftX, muBottomLeftY, muTopRightX, muTopRightY);
      }
      int pixelx1, pixelx2;
      int pixely1, pixely2;
      m_ds.MUToDU(m_vs.MUBottomLeftX, m_vs.MUBottomLeftY, out pixelx1, out pixely1);
      m_ds.MUToDU(m_vs.MUTopRightX, m_vs.MUTopRightY, out pixelx2, out pixely2);

      TSLNTerrainViewShedData sheddata = m_vs.getData();

      if (pixely2 < pixely1)
      {
        int tmp = pixely1;
        pixely1 = pixely2;
        pixely2 = tmp;
      }

      int dxPixels = pixelx2 - pixelx1;
      int dyPixels = pixely2 - pixely1;

      double cellSizeX = (double)dxPixels / (double)sheddata.numColumns;
      double cellSizeY = (double)dyPixels / (double)sheddata.numRows;

      int xoffset = (int)(cellSizeX / 2);
      int yoffset = (int)(cellSizeY / 2);

      int numcols = dxPixels;
      int numrows = dyPixels;

      if (numcols == 0 || numrows == 0)
        return false;

      int crossSizeX = 1;
      int crossSizeY = 1;

      if (useDarkShaded)
      {
        crossSizeX = (int)cellSizeX;
        crossSizeY = (int)cellSizeY;
      }

      if (m_bitmap == null || (m_bitmap.Width != numcols || m_bitmap.Height != numrows))
      {
        m_bitmap = new Bitmap(numcols, numrows);
      }

      Graphics bg = Graphics.FromImage(m_bitmap);
      bg.FillRectangle(new SolidBrush(Color.White), new Rectangle(0, 0, numcols, numrows));
      bg.Dispose();

      m_bitmap.MakeTransparent(Color.White);

      Color[] colours = new Color[2];
      colours[0] = Color.FromArgb(255, 0, 0);
      colours[1] = Color.FromArgb(0, 255, 0);

      int noRows = sheddata.numRows;
      int noColumns = sheddata.numColumns;
      int[] accurateLOS = useHighestRes ? sheddata.getAccurateLos() : null;
      TSLNTerrainViewShedData.LOSTriStateEnum [] los = useHighestRes ? null : sheddata.getLos();

      int ACCURATE_LOS_CAN_SEE = 0x1;
      int ACCURATE_LOS_NO_DATA = 0x8;

      for (int colInd = 0; colInd < noColumns; ++colInd)
      {
        int x = (int)(colInd * cellSizeX + pixelx1);
        for (int rowInd = 0; rowInd < noRows; ++rowInd)
        {
          int cellIndex = rowInd * noColumns + colInd;
          int thisValue = 0 ;

          if (useHighestRes)
          {
            if ((accurateLOS[cellIndex] & ACCURATE_LOS_CAN_SEE) == ACCURATE_LOS_CAN_SEE)
            {
              thisValue = 1;
            }
            else if ((accurateLOS[cellIndex] & ACCURATE_LOS_NO_DATA) == ACCURATE_LOS_NO_DATA)
            {
              thisValue = 2;
            }
          }
          else
          {
            if ( los[cellIndex] == TSLNTerrainViewShedData.LOSTriStateEnum.LOSTriStateVisible )
            {
              thisValue = 1;
            }
            else if (los[cellIndex] == TSLNTerrainViewShedData.LOSTriStateEnum.LOSTriStateNoData)
            {
              thisValue = 2;
            }
          }

          if (onlyFailedCells && thisValue == 2) // Skip LOS if there's no data, otherwise show.
            continue;

          int y = (int)(rowInd * cellSizeY);

          if (useDarkShaded)
          {
            for (int crossX = 0; crossX < crossSizeX; ++crossX )
            {
              m_bitmap.SetPixel(x + crossX, dyPixels - (y + 1), colours[thisValue]);
            }

            for (int crossY = 0; crossY < crossSizeY; ++crossY )
            {
              m_bitmap.SetPixel(x + xoffset, dyPixels - (y + 1 + crossY), colours[thisValue]);
            }
          }
          else
          {
            m_bitmap.SetPixel(x, dyPixels - (y + 1), colours[thisValue]);
          }
        }
      }

      Graphics g = renderingInterface.graphics();

      g.DrawImage(m_bitmap, new Rectangle(pixelx1, pixely1, dxPixels, dyPixels), new Rectangle(0, 0, dxPixels, dyPixels), GraphicsUnit.Pixel);
      double radius_range = m_options.LOSAreaRangeMU;
      if (radius_range > 0)
      {
        int x = (int)(sheddata.losOriginX * cellSizeX + pixelx1 + xoffset);
        int y = (int)(sheddata.losOriginY * cellSizeY + yoffset);
        y = pixely2 - y;
        int xrange = (int)((radius_range) * cellSizeX / sheddata.cellDistanceMUInX); // RCB
        int yrange = (int)((radius_range) * cellSizeY / sheddata.cellDistanceMUInY);
        Pen pen = new Pen(Color.Blue);
        g.DrawEllipse(pen, x - xrange, y - yrange, xrange * 2, yrange * 2);
      }

      //don't dispose the graphics retrieved from the rendering interface

      return true;  
    }

    public TSLNTerrainViewShed viewShed
    {
      get
      {
        return m_vs;
      }
    }

    private TSLNTerrainDatabase m_db;
    private TSLNDrawingSurface m_ds;
    private TSLNMapDataLayer m_mdl;
    private MapPanel m_mf;
    private TSLNTerrainViewShed m_vs;
  }
}
