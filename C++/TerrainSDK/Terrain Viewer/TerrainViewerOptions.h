
#ifndef _TERRAINVIEWEROPTIONS_H_
#define _TERRAINVIEWEROPTIONS_H_

struct TerrainViewerOptions
{
  public:
    TerrainViewerOptions() {};
    ~TerrainViewerOptions() {};
    void load();
    void save();

    // Options for terrain profile display
    double m_manual_scale ;
    double m_terr_scale ;
    double m_bathy_scale ;
    bool m_terrainAutoscale ;
    bool m_bathy;

    // Options for single LOS calculation
    double m_losStartHeight;
    double m_losEndHeight;
    bool m_losFromSeaLevel;
    bool m_losEarthCurvature;

    // Options for area LOS calculation (viewshed)
    double m_viewShedMaximumRadius;
    // These Colours are stored as ARGB
    uint32_t m_viewShedColourVisible;
    uint32_t m_viewShedColourNotVisible;
    uint32_t m_viewShedColourNoData;

    TSLRGBA viewShedColourVisible() const
    {
      TSLRGBA result(
        (m_viewShedColourVisible & 0x00FF0000) >> 16,
        (m_viewShedColourVisible & 0x0000FF00) >> 8,
        (m_viewShedColourVisible & 0x000000FF) >> 0,
        (m_viewShedColourVisible & 0xFF000000) >> 24
      );
      m_viewShedColourVisibleTransparent ? result.m_a = 0x00 : result.m_a = 0xFF;
      return result;
    }

    TSLRGBA viewShedColourNotVisible() const
    {
      TSLRGBA result(
        (m_viewShedColourNotVisible & 0x00FF0000) >> 16,
        (m_viewShedColourNotVisible & 0x0000FF00) >> 8,
        (m_viewShedColourNotVisible & 0x000000FF) >> 0,
        (m_viewShedColourNotVisible & 0xFF000000) >> 24
      );
      m_viewShedColourNotVisibleTransparent ? result.m_a = 0x00 : result.m_a = 0xFF;
      return result;
    }

    TSLRGBA viewShedColourNoData() const
    {
      TSLRGBA result(
        (m_viewShedColourNoData & 0x00FF0000) >> 16,
        (m_viewShedColourNoData & 0x0000FF00) >> 8,
        (m_viewShedColourNoData & 0x000000FF) >> 0,
        (m_viewShedColourNoData & 0xFF000000) >> 24
      );
      m_viewShedColourNoDataTransparent ? result.m_a = 0x00 : result.m_a = 0xFF;
      return result;
    }

    double m_viewShedStartHeight;
    double m_viewShedEndHeight;
    bool m_viewShedStartRelativeToGround;
    bool m_viewShedEndRelativeToGround;
    int m_viewShedCurvatureBoxIndex;

    bool m_viewShedColourNoDataTransparent ;
    bool m_viewShedColourVisibleTransparent ;
    bool m_viewShedColourNotVisibleTransparent ;
};

#endif