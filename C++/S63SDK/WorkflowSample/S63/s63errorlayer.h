#ifndef _S63ERRORLAYER_H_
#define _S63ERRORLAYER_H_

//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include <fstream>
 
/////////////////////////////////////
// Test Plan Reference: 3.4.1
//  - Permanent warnings displayed via a custom layer.
/////////////////////////////////////

/*
  The S63 Error Layer is used to display permanent warnings on top
  of the currently displayed S63 data.

  It uses the known extents of the cell to display any warnings 
  at the bottom left corner of the visible cell extent. 
*/


class S63ErrorLayer : public TSLClientCustomDataLayer
{
public:

  /*
    Constructor
  */
  S63ErrorLayer(const map<pair<string, string>, TSLGeodeticExtent> &cellExtents, TSLCoordinateSystem * coordSys);

  /*
    Destructor
  */
  ~S63ErrorLayer ();

  /*	
    Provides callbacks to the application when the layer is redrawing.
  
    The developer should provide an implementation of this method to allow
    custom drawing within the application.
  
    Argument List:
  
    renderingInterface: Provides a handle to the drawing operations within a
    layer.
  
    extent: Provides the extent of the custom layer.
  
    The implementation should return true if the drawing suceeds, or false
    otherwise.
  */
  virtual bool drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope *extent, TSLCustomDataLayerHandler &handler);

  /*
    The developer should implement this method if picking support is required.

    Argument list description:

    extent:  the pick extent
    results:  the picking results

    Returns:
      false don't exit pick.
      true exit pick.
  */
  virtual bool pick (const TSLRenderingInterface*, const TSLEnvelope& extent, TSLPickResultSet* results);

  /*
    This method allows the errors displayed by the S63 Error Layer to
    be toggled on and off.

    Argument list description:

    errorFlag: A boolean to determine whether or not errors are disabled.
  */
  void disableErrors(const bool errorFlag);

  /*
    This method adds an error to the error list for a specified cell. 

    Argument list description:

    cellName: The name of the cell which has the error.

    dataServerID: The data server ID of the cell which has the error.

    additionalInformation: The error to display.
  */
  void addError( const char * cellName, const char * dataServerID, const char * additionalInformation );

  /*
    This notify error callback recieves errors from the S63 Workflow Client.

    Argument list description:

    s63ErrorCode: The code of the error.

    cellName: The name of the cell which has the error.

    dataServerID: The data server ID of the cell which has the error.

    additionalInformation: The error to display.

    arg: An additional user-defined argument.
  */
  static void notifyErrorCallback(const int s63ErrorCode, const char * cellName, const char * dataServerID, const char* additionalInformation, void * arg);

private:


  // A private default-constructor. This is not required for use.
  S63ErrorLayer();

  // A collection of cell extents, uniquely identified by a CellName/DataServer string pair.
  map< pair< string, string >, TSLGeodeticExtent> m_cellExtents;

  // A collection of cell errors, uniquely identified by a CellName/DataServer string pair.
  map< pair< string, string >, string> m_errorList;

  // A flag to determine whether or not errors have been disabled.
  bool m_disableErrors;
  
  // The coordinate system to use when displaying the errors and calculating extents.
  TSLCoordinateSystem * m_coordSys;

}; 

#endif