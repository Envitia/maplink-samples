#ifndef _S63WORKFLOW_H_
#define _S63WORKFLOW_H_

#include "s57workflowclient.h"
#include "s63workflowclient.h"
#include "s63errorlayer.h"
#include "../s52/statecallbacks.h"

#define CM_PER_INCH 2.54
 
class S52StateObject ;

/////////////////////////////////////
// Test Plan Reference: 3.4
//  - GUI & Code Separation.
/////////////////////////////////////

/*
  The S63 Workflow class performs S63-related operations which are best
  separated from the GUI.

  A description of each of the methods contained within the class are
  provided below.

  IMPORTANT NOTE ON SCALE BANDING
  -------------------------------
  Scale banding has been provided in this sample for example purposes only
  and is not guaranteed to be accurate.

  Scale banding suffers from the following limitations:

    - The accuracy of the Scale Banding is affected by the projection of 
      the coordinate system in use.

    - Scale banding depends on the DPI ( Dots Per Inch ) of the monitor in use.
      The queried value is rarely 100% accurate and will therefore also
      affect the scale bands.

  With dynamic arc projections (such as the one used in the sample) the scale band
  does not directly correspond to the set scale because the X-Axis values are scaled
  based upon the view position north and south of the equator.
*/

using namespace std;

class S63Workflow : public WorkFlowCallback
{
public:

  /*
    Constructor
  */

  S63Workflow(const LocateCatalogueCallback catalogueCallback, const NotifyErrorCallback notifyErrorCallback,
              const IngestCellCallback ingestCellCallback, const CancelledCellCallback cancelledCellCallback,
              const ProgressCallback progressCallback, void *arg, S52StateObject *s52StateObject);

  /*
    Destructor
  */

  ~S63Workflow();
 
  /*
    This method removes returns a vector of pairs, which represent the 
    cell permits and associated data server ID's which are installed 
    in the sample.
  */
  vector<pair<const TSLS63CellPermit*, string> >& cellPermits();
 
  /*
    Removes all of the scale bands in use.
  */
  void clearScaleBands();

  /*
    This method allows the 'current time' to be specified to the workflow.
    This allows expiry dates to be considered as valid, provided this value is
    set between the expiry dates.
  */
  void currentTime(const long long _currentTime);

  /*
    This method returns the currently installed data server certificate.
  */
  const string& dataServerCertificate() const;
 
  /*
    Allows the user to set whether or not the Ingest Cell dialog is 
    displayed.

    Setting the 'displayDialog' value to true will display the dialog.
  */
  void displayIngestDialog(const bool displayDialog);
  
  bool addS63DataSource(TSLS63DataLayer * s63Layer,  const string& dataStoreID, const string& dataStore, vector<pair<const TSLS63CellPermit*, string> >& cellPermits, S63ErrorLayer *& s63ErrorLayer);

  bool addS57DataSource(TSLS63DataLayer * s63Layer, const string& dataStoreID, const string& dataStore, S63ErrorLayer *& s63ErrorLayer);

  void generateDataLayer(const string &hw_id, const long cacheSize, TSLS63DataLayer *& s63Layer);
  
  TSLCoordinateSystem * getDefaultCoordinateSystem() const;
  /*
    Method for generating a user permit, using the provided HW_ID, M_KEY and M_ID's.

    The result field is populated with the resulting user permit value.
    
    Returns true on success, false otherwise.
  */
  bool generateUserPermit(const string &hw_id, const string &m_key, const string &m_id, string &result) const;

  /*
    This method populates the 'expiryDate' field with the expiry date for
    the specified cell. The provided cell name and data server ID should 
    uniquely identify an installed cell permit.
  */
  void getCellExpiry(const string &cellName, const string &dataServerID, long long &expiryDate);

  /*
    This method populates the provided variables with the horizontal and vertical DPI 
    as provided by the 'setDPI' method.
  */
  void getDPI(int& vdpi, int& hdpi) const;

  /*
    Provided with a MEDIA.TXT file, this method will return the media records
    contained within the file.
  */
  bool getMediaRecords(const string &mediaFilePath, vector<string>& mediaRecords) const ;

  /*
    This method returns the scale bands which will be used by the S63 Data Layer.
  */
  TSLS63DataLayerScaleBands * getScaleBands();

  /*
    This method provided the ability to set the HW_ID which will be used by the 
    reference workflow.
  */
  bool hwid(const string &hw_id);

  /*
    This method provided the ability to retrieve the HW_ID which is being used by the 
    reference workflow.
  */
  string hwid() const;

  /*
    This method returns the path to the scheme administrator certificate.
  */
  string ihoCertificate() const;

  /*
    This method installs the specified PERMIT.TXT file in the sample, optionally with
    a data server ID. 

    Returns true if the permits were installed successfully.
  */
  bool installCellPermits(const string &permitPath, const string &dataServerID);

  /*
    This method installs a certificate in the reference workflow. If the dataServerCertificate
    flag is set, this certificate is considered to be a data server certificate.

    Returns true if the certificate is valid, false otherwise.
  */
  bool installCertificate(const string &certificatePath, const bool dataServerCertificate);

  /*
    This method installs a user permit on the sample.

    Returns true on success, false otherwise.
  */
  bool installUserPermit(const string &permitPath);

  /*
    This method ensures the specified certificate file is valid.

    If the certificate is valid, the information fields are populated with expiry times and
    certificate issuer respectively.

    Returns true if the certificate is valid, false otherwise.
  */
  bool queryCertificate(const string &certificatePath, TSLTimeType& certificateValidFromDate, TSLTimeType& certificateExpiryDate, TSLSimpleString& certificateIssuer) const;

  /*
    This method removes the specified from the installed cell permits.
    The provided cell name and data server ID should uniquely identify
    an installed cell permit.

    Returns true if the permits were installed successfully.
  */
  void removeCellPermit(const string &cellName, const string &dataServerID);

  /*
    This method removes the installed data server certificate.
  */
  void removeDataServerCertificate();

  /*
    This method removes the installed scheme administrator certificate.
  */
  void removeIHOCertificate();

  /*
    This method populates the 'cellPermits' vector with a list of cell permits
    contained within the specified file.
  */
  void retrieveCellPermits(const string &fileName, const string &dataServerID, vector<pair<const TSLS63CellPermit*, string> >& cellPermits);
  
  /*
    This method provided the ability to set the S-57 configuration file which will be used by the 
    reference workflow.
  */
  void s57ConfigFile(const string &configFile);

  /*
    This method provided the ability to retrieve the  S-57 configuration file which is being used by the 
    reference workflow.
  */
  string s57ConfigFile() const;

  /*
    This method populates the horizontal and vertical DPI (dots per inch) values
    with the values passed in.
  */
  void setDPI(const int vdpi, const int hdpi);

  /*
    This method sets the drawing surface to be used by the reference workflow.
  */
  void setDrawingSurface(TSLDrawingSurface * drawingSurface);

  /*
    This method sets the upper and lower thresholds of the scale band specified by the 
    scale band type.

    Returns true on success, false otherwise.
  */
  bool setScaleBand(const TSLS63ScaleBandEnum type, const double lowerThreshold, const double upperThreshold, const bool updateUserScaleBands = true);

  /*
    This method begins the ingest using the supplied PRODUCTS.TXT and SERIAL.ENC. 
    All ingested data will be stored in the dataStore location.

    Returns true on success, false otherwise.
  */
  bool startIngest(const string &dataStore, const string &productFile, const string &serialFile);
  
  /*
    This method begins the ingest using the supplied CATALOG file or S57 Cell. 
    All ingested data will be stored in the dataStore location.

    Returns true on success, false otherwise.
  */
  bool startS57Ingest(const string &dataStore, const string &s57File);
  
  /*
    This method is used to unlock the S63 functionality.

    Returns true on success, false otherwise.
  */
  bool unlock(const string &unlockKey);

  /*
    Returns whether or not the S63 functionality has been unlocked.
  */
  bool unlocked() const;

  /*
    Refreshes the scale bands to ensure they contain the latest values.
  */
  void updateScaleBands();

  /*
    This method returns the path to the installed user permit file.
  */
  string userPermit() const;

  /*
    Writes configuration settings to INI file.
  */
  void writeConfigurationData(bool initialSettings);

  // save the state setup
  virtual void saveSettings(S52StateObject *) ;

  /*
    Configures the scale bands settings
   */
   void configureScaleBands();
private:

  // A default constructor. This is not required, but has been included for good practice.
  S63Workflow(); 

  // A pointer to the Drawing Surface, this is used for S-63 display purposes.
  TSLDrawingSurface * m_drawingSurface;

  // The location of the 'installed' user permit.
  string m_userPermit;

  // The location of the installed IHO certificate.
  string m_ihoCertificate;

  // Stores the currently installed data server certificate.
  string m_dataServerCertificate;

  // Stores the path to the currently active S57 configuration file.
  string m_s57ConfigFile;

  // A collection of installed cell permits.
  vector<pair<const TSLS63CellPermit*, string> > m_cellPermitStore;

  // A collection of Cell Permit Collections which have been installed, these are kept to be destroyed later.
  vector< TSLS63CellPermitCollection* > m_cellPermitCollections;

  // The current time to use when asked for by the S63 SDK.
  long long m_currentTime;

  // The hardware ID to use throughout.
  TSLS63HWID * m_hw_id;

  // The Media Ingest Manager to use when ingesting an exchange set.
  TSLS63MediaIngestManager * m_manager;

  // A boolean to store whether or not the S63 functionality has been unlocked.
  bool m_isUnlocked;

  // The vertical DPI of the current monitor.
  int m_verticalDPI;

  // The horizontal DPI of the current monitor.
  int m_horizontalDPI;

  // The number of pixels per map unit for use with the scale banding.
  double m_DUPerMU;

  // GUI CALLBACKS
  LocateCatalogueCallback m_catalogueCallback;
  NotifyErrorCallback m_notifyErrorCallback;
  IngestCellCallback m_ingestCellCallback;
  CancelledCellCallback m_cancelledCellCallback;
  ProgressCallback m_progressCallback;

  // The scale bands to use when displaying the S63 Data Layer.
  TSLS63DataLayerScaleBands * m_scaleBands;

  // The scale bands to show the user in the scale reciprocal format.
  TSLS63DataLayerScaleBands * m_userScaleBands;

  // The S63 Data Client to use with the S63 Data Layer. This has been stored for removal later.
  vector<S57WorkflowClient *> m_s57DataClients;
  vector<S63WorkflowClient *> m_s63DataClients;

  // Application-defined argument to pass to callback functions
  void *m_callbackArgument;

  // A flag to determine whether or not to display the ingest cell dialog
  bool m_displayIngestDialog;

  // location of folder containing INI file for persistent settings.
  string m_iniFileLocation;

  // S52 state object
  S52StateObject *m_s52StateObject;
  /*
    A private helper function to populate a collection with Cell Permits, based upon
    the provided file name and data server ID.

    Argument list description:

    fileName: The file name of the PERMIT.TXT file to open.

    dataServerID: The data server ID to use in association with the permit file.

    cellPermits: A vector which will be populated with cell permits upon success.

    Returns true on success, false otherwise.
  */
  bool generateCellPermits(const string &fileName, const string &dataServerID, vector<pair<const TSLS63CellPermit*, string> >& cellPermits);

}; 

inline void S63Workflow::currentTime(const long long _currentTime)
{
  m_currentTime = _currentTime;
}

inline string S63Workflow::userPermit() const
{
  return m_userPermit;
}

inline vector<pair<const TSLS63CellPermit*, string> >& S63Workflow::cellPermits()
{
  return m_cellPermitStore;
}

inline TSLS63DataLayerScaleBands * S63Workflow::getScaleBands()
{
  return m_userScaleBands;
}

inline string S63Workflow::ihoCertificate() const
{
  return m_ihoCertificate;
}

inline const string& S63Workflow::dataServerCertificate() const
{
  return m_dataServerCertificate;
}

inline void S63Workflow::displayIngestDialog(const bool displayIngestDialog)
{
  m_displayIngestDialog = displayIngestDialog;
}

inline bool S63Workflow::hwid(const string &hw_id)
{
  if ( m_hw_id )
  {
    delete m_hw_id;
  }

  m_hw_id = new TSLS63HWID(hw_id.c_str());

  return m_hw_id->isValid();
}

inline string S63Workflow::hwid() const
{
  if ( m_hw_id && m_hw_id->asciiRepresentation() )
  {
    return string(m_hw_id->asciiRepresentation(), 5);
  }

  return string("");
}

inline void S63Workflow::s57ConfigFile(const string &configFile)
{
  m_s57ConfigFile = configFile;
}

inline string S63Workflow::s57ConfigFile() const
{
  return m_s57ConfigFile;
}

#endif