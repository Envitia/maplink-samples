/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "drawingsurfacewidget.h"

#include <QPaintEvent>
#include <QResizeEvent>

#ifdef WIN32
# include <Windows.h>
#else
# include <QX11Info>
#endif

#include "MapLink.h"
#include "MapLinkDrawing.h"
DrawingSurfaceWidget::DrawingSurfaceWidget( QWidget *parent )
  : QWidget( parent )
  , m_surface( NULL )
{
  setAttribute( Qt::WA_OpaquePaintEvent );
  setAttribute( Qt::WA_PaintOnScreen );
  setAttribute( Qt::WA_NativeWindow );
  setAutoFillBackground( false );

#ifdef WIN32
  m_surface = new TSLNTSurface( (HWND)winId(), false );
#elif QT_VERSION >= 0x50100
  Display *display = QX11Info::display();
  WId wid = winId();
  
  XWindowAttributes attribs;
  XGetWindowAttributes( display, wid, &attribs );

  m_surface = new TSLMotifSurface( display, attribs.screen, attribs.colormap, wid, 0, attribs.visual );
#else
# error "This sample currently doesn't build with < Qt5.1"
#endif

  m_surface->setOption( TSLOptionDoubleBuffered, true );
  m_surface->wndResize( 0, 0, width(), height(), false );

  connect( this, SIGNAL(signalRefreshView()), this, SLOT(refreshView()) );
  connect( this, SIGNAL(signalResetView()), this, SLOT(resetView()) );
}

DrawingSurfaceWidget::~DrawingSurfaceWidget()
{
  delete m_surface;
}

void DrawingSurfaceWidget::paintEvent( QPaintEvent* /*event*/ )
{
  if( m_surface )
  {
    m_surface->redraw();
    emit mapDrawn();
  }
}

void DrawingSurfaceWidget::resizeEvent( QResizeEvent *event )
{
  QWidget::resizeEvent( event );

  if( m_surface )
  {
    m_surface->wndResize( 0, 0, event->size().width(), event->size().height(), true, TSLResizeActionMaintainTopLeft );
  }
}

QPaintEngine* DrawingSurfaceWidget::paintEngine() const
{
  // Override the default paint engine for this widget as MapLink handles the drawing itself
  return NULL;
}

bool DrawingSurfaceWidget::event(QEvent * event)
{
  bool result = QWidget::event( event );

  if( m_surface && event->type() == QEvent::WinIdChange )
  {
    // The native window handle has changed, reattach the drawing surface to the new handle
    m_surface->attach( (TSLWindowHandle)winId() );
  }

  return result;
}

void DrawingSurfaceWidget::refreshView()
{
  // Mark all data layers as changed to ensure that they are redrawn
  int numDataLayers = m_surface->getNumDataLayers();
  for( int i = 0; i < numDataLayers; ++i )
  {
    TSLDataLayer *layer = NULL;
    const char *layerName = NULL;
    m_surface->getDataLayerInfo( i, &layer, &layerName );
    layer->notifyChanged();
  }

  emit update();
}

void DrawingSurfaceWidget::resetView()
{
  m_surface->reset( false );
  update();
}
