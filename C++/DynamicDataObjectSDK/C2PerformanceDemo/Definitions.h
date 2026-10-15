///////////////////////////////////////////////////////
// Definitions.h
// General declarations for the C2 Demo Application

// Describes the hostility of the track...
enum HOSTILE_TYPE {
    HOSTILE = 0,
    FRIENDLY = 1,
    NEUTRAL = 2,
    UNKNOWN = 3,
    ASSUMED_FRIEND = 4,
    SUSPECT = 5,
    JOKER = 6,
    FAKER = 7,
    PENDING = 8 // adds a '?' to the symbol which makes no sense when we have an icon.
};

// Describes the APP6A Annotation Level...
enum AnnotationLevelEnum {
    AnnotationLevelNone = 0,
    AnnotationLevelLow = 1,
    AnnotationLevelMedium = 2,
    AnnotationLevelHigh = 3
};


#define NUM_HOSTILE_CODES 4
#define NUM_HOSTILE_CODES_APP6A 9

// Used for accessing the hostility and type of a track
// from an array stored in the Track object
enum PLATFORM_CODE { HOSTILITY, TYPE } ;

// Used for identifying the type of track
enum TRACK_TYPE { AIRCRAFT_CARRIER, BOMBER, DESTROYER, FRIGATE, GENERAL_AIR, HELICOPTER, INTERCEPTOR, SUBMARINE, UNKNOWN_TRACK, OWN_SHIP, RANGE_RINGS } ;
#define NUM_TRACK_TYPES 9  // Two less than in the enum because we don't want to randomly create own ship and range rings randomly ( in CC2PerformanceDemoDoc )

// Update hints for the views
#define HINT_REDRAW_MAP_DISPLAY			  41001
#define HINT_UPDATE_SELECTED_TRACK		  41002
#define HINT_UPDATE_TOTE			  41003
#define HINT_NEW_MAP_LOADED			  41004
#define	HINT_UPDATE_DECLUTTER_STATUS_ON		  41005
#define	HINT_UPDATE_DECLUTTER_STATUS_OFF	  41006
#define	HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_ON	  41007
#define	HINT_UPDATE_VIRTUAL_DECLUTTER_STATUS_OFF  41008

// Extra define to determine when we are dragging a track to the tote
#define ID_TOOLS_TRACKSELECT_DRAG     40001
#define ID_TOOLS_GRABBING	      40002

// Length of a buffer required to pass back a platform code
#ifndef PLATFORM_DESCRIPTION
#define PLATFORM_DESCRIPTION 3
#endif

// Playback rates for the animator control
enum PLAYBACK_RATE { REAL_TIME, X2_SPEED, X5_SPEED, X10_SPEED, X50_SPEED } ;

#ifndef PI
#define PI 3.14159265358979323846
#endif

// Define the various lengths to indicate the
// magnitude of velocity of a track
#ifndef VELOCITY_SLOW
#define VELOCITY_SLOW 30
#endif

#ifndef VELOCITY_MEDIUM
#define VELOCITY_MEDIUM 55
#endif

#ifndef VELOCITY_FAST
#define VELOCITY_FAST 75
#endif

// Text and fill colours for the tracks
#define HOSTILE_FILL  0x000000FF
#define FRIENDLY_FILL 0x00FF9933
#define UNKNOWN_FILL  0x0000DCFF
#define NEUTRAL_FILL  0x0000A800

#define HOSTILE_FILL_256  0x000000FF
#define FRIENDLY_FILL_256 0x00FF0000
#define UNKNOWN_FILL_256  0x0000FFFF
#define NEUTRAL_FILL_256  0x0000FF00

#define HOSTILE_TEXT  0x00F0F0F0
#define FRIENDLY_TEXT 0x00F0F0F0
#define UNKNOWN_TEXT  0x00000000
#define NEUTRAL_TEXT  0x00F0F0F0

#define HOSTILE_TEXT_256  0x00F0F0F0
#define FRIENDLY_TEXT_256 0x00F0F0F0
#define UNKNOWN_TEXT_256  0x00000000
#define NEUTRAL_TEXT_256  0x00000000

#define RING_COLOUR 0x00d0d0d0
#define OUTLINE_COLOUR 0x00f0f0f0
