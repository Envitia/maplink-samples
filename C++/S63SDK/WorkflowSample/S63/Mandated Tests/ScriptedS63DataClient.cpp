//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
#ifdef WINNT
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#endif

#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkS63.h"
#include "MapLinkS52.h"
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;
using std::less;

#include "ScriptedS63DataClient.h"


namespace S63OEMTests
{
  OrderObject::OrderObject(const MethodEnum orderType)
    : m_orderType(orderType)
  {
  }

  OrderObject::~OrderObject()
  {
  }

  //MethodattachmentForCell = 0,
  //MethodcellCancelled = 1,
  //MethodcoordinateSystemForCell = 2,
  //MethodcurrentCellVersion = 3,
  //MethodcurrentTime = 4,
  //MethodloadCellIntermediaryData = 5,
  //MethodloadCellTMFData = 6,
  //MethodlocateCatalogueForCell = 7,
  //MethodnotifyError = 8,
  //MethodnotifyWarning = 9,
  //MethodpopulateEntityInformation = 10,
  //MethodstoreIngestedCell = 11

  class AttachmentForCellOO : public OrderObject
  {
  public:
    AttachmentForCellOO(const string& filename, const string& cellName, const string& dsid )
      : OrderObject(MethodattachmentForCell), m_filename(filename), m_cellName(cellName), m_dsid(dsid)
    {}
    virtual ~AttachmentForCellOO(){}
    string m_filename, m_cellName, m_dsid;

  private:
    AttachmentForCellOO(); // not required

  };

  class CancelledCellOO : public OrderObject
  {
  public:
    CancelledCellOO(const string& cellName, const string& dsid, const vector<string>& expectedReplacementCells)
      : OrderObject(MethodcellCancelled), m_cellName(cellName), m_dsid(dsid), m_expectedReplacementCells(expectedReplacementCells)
    {}
    virtual ~CancelledCellOO(){}
    string m_cellName, m_dsid;
    vector<string> m_expectedReplacementCells;

  private:
    CancelledCellOO(); // not required

  };

  class CoordinateSystemForCellOO : public OrderObject
  {
  public:
    CoordinateSystemForCellOO(const string& cellName, const string& dsid )
      : OrderObject(MethodcoordinateSystemForCell), m_cellName(cellName), m_dsid(dsid)
    {}
    virtual ~CoordinateSystemForCellOO(){}
    string m_cellName, m_dsid;

  private:
    CoordinateSystemForCellOO(); // not required
  };

  class CurrentCellVersionOO : public OrderObject
  {
  public:
    CurrentCellVersionOO(const string& cellName, const string& dsid )
      : OrderObject(MethodcurrentCellVersion), m_cellName(cellName), m_dsid(dsid)
    {}
    virtual ~CurrentCellVersionOO(){}
    string m_cellName, m_dsid;

  private:
    CurrentCellVersionOO(); // not required
  };

  class CurrentTimeOO : public OrderObject
  {
  public:
    CurrentTimeOO() :OrderObject(MethodcurrentTime) {}
    virtual ~CurrentTimeOO(){}
  };

  class LoadCellIntermediaryDataOO : public OrderObject
  {
  public:
    LoadCellIntermediaryDataOO(const string& cellName, const string& dsid )
      : OrderObject(MethodloadCellIntermediaryData), m_cellName(cellName), m_dsid(dsid)
    {}
    virtual ~LoadCellIntermediaryDataOO(){}
    string m_cellName, m_dsid;

  private:
    LoadCellIntermediaryDataOO(); // not required
  };

  class LoadCellTMFDataOO : public OrderObject
  {
  public:
    LoadCellTMFDataOO(const string& cellName, const string& dsid )
      : OrderObject(MethodloadCellTMFData), m_cellName(cellName), m_dsid(dsid)
    {}
    virtual ~LoadCellTMFDataOO(){}
    string m_cellName, m_dsid;

  private:
    LoadCellTMFDataOO(); // not required
  };

  class LocatedCatalogueForCellOO : public OrderObject
  {
  public:
    LocatedCatalogueForCellOO(const string& cellName, const string& dsid, const int exchangeSetNo )
      : OrderObject(MethodlocateCatalogueForCell), m_cellName(cellName), m_dsid(dsid), m_exchangeSetNo(exchangeSetNo)
    {}
    virtual ~LocatedCatalogueForCellOO(){}
    string m_cellName, m_dsid;
    int m_exchangeSetNo;

  private:
    LocatedCatalogueForCellOO(); // not required
  };

  class NotifyErrorOO : public OrderObject
  {
  public:
    NotifyErrorOO(const int errorCode, const string& additionalInfo )
      : OrderObject(MethodnotifyError), m_s63errorCode(errorCode), m_additionalInfo(additionalInfo)
    {}
    virtual ~NotifyErrorOO(){}
    string m_additionalInfo;
    int m_s63errorCode;

  private:
    NotifyErrorOO(); // not required
  };

  class NotifyWarningOO : public OrderObject
  {
  public:
    explicit NotifyWarningOO(const string& additionalInfo )
      : OrderObject(MethodnotifyWarning), m_additionalInfo(additionalInfo)
    {}
    virtual ~NotifyWarningOO(){}
    string m_additionalInfo;

  private:
    NotifyWarningOO(); // not required
  };

  class PopulateEntityInformationOO : public OrderObject
  {
  public:
    PopulateEntityInformationOO() :OrderObject(MethodpopulateEntityInformation) {}
    virtual ~PopulateEntityInformationOO(){}
  };

  class StoreIngestedCellOO : public OrderObject
  {
  public:
    StoreIngestedCellOO(const string& cellName, const string& dsid, const double latBL, const double latTR, const double lonBL, const double lonTR, const int cellEdition, const int cellUpdateNumber )
      : OrderObject(MethodstoreIngestedCell), m_cellName(cellName), m_dsid(dsid)
      , m_latBL(latBL), m_latTR(latTR), m_lonBL(lonBL), m_lonTR(lonTR)
      , m_cellEdition(cellEdition), m_cellUpdateNumber(cellUpdateNumber)
    {}
    virtual ~StoreIngestedCellOO(){}
    string m_cellName, m_dsid;
    double m_latBL, m_latTR, m_lonBL, m_lonTR;
    int m_cellEdition, m_cellUpdateNumber;

  private:
    StoreIngestedCellOO(); // not required
  };

  bool splitStringBySemiColons (char *str, int expectedCount, ... )
  {
    va_list marker;

    const char* last = str;

    va_start( marker, expectedCount );     /* Initialize variable arguments. */
    for ( int i = 0; i < expectedCount; ++i )
    {
      if ( !last )
        return false;

      string* refstr = va_arg( marker, string*);
      const char* next = strchr(last, ';');
      if ( next )
      {
        *(char*)next = '\0';
      }
      *refstr = last;
      last = next ? next + 1 : next;
    }

    va_end( marker ); 

    return true;
  }

  bool splitCellCancelledString( char * str, string& cellName, string & dsid, vector<string>& replacements )
  {
    const char * last = str;
    const char * next = strchr(last, ';');

    if ( !next )
      return false;

    *(char*)next = '\0';
    cellName = last;
    last = next +1;

    next = strchr(last, ';');
    if ( !next )
      return false;

    *(char*)next = '\0';
    dsid = last;
    last = next +1;

    while ( (next = strchr(last, ';')) != 0 )
    {
      *(char*)next = '\0';
      replacements.push_back(last);
      last = next +1;
    }

    if ( strlen(last) > 0 )
      replacements.push_back(last);

    return true;
  }

  ScriptedS63DataClient::ScriptedS63DataClient(const string& inifileScript, const string& exchangeSetPrefix, const string& dataStorePrefix, const TSLCoordinateSystem* csys, bool ignoreCallOrder)
    : m_didItWork(true)
    , m_ignoreCallOrder(ignoreCallOrder)
    , m_currentPositionInCallOrder(0)
    , m_csys(csys)
    , m_iniFile(inifileScript)
    , m_exchangeSetPathPrefix(exchangeSetPrefix)
    , m_dataStorePathPrefix(dataStorePrefix)
    , m_cellVersionMap()
    , m_firstErrorEncountered()
    , m_callOrder()
    , m_timeToUse()
  {
    // reset call counts
    m_numCallsToMethod[MethodattachmentForCell] = 0;
    m_numCallsToMethod[MethodcellCancelled] = 0;
    m_numCallsToMethod[MethodcoordinateSystemForCell] = 0;
    m_numCallsToMethod[MethodcurrentCellVersion] = 0;
    m_numCallsToMethod[MethodcurrentTime] = 0;
    m_numCallsToMethod[MethodloadCellIntermediaryData] = 0;
    m_numCallsToMethod[MethodloadCellTMFData] = 0;
    m_numCallsToMethod[MethodlocateCatalogueForCell] = 0;
    m_numCallsToMethod[MethodnotifyError] = 0;
    m_numCallsToMethod[MethodnotifyWarning] = 0;
    m_numCallsToMethod[MethodpopulateEntityInformation] = 0;
    m_numCallsToMethod[MethodstoreIngestedCell] = 0;

    char buffer[1024];

    // TimeToReturn=<Time or the string "Current">


    if ( TSLProfileHelper::lookupProfile(inifileScript.c_str(), 
      "General",
      "TimeToReturn",
      buffer,
      1024) )
    {
      if ( stricmp(buffer, "Current") == 0 )
      {
        // Certificate has expired and there is no new one to use

        //_time64(&m_timeToUse);
        m_timeToUse = 1376819995; // 8/18/13 9:59:55 GMT
      }
      else
      {
        m_timeToUse = _atoi64(buffer);
      }
    }
    else
    {
      //_time64(&m_timeToUse);
      m_timeToUse = 1376819995; // 8/18/13 9:59:55 GMT
    }

    if ( !m_ignoreCallOrder )
    {

      int currentExtension = 0;


      while ( TSLProfileHelper::lookupProfile(inifileScript.c_str(), 
        "CallOrder", 
        "Call", 
        buffer, 
        1024, 
        "", 
        currentExtension)
        && buffer[0] != '\0' )
      {
        if( strnicmp(buffer, "attachmentForCell", 17) == 0 )
        {
          string filename, cellname, dsserverID;
          if ( splitStringBySemiColons(&buffer[18], 3, &filename, &cellname, &dsserverID ) )
          {
            m_callOrder.push_back( new AttachmentForCellOO(filename, cellname, dsserverID) );
          }
          else 
          {
            raiseError("ScriptedS63DataClient ini file is invalid (attachmentForCell invalid string)");
          }
        }
        else if ( strnicmp(buffer, "cellCancelled", 13) == 0 )
        {
          string cellname, dsserverID;
          vector<string> replacements;
          if ( splitCellCancelledString( &buffer[14], cellname, dsserverID, replacements) )
          {
            m_callOrder.push_back( new CancelledCellOO(cellname, dsserverID, replacements) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (cellCancelled invalid string)");
          }

        }
        else if ( strnicmp(buffer, "coordinateSystemForCell", 23) == 0 )
        {
          string cellname, dsserverID;
          if ( splitStringBySemiColons(&buffer[24], 2, &cellname, &dsserverID ) )
          {
            m_callOrder.push_back( new CoordinateSystemForCellOO(cellname, dsserverID) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (coordinateSystemForCell invalid string)");
          }
        }
        else if ( strnicmp(buffer, "currentCellVersion", 18) == 0 )
        {
          string cellname, dsserverID;
          if ( splitStringBySemiColons(&buffer[19], 2, &cellname, &dsserverID ) )
          {
            m_callOrder.push_back( new CurrentCellVersionOO(cellname, dsserverID) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (currentCellVersion invalid string)");
          }
        }
        else if ( strnicmp(buffer, "currentTime", 11) == 0 )
        {
          m_callOrder.push_back( new CurrentTimeOO() );
        }
        else if ( strnicmp(buffer, "loadCellIntermediaryData", 24) == 0 )
        {
          string cellname, dsserverID;
          if ( splitStringBySemiColons(&buffer[25], 2, &cellname, &dsserverID ) )
          {
            m_callOrder.push_back( new LoadCellIntermediaryDataOO(cellname, dsserverID) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (loadCellIntermediaryData invalid string)");
          }
        }
        else if ( strnicmp(buffer, "loadCellTMFData", 15) == 0 )
        {
          string cellname, dsserverID;
          if ( splitStringBySemiColons(&buffer[16], 2, &cellname, &dsserverID ) )
          {
            m_callOrder.push_back( new LoadCellTMFDataOO(cellname, dsserverID) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (loadCellTMFData invalid string)");
          }
        }
        else if ( strnicmp(buffer, "locateCatalogueForCell", 22) == 0 )
        {
          string cellname, dsserverID, exchangeSetNo;
          if ( splitStringBySemiColons(&buffer[23], 3, &cellname, &dsserverID, &exchangeSetNo ) )
          {
            m_callOrder.push_back( new LocatedCatalogueForCellOO(cellname, dsserverID, atoi(exchangeSetNo.c_str()) ) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (locateCatalogueForCell invalid string)");
          }
        }
        else if ( strnicmp(buffer, "notifyError", 11) == 0 )
        {
          string errNo, add;
          if ( splitStringBySemiColons(&buffer[12], 2, &errNo, &add ) )
          {
            m_callOrder.push_back( new NotifyErrorOO(atoi(errNo.c_str()), add) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (notifyError invalid string)");
          }
        }
        else if ( strnicmp(buffer, "notifyWarning", 13) == 0 )
        {
          string add;
          if ( splitStringBySemiColons(&buffer[14], 1, &add ) )
          {
            m_callOrder.push_back( new NotifyWarningOO(add) );
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (notifyWarning invalid string)");
          }
        }
        else if ( strnicmp(buffer, "populateEntityInformation", sizeof("populateEntityInformation")-1) == 0 )
        {
          m_callOrder.push_back( new PopulateEntityInformationOO() );
        }
        else if ( strnicmp(buffer, "storeIngestedCell", 17) == 0 )
        {
          string cellname, dsserverID, latbl, lonbl, lattr, lontr, celledition, cellupdateno;
          if ( splitStringBySemiColons(&buffer[18], 8, &cellname, &dsserverID, &latbl, &lonbl, &lattr, &lontr, &celledition, &cellupdateno ) )
          {
            m_callOrder.push_back( new StoreIngestedCellOO(cellname, dsserverID,
              atof(latbl.c_str()),
              atof(lattr.c_str()),
              atof(lonbl.c_str()),
              atof(lontr.c_str()),
              atoi(celledition.c_str()),
              atoi(cellupdateno.c_str())));
          }
          else
          {
            raiseError("ScriptedS63DataClient ini file is invalid (storeIngestedCell invalid string)");
          }
        }
        else
        {
          raiseError("ScriptedS63DataClient ini file is invalid (CallOrder encountered invalid string)");
        }
        ++currentExtension;
      }
    }
  }

  ScriptedS63DataClient::~ScriptedS63DataClient(void)
  {
    vector<OrderObject*>::iterator it(m_callOrder.begin()), itEnd(m_callOrder.end());
    for ( ; it != itEnd; ++it )
    {
      delete *it;
    }

    m_csys = 0;
  }

  bool ScriptedS63DataClient::anyErrorSoFar(string* firstErrorEncountered) const
  {
    if ( !m_didItWork && firstErrorEncountered )
    {
      *firstErrorEncountered = m_firstErrorEncountered;
    }
    return m_didItWork;
  }


  bool ScriptedS63DataClient::didItWork(string* firstErrorEncountered) const
  {
    if ( !m_didItWork )
    {
      if ( firstErrorEncountered )
      {
        *firstErrorEncountered = m_firstErrorEncountered;
      }
      return false;
    }

    return m_ignoreCallOrder || m_currentPositionInCallOrder >= (int)m_callOrder.size() ;
  }


  void ScriptedS63DataClient::attachmentForCell (const char* filename, const char* cellName, const char* dataServerID, TSLSimpleString& attachmentID, TSLSimpleString& attachmentLookup)
  {
    const AttachmentForCellOO* warOO = (const AttachmentForCellOO* )checkOrder(MethodattachmentForCell);
    if ( warOO &&
      (stricmp(filename        , (m_exchangeSetPathPrefix + warOO->m_filename).c_str()) ||
      stricmp(cellName        , warOO->m_cellName.c_str()) ||
      stricmp(dataServerID    , warOO->m_dsid.c_str()) ) )
    {
      raiseError("Expected ::attachmentForCell parameters do not match");
    }

    //from the filename we need to remove the m_pathPrefix
    string modifiedFilename(filename);
    if ( modifiedFilename.length() > m_exchangeSetPathPrefix.length() &&
      strnicmp(m_exchangeSetPathPrefix.c_str(), modifiedFilename.c_str(), m_exchangeSetPathPrefix.length()) == 0 )
    {
      modifiedFilename = filename + m_exchangeSetPathPrefix.length();
    }
        
    char buffer[1024];
    bool found = false;
    int currentExtension = 0;

    // If this setting is true, the search for response data will effictively ignore data from all previous calls to this method.
    // This setting must be true if multiple overlapping exchange sets are loaded into the same data client.
    if ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "attachmentForCell", 
      "startSearchAtCurrentCall", 
      buffer, 
      1024, 
      "0") && atoi(buffer) )
    {
      currentExtension = m_numCallsToMethod[MethodattachmentForCell];
    }
    m_numCallsToMethod[MethodattachmentForCell]++;

    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "attachmentForCell", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "attachmentForCell", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServerID) )
      {
        ++currentExtension;
        continue;
      }


      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "attachmentForCell", 
        "filename", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, modifiedFilename.c_str()) )
      {
        ++currentExtension;
        continue;
      }
      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received attachmentForCell call but couldn't find appropriate settings in ini file");
      return;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "attachmentForCell", 
      "attachmentID", 
      buffer, 
      1024, 
      "", 
      currentExtension);
    attachmentID = buffer;

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "attachmentForCell", 
      "attachmentLookup", 
      buffer, 
      1024, 
      "", 
      currentExtension);
    attachmentLookup = buffer;
  }

  bool ScriptedS63DataClient::cellsInExtent (const TSLGeodeticExtent& extent, const char **&cellNames, const char **&dataServerIDs, int& numberOfCells)
  {
    return false;
  }

  bool ScriptedS63DataClient::cellCancelled (const char* cellName, const char* dataServerID, const char** replacementCells, int numberOfReplacementCells)
  {
    const CancelledCellOO* warOO = (const CancelledCellOO* )checkOrder(MethodcellCancelled);
    if ( warOO )
    {
      if ( stricmp(cellName        , warOO->m_cellName.c_str()) ||
        stricmp(dataServerID    , warOO->m_dsid.c_str()) ||
        (int)warOO->m_expectedReplacementCells.size() != numberOfReplacementCells )
      {
        raiseError("Expected ::cellCancelled parameters do not match");
      }
      else
      {
        for ( int i = 0; i < numberOfReplacementCells; ++i )
        {
          if ( stricmp(replacementCells[i], warOO->m_expectedReplacementCells[i].c_str()) )
          {
            m_didItWork = false;
            m_firstErrorEncountered = "Expected ::cellCancelled parameters do not match";
            break;
          }
        }
      }
    }

    char buffer[1024];
    bool found = false;
    int currentExtension = 0;
    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "cellCancelled", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "cellCancelled", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServerID) )
      {
        ++currentExtension;
        continue;
      }
      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received cellCancelled call but couldn't find appropriate settings in ini file");
      return false;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "cellCancelled", 
      "retval", 
      buffer, 
      1024, 
      "", 
      currentExtension);
    return atoi(buffer) ? true : false;
  }

  const TSLCoordinateSystem* ScriptedS63DataClient::coordinateSystemForCell (const char* cellName, const char* dataServerID)
  {
    const CoordinateSystemForCellOO* warOO = (const CoordinateSystemForCellOO* )checkOrder(MethodcoordinateSystemForCell);
    if ( warOO &&
      (stricmp(cellName        , warOO->m_cellName.c_str()) ||
      stricmp(dataServerID    , warOO->m_dsid.c_str()) ) )
    {
      raiseError("Expected ::coordinateSystemForCell parameters do not match");
    }

    return m_csys;
  }

  int ScriptedS63DataClient::currentCellVersion (const char* cellName, const char* dataServerID)
  {
    const CurrentCellVersionOO* warOO = (const CurrentCellVersionOO* )checkOrder(MethodcurrentCellVersion);
    if ( warOO &&
      (!(stricmp(cellName, warOO->m_cellName.c_str()) == 0 || stricmp("?", warOO->m_cellName.c_str()) == 0)  ||
      stricmp(dataServerID, warOO->m_dsid.c_str()) ) )
    {
      raiseError("Expected ::currentCellVersion parameters do not match");
    }

    char buffer[1024];

    //This means that we will keep a count of all cells that are passed to the store Ingested cell functions
    if ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "currentCellVersion", 
      "KeepCountOfAll", 
      buffer, 
      1024, 
      "0") && atoi(buffer) )
    {
      map<string, int, less<string> >& dsVersionMap = m_cellVersionMap[cellName];
      map<string, int, less<string> >::iterator findIt(dsVersionMap.find(dataServerID));
      if ( findIt == dsVersionMap.end() )
      {
        return -1;
      }
      else
      {
        return findIt->second;
      }
    }

    bool found = false;
    int currentExtension = 0;

    // If this setting is true, the search for response data will effictively ignore data from all previous calls to this method.
    // This setting must be true if multiple overlapping exchange sets are loaded into the same data client.
    if ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "currentCellVersion", 
      "startSearchAtCurrentCall", 
      buffer, 
      1024, 
      "0") && atoi(buffer) )
    {
      currentExtension = m_numCallsToMethod[MethodcurrentCellVersion];
    }
    m_numCallsToMethod[MethodcurrentCellVersion]++;

    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "currentCellVersion", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "currentCellVersion", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServerID) )
      {
        ++currentExtension;
        continue;
      }
      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received currentCellVersion call but couldn't find appropriate settings in ini file");
      return -1;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "currentCellVersion", 
      "keepCount", 
      buffer, 
      1024, 
      "0", 
      currentExtension);
    if ( atoi(buffer) )
    {
      map<string, int, less<string> >& dsVersionMap = m_cellVersionMap[cellName];
      map<string, int, less<string> >::iterator findIt(dsVersionMap.find(dataServerID));
      if ( findIt == dsVersionMap.end() )
      {
        return -1;
      }
      else
      {
        return findIt->second;
      }
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "currentCellVersion", 
      "retval", 
      buffer, 
      1024, 
      "0", 
      currentExtension);
    return atoi(buffer);
  }

  TSLGeodeticExtent ScriptedS63DataClient::currentExtent ()
  {
    return TSLGeodeticExtent(0,0,0,0);
  }

  TSLTimeType ScriptedS63DataClient::currentTime ()
  {
    //we don't care about return value of check order as there is nothing to check
    checkOrder(MethodcurrentTime);
    return m_timeToUse;
  }

  TSLS63CellLoadResultEnum ScriptedS63DataClient::loadCellIntermediaryData (const char* cellName, const char* dataServerID, TSLS63EncryptedData* intermediaryData)
  {
    const LoadCellIntermediaryDataOO* warOO = (const LoadCellIntermediaryDataOO* )checkOrder(MethodloadCellIntermediaryData);
    if ( warOO &&
      (stricmp(cellName        , warOO->m_cellName.c_str()) ||
      stricmp(dataServerID    , warOO->m_dsid.c_str()) ) )
    {
      raiseError("Expected ::loadCellIntermediaryData parameters do not match");
    }

    char buffer[1024];
    bool found = false;
    int currentExtension = 0;
    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "loadCellIntermediaryData", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "loadCellIntermediaryData", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServerID) )
      {
        ++currentExtension;
        continue;
      }
      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received loadCellIntermediaryData call but couldn't find appropriate settings in ini file");
      return TSLS63CellLoadResultNotFound;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "loadCellIntermediaryData", 
      "filename", 
      buffer, 
      1024, 
      "0", 
      currentExtension);

    string intLocation(m_dataStorePathPrefix);
    intLocation += buffer;
    TSLifstream strm(intLocation.c_str());
    if ( !intermediaryData->loadData(strm) )
    {
      strm.close();
      raiseError("Received loadData but loadData call failed");
      return TSLS63CellLoadResultNotFound;
    }
    strm.close();


    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "loadCellIntermediaryData", 
      "retval", 
      buffer, 
      1024, 
      "0", 
      currentExtension);

    return (TSLS63CellLoadResultEnum) atoi(buffer);
  }

  TSLS63CellLoadResultEnum ScriptedS63DataClient::loadCellTMFData (const char* cellName, const char* dataServer, TSLS63EncryptedData* tmfData)
  {
    const LoadCellTMFDataOO* warOO = (const LoadCellTMFDataOO* )checkOrder(MethodloadCellTMFData);
    if ( warOO &&
      (!(stricmp(cellName, warOO->m_cellName.c_str()) == 0 || stricmp("?", warOO->m_cellName.c_str()) == 0) ||
      stricmp(dataServer, warOO->m_dsid.c_str()) ) )
    {
      raiseError("Expected ::loadCellTMFData parameters do not match");
    }


    char buffer[1024];
    bool found = false;
    int currentExtension = 0;
    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "loadCellTMFData", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "loadCellTMFData", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServer) )
      {
        ++currentExtension;
        continue;
      }
      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received loadCellTMFData call but couldn't find appropriate settings in ini file");
      return TSLS63CellLoadResultNotFound;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "loadCellTMFData", 
      "retval", 
      buffer, 
      1024, 
      "0", 
      currentExtension);

    TSLS63CellLoadResultEnum retval = (TSLS63CellLoadResultEnum) atoi(buffer);

    if ( retval == TSLS63CellLoadResultOK )
    {
      TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "loadCellTMFData", 
        "filename", 
        buffer, 
        1024, 
        "", 
        currentExtension);

      string tmfLocation(m_dataStorePathPrefix);
      tmfLocation += buffer;
      TSLifstream strm(tmfLocation.c_str());
      if ( !tmfData->loadData(strm) )
      {
        strm.close();
        raiseError("Received loadCellTMFData but loadData call failed");
        return TSLS63CellLoadResultNotFound;
      }

      strm.close();
    }

    return retval;
  }

  bool ScriptedS63DataClient::locateCatalogueForCell (const char* cellName, const char* dataServerID, int exchangeSetNumber, TSLSimpleString& locationOfCatalogue)
  {
    const LocatedCatalogueForCellOO* warOO = (const LocatedCatalogueForCellOO* )checkOrder(MethodlocateCatalogueForCell);
    if ( warOO &&
      (stricmp(cellName        , warOO->m_cellName.c_str()) ||
      stricmp(dataServerID    , warOO->m_dsid.c_str()) ||
      exchangeSetNumber !=      warOO->m_exchangeSetNo) )
    {
      raiseError("Expected ::locateCatalogueForCell parameters do not match");
    }

    char buffer[1024];
    bool found = false;
    int currentExtension = 0;

    // If this setting is true, the search for response data will effictively ignore data from all previous calls to this method.
    // This setting must be true if multiple overlapping exchange sets are loaded into the same data client.
    if ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "locateCatalogueForCell", 
      "startSearchAtCurrentCall", 
      buffer, 
      1024, 
      "0") && atoi(buffer) )
    {
      currentExtension = m_numCallsToMethod[MethodlocateCatalogueForCell];
    }
    m_numCallsToMethod[MethodlocateCatalogueForCell]++;

    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "locateCatalogueForCell", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "locateCatalogueForCell", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServerID) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "locateCatalogueForCell", 
        "exchangeSetNo", 
        buffer, 
        1024, 
        "", 
        currentExtension) || atoi(buffer) != exchangeSetNumber )
      {
        ++currentExtension;
        continue;
      }

      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received locateCatalogueForCell call but couldn't find appropriate settings in ini file");
      return false;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "locateCatalogueForCell", 
      "locationOfCatalogue", 
      buffer, 
      1024, 
      "", 
      currentExtension);

    //special case for testing that a blank catalogue location is caught
    if ( buffer[0] != '\0' )
    {
      locationOfCatalogue = m_exchangeSetPathPrefix.c_str();
      locationOfCatalogue += buffer;  
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "locateCatalogueForCell", 
      "retval", 
      buffer, 
      1024, 
      "", 
      currentExtension);


    return atoi(buffer) > 0 ;
  }

  void ScriptedS63DataClient::notifyError (TSLS63ErrorCodeEnum s63ErrorCode, const char* cellName, const char* dataServerID, const char* additionalInformation)
  {
    const NotifyErrorOO* errOO = (const NotifyErrorOO* )checkOrder(MethodnotifyError);
    if ( errOO && m_didItWork && 
      (errOO->m_s63errorCode != s63ErrorCode ||
      (errOO->m_additionalInfo.length() > 0 && 
      (!additionalInformation || stricmp(additionalInformation, errOO->m_additionalInfo.c_str()) ) ) ) )
    {
      raiseError("Expected ::notifyError error code/additionalinformation does not match");
    }
  }

  void ScriptedS63DataClient::notifyWarning (const char* additionalInformation, const char* cellName, const char* dataServerID)
  {
    const NotifyWarningOO* warOO = (const NotifyWarningOO* )checkOrder(MethodnotifyWarning);
    if ( warOO && m_didItWork && warOO->m_additionalInfo.length() > 0 && 
      (!additionalInformation || stricmp(additionalInformation, warOO->m_additionalInfo.c_str()) ) )
    {
      raiseError("Expected ::notifyWarning additionalinformation does not match");
    }
  }

  void ScriptedS63DataClient::populateEntityInformation (TSLEntity* entity, TSLEntitySet* entitySet, const char *cellName, const char *dsID)
  {
    //We're not going to both with this one as it will be called many times!!!! 
    //checkOrder(MethodpopulateEntityInformation);
  }

  bool ScriptedS63DataClient::progress (double percentOfExchangeSetDone, double percentOfCurrentCellDone)
  {
    // This will get called numerous times, so rather than include it in checkOrder just get
    // the return value to use by looking up the entry in the .ini for the current call number
    int expectedExchangeSetProgress = 0, expectedCellProgress = 0;
    bool retval = true;
    int currentExtension = 0;
    while ( TSLProfileHelper::lookupProfile( m_iniFile.c_str(), 
      "progress", 
      "exchangeSetProgress", 
      &expectedExchangeSetProgress, 
      0, 
      currentExtension) )
    {
      TSLProfileHelper::lookupProfile( m_iniFile.c_str(), 
        "progress", 
        "cellProgress", 
        &expectedCellProgress, 
        0, 
        currentExtension);

      if( (int)percentOfExchangeSetDone == expectedExchangeSetProgress &&
        (int)percentOfCurrentCellDone == expectedCellProgress )
      {
        int value = 0;
        TSLProfileHelper::lookupProfile( m_iniFile.c_str(), 
          "progress", 
          "retval", 
          &value, 
          0, 
          currentExtension);
        retval = value > 0 ? true : false;
        break;
      }

      ++currentExtension;
    }
     
    return retval;
  }

  void ScriptedS63DataClient::storeIngestedCell (const TSLS63EncryptedData* cellTMF, const TSLS63EncryptedData* cellIntermediaryData, const char* cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber)
  {
    const StoreIngestedCellOO* warOO = (const StoreIngestedCellOO* )checkOrder(MethodstoreIngestedCell);
    if ( warOO &&
      (stricmp(cellName        , warOO->m_cellName.c_str()) ||
      stricmp(dataServerID    , warOO->m_dsid.c_str()) ||
      cellEdition !=            warOO->m_cellEdition ||
      cellUpdateNumber !=       warOO->m_cellUpdateNumber) )
      //not checking the extents at the mo
    {
      raiseError("Expected ::storeIngestedCell parameters do not match");
    }

    char buffer[1024];
    bool found = false;
    int currentExtension = 0;

    // If this setting is true, the search for response data will effictively ignore data from all previous calls to this method.
    // This setting must be true if multiple overlapping exchange sets are loaded into the same data client.
    if ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "storeIngestedCell", 
      "startSearchAtCurrentCall", 
      buffer, 
      1024, 
      "0") && atoi(buffer) )
    {
      currentExtension = m_numCallsToMethod[MethodstoreIngestedCell];
    }
    m_numCallsToMethod[MethodstoreIngestedCell]++;

    while ( TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "storeIngestedCell", 
      "cellName", 
      buffer, 
      1024, 
      "", 
      currentExtension)
      && buffer[0] != '\0' )
    {
      if ( strcmp(cellName, buffer) )
      {
        ++currentExtension;
        continue;
      }

      if ( !TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "storeIngestedCell", 
        "dataServerID", 
        buffer, 
        1024, 
        "", 
        currentExtension) || strcmp(buffer, dataServerID) )
      {
        ++currentExtension;
        continue;
      }
      found = true;
      break;
    }

    if ( !found )
    {
      raiseError("Received storeIngestedCell call but couldn't find appropriate settings in ini file");
      return;
    }

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "storeIngestedCell", 
      "dontSave", 
      buffer, 
      1024, 
      "", 
      currentExtension);

    if ( atoi(buffer) )
      return;

    TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
      "storeIngestedCell", 
      "tmffilename", 
      buffer, 
      1024, 
      "", 
      currentExtension);
    string tmfSaveLocation(m_dataStorePathPrefix);
    tmfSaveLocation += buffer;
    TSLofstream strm1(tmfSaveLocation.c_str());
    if ( !cellTMF->saveData(strm1) )
    {
      raiseError("Received storeIngestedCell but failed to save TMF data to file");
    }
    strm1.close();

    if ( cellIntermediaryData )
    {
      TSLProfileHelper::lookupProfile(m_iniFile.c_str(), 
        "storeIngestedCell", 
        "intfilename", 
        buffer, 
        1024, 
        "", 
        currentExtension);
      string intSaveLocation(m_dataStorePathPrefix);
      intSaveLocation += buffer;
      TSLofstream strm2(intSaveLocation.c_str());
      if ( !cellIntermediaryData->saveData(strm2) )
      {
        raiseError("Received storeIngestedCell but failed to save Intermediary data to file");
      }
      strm2.close();
    }

    map<string, int, less<string> >& dsVersionMap = m_cellVersionMap[cellName];
    dsVersionMap[dataServerID] = cellUpdateNumber;

    m_combinedCellVersionMap[cellName] = std::pair<int, int>(cellEdition, cellUpdateNumber);
  }

  const OrderObject* ScriptedS63DataClient::checkOrder( const MethodEnum expectedMethod )
  {
    if ( m_ignoreCallOrder || !m_didItWork)
      return 0;

    if ( m_currentPositionInCallOrder >= (int)m_callOrder.size() )
    {
      raiseError("Received too many calls than was defined in the ScriptedS63DataClient order");
      ++m_currentPositionInCallOrder;
      return 0;
    }

    const OrderObject* current = m_callOrder[m_currentPositionInCallOrder];
    if ( !current )
    {
      raiseError("Unexpected null OrderObject object");
      ++m_currentPositionInCallOrder;
      return 0;
    }

    if ( current->m_orderType != expectedMethod )
    {
      raiseError("ScriptedS63DataClient method called when not expected");
      ++m_currentPositionInCallOrder;
      return 0;
    }

    ++m_currentPositionInCallOrder;
    return current;
  }

  unsigned int ScriptedS63DataClient::numIngestedCells()
  {
    return m_combinedCellVersionMap.size();
  }

  const char* ScriptedS63DataClient::getIngestedCellName(unsigned int index)
  {
    if( index >= m_combinedCellVersionMap.size() )
    {
      return NULL;
    }
    std::map<std::string, std::pair<int, int> >::const_iterator it( m_combinedCellVersionMap.begin() );
    std::advance(it, index);
    return (*it).first.c_str();
  }

  bool ScriptedS63DataClient::getIngestedCellInfo(const char* cellName, int& edition, int& update)
  {
    std::map<std::string, std::pair<int, int> >::const_iterator it( m_combinedCellVersionMap.find(cellName) );
    if( it == m_combinedCellVersionMap.end() )
    {
      return false;
    }
    
    edition = (*it).second.first;
    update = (*it).second.second;
    return true;
  }

};

