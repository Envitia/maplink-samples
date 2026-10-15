#ifndef _S57WORKFLOWCLIENT_H_
#define _S57WORKFLOWCLIENT_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63workflowclient.h"
#include "MapLinkS52.h"


using namespace std;

// The callback which will be invoked when a new cell requires ingest.
typedef bool (*IngestCellCallback)(const TSLSimpleString& cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber);

// The callback which will be hit when a new error is raised.
typedef void (*NotifyErrorCallback)(int s63ErrorCode, const char* cellName, const char * dataServerID, const char* additionalInformation, void * arg);

// The callback which will be invoked when a cell has been cancelled 
typedef bool (*CancelledCellCallback)(const char* cellName, const char* dataServerID, const char ** cellReplacements, int numberOfCells);

// The callback which will be invoked when progress information is being reported.
typedef bool (*ProgressCallback)(double percentOfExchangeSetDone, double percentOfCurrentCellDone, void *arg);

class S57WorkflowClient : public TSLS57DataClient
{
public:
  
  /*
    Constructor

    Argument list description:

    dataStore: The path to a data store to be populated

    currentTime: The time to return in the currentTime() callback. 
                 This value will be used to determine whether or not data is 'in date'.
  */
  S57WorkflowClient(const string &dataStore, const long long currentTime, TSLDrawingSurface *surface);
  
  /*
    Destructor
  */
  ~S57WorkflowClient();

  /*
    This callback will be invoked when the ingest discovers an attachment for a
    cell. You should populate the attachmentLookup parameter with a unique value
    that will allow lookup of the attachment at display time.

    Argument list description:

    filename: The full path to the filename of the attachment.

    cellName: The name of the cell associated with the attachment.

    dataServerID: The two-character ID of the data server that issued the cell.

    attachmentID: This should be set to a two-character ID that can be used to
    retrieve the attachment lookup information at display time from an entity's
    dataset.

    attachmentLookup: This should be set to a unique value that will allow
    lookup of the attachment at runtime.
  */
  virtual void attachmentForCell (const char* filename, const char* cellName, const char* dataServerID, TSLSimpleString& attachmentID, TSLSimpleString& attachmentLookup);

  /*
    This callback will be invoked when a cell within an exchange set is
    cancelled. A cancelled cell will recieve no further updates in future
    exchange sets and so should not be used for navigation purposes. A cancelled
    cell may optionally be retained in the exchange set, in which case it will
    continue to be available for display.
  
    Argument list description:

    cellName: The name of the cell that has been cancelled.

    dataServerID: The two-character ID of the data server that issued the cell.

    replacementCells: A cancelled cell may be replaced by one or more cells.
    This array contains the names of these replacement cells.

    numberOfReplacementCells: The number of items in the replacementCells array.

    Return true to keep the cancelled cell in the exchange set, or false to
    remove it.
  */
  virtual bool cellCancelled (const char* cellName, const char* dataServerID, const char** replacementCells, int numberOfReplacementCells);

  /*
    This callback will be invoked during media ingest when a new base cell is
    ingested. An implementation of this method should return the TSLCoordinate
    System object that describes the coordinate system that the given cell is
    in. This coordinate system should have the TMC to MU value configured
    appropriately for the source data.
  
    This callback will not be invoked if the return value from currentCell
    Version() indicated that a previous version of the cell exists. In this case
    the loadCellTMFData() callback will be invoked for the cell instead.
  
    Argument list description:
  
    cellName: The name of the cell
  
    dataServerID: The two character ID of the data server that issued the cell.
  */
  virtual const TSLCoordinateSystem* coordinateSystemForCell (const char* cellName, const char* dataServerID);

   /*     
    This callback will be invoked when drawing the S57 dataset. This callback
    should return an array of cell names and their respective data server IDs
    which fall inside the provided extent. This will indicate to the data layer
    the cells which should be drawn, provided they fall into the current scale
    band.
      
    Returning true will indicate that cells do fall in the extent. Returning
    false will indicate the cells do not fall within the provided extent.
      
    Argument list description:
    extent: The extent which the cells should fall in.
      
    cellNames: An array of cell names which fall inside the extent parameter.
    This variable should be populated by the callback.
      
    dataServerIDs: An equivalent data server IDs of the cells which fall inside
    the extent parameter. This variable should be populated by the callback.
      
    numberOfCells: The number of cells which fall inside the extent.
  */
  virtual bool cellsInExtent (const TSLGeodeticExtent& extent, const char **&cellNames, const char **&dataServerIDs, int& numberOfCells);

  /*
    This callback will be invoked during the media ingest, started by TSLMedia
    IngestManager::loadExchangeSet(). The implementation of this function should
    return the currently held version for the given cell that was imported
    during a previous ingest. If no previous version of the cell was ingested or
    no information is held on the current version of the cell then this method
    should return -1.
  
    Argument list description:
  
    cellName:  The name of the cell for which the last imported version number
    should be returned.
  
    dataServerID: The two-character ID of the data server that issued the cell.
  */
  virtual int currentCellVersion (const char* cellName, const char* dataServerID);
  
  /*
    This callback will be invoked during display of S57 data.
      
    The implementation of this method should return the full extent of all the
    cells in the current S57 data store.
  */
  virtual const TSLGeodeticExtent& dataStoreExtent ();

  /*
    This callback will be invoked during media ingest and display of S63 data.
    The implementation of this method should return the current time expressed
    in UTC (number of seconds elapsed since midnight (00:00:00), January 1,
    1970).
  
    It is the OEM's responsibility to ensure the time is accurate by checking
    the time returned from the system clock against an external source such as a
    GPS unit if possible.
  */
  virtual TSLTimeType currentTime ();

  /*
    This callback will either be invoked during the media ingest process when
    the return value from currentCellVersion() indicated that an older version
    of the cell was ingested previously. The implementation of this method
    should populate the intemediaryData argument with the data that represents
    the encrypted intermediary data that was returned by the last ingest of that
    cell.
  
    Argument list description:
  
    cellName: The name of the cell for which the result of the last ingest is
    wanted.
  
    dataServerID: The two character data server ID of the data server that
    issued the cell.
  
    intermediaryData: This should be populated with the intermediary data of the
    cell in question.
  
    The implementation of this function should return the appropriate value from
    the TSLS63CellLoadResultEnum enumeration describing the result of the cell
    loading. If TSLS63CellLoadResultDisregardAllCells is returned during an
    ingest then the ingest will be aborted.
  */
  virtual TSLS63CellLoadResultEnum loadCellIntermediaryData (const char* cellName, const char* dataServerID, TSLS63EncryptedData* intermediaryData);

  /*
    This callback will either be invoked during the runtime display and
    decryption process when MapLink requires the encrypted TMF data for that
    cell. The implementation of this method should populate the tmfData argument
    with the data that represents the encrypted tmf data that was returned by
    the last ingest of that cell.
  
    Argument list description:
    
    cellName: The name of the cell for which the result of the last ingest is
    wanted.
  
    dataServer: The two character data server ID of the data server that issued
    the cell.
  
    tmfData: This should be populated with the TMF data of the cell in question.
  
    The implementation of this method should return the appropriate value from
    the TSLS63CellLoadResultEnum enumeration describing the result of the cell
    loading. If TSLS63CellLoadResultDisregardAllCells is returned during an
    ingest then the ingest will be aborted.
  */
  virtual TSLS63CellLoadResultEnum loadCellTMFData (const char* cellName, const char* dataServer, TSLS63EncryptedData* tmfData);

  /*
    This callback will be invoked for each cell that will be ingested. The
    implementation of this method should populate the locationOfCatalogue and
    coordinateSystemToUseForCell fields with the full path to the location of
    the CATALOG.031 file and TSLCoordinateSystem to use for that cell
    respectively.
    If the CATALOG.031 file cannot be found the implementation of this method
    should return false.
  
    Argument list description:
    cellName: The name of the cell that is to be ingested.
  
    dataServerID: The two-character ID of the data server that issued the cell.
  
    exchangeSetNumber: The disk number in the exchange set for the cell on which
    the CATALOG.031 file should be located.
  
    locationOfCatalogue: This should be set to the location of the CATALOG.031
    file associated with the given cell.
  */
  virtual bool locateCatalogueForCell (const char* cellName, const char* dataServerID, int exchangeSetNumber, TSLSimpleString& locationOfCatalogue);

  /*
    This method will be called whenever MapLink encounters an error that is
    described in the S63 standard.
  
    Argument list description:
  
    s63ErrorCode: The error code from the S63 standard that defines the event
    that has occured. See the documentation for TSLS63ErrorCodeEnum and the S63
    specificaton for more information on these errors.
  
    cellName: For errors that relate to a particular cell this argument will
    contain the name of the cell for which the error occured. If no cell is
    associated with the error this argument will be NULL.
  
    dataServerID: For errors that relate to a particular data server this
    argument will contain the two-character ID of the data server for which the
    error occured. If no data server is associated with the error this argument
    will be NULL.
  
    additionalInformation: Any additional information about the cause error.
  */
  virtual void notifyError (TSLS63ErrorCodeEnum s63ErrorCode, const char* cellName, const char* dataServerID, const char* additionalInformation = NULL);

  /*
    This method will be called whenever MapLink encounters an error that is
    described in the S63 standard but for which there is not an associated S63
    error code. According to the S63 standard, the user should still be notified
    of these errors.
  
    Argument list description
  
    additionalInformation: Any additional information about the cause error.
  
    cellName: For errors that relate to a particular cell this argument will
    contain the name of the cell for which the error occured. If no cell is
    associated with the error this argument will be NULL.
  
    dataServerID: For errors that relate to a particular data server this
    argument will contain the two-character ID of the data server for which the
    error occured. If no data server is associated with the error this argument
    will be NULL.
  */
  virtual void notifyWarning (const char* additionalInformation, const char* cellName = NULL, const char* dataServerID = NULL);

  /*
    This callback will be invoked for each entity created from an ingested cell.
    The implementation of this method should populate the feature ID, render
    level and any other information you consider necessary.
  
    Any associated primitives for the entity can be added to the provided
    TSLEntitySet.
  
    Argument list description:
  
    entity: The entity to populate with additional information.
  
    entitySet: A TSLEntitySet to which associated primitives can be added.
  
    cellName: The name of the cell which contains the entity.
  
    dataServerID: The two character ID of the data server that issued the cell.
  */
  virtual void populateEntityInformation (TSLEntity* entity, TSLEntitySet* entitySet, const char* cellName, const char* dataServerID);

  /*
    This callback will be invoked during media ingest to report on the progress
    of the ingest for both the exchange set being imported and the current cell
    being processed within the set.
  
    Argument list description:
  
    percentOfExchangeSetDone: This will contain the percentage of the current
    exchange set that has been ingested so far. This indicates the amount of
    data the ingest still has to process
  
    percentOfCurrentCellDone: This will contain the percentage of the ingest for
    the current cell being processed that has been completed. This value
    includes the application of any applicable updates to the cell.
  
    To abort the entire ingest, return false. To continue the ingest return true.
  */
  virtual bool progress (double percentOfExchangeSetDone, double percentOfCurrentCellDone);

  /*
    This callback will be invoked after a cell has been ingested and the result
    needs to be stored. This will happen for each cell ingested by the manager.
  
    Argument list description:
  
    cellTMF: This contains the encrypted TMF to be stored. This memory is owned
    by the media ingest manager. MapLink will require this data both when
    displaying the cell and when importing updates to the cell.
  
    cellIntermediaryData: This contains the encrypted intermediary data to be
    stored. This memory is owned by the media ingest manager. MapLink will
    require this data when apllying updates to the cell during a future ingest.
  
    cellName: The name of the cell.
  
    dataServerID: The two-character ID of the data server that issued the cell.
  
    cellExtent: The extent of the cell in latitude/longitude.
  
    cellEdition: The base edition of the cell.
  
    cellUpdateNumber: The update number of the last cell update ingested. This
    will be required when updates for a cell are discovered during a subsequent
    ingest so that applicable updates to the cell held by the OEM can be
    determined.
  */
  virtual void storeIngestedCell (const TSLS63EncryptedData* cellTMF, const TSLS63EncryptedData* cellIntermediaryData, const char* cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber);

  /*
    This methods allow callbacks to be set which will be invoked when the notifyError()
    callback is hit.

    Argument List Description:

    callback: The callback to be invoked when a new error is recieved.

    arg: A user defined argument to accompany the callback.
  */

  void addErrorCallback(const NotifyErrorCallback callback, void * arg = NULL);

  /*
    This method allows the user to set whether or not to display the 
    Ingest Cell dialog when performing a media ingest.

    Argument List Description:

    displayIngestDialog: Whether or not the Ingest Cell dialog should 
    be displayed (true signifies that the dialog should be displayed).
  */
  void displayIngestDialog(const bool displayIngestDialog);

  /*
    This method sets the callback which will be invoked when a cell has been
    cancelled by an update.

    Argument List Description:

    callback: The callback to be invoked when a cell has been cancelled.
  */
  void setCancelledCellCallback(const CancelledCellCallback callback);

  /*
    This method sets the callback which will be invoked when the system
    recieves a new cell to be ingested.

    Argument List Description:

    callback: The callback to be invoked when the system wishes to ingest
              a new cell.
  */
  void setIngestCallback(const IngestCellCallback callback);
  
  /*
    This method allows users to specify a callback that will be called as the 
    data is processed by the S63 manager.

    Argument List Description:

    callback: The callback to call to report progress
  */
  void setProgressCallback(const ProgressCallback callback);

  /*
    This method sets the path to the SERIAL.ENC file to use in
    the next ingest.

    Argument List Description:

    serialPath: A path to the SERIAL.ENC file
  */
  void setSerialPath(const string &serialPath); 

  /*
    This method allows users to specify an argument which will be 
    provided to the progress callback. 

    Argument List Description:

    userArg: The argument to use with the callback
  */
  void setUserCallbackArgument( void *userArg );

  /*
    This method returns the lat/lon extents of the cells being managed by
    the data client.
  */
  map<pair<string, string>, TSLGeodeticExtent>& cellExtents();

  /*
    This method returns the collection of data servers which have cells 
    being managed by the data client.
  */
  vector<string>& dataServers();

protected:

  //HELPERS
  void constructBaseDirectory(const string &dataServer, const string &cellName, string &cellPath) const;
  bool fileExists(const TSLSimpleString &filePath) const;

  const char** allocateArrayPointers ( int numElements );
  void deallocateArrayPointers( const char** constCharArray );


  // Storage for the various callbacks being used by the data client, see the individual
  // declarations for further information.
  IngestCellCallback m_ingestCellCallback;
  CancelledCellCallback m_cancelledCellCallback;
  ProgressCallback m_progressCallback;

  // Coordinate system to return.
  TSLCoordinateSystem * m_coordSys;

  // A collection of error callbacks. This allows the user to specify multiple locations
  // to be alerted upon recieving a new error message.
  list< pair< NotifyErrorCallback, void * > > m_notifyErrorCallbacks;

  // User defined argument for callbacks
  void *m_userCallbackArgument;

  // The current time to use.
  long long m_currentTime;

  // Flag to signify whether or not to display the Ingest Cell dialog 
  // during a media ingest.
  bool m_displayIngestDialog;

  // The data store to output to.
  string m_dataStore;

  // The path to the serial file which is to be used in the next media ingest.
  string m_serialPath;

  // A collection of the data servers which have cells managed by the data client.
  vector<string> m_dataServers;

  // A collection of cell versions for cells being managed by this data client.
  map<pair<string, string>, int> m_cellVersions;

  // A collection of cell extents for cells being managed by this data client.
  map<pair<string, string>, TSLGeodeticExtent> m_cellExtents;

  // calculated extent
  TSLGeodeticExtent  m_extent;

  // Array pointers 
  const char** m_cellNames;
  const char** m_dataServerIDs;

};

#endif