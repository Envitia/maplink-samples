#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

//This implementation of the TSLS63DataClient runs from an ini file that dictates what
//order of calls should be received by the data client (although checking of this order
//is determined by the ignoreCallOrder parameter). The ini file also states the
//values that should be returned whenever a call is made to the data client
//
// Example ini file
//
// [General]
// TimeToReturn=<Time (seconds since blah blah) or the string "Current">
//
// [CallOrder]
// Call0=...
// Call1=..
// ; Where the value of each CallN parameter maybe one of the following
// ;    attachmentForCell;<Expected filename>;<Expected cellName>;<Expected dataServerID>
// ;    cellCancelled;<Expected cellName>;<Expected dataServerID>;<Expected replacement cell0>;<Expected replacement cell1>....
// ;    coordinateSystemForCell;<Expected cellName>;<Expected dataServerID>
// ;    currentCellVersion;<Expected cellName>;<Expected dataServerID>
// ;    currentTime
// ;    loadCellIntermediaryData;<Expected cellName>;<Expected dataServerID>
// ;    loadCellTMFData;<Expected cellName>;<Expected dataServerID>
// ;    locateCatalogueForCell;<Expected cellName>;<Expected dataServerID>;<Expected Exchange Set Number>
// ;    notifyError;<S63 Error number>;<Additional information (Leave blank to ignore)>
// ;    notifyWarning;<Additional information (Leave blank to ignore)>
// ;    populateEntityInformation NOTE YOU DON'T NEED TO ADD THIS ONE
// ;    storeIngestedCell;<Expected cellName>;<Expected dataServerID>;<Expected Lat BL>;<Expected Lon BL>;<Expected Lat TR>;<Expected Lon TR>;<Expected Cell Edition>;<Expected Cell Update Number>
//
// [attachmentForCell]
// ; Contains an entry for the expected inputs and the values to return
// filename0=...
// cellName0=...
// dataServerID0=..
// attachmentID0=...
// attachmentLookup0...
// ;etc...
//
// [cellCancelled]
// cellName0=...
// dataServerID0=..
// retval0=<Either 1 or 0 to indicate true or false return value>
// ;etc..
//
// [currentCellVersion]
// KeepCountOfAll=<Either 1 or 0 to indicate whether a count should be maintained for ALL cells>
// cellName0=...
// dataServerID0=..
// retval0=<Cell version to return (this can be used in place of keepCount>
// keepCount0=<Either 1 or 0 to indicate whether a count should be maintained>
// ;etc..
//
// [loadCellIntermediaryData]
// cellName0=...
// dataServerID0=...
// filename0=<location where data will be loaded from file>
// retval0=<value of enumeration to return>
// ;etc..
//
// [loadCellTMFData]
// cellName0=...
// dataServerID0=...
// filename0=<location where data will be loaded from file>
// retval0=<value of enumeration to return>
// ;etc..
//
// [locateCatalogueForCell]
// cellName0=...
// dataServerID0=...
// exchangeSetNo0=...
// locationOfCatalogue0=...
// retval0=<Either 1 or 0 to indicate true or false return value>
// ;etc..
//
// [storeIngestedCell]
// cellName0=
// dataServerID0=...
// dontSave0=<Either 1 or 0 to indicate whether we should both saving>
// tmffilename0=<location where data will be save to file>
// intfilename0=<location where data will be save to file>
// ;etc..

namespace S63OEMTests
{

  enum MethodEnum
  {
    MethodattachmentForCell = 0,
    MethodcellCancelled = 1,
    MethodcoordinateSystemForCell = 2,
    MethodcurrentCellVersion = 3,
    MethodcurrentTime = 4,
    MethodloadCellIntermediaryData = 5,
    MethodloadCellTMFData = 6,
    MethodlocateCatalogueForCell = 7,
    MethodnotifyError = 8,
    MethodnotifyWarning = 9,
    MethodpopulateEntityInformation = 10,
    MethodstoreIngestedCell = 11
  };

  class OrderObject
  {
  public:
    explicit OrderObject(MethodEnum orderType);
    virtual ~OrderObject() = 0;
    MethodEnum m_orderType;
  private:
    OrderObject(); // not required
  };

  class ScriptedS63DataClient : public TSLS63DataClient
  {
  public:
    //
    // inifileScript : ini file that the client will use
    // exchangeSetPrefix : This is the prefix for files such as the attachments
    //                     and catalogue location so that the ini file entries can
    //                     can use relative pathes
    // dataStorePrefix : The is the prefix for the tmf/intermediate data when
    //                   queries through the loadCellIntermediaryData and 
    //                   loadTMFData calls. It is also used when storing data
    // csys : Coordinate system
    // ignoreCallOrder : If true then the order that the calls are made to the data client are not validated
    ScriptedS63DataClient(const string& inifileScript, const string& exchangeSetPrefix, const string& dataStorePrefix, const TSLCoordinateSystem* csys, bool ignoreCallOrder);

    virtual ~ScriptedS63DataClient(void);

    //This method may be called to check if an error has occured up to this point
    //it differs from 'didItWork' as it doesn't check that all expected instructions
    //have been completed. Returns true on NO errors, false otherwise
    bool anyErrorSoFar(string* firstErrorEncountered = NULL) const;

    //This should only be called once all actions that are liable to cause a callback
    //have been completed as it no only checks whether all actions to this point
    //were completed fully but also, should the order be being checked, that all
    //expected actions have been completed. Returns true on NO errors, false otherwise
    bool didItWork(string* firstErrorEncountered = NULL) const;

    //Replaces current exchange set prefix
    void setExchangeSetPrefix( const string & newPrefix )
    {
      m_exchangeSetPathPrefix = newPrefix;
    }

    void resetCallOrder()
    {
      m_currentPositionInCallOrder = 0;
    }

    //	This callback will be invoked when the ingest discovers an attachment for a
    //	cell. You should populate the attachmentLookup parameter with a unique value
    //	that will allow lookup of the attachment at display time.
    //
    //	Argument list description:
    //
    //	filename: The full path to the filename of the attachment.
    //
    //	cellName: The name of the cell associated with the attachment.
    //
    //	dataServerID: The two-character ID of the data server that issued the cell.
    //
    //	attachmentID: This should be set to a two-character ID that can be used to
    //	retrieve the attachment lookup information at display time from an entity's
    //	dataset.
    //
    //	attachmentLookup: This should be set to a unique value that will allow
    //	lookup of the attachment at runtime.
    virtual void attachmentForCell (const char* filename, const char* cellName, const char* dataServerID, TSLSimpleString& attachmentID, TSLSimpleString& attachmentLookup);

    //## Operation: cellCancelled%4862516002BA
    //	This callback will be invoked when a cell within an exchange set is
    //	cancelled. A cancelled cell will recieve no further updates in future
    //	exchange sets and so should not be used for navigation purposes. A cancelled
    //	cell may optionally be retained in the exchange set, in which case it will
    //	continue to be available for display.
    //
    //	Argument list description:
    //
    //	cellName: The name of the cell that has been cancelled.
    //
    //	dataServerID: The two-character ID of the data server that issued the cell.
    //
    //	replacementCells: A cancelled cell may be replaced by one or more cells.
    //	This array contains the names of these replacement cells.
    //
    //	numberOfReplacementCells: The number of items in the replacementCells array.
    //
    //	Return true to keep the cancelled cell in the exchange set, or false to
    //	remove it.
    virtual bool cellCancelled (const char* cellName, const char* dataServerID, const char** replacementCells, int numberOfReplacementCells);

    virtual bool cellsInExtent (const TSLGeodeticExtent& extent, const char **&cellNames, const char **&dataServerIDs, int& numberOfCells);

    //## Operation: coordinateSystemForCell%485BBE700304
    //	This callback will be invoked during media ingest when a new base cell is
    //	ingested. An implementation of this method should return the TSLCoordinate
    //	System object that describes the coordinate system that the given cell is
    //	in. This coordinate system should have the TMC to MU value configured
    //	appropriately for the source data.
    //
    //	This callback will not be invoked if the return value from currentCell
    //	Version() indicated that a previous version of the cell exists. In this case
    //	the loadCellTMFData() callback will be invoked for the cell instead.
    //
    //	Argument list description:
    //
    //	cellName: The name of the cell
    //
    //	dataServerID: The two character ID of the data server that issued the cell.
    virtual const TSLCoordinateSystem* coordinateSystemForCell (const char* cellName, const char* dataServerID);

    //## Operation: currentCellVersion%483C20110249
    //	This callback will be invoked during the media ingest, started by TSLMedia
    //	IngestManager::loadExchangeSet(). Your implementation of this function
    //	should return the currently held version for the given cell that was
    //	imported during a previous ingest. If no previous version of the cell was
    //	ingested or no information is held on the current version of the cell then
    //	this function should return -1.
    //
    //	Argument list description:
    //
    //	cellName:  The name of the cell for which the last imported version number
    //	should be returned.
    //
    //	dataServerID: The two-character ID of the data server that issued the cell.
    virtual int currentCellVersion (const char* cellName, const char* dataServerID);

    //## Operation: currentTime%485255990180
    //	This callback will be invoked during media ingest and display of S63 data.
    //	Your implementation should return the current time expressed in UTC (number
    //	of seconds elapsed since midnight (00:00:00), January 1, 1970).
    //
    //	It is your responsibility to ensure the time is accurate by checking the
    //	time returned from the system clock against an external source such as a GPS
    //	unit if possible.
    virtual TSLTimeType currentTime ();

    virtual TSLGeodeticExtent currentExtent();

    //## Operation: loadCellIntermediaryData%483D662702AC
    //	This callback will either be invoked during the media ingest process when
    //	the return value from currentCellVersion() indicated that an older version
    //	of the cell was ingested previously. You should populate the intemediaryData
    //	argument with the data that represents the encrypted intermediary data that
    //	was returned by the last ingest of that cell.
    //
    //	Argument list description:
    //
    //	cellName: The name of the cell for which the result of the last ingest is
    //	wanted.
    //
    //	dataServerID: The two character data server ID of the data server that
    //	issued the cell.
    //
    //	intermediaryData: This should be populated with the intermediary data of the
    //	cell in question.
    //
    //	You should return the appropriate value from the TSLS63CellLoadResultEnum
    //	enumeration describing the result of the cell loading. If TSLS63CellLoad
    //	ResultDisregardAllCells is returned during an ingest then the ingest will be
    //	aborted.
    virtual TSLS63CellLoadResultEnum loadCellIntermediaryData (const char* cellName, const char* dataServerID, TSLS63EncryptedData* intermediaryData);

    //## Operation: loadCellTMFData%48592D8E00B2
    //	This callback will either be invoked during the runtime display and
    //	decryption process when MapLink requires the encrypted TMF data for that
    //	cell. You should populate the tmfData argument with the data that represents
    //	the encrypted tmf data that was returned by the last ingest of that cell.
    //
    //	Argument list description:
    //
    //	cellName: The name of the cell for which the result of the last ingest is
    //	wanted.
    //
    //	dataServer: The two character data server ID of the data server that issued
    //	the cell.
    //
    //	tmfData: This should be populated with the TMF data of the cell in question.
    //
    //	You should return the appropriate value from the TSLS63CellLoadResultEnum
    //	enumeration describing the result of the cell loading. If TSLS63CellLoad
    //	ResultDisregardAllCells is returned during an ingest then the ingest will be
    //	aborted.
    virtual TSLS63CellLoadResultEnum loadCellTMFData (const char* cellName, const char* dataServer, TSLS63EncryptedData* tmfData);

    //## Operation: locateCatalogueForCell%483D60490178
    //	This callback will be invoked for each cell that will be ingested. Your
    //	implementation of this function should populate the locationOfCatalogue and
    //	coordinateSystemToUseForCell fields with the full path to the location of
    //	the CATALOG.031 file and TSLCoordinateSystem to use for that cell
    //	respectively.
    //	If the CATALOG.031 file cannot be found you should have this function return
    //	false.
    //
    //	Argument list description:
    //	cellName: The name of the cell that is to be ingested.
    //
    //	dataServerID: The two-character ID of the data server that issued the cell.
    //
    //	exchangeSetNumber: The disk number in the exchange set for the cell on which
    //	the CATALOG.031 file should be located.
    //
    //	locationOfCatalogue: This should be set to the location of the CATALOG.031
    //	file associated with the given cell.
    virtual bool locateCatalogueForCell (const char* cellName, const char* dataServerID, int exchangeSetNumber, TSLSimpleString& locationOfCatalogue);

    //## Operation: notifyError%4828390901EE
    //	This function will be called whenever MapLink encounters an error that is
    //	described in the S63 standard.
    virtual void notifyError (TSLS63ErrorCodeEnum s63ErrorCode, const char* cellName = NULL, const char* dataServerID = NULL, const char* additionalInformation = NULL);

    //## Operation: notifyWarning%4859157D0016
    void notifyWarning (const char* additionalInformation, const char* cellName = NULL, const char* dataServerID = NULL);

    //## Operation: populateEntityInformation%483D871E0088
    //	This callback will be invoked for each entity created from an ingested cell.
    //	You should populate the feature ID, render level and any other information
    //	you consider necessary.
    //	Should you wish to add any associate primitives, these can be added to the
    //	provided TSLEntitySet.
    //
    //	Argument list description:
    //	entity: The entity to populate with additional information.
    //	entitySet: A TSLEntitySet to which associated primitives can be added.
    virtual void populateEntityInformation (TSLEntity* entity, TSLEntitySet* entitySet, const char *cellName, const char *dsID);

    //## Operation: progress%48760C060382
    //	This callback will be invoked during media ingest to report on the progress
    //	of the ingest for both the exchange set being imported and the current cell
    //	being processed within the set.
    //
    //	Argument list description:
    //
    //	percentOfExchangeSetDone: This will contain the percentage of the current
    //	exchange set that has been ingested so far. This indicates the amount of
    //	data the ingest still has to process
    //
    //	percentOfCurrentCellDone: This will contain the percentage of the ingest for
    //	the current cell being processed that has been completed. This value
    //	includes the application of any applicable updates to the cell.
    //
    //	To abort the entire ingest, return false. To continue the ingest return true.
    virtual bool progress (double percentOfExchangeSetDone, double percentOfCurrentCellDone);

    //## Operation: storeIngestedCell%483D88910217
    //	This callback will be invoked after a cell has been ingested and the result
    //	needs to be stored. This will happen for each cell ingested by the ingest
    //	manager.
    //
    //	Argument list description:
    //
    //	cellTMF: This contains the encrypted TMF to be stored. This memory is owned
    //	by the media ingest manager. MapLink will require this data when displaying
    //	the cell
    //
    //	cellIntermediaryData: This contains the encrypted intermediary data to be
    //	stored. This memory is owned by the media ingest manager. MapLink will
    //	require this data when apllying updates to the cell during a future ingest.
    //
    //	cellName: The name of the cell.
    //
    //	dataServerID: The two-character ID of the data server that issued the cell.
    //
    //	cellExtent: The extent of the cell in latitude/longitude.
    //
    //	cellEdition: The base edition of the cell.
    //
    //	cellUpdateNumber: The update number of the last cell update ingested.
    virtual void storeIngestedCell (const TSLS63EncryptedData* cellTMF, const TSLS63EncryptedData* cellIntermediaryData, const char* cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber);

    // Methods for querying the state, for use after an ingest
    unsigned int numIngestedCells();
    const char* getIngestedCellName(unsigned int index);
    bool getIngestedCellInfo(const char* cellName, int& edition, int& update);

  protected:

    void raiseError( const char * err )
    {
      if ( m_didItWork )
      {
        m_didItWork = false;
        m_firstErrorEncountered = err;
      }
    }

    const OrderObject* checkOrder( const MethodEnum expectedMethod );

    // The versions of each cell, on a per-DS basis
    // Used by the data client to provide the current cell versions
    map<string, map<string, Int, less<string> >, less<string> > m_cellVersionMap;
    
    // The versions of each cell, for any DS
    // Used in the tests to query the state of the ingest process.
    // string is cell name
    // pair.first is edition
    // pair.second is update
    std::map<std::string, std::pair<int, int> > m_combinedCellVersionMap;

    bool m_didItWork;
    string m_firstErrorEncountered;
    vector<OrderObject*> m_callOrder;
    bool m_ignoreCallOrder;
    int m_currentPositionInCallOrder;
    const TSLCoordinateSystem * m_csys;
    string m_exchangeSetPathPrefix;
    string m_dataStorePathPrefix;
    string m_iniFile;
    TSLTimeType m_timeToUse;

  private:

    ScriptedS63DataClient(); // not required

    std::map<MethodEnum, unsigned int> m_numCallsToMethod;

  };

};