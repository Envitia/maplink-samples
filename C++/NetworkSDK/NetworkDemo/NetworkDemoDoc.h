// NetworkDemoDoc.h : interface of the CNetworkDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CNetworkDemoDoc : public CDocument
{
  protected: // create from serialization only
    CNetworkDemoDoc();
    DECLARE_DYNCREATE(CNetworkDemoDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurface * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );

    bool loadStandardNetwork(LPCTSTR filename);

    bool isNetworkLoaded() const
    {
      return m_network != 0 ? true : false;
    }

    bool endPinAdded() const
    {
      return m_endPinAdded;
    }

    bool isRouteDataLayerEmpty() const
    {
      return m_routeDataLayer->entitySet()->size() > 0 ? false : true;
    }

    RoutingType routingType() const
    {
      return m_routingType;
    }

    void routingType(RoutingType type)
    {
      m_routingType = type;
    }

    // pinSize is the 'size' of the pin.
    // endPin is true if this is the end pin being added, false otherwise.
    // Returns true if added successfully.
    bool addRoutePin(long xTMC, long yTMC, double TMCperMU, long pinSize, bool endPin);



    // pinSize is the 'size' of the pin.
    // Returns true if added successfully.
    bool addSpatialQuerySearchPointPin(long xTMC, long yTMC, double TMCperMU, long pinSize);

    // Clears any Spatial Query Data displayed
    void clearSpatialQueryData();

    // Displays a line from the search point and the extent of the search range
    bool displaySpatialQueryOrientationRange( 
           const TSLEnvelope& extent, 
           const TSLCoord& endPt, 
           double TMCperMU );

    // Performs find_links test
    int performSpatialQuery( 
          const TSLEnvelope& extent, 
          const double& range, 
          const double& orientation, 
          bool useSpatialIndex, 
          std::string &displayResult );


    // Calculates the route.
    void calculateRoute();

    // Clears all displayed routes.
    void clearRouteDisplay();
    // Clears all displayed routes and pins.
    void clearRoutes();

    // Methods to allow doc to change route sizes.
    void prepareForPrinting();
    void finishedPrinting();
  
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNetworkDemoDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CNetworkDemoDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CNetworkDemoDoc)
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
    void removeRouteAndPinData();

    long findNodeID(long xTMC, long yTMC, long aperture) const;

    void changeRouteSize(double routeFactor, double pinFactor);

    // Retrieves Data about the Network and formats it into a single string
    // ready for output to a message box.
    // Outputs the following information about the network:-
    //   Number of Nodes
    //   Number of Links
    //   Node Extent Height in kilometres
    //   Node Extent Width in kilometres
    string getNetworkDataString( void ) const;

    // Function that loads from file or builds the Spatial Index.
    // Initally checks the current Spatial Index status as it only needs to 
    // load or build it if the status is invalid.
    // Function returns if valid.
    // If invalid, the function attempts to load ithe Spatial Index from a file.
    // It assumes the path to the file and the filename will be the same as that
    // used to load the network but with the extension .nsi.
    // If any errors occur then the Spatial Index is built from scratch.
    // If this fails then a message box is output to the user.
    bool loadSpatialIndex( void );

    // Formats the input Search Results data into a single string
    // ready for output to a message box.
    // Outputs the following information:-
    //   Closest Link Segment Start Point Latitude/Longitude (in degrees)
    //   Distance to Closest Link Segment (in metres)
    //   Orientation of Closest Link Segment (in degrees)
    //   Time to find Closest Link Segment (in seconds)
    //   Total Test Time (in seconds)
   string buildSpatialQuerySearchResultsString(
            const TSLLink& closestLink,
            const TSLCoord& bestLinkSegStartPt,
            const TSLCoord& bestLinkSegEndPt,
            const double& bestDistance,
            const double& findTime,
            const double& totalTime ) const;

  // Formats the input Search Criteria data into a single string
  // ready for output to a message box.
  // Outputs the following information:-
  //   Search Point Latitude/Longitude (in degrees)
  //   Search Range (in metres)
  //   Orientation (in degrees)
  //   Tolerance (+/- degrees)
  string buildSpatialQuerySearchCriteriaString( 
          const double& searchPtLat,
          const double& searchPtLong,
          const double& searchRangeMapUnits,
          const double& orientation,
          const double& tolerance ) const;


  private:
  
    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;
  
    // Name of my map layer
    static const char * m_mapLayerName ;

    double m_TMCperMU; // The number of TMC units per Map Unit.
  
    // Network data.
    static const char*      m_routeLayerName;   // Name of the route layer.
    static const char*      m_pinLayerName;     // Name of the pin layer.
    static const char*      m_sqLayerName;      // Name of the spatial query layer.
    
    TSLStandardDataLayer*   m_routeDataLayer;   // Contains the route display.
    TSLStandardDataLayer*   m_pinDataLayer;     // Contains the route pins.

    TSLStandardDataLayer*   m_sqDataLayer;     // Contains the Spatial Query display.

    TSLNetwork*             m_network;          // The standard network.
    string                  m_networkFilename;   // File Network is loaded from 
    
    RoutingType             m_routingType;      // The type of routing.

    // The journey is defined as a vector of node indices.
    std::vector<long>	    m_routeNodeIDs;

    // This is true, if the end pin has been added, false otherwise.
    bool		    m_endPinAdded;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
