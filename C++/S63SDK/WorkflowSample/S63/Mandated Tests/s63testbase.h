#ifndef _S63TESTBASE_H_
#define _S63TESTBASE_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

namespace S63OEMTests
{
  struct S63ConfigurationObject
  {
    string m_testDataLocation;
    string m_configurationLocation;
    string m_resultsDirectory;
  };

  class S63TestBase
  {
  public:

    explicit S63TestBase(const S63ConfigurationObject &configurationObject);
    virtual ~S63TestBase();

    void createMapLinkSurface();
    void destroyMapLinkSurface();
    bool drawToBitmap(const char* filename);
  protected:
    TSLS63DataLayerScaleBands* createScaleBands();
    bool setupFeatureRendering(TSLDrawingSurface* surface);
    bool writeBMP(HBITMAP bitmap, HDC hDC, const char* filename);

    string m_resultsPath;
    string m_configurationLocation;
    string m_dataLocation;

    TSLDrawingSurface *m_surface;
    HDC m_screen;
    HDC m_display;
    HBITMAP m_bitmap;
    HBITMAP m_oldBitmap;

    unsigned int m_dsWidth;
    unsigned int m_dsHeight;


    S63TestBase();
  };
};

#endif