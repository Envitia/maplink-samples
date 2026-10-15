//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "s63errorlayer.h"

S63ErrorLayer::S63ErrorLayer(const map<pair<string, string>, TSLGeodeticExtent> &cellExtents, TSLCoordinateSystem * coordSys)
  : TSLClientCustomDataLayer()
  , m_disableErrors(true)
  , m_coordSys(coordSys)
  , m_cellExtents(cellExtents)
  , m_errorList()
{
}

S63ErrorLayer::~S63ErrorLayer()
{
  if ( m_coordSys ) 
  {
    m_coordSys->destroy();
    m_coordSys = NULL;
  }
}

bool S63ErrorLayer::drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope *extent, TSLCustomDataLayerHandler &handler)
{
  // Only display errors if necessary.
  if ( !m_disableErrors )
  {
    // loop through all the errors.
    map< pair< string, string >, string>::const_iterator errorIterator;

    for ( errorIterator = m_errorList.begin() ; errorIterator != m_errorList.end(); ++errorIterator )
    {
      // Use the key from the error list to retrieve the cell extents
      map< pair< string, string >, TSLGeodeticExtent >::iterator extentIterator = m_cellExtents.find( errorIterator->first );

      if ( extentIterator == m_cellExtents.end() )
      {
        // If we don't find it, move on to the next error
        continue;
      }

      const TSLGeodeticExtent cellExtent = extentIterator->second;

      TSLTMC tmcX1, tmcX2, tmcY1, tmcY2;

      // Convert the lat/lon extent of the cell in question to TMC's.
      m_coordSys->latLongToTMC( cellExtent.latitudeBL(), cellExtent.longitudeBL(), &tmcX1, &tmcY1) ;
      m_coordSys->latLongToTMC( cellExtent.latitudeTR(), cellExtent.longitudeTR(), &tmcX2, &tmcY2) ;

      // Generate an envelope of the cell extent
      TSLEnvelope cellEnvelope( tmcX1, tmcY1, tmcX2, tmcY2 );

      // Check to see if the current drawing extent overlaps the cell extent
      if ( extent->overlaps(cellEnvelope) )
      {
        // If it does, we create an intersection to determine the visible extent
        const TSLEnvelope intersection = extent->intersection(cellEnvelope);

        // Setup text attributes - Make it red text in a black square of a fixed size.
        renderingInterface->setupTextAttributes(1, 181, 100.0, TSLDimensionUnitsPixels, 10, 10, 0.0, 0.0, TSLDimensionUnitsUndefined, 0.0, TSLHorizontalAlignmentLeft, TSLVerticalAlignmentFullBottom, TSLTextBackgroundModeRectangle, 1, 1);
        
        // Finally draw the text in the bottom left hand corner.
        renderingInterface->drawText(intersection.bottomLeft(), errorIterator->second.c_str());
      }
    }
  }

  return true;
}

bool S63ErrorLayer::pick (const TSLRenderingInterface*, const TSLEnvelope& extent, TSLPickResultSet* results)
{
  // Picking is disabled for errors.
  return true;
}

void S63ErrorLayer::notifyErrorCallback(const int s63ErrorCode, const char * cellName, const char * dataServerID, const char* additionalInformation, void * arg)
{
  // If we don't have an argument, we can't pass on our error
  if ( !arg )
  {
    // So we must return
    return;
  }

  // If we have an argument, we convert it to our non-static data layer
  S63ErrorLayer * errorLayer = (S63ErrorLayer*) arg;

  // And add the layer to the list 
  errorLayer->addError( cellName, dataServerID, additionalInformation);
}


void S63ErrorLayer::disableErrors(const bool errorFlag)
{
  m_disableErrors = errorFlag;
}
  
  
void S63ErrorLayer::addError( const char * cellName, const char * dataServerID, const char * additionalInformation )
{
  // Convert parameters to strings for consistency
  string cell(cellName);
  string dataServer(dataServerID);

  // Generate the error message
  string message("[");
  message += cellName;
  message += ", ";
  message += dataServerID;
  message += "] - ";
  message += additionalInformation;

  // See if an error has already been added for this cell.
  map< pair< string, string >, string>::iterator it = m_errorList.find( make_pair( cell, dataServer ) );
  
  if ( it != m_errorList.end() )
  {
    // If an error has already been added, append the error to the back of the list.
    message += "\n";
    message += it->second;
    m_errorList.erase(it);
  }

  // Add the error to the error list
  m_errorList.insert( make_pair( make_pair( cell, dataServer ), message ) ) ;
}
  
