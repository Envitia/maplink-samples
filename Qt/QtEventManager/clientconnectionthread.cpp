#include "clientconnectionthread.h"

//! call back function to process the received message from the tracks channel.
class CustomTracksReceivedMessage : public TSLReceivedMessage
{
private:
  ClientConnectionThread* m_currentThread;

public:
  CustomTracksReceivedMessage(ClientConnectionThread * currentThread)
    : m_currentThread(currentThread)
  {
  }

  void processReceivedMessage(const std::string &message, int msgBodyIndex)
  {
    if (msgBodyIndex > 0)
    {
      //! update Tracks Positions
      std::string msgBody = message.substr(msgBodyIndex);
      if (!m_currentThread->updateTracksPositions(msgBody))
      {
        //printf("\n\n%s\n\n", errorMsg.c_str());
      }
    }
  }
};

//! call back function to process the received message from the track channel.
class CustomTrackReceivedMessage : public TSLReceivedMessage
{
private:
  ClientConnectionThread* m_currentThread;

public:
  CustomTrackReceivedMessage(ClientConnectionThread * currentThread)
    : m_currentThread(currentThread)
  {
  }

public:
  void processReceivedMessage(const std::string &message, int msgBodyIndex)
  {
    if (msgBodyIndex > 0)
    {
      //! update Tracks Positions
      std::string msgBody = message.substr(msgBodyIndex);
      if (!m_currentThread->updateTrackedItem(msgBody))
      {
        //printf("\n\n%s\n\n", errorMsg.c_str());
      }
    }
  }
};

//! call back function to process the received message from the errors channel.
class CustomErrorsReceivedMessage : public TSLReceivedMessage
{
private:
  ClientConnectionThread* m_currentThread;

public:
  CustomErrorsReceivedMessage(ClientConnectionThread * currentThread)
    : m_currentThread(currentThread)
  {
  }

public:
  void processReceivedMessage(const std::string &message, int msgBodyIndex)
  {
    if (msgBodyIndex > 0)
    {
      //! update errors message
      std::string msgBody = message.substr(msgBodyIndex);
      if (!m_currentThread->showErrorsChannelMsg(message))
      {
        //printf("\n\n%s\n\n", errorMsg.c_str());
      }
    }
  }
};


//! constructor to initialize the thread.
ClientConnectionThread::ClientConnectionThread(QObject *parent)
  : QThread(parent)
  , m_websocket(NULL)
{
}

//! initialize web socket.
bool ClientConnectionThread::initializeWebSocket(const string &settingsFilePath, string &errorMsg)
{
  try
  {
    string errMsg;
    if (settingsFilePath != "")
    {
      //! read settings from a file
      if (!m_settings.readSettings(settingsFilePath, m_settings, errMsg))
      {
        errorMsg += "Failed to read the setting from the config file: [" + settingsFilePath + "].\n";
        errorMsg += "\nErrors:\n" + errorMsg + "\n";
        return false;
      }
    }
    else
    {
      //! Hard coded settings
      //! web socket
      m_settings.m_serverAddress = "localhost"; //"env-eng-016";// "localhost";
      m_settings.m_serverPort = 8180; //8380;// 8180;
      m_settings.m_serverSubscriptionId = ""; //< http post request will be sent to the server to get the Subscription Id.
      m_settings.m_serverPath = "/EventManagerService/cpws?subscriptionid=" + m_settings.m_serverSubscriptionId;
      //! http post request
      m_settings.m_publicationURL = "/EventManagerService/events";
      m_settings.m_publicationDeliveryLocation = "/EventService/ws";
      m_settings.m_publicationMimeType = "application/xml";
      m_settings.m_publicationIdentifier = "AVL";
      m_settings.m_publicationBody = "C:/Users/Ahmed.Ibrahim/Desktop/httppostbody.ini";
      //! general settings
      m_settings.m_logging = true;
      m_settings.m_maxNumberToReconnect = 2;

      if (!m_settings.checkValidSettings(m_settings, errMsg))
      {
        errorMsg += "Invalid settings.\n";
        errorMsg += "Errors:\n" + errMsg + "\n";
        return false;
      }
    }

    //! initialize web socket variables
    m_websocket = new TSLClientWebSocket(m_settings, errorMsg);
    if (errorMsg != "")
    {
      errorMsg += "Failed to initialize the client web socket.\n";
      errorMsg += "Errors:\n" + errMsg + "\n";
      return false;
    }

    //! get the client web socket's subscription id if it was got by http post
    if (m_settings.m_serverSubscriptionId == "")
    {
      const char* subscriptionId;
      if (!getSubscriptionId(subscriptionId, errorMsg))
      {
        errorMsg += "Failed to get Subscription Id.\n";
        errorMsg += "Errors:\n" + errMsg + "\n";
        return false;
      }
      m_settings.m_serverSubscriptionId = subscriptionId;
    }
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to initialize the client web socket(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! run the thread.
void ClientConnectionThread::run()
{
  //! run the socket connection.
  string errorMsg;
  runClientSocket(errorMsg);
}

//! exit the thread.
bool ClientConnectionThread::exit()
{
  //! exit the socket connection.
  string errorMsg;
  return exitClientSocket(errorMsg);
}

//! run the client socket
bool ClientConnectionThread::runClientSocket(string &errorMsg)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }
    m_websocket->run();
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to run client web socket(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! exit the client socket
bool ClientConnectionThread::exitClientSocket(string &errorMsg)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }
    m_websocket->exit();
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to exit client web socket(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! subscribe channels with the server to receive updates
bool ClientConnectionThread::subscribe(string &errorMsg)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }

    //! Set subscription info
    TSLSubscriptionInfo subInfo[]
    {
      TSLSubscriptionInfo("tracks", new CustomTracksReceivedMessage(this)),
      TSLSubscriptionInfo("track",  new CustomTrackReceivedMessage(this)),
      TSLSubscriptionInfo("error",  new CustomErrorsReceivedMessage(this)),
    };

    string errMsg;
    if (!m_websocket->subscribe(subInfo, sizeof(subInfo) / sizeof(subInfo[0]), errMsg))
    {
      errorMsg += "Failed to subscribe channels.\n";
      errorMsg += "Errors:\n" + errMsg + "\n";
      return false;
    }
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to subscribe channels(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! unsubscribe channels with the server to stop receiving updates
bool ClientConnectionThread::unsubscribe(string &errorMsg)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }

    //! Set subscription info
    string subscriptionChannels[]
    {
      "tracks"
      //, "track"
      //, "error"
    };
    string errMsg;
    if (!m_websocket->unsubscribe(subscriptionChannels, sizeof(subscriptionChannels) / sizeof(subscriptionChannels[0]), errMsg))
    {
      errorMsg += "Failed to unsubscribe channels.\n";
      errorMsg += "Errors:\n" + errMsg + "\n";
      return false;
    }
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to unsubscribe channels(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! Send updateViewExtent STOMP command to the server
bool ClientConnectionThread::send_UpdateViewExtent
(
  const std::string &srsName,
  const double &lowerCornerX, const double &lowerCornerY,
  const double &upperCornerX, const double &upperCornerY,
  string &errorMsg
)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }

    if (!m_websocket->send_UpdateViewExtent(srsName, lowerCornerX, lowerCornerY, upperCornerX, upperCornerY))
    {
      errorMsg += "Failed to send Update View Extent.\n";
      return false;
    }
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to send Update View Extent(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! Send getTrackedItemByID STOMP command to the server
bool ClientConnectionThread::send_getTrackedItemByID
(
  const std::string &sourceid,
  const std::string & trackid,
  const std::string &subscriptionid,
  string &errorMsg
)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }

    if (!m_websocket->send_getTrackedItemByID(sourceid, trackid, subscriptionid))
    {
      errorMsg += "Failed to send Tracked Item By ID.\n";
      return false;
    }
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to send Tracked Item By ID(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! get the client web socket's subscription id
bool ClientConnectionThread::getSubscriptionId(const char* &subscriptionId, string &errorMsg)
{
  try
  {
    if (!m_websocket)
    {
      errorMsg += "m_websocket is Null";
      return false;
    }

    if (!m_websocket->getSubscriptionId(subscriptionId))
    {
      errorMsg += "Failed to get Subscription Id.\n";
      return false;
    }
  }
  catch (std::exception const& _exc)
  {
    //! something threw an exception.
    errorMsg += "Failed to get Subscription Id(Exception thrown).\n";
    errorMsg += "Errors:\n";
    errorMsg += _exc.what();
    return false;
  }

  return true;
}

//! update Tracks Positions
bool ClientConnectionThread::updateTracksPositions(const string &msgBody)
{
  //! parse the received message into (TracksDelivery) object 
  std::string errorMsg;
  TracksDelivery tracksDelivery;
  if (!TSLEventManagerJSonMessageDecoder::parseTracksDelivery(msgBody, tracksDelivery, errorMsg))
  {
    return false;
  }

  //! update tracks 
  m_mutexTracks.lock();
  m_tracksDelivery = tracksDelivery;
  m_mutexTracks.unlock();

  //! send tracks updated signal
  emit tracksUpdated();

  //! add small sleep to improve GUI performance.
  msleep(50);

  return true;
}

//! update Tracked Item
bool ClientConnectionThread::updateTrackedItem(const string &msgBody)
{
  //! parse the received message into (TrackedItem) object 
  std::string errorMsg;
  TrackedItem trackedItem;
  if (!TSLEventManagerJSonMessageDecoder::parseTrackedItem(msgBody, trackedItem, errorMsg))
  {
    return false;
  }

  //! update tracks
  m_mutexTrackedItem.lock();
  m_trackedItem = trackedItem;
  m_mutexTrackedItem.unlock();

  //! send tracks updated signal
  emit trackedItemUpdated();

  return true;
}


//! display errors received from the errors subscribed channel.
bool ClientConnectionThread::showErrorsChannelMsg(const string &msg)
{
  //! update tracks
  m_mutexErrorsMsg.lock();
  m_errorsMsg = msg;
  m_mutexErrorsMsg.unlock();

  //! send errors signal
  emit errorsUpdated();

  return true;
}