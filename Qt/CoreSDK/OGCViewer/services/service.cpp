/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "service.h"
#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"

namespace Services
{
  Service::ServiceActionCallback::ServiceActionCallback()
  {
  }

  Service::ServiceActionCallback::~ServiceActionCallback()
  {
  }

  void Service::ServiceActionCallback::coordinateSystemChoiceRequired()
  {
    // Not all users care about this callback, so a default implementation is provided that
    // does nothing
  }

  Service::Service()
    : m_muShiftX( 0.0 ) 
      , m_muShiftY( 0.0 )
      , m_tmcPerMU( 0.0 )
      , m_useFixedTransformParameters( false )
      , m_loadCancelled( false )
  {
  }

  Service::~Service()
  {
  }

  void Service::useTransformParametersFromLayer( TSLDataLayer *coordinateProvidingLayer )
  {
    if( coordinateProvidingLayer )
    {
      // Extract the coordinate system object from the data layer
      const TSLCoordinateSystem *coordSys = NULL;
      switch( coordinateProvidingLayer->layerType() )
      {
        case TSLDataLayerTypeWMSDataLayer:
          {
            TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( coordinateProvidingLayer );
            coordSys = wmsLayer->queryCoordinateSystem();
            break;
          }

        case TSLDataLayerTypeWMTSDataLayer:
          {
            TSLWMTSDataLayer *wmtsLayer = reinterpret_cast< TSLWMTSDataLayer* >( coordinateProvidingLayer );
            coordSys = wmtsLayer->queryCoordinateSystem();
            break;
          }

        default:
          return;
      }

      if( !coordSys )
      {
        return;
      }

      // Store the coordinate system properties for use when connecting to new services
      coordSys->getMapOffsets( &m_muShiftX, &m_muShiftY );
      m_tmcPerMU = coordSys->getTMCperMU();
      m_useFixedTransformParameters = true;
    }
  }

  void Service::setDrawingSurfaceName( const char *name )
  {
    m_drawingSurfaceName = name;
  }

  void Service::pushCallbackObject( Service::ServiceActionCallback *callbacks )
  {
    m_callbacks.push( callbacks );
  }

  void Service::popCallbackObject()
  {
    m_callbacks.pop();
  }

  void Service::advanceConnectionSequence()
  {
    // Signal the background thread in which the service load is occurring
    m_mutex.lock();
    m_waitCond.wakeAll();
    m_mutex.unlock();
  }

  void Service::cancelSequence()
  {
    // Signal the background thread in which the service load is occurring
    m_mutex.lock();
    m_loadCancelled = true;
    m_waitCond.wakeAll();
    m_mutex.unlock();
  }
};
