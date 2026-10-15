#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_
/******************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
===============================================================================*/

#ifndef WIN32
 using std::map;
 using std::string;
 using std::less;
#endif

class MapFileLoadedTrackingData
{
public:
  MapFileLoadedTrackingData()
    : m_fileLoader(NULL), m_firstSeen(true), m_inFileLoader(false)
#ifdef WIN32
    , m_hwnd(NULL)
#else
    , m_writeFifo(0)
#endif
  {
  }


#ifdef WIN32
  HWND m_hwnd; // Used for posting to the main GUI thread.
#else
  int m_writeFifo; // Used for sending a message to the main GUI.
#endif

  // This is used to load the full data if we can not find the data.
  TSLFileLoader *m_fileLoader;

  // If we have a file which contains raster data the raster tiles may be a compand file (pyramid).
  // The callback will only receive data for the current slice we are loading rather than the whole file.
  // We therefore record the files we have been asked to load so we can skip the CRC check on subsequent slices being loaded.
  map<string, TSLLoaderCallbackReturn, less<string> > m_filenames;

  // We only report missing CRC once in case we loaded a map without CRCs added.
  bool m_firstSeen;

  // We are using the same file loader to load a file when we can not find the file locally.
  // This flag stops us from doing a recursive load and check.
  bool m_inFileLoader;
};

// TSLLoaderFileLoadedCallback callback for the Drawing Surface Loader.
extern TSLLoaderCallbackReturn drawingSurfaceFileLoadCallback(void *arg, TSLifstream* stream, const char* fullPath, bool calledBeforeDecompression);

// TSLLoaderFileLoadedCallback callback for the Map Data-layer Loader.
extern TSLLoaderCallbackReturn mapDataLayerFileLoadCallback(void *arg, TSLifstream* stream, const char* fullPath, bool calledBeforeDecompression);

#endif
