// TrackManager.h: interface for the CTrackManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKMANAGER_H__FAF2F6D4_4A96_11D5_BD2F_00104BB05CAE__INCLUDED_)
#define AFX_TRACKMANAGER_H__FAF2F6D4_4A96_11D5_BD2F_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTrack;
class CDOTrack;

////////////////////////////////////////////////////////
// The CTrackManager class contains static variables
// and member functions for setting and retrieving
// data useful for tracks such as tmc per metre values
// and map extents

class CTrackManager  
{
public:

	// Query and store the tmc per metre values for current map..
	static double tmcPerMetre() { return m_tmcPerMetre ; }
	static void tmcPerMetre( double tpm ) { m_tmcPerMetre = tpm ; }
	
	// Query and store the extent for the current map...
	static const TSLEnvelope& getTMCExtent() { return m_extent ; } ;
	static void setTMCExtent( long x1, long y1, long x2, long y2 ) ;

	//////////////////////////////////////////////
	// Track Management
	//////////////////////////////////////////////

	static void initialiseTrack( TRACK_TYPE type, double& velocity, double& altitude ) ;
	static void redesignateSelectedTrack( TRACK_TYPE newTrackType ) ;
	static CDOTrack * getDisplayObject( TRACK_TYPE type ) ;

	static void displayTrackNumbers( bool display ) ;
	static bool displayTrackNumbers()
        {
          return m_displayTrackNums ;
        }
	static void displayPlatformCodes( bool display ) ;
	static bool displayPlatformCodes() 
        {
          return m_displayPlatformCodes ;
        }
	static void displayVelocityVectors( bool display ) ;
	static bool displayVelocityVectors()
        {
          return m_displayVelocityVectors ;
        }

	static void displayHostileTracks( bool display ) ;
	static bool displayHostileTracks()
        {
          return m_displayFriendlyTracks ;
        }
	static void displayFriendlyTracks( bool display ) ;
	static bool displayFriendlyTracks()
        {
          return m_displayFriendlyTracks ;
        }
	static void displayNeutralTracks( bool display ) ;
	static bool displayNeutralTracks()
        {
          return m_displayNeutralTracks ;
        }
	static void displayUnknownTracks( bool display ) ;
	static bool displayUnknownTracks()
        {
          return m_displayUnknownTracks ;
        }
        static void displayAssumedFriendlyTracks( bool display ) ;
        static bool displayAssumedFriendlyTracks()
        {
          return m_displayAssumedFriendlyTracks ;
        }
        static void displayPendingTracks( bool display ) ;
        static bool displayPendingTracks()
        {
          return m_displayPendingTracks;
        }
        static void displaySuspectTracks( bool display ) ;
        static bool displaySuspectTracks()
        {
          return m_displaySuspectTracks ;
        }
        static void displayJokerTracks( bool display ) ;
        static bool displayJokerTracks()
        {
          return m_displayJokerTracks ;
        }
        static void displayFakerTracks( bool display ) ;
        static bool displayFakerTracks()
        {
          return m_displayFakerTracks ;
        }
	static bool displayTrackType( HOSTILE_TYPE hostility ) ;
	static void displayRangeRings( bool display ) ;
	static bool displayRangeRings()
        {
          return m_displayRangeRings ;
        }
	static void displayHistoryPoints( bool display ) ;
	static bool displayHistoryPoints()
        {
          return m_displayHistoryPoints ;
        }
	static void numberHistoryPoints( unsigned int points ) ;
	static unsigned int numberHistoryPoints() 
        {
          return m_numHistoryPoints ;
        }
	static void northUp( bool apply ) ;
	static bool northUp() 
        {
          return m_northUp ;
        }
	static void trackHeadingUp( bool apply ) ;
	static bool trackHeadingUp()
        {
          return m_trackHeadingUp ;
        }
	static void followSelectedTrack( bool apply ) ;
	static bool followSelectedTrack()
        {
          return m_followSelectedTrack ;
        }
	static void latitudeAsString( double latitude, char * buffer ) ;
	static void longitudeAsString( double longitude, char * buffer ) ;

	/////////////////////////////////////////////
	// Track objects
	/////////////////////////////////////////////
	static void ownShip( CTrack * ship ) ;
	static CTrack * ownShip() 
        {
          return m_ownship ;
        }
	static void selectedTrack( CTrack * track ) ;
	static CTrack * selectedTrack() 
        {
          return m_selectedTrack ;
        }

	static void dragTrack( CTrack * track ) ;
	static CTrack * dragTrack()
        {
          return m_dragTrack;
        }

	/////////////////////////////////////////////
	// Zoom to specified range around track
	/////////////////////////////////////////////
	static bool ZoomTo( TSLDrawingSurface * pSurface, double range ) ;

	/////////////////////////////////////////////
	// Generate Track IDs
	/////////////////////////////////////////////
	static int getNewTrackID() ;
        static void resetTrackIDCount();
        static void setTrackIDCount(int count);
	// Generate a string  describing the track hostility and type
	static void platformDescription( CTrack * pTrack, char * buffer ) ;   

	/////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////

	static int random( int min, int max ) ;	    // Create a random integer
	static long round ( double val )	    // Round a double up or down
        {
          return (val < 0.0) ? (long)(val - 0.5) : (long)(val + 0.5) ;
        }

        /////////////////////////////////////////////
        // APP6A Annotation Level
        /////////////////////////////////////////////

        static AnnotationLevelEnum getAnnotationLevel ()
        {
          return m_annotationLevel;
        }

        static void setAnnotationLevel ( AnnotationLevelEnum level );

        static void bitmapRendering(bool useBitmap)
        {
          m_bitmapRendering = useBitmap;
        }

        static bool bitmapRendering()
        {
          return m_bitmapRendering;
        }

        static HBITMAP historyBitmap(HDC hDC, HBRUSH brush);
        static void destroyHistoryBitmap();
        static HDC historyDC()
        {
          return m_memDC;
        }

private:
	static double m_tmcPerMetre ;	// 1 TMC = 1 metre
	static TSLEnvelope m_extent ;	// Store for the extent of the map

	static bool m_displayTrackNums ;	// Display the Track IDs
	static bool m_displayPlatformCodes ;	// Display the platform codes for the tracks
	static bool m_displayVelocityVectors ;	// Display the velocity vectors for the tracks
	static bool m_displayHostileTracks ;	// Display hostile tracks
	static bool m_displayFriendlyTracks ;	//    "	   friendly   "
	static bool m_displayNeutralTracks ;	//    "	   neutral    "
	static bool m_displayUnknownTracks ;	//    "	   unknown    "
        static bool m_displayAssumedFriendlyTracks;
        static bool m_displayPendingTracks;
        static bool m_displaySuspectTracks;
        static bool m_displayJokerTracks;
        static bool m_displayFakerTracks;
	static bool m_displayRangeRings ;	// Display the range rings on the map
	static bool m_displayHistoryPoints ;	// Display the track's movement history
	static unsigned int m_numHistoryPoints ;	// Number of history points for the tracks
	static bool m_northUp ;			// Draw map with north up
	static bool m_trackHeadingUp ;		// Draw the map with the track heading up
	static bool m_followSelectedTrack ;	// Follow the selected track

	static CTrack * m_ownship ;	        // Own Ship - used for zooming to ranges from the
						// own ship position
	static CTrack * m_selectedTrack ;	// Currently selected track

	static CTrack * m_dragTrack ;		// Track that is currently being dragged

	static int  m_trackNumber ;		// Counter for track number, new Tracks call getNewTrackID()
						// to receive their ID number

        static AnnotationLevelEnum m_annotationLevel;

        static HBITMAP m_historyBitmap;
        static HBITMAP m_historyBitmapOld;
        static HDC     m_memDC;
        static RECT    m_historyRect;

        static bool    m_bitmapRendering;
};

#endif // !defined(AFX_TRACKMANAGER_H__FAF2F6D4_4A96_11D5_BD2F_00104BB05CAE__INCLUDED_)
