#include "viewereventfilter.h"

#include <QEvent>
#include <QKeyEvent>

ViewerEventFilter::ViewerEventFilter(QObject* parent)
  : QObject( parent )
{
}

bool ViewerEventFilter::eventFilter(QObject* object, QEvent* event)
{
  if(event->type() == QEvent::KeyRelease)
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if(keyEvent->key() == Qt::Key_M)
    {
      // The default key bindings for osgEarthQt uses 'm' to change
      // the threading model.
      // As multi-threaded models don't work with Qt5 we discard this event
      return true;
    }
  }
  return false;
}
