/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include <QtGui>
#ifndef WINNT
# include <X11/Xlib.h>
#else
#endif

/////////////////////////////////////////////////////////////////////
//! Include MapLink Pro Headers...
//
//! Define some required Macros and include X11 and Win32 headers as
//! necessary.
//
//! Define: TTLDLL & WIN32 within the project make settings.
//
/////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		//! Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif
# include <windows.h>
#endif

#include "MapLink.h"
#include "MapLinkIMode.h"

#include "tsltrackdisplaymanager.h"
#include "tsltrackselectionsymbol.h"
#include "clientconnectionthread.h"

////////////////////////////////////////////////////////////////
//! Main Application class.
//
//! Contains the calls to MapLink and the simple application
//! code.
////////////////////////////////////////////////////////////////
class ClientManager
{
public:
#ifndef WINNT
  ClientManager(QWidget *parent, TSLMotifSurface *drawingSurface);
#else
  ClientManager(QWidget *parent, TSLNTSurface *drawingSurface);
#endif
  virtual ~ClientManager();

  //! create and initialize the track manager
  void createTrackManager();

private:
#ifndef WINNT
  //! The MapLink drawing surface
  TSLMotifSurface *m_drawingSurface;
#else
  //! The MapLink drawing surface
  TSLNTSurface *m_drawingSurface;
#endif

  //! parent widget
  QWidget *m_parentWidget;

  //////////////////////////////////////////////////////////////////////////////////////
  //! track manager
  TSLTrackDisplayManager*  m_trackManager;

  //! Name of my track manager
  static const char * m_trackManagerName;

  //! drawing surface's Id used by the track manager.
  uint32_t m_surfaceId;

  //! static counter used to set track number for track manager.
  static int trackNumbersCounter;

  //////////////////////////////////////// Symbols Templates////////////////////////////////////////
private:
  //! selection symbol template to be used when a track is selected(clicked).(symbol around the track when it is selected).
  TSLTrackSelectionSymbol* m_selectionSymbolTemplate;
  //! create selection track symbol template to be reused.(symbol around the track when it is selected).
  TSLTrackSelectionSymbol* createSelectionSymbolTemplate();

  //! history symbol template to be used when displaying history points.
  TSLTrackHistorySymbol* m_historySymbolTemplate;
  //! create history track symbol template to be reused.
  TSLTrackHistorySymbol* createHistorySymbolTemplate(bool showHistorySquare);
  //! flag to be set if we need to display history points as small squares.
  bool m_showHistorySquare;

  //! point symbol template to be used if the track is invalid/ not supported.
  TSLTrackSymbol* m_pointSymbolTemplate;
  //! create point track symbol template to be reused.
  TSLTrackPointSymbol* createPointSymbolTemplate(int symbolID);
  //! Add entity to the point symbol
  static void addEntityToPointSymbol(TSLTrackPointSymbol * symbol, int symbolIDVal, TSLRGBA colour);

  //! saved symbol templates to be re-used.[save any symbol template to be re-used for other tracks with the same symbol]
  std::map<string, TSLTrackSymbol*> m_savedSymbolTemplates;
  //! create military track symbol template to be reused[if symbol template was previously created, re-use it].
  TSLTrackSymbol* createMilitarySymbolTemplate(const string &symbolID, TSLTrackMilitarySymbol::Specification spec, TSLTrackMilitarySymbol::SpecificationTypeID specTypeID);

  //////////////////////////////////////// Display tracks ////////////////////////////////////////
private:
  //! Displaying tracks in the drawing surface
  struct DisplayingTracks
  {
    //! track number
    int m_trackNumber;

    //! displaying track
    TSLTrack* m_track;

    //! displaying track's symbol.
    TSLTrackSymbol* m_symbol;

    //! symbol
    string m_sym;

    //! affliation.
    string m_affl;
  };

  //! map of displaying tracks in the drawing surface.
  std::map<string, DisplayingTracks> m_displayingTracks;

  //! clone symbol template, create a display track, and add the track to the track manager.
  bool createDisplayTrack(TSLTrackSymbol* symbolTemplate, const string& trackId, int trackNumber);

  //////////////////////////////////////// Selected tracks ////////////////////////////////////////
private:
  //! selected track id [when clicking on a track to show its history]
  string m_selectedTrackId;
  //! previously selected track id [used to unselect the previously selected track when a new track is selected].
  string prev_selectedTrackId;

  //! vector of key-value pairs of metadata information for the selected track.
  std::vector<std::pair<string, string>> m_selectedMetadatPairs;

private:
  //! decode (TrackedItem) object into metadata key-value pairs.
  static bool decodeMetadata(const TrackedItem &updatedtrackedItem, std::vector<std::pair<string, string>> &metadatPairs);

  //! decode (TracksDelivery) object for the selected track into metadata key-value pairs.
  static void getSelectedTrackMetadata(const TracksDelivery &updatedtracksDelivery, const string &selectedTrackId, std::vector<std::pair<string, string>> &metadatPairs);

  //! decode (TrackedItem) object for the selected track into metadata key-value pairs.
  static void getSelectedTrackMetadata(const TrackedItem &updatedtrackedItem, std::vector<std::pair<string, string>> &metadatPairs);

  //////////////////////////////////////// Record history tracks ////////////////////////////////////////
private:
  //! flag to record tracks history
  bool m_recordTracksHistory;

  //! current time when recording history.
  int  m_currentTime;

public:
  //! set the record tracks history and set The maximum period to store historic Track data for. 
  //!
  //! Data older than (currentTime - historicDataExpiry) will be deleted.
  void setRecordTracks(bool recordTracksHistory, int historicDataExpiryTime);

  //! get current time when recording history.
  int getCurrentTime();

  //! Set the display time for the manager.
  void displayTime(int time);

  //! Clear all history points if not in tracks mode.
  void clearHistoryPoints();

  //////////////////////////////////////// Client connection Thread ////////////////////////////////////////
private:
  //! tracks thread.
  ClientConnectionThread *m_clientConnectionThread;

public:
  //! set client connection Thread
  void setClientConnectionThread(ClientConnectionThread *_clientConnectionThread);

  ////////////////// Tracks Updated //////////////////
  //! handles tracks updated slot sent by the thread
  void onTracksUpdated(std::vector<std::pair<string, string>> &metadatPairs);

  //! decode hostility string into TSLTrackMilitarySymbol::Hostility enum value
  static TSLTrackMilitarySymbol::Hostility decodeHostility(const string & affCode);

  //! Process and update the track manager with the updated tracks.
  void processUpdatedTracks(const TracksDelivery &updatedtracksDelivery, std::vector<std::pair<string, string>> &metadatPairs);

  //! redraw the drawing surface.
  bool redrawSurface();

  ////////////////// Tracked Item Updated //////////////////
  //! handles tracks updated slot sent by the thread
  bool onTrackedItemUpdated(std::vector<std::pair<string, string>> &metadatPairs);

  //! Process and update the track manager with the updated tracks.
  bool processUpdatedTrackedItem(const TrackedItem &updatedtrackedItem, std::vector<std::pair<string, string>> &metadatPairs);

  //! handles clicking left click in the tracks mode.
  bool handleTracksModeLeftClick(TSLDeviceUnits x, TSLDeviceUnits y);

  ////////////////// Errors Updated //////////////////
  //! handles errors updated slot sent by the thread
  void onErrorsUpdated();

  //! update the server with the current view extent.
  void updateViewExtent(TSLDrawingSurface* drawingSurface);
};

#endif

