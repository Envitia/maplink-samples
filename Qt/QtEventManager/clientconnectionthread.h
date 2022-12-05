#ifndef CLIENTCONNECTIONTHREAD_H
#define CLIENTCONNECTIONTHREAD_H

#include <QThread>
#include <qmutex.h>
#include "TSLClientWebsocket.h"
#include "TSLEventManagerJSonMessageDecoder.h"

class ClientConnectionThread : public QThread
{
  Q_OBJECT
public:
  //! constructor to initialize the thread.
  explicit ClientConnectionThread(QObject *parent = 0);

  //! run the thread.
  void run();

  //! exit the thread.
  //!
  //! @return true if successful. false otherwise.
  bool exit();

public:// Web socket
  //! initialize web socket.
  //!
  //!
  //! @param settingsFilePath file path of the web socket settings. 
  //! If empty string parameter, use default hardcoded settings.
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool initializeWebSocket(const string &settingsFilePath, string &errorMsg);

  //! subscribe channels with the server to receive updates
  //!
  //!
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool subscribe(string &errorMsg);

  //! unsubscribe channels with the server to stop receiving updates
  //!
  //!
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool unsubscribe(string &errorMsg);

  //! Send updateViewExtent STOMP command to the server
  //!
  //!
  //! @param srsName coordinate system srs name.
  //! @param lowerCornerX bounding box lower corner X.
  //! @param lowerCornerY bounding box lower corner Y.
  //! @param upperCornerX bounding box upper corner X.
  //! @param upperCornerY bounding box upper corner Y.
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool send_UpdateViewExtent
  (
    const std::string &srsName,
    const double &lowerCornerX, const double &lowerCornerY,
    const double &upperCornerX, const double &upperCornerY,
    string &errorMsg
  );

  //! Send getTrackedItemByID STOMP command to the server
  //!
  //!
  //! @param sourceid data's source id.
  //! @param trackid track id to query.
  //! @param subscriptionid connection's subscription id.
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool send_getTrackedItemByID
  (
    const std::string &sourceid,
    const std::string & trackid,
    const std::string &subscriptionid,
    string &errorMsg
  );

public:
  //! web socket settings
  TSLWebSocketSettings m_settings;

private:
  //! run the client socket
  //!
  //!
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool runClientSocket(string &errorMsg);

  //! exit the client socket
  //!
  //!
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool exitClientSocket(string &errorMsg);

  //! get the client web socket's subscription id
  //!
  //!
  //! @param subscriptionId subscription id.
  //! @param errorMsg error message showing the reason if failed.
  //!
  //! @return true if successful. false otherwise.
  bool getSubscriptionId(const char* &subscriptionId, string &errorMsg);

  /////////////////////////////////////////tracksDelivery//////////////////////////////////////////////////
public:
  //! map of tracks
  TracksDelivery m_tracksDelivery;

  //! mutex to protect the map of tracks.
  QMutex m_mutexTracks;

  //! update Tracks Positions
  //!
  //!
  //! @param msgBody message body received from the server.
  //!
  //! @return true if successful. false otherwise.
  bool updateTracksPositions(const string &msgBody);

signals:
  //! Signal to be sent by the thread when tracks are updated.
  void tracksUpdated();

  ////////////////////////////////////////trackedItem///////////////////////////////////////////////////
public:
  //! tracked item object
  TrackedItem m_trackedItem;

  //! mutex to protect the map of tracks.
  QMutex m_mutexTrackedItem;

  //! update Tracked Item
  //!
  //!
  //! @param msgBody message body received from the server.
  //!
  //! @return true if successful. false otherwise.
  bool updateTrackedItem(const string &msgBody);

signals:
  //! Signal to be sent by the thread when tracked item is updated.
  void trackedItemUpdated();

  ////////////////////////////////////////errors///////////////////////////////////////////////////
public:
  //! errors object
  string m_errorsMsg;

  //! mutex to protect the map of tracks.
  QMutex m_mutexErrorsMsg;

  //! display errors received from the errors subscribed channel.
  //!
  //!
  //! @param msg message  received from the server.
  //!
  //! @return true if successful. false otherwise.
  bool showErrorsChannelMsg(const string &msg);

signals:
  //! Signal to be sent by the thread when tracked item is updated.
  void errorsUpdated();

private:
  //! web socket
  TSLClientWebSocket* m_websocket;
};

#endif // CLIENTCONNECTIONTHREAD_H
