/******************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
===============================================================================*/
# include "stdafx_tlt.h"

#include "crc_32.h"
#include "callbacks.h"

// We mark the CRC in the file so that we can be sure that we have one.
// We also use this mark to make sure that we have read the CRC using the correct endian'ess.
const unsigned int TCRC = 0x54435243; // ASCII for TCRC
const unsigned int CRCT = 0x43524354; // ASCII for CRCT

// Size of the marker plus CRC - see the CreateMapCRCs program source
const int _size = (sizeof(unsigned int) + sizeof(unsigned int));

// Return value when a failure occurs.
const TSLLoaderCallbackReturn g_returnValueMap = TSLIgnoreFile; // TSLIgnoreFile or TSLBadFileStatus.
const TSLLoaderCallbackReturn g_returnValueDS  = TSLIgnoreFile; // TSLIgnoreFile or TSLBadFileStatus. 


// Methods to send information to the main application with. Required if using Multi-threaded loader.
static void sendMessage(MapFileLoadedTrackingData *trackingData, const char *fullPath, int messageNum)
{
#ifdef WIN32
  TSLSimpleString *filename = new TSLSimpleString(fullPath);
  ::PostMessage( trackingData->m_hwnd, messageNum, NULL, (LPARAM)filename );
#else
  sendMessage(trackingData->m_writeFifo, messageNum, (void*)fullPath, fullPath );
#endif
}
static void sendMessage(void *arg, const char *fullPath, int messageNum)
{
#ifdef WIN32
  TSLSimpleString *filename = new TSLSimpleString(fullPath);
  ::PostMessage( (HWND)arg, PWM_DRAWING_SURFACE_CRC_FAILURE, NULL, (LPARAM)filename );
#else
  sendMessage(*(int*)arg, PWM_DRAWING_SURFACE_CRC_FAILURE, (void*)fullPath, strlen(fullPath) );
#endif
}

// Method to check the CRC is valid - takes into account the endianness of the read CRC.
TSLLoaderCallbackReturn validateCRC(unsigned int crcMarker, unsigned int crc, unsigned int calculatedCRC)
{
  if (crcMarker != TCRC)
  {
    // Different endian'ness. Swap the values.
    unsigned char *s = (unsigned char *)&crc;
    unsigned int swapped;
    unsigned char *d = (unsigned char *)&swapped;       
    *d++ = s[3] ;
    *d++ = s[2] ;
    *d++ = s[1] ;
    *d++ = s[0] ;
    crc = swapped;
  }
  if (calculatedCRC != crc)
  {
    // The CRC check has failed.
    return g_returnValueMap;  // Return a bad file status - this file will be discarded.
  }
  return TSLContinue; // CRC check worked
}

//
// This is the callback for the loader added to the Drawing Surface.
//
// We have a different loader for the drawing surface as we want to handle files with no crc slightly differently.
//
// In addition maps may contain complex files which we have to handle slightly differently.
//
TSLLoaderCallbackReturn drawingSurfaceFileLoadCallback(void *arg, TSLifstream* stream, const char* fullPath, bool calledBeforeDecompression)
{
  // Query the size of the stream
  TSLFileLength lengthOfStream = stream->size();

  // Seek to the end minus the marker plus crc.
  stream->seekg(lengthOfStream - _size);

  // Read the marker plus CRC.
  unsigned int crcMarker;
  unsigned int crc;
  stream->read (&crcMarker, sizeof(unsigned int));
  stream->read (&crc, sizeof(unsigned int));

  // Check that we have a CRC
  UNS_32_BITS calculatedCRC = 0xffffffff;
  if (crcMarker != TCRC && crcMarker != CRCT)
  {
    static bool firstSeen = true;
    // The Drawing surface does not use complex file formats so we can safely say that
    // the file has no CRC.
    if (firstSeen)
    {
      // Just notify the user once. We normally only load the configuration files once.
      firstSeen = false;
      sendMessage(arg, fullPath, PWM_DRAWING_SURFACE_NO_CRC);
    }
    return TSLContinue; // We accept the file.
  }
  else
  {
    // We have a CRC in the file.
    calculatedCRC = crc32buf((const char *)stream->dataPointer(), (size_t)lengthOfStream - _size);

    // Remove the CRC and marker from the stream passed in.
    TSLFileOffset end = lengthOfStream - 1;
    TSLFileOffset start = lengthOfStream - _size;
    stream->erase(start, end);
  }
  TSLLoaderCallbackReturn retVal = validateCRC(crcMarker, crc, calculatedCRC);
  if (retVal != TSLContinue)
  {
    sendMessage(arg, fullPath, PWM_DRAWING_SURFACE_CRC_FAILURE);
    return g_returnValueDS;  // Return a bad file status - this file will be discarded.
  }
  return TSLContinue; // CRC check worked
}



static TSLLoaderCallbackReturn checkCRC(TSLifstream &stream, const string &filename, MapFileLoadedTrackingData *trackingData)
{
  // Query the size of the stream
  TSLFileLength lengthOfStream = stream.size();

  // Seek to the end minus the marker plus crc.
  stream.seekg(lengthOfStream - _size);

  unsigned int crcMarker;
  unsigned int crc;
  // Read the marker plus CRC.
  stream.read (&crcMarker, sizeof(unsigned int));
  stream.read (&crc, sizeof(unsigned int));
  if (crcMarker != TCRC && crcMarker != CRCT)
  {
    // No CRC marker so report first file seen.
    if (trackingData->m_firstSeen)
    {
      // Just notify the user once. If this is a map we could be quite a few of these.
      trackingData->m_firstSeen = false;
      sendMessage(trackingData, filename.c_str(), PWM_MAP_NO_CRC);
    }
  }
  else
  {
    // We have a CRC in the file
    UNS_32_BITS calculatedCRC = crc32buf((const char *)stream.dataPointer(), (size_t)lengthOfStream - _size);

    // We ignore the marker and CRC in the original stream as we have not read it.
    TSLLoaderCallbackReturn retVal = validateCRC(crcMarker, crc, calculatedCRC);

    if (retVal != TSLContinue)
    {
      sendMessage(trackingData, filename.c_str(), PWM_MAP_CRC_FAILURE);
      trackingData->m_filenames[filename] = g_returnValueMap;
      return retVal;
    }
  }
  trackingData->m_filenames[filename] = TSLContinue;
  return TSLContinue;
}

static TSLLoaderCallbackReturn checkCRC(FILE *file, TSLFileLength fileLength, const string &filename, MapFileLoadedTrackingData *trackingData)
{
  // Read the CRC marker and CRC
  TSLFileHelper::fseek(file, -_size, SEEK_END);
  unsigned int crcMarker;
  unsigned int crc;
  fread (&crcMarker, 1, sizeof(unsigned int), file);
  fread (&crc, 1, sizeof(unsigned int), file);

  if (crcMarker != TCRC && crcMarker != CRCT)
  {
    // No CRC Marker
    if (trackingData->m_firstSeen)
    {
      // Just notify the user once. If this is a map we could be quite a few of these.
      trackingData->m_firstSeen = false;
      sendMessage(trackingData, filename.c_str(), PWM_MAP_NO_CRC);
    }
    TSLFileHelper::fclose(file);
    return TSLContinue;
  }
  // Calculate the CRC.
  TSLFileHelper::fseek(file, 0, SEEK_SET);
  UNS_32_BITS calculatedCRC = 0xffffffff;
  if (!crc32fileSpecificBytesOnly(file, (size_t)fileLength-_size, calculatedCRC))
  {
    TSLFileHelper::fclose(file);
    sendMessage(trackingData, filename.c_str(), PWM_MAP_NO_CRC);
    return g_returnValueMap;
  }
  TSLFileHelper::fclose(file);

  // We ignore the marker and CRC in the original stream as we have not read it.
  TSLLoaderCallbackReturn retVal = validateCRC(crcMarker, crc, calculatedCRC);

  if (retVal != TSLContinue)
  {
    sendMessage(trackingData, filename.c_str(), PWM_MAP_CRC_FAILURE);
  }
  return retVal;
}

//
// This is the callback for the loader added to the Map Data-layer.
//
// Maps may contain complex files (Raster Pyramid files).
//
TSLLoaderCallbackReturn mapDataLayerFileLoadCallback(void *arg, TSLifstream* stream, const char* fullPath, bool calledBeforeDecompression)
{
  MapFileLoadedTrackingData *trackingData = (MapFileLoadedTrackingData*)arg;
  if (trackingData == NULL || trackingData->m_fileLoader == NULL)
  {
    // We need the information passed in so fail.
    return g_returnValueMap;
  }

  // We are using the same file loader as we added to the Map Data-layer so we need to
  // ensure we do not attempt to check the same file.
  if (trackingData->m_inFileLoader)
  {
    return TSLContinue;
  }

  //////////////////////////////////////////////////////////
  // Step 1. Check the stream for a Marker
  //////////////////////////////////////////////////////////


  // Query the size of the stream
  TSLFileLength lengthOfStream = stream->size();

  // Seek to the end minus the marker plus crc.
  stream->seekg(lengthOfStream - _size);

  // Read the marker plus CRC.
  unsigned int crcMarker;
  unsigned int crc;
  stream->read (&crcMarker, sizeof(unsigned int));
  stream->read (&crc, sizeof(unsigned int));

  // Check that we have a CRC
  UNS_32_BITS calculatedCRC = 0xffffffff;
  if (crcMarker != TCRC && crcMarker != CRCT)
  {
    //////////////////////////////////////////////////////////
    // Step 2. There is no CRC Marker - look for the original file
    //////////////////////////////////////////////////////////

    // This may be a complex file format - we need to read and check the file ourselves.
    //
    // We see if we have seen the file before. If we have then we can return straight away.
    // If we have not seen the file we do the normal checks.
    TSLSimpleString directory, basename, extension;
    TSLFileHelper::decomposePathname(fullPath, directory, basename, extension);

    string filename(basename.c_str());
    filename += ".";
    filename += extension.c_str();

    // NOTE: If you want to always check the file do not store or search for the filenames.
    map<string, TSLLoaderCallbackReturn, less<string> >::iterator it = trackingData->m_filenames.find(filename);
    if ( it != trackingData->m_filenames.end())
    {
      return (*it).second; // Seen the file before and it was OK.
    }

    // We have not seen the file before.
    TSLFileLength fileLength = TSLFileHelper::fileSize(fullPath);
    FILE *file = TSLFileHelper::fopen(fullPath, "rb");
    if (file == NULL)
    {
      //////////////////////////////////////////////////////////
      // Step 2.a. No Original file ask MapLink for the file
      //////////////////////////////////////////////////////////
      // This is likly to occur with Memory loader or Remote loader as the file will not
      // be on the local disk.
      //
      // We would then need to load the full file from a loader using loadImmediate.
      TSLifstream newStream;
      trackingData->m_inFileLoader = true; // we are calling the same loader that loaded the current file.
      TSLLoaderStatus status = trackingData->m_fileLoader->loadImmediate (NULL, fullPath, &newStream);
      trackingData->m_inFileLoader = false; // clear the flag or we will accept all files without checking.

      if (status == TSLLoadingOK)
      {
        // Check the CRC is valid... We do not drop through in this case.
        return checkCRC(newStream, filename, trackingData);
      }
      else
      {
        // We still could not find the file.
        sendMessage(trackingData, filename.c_str(), PWM_MAP_CANN0T_FIND_FILE);
        trackingData->m_filenames[filename] = g_returnValueMap;
        return g_returnValueMap;
      }
    }
    else
    {
      //////////////////////////////////////////////////////////
      // Step 2.b. We have found the original File. Check it.
      //////////////////////////////////////////////////////////
      TSLLoaderCallbackReturn retVal = checkCRC(file, fileLength, filename, trackingData);
      trackingData->m_filenames[filename] = retVal;
      return retVal;
    }
  }
  else
  {
    //////////////////////////////////////////////////////////
    // Step 3. There is a CRC Marker - Calculate the CRC
    //////////////////////////////////////////////////////////
    // We have a CRC in the file.
    calculatedCRC = crc32buf((const char *)stream->dataPointer(), (size_t)lengthOfStream - _size);

    // Remove the CRC and marker from the stream passed in.
    TSLFileOffset end = lengthOfStream - 1;
    TSLFileOffset start = lengthOfStream - _size;
    stream->erase(start, end); // Remove the CRC marker and CRC
  }
  //////////////////////////////////////////////////////////
  // Step 4. Validate the CRC matches the stored CRC.
  //////////////////////////////////////////////////////////
  TSLLoaderCallbackReturn retVal = validateCRC(crcMarker, crc, calculatedCRC);

  if (retVal != TSLContinue)
  {
    sendMessage(trackingData, fullPath, PWM_MAP_CRC_FAILURE);
  }

  return retVal;
}

