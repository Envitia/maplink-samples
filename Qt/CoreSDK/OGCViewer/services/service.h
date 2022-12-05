/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <stack>
#include <QMutex>
#include <QWaitCondition>
#include <QAbstractTableModel>
#include <QAbstractItemModel>


#include "servicetypeenum.h"
#include "tgmapidll.h"
#include "tslsimplestring.h"
#include "tslatomic.h"

class TSLDataLayer;
class TSLDrawingSurface;
class QAbstractItemView;

// This class defines an abstract interface that can be used to
// interact with both a WMS and WMTS. Each type of service that the
// viewer can use (WMS and WMTS) has an implementation of this class
// 

namespace Services
{
  class ServiceLayerPreview;

  class Service
  {
    public:
      // Abstract class for implementation by UI components to handle
      // progress actions and errors generated in background threads during
      // service loading through the add service wizard.
      class ServiceActionCallback
      {
        public:
          ServiceActionCallback();
          virtual ~ServiceActionCallback();

          virtual void onError( const std::string &message) = 0;
          virtual void onNextSequenceAction() = 0;
          virtual void coordinateSystemChoiceRequired();
      };

      class ServiceCredentialsCallback
      {
        public:
          virtual void onCredentialsRequired (TSLSimpleString& username, TSLSimpleString& password) = 0;
      };

      // Abstract class for services to implement to provide an adapter
      // that shows information about a specific layer on a service
      // in a Qt user interface component.
      class ServiceLayerInfoModel : public QAbstractTableModel
      {
        public:
          ServiceLayerInfoModel()
          {
          }

          virtual ~ServiceLayerInfoModel()
          {
          }

          // Tells the adapter to show information about the layer identified
          // by the given model index from the service's ServiceLayerModel.
          virtual void setSelectedLayer( const QModelIndex &layer ) = 0;
      };

      // Abstract class for services to implement to provide an adapter
      // that shows information about all the layers available from a service
      // in a Qt user interface component.
      class ServiceLayerModel : public QAbstractItemModel
      {
        public:
          ServiceLayerModel()
          {
          }

          virtual ~ServiceLayerModel()
          {
          }

          // This method should return true if the service is correctly configured, and thus
          // the user interface is allowed to continue to the next step.
          virtual bool configurationValid() const = 0;

          // This method should return true if the currently selected layers offer any dimensions,
          // and thus if the user interfae should show the configuration page for dimensions.
          virtual bool selectionsHaveDimensions() const = 0;
      };

      // Abstract class for services to implement to provide an adapter
      // that shows a list of styles available on a specific layer from the service's
      // ServiceLayerModel in a Qt user interface component.
      class ServiceLayerStylesModel : public QAbstractListModel
      {
      public:
        ServiceLayerStylesModel()
        {
        }

        virtual ~ServiceLayerStylesModel()
        {
        }

        // Tells the adapter to show information about the layer identified
        // by the given model index from the service's ServiceLayerModel.
        virtual void setSelectedLayer( const QModelIndex &layer ) = 0;

        // Returns the name of the current style value for the selected layer
        virtual const char* currentStyle() const = 0;

        // Sets the current style for the selected layer to the given value
          virtual void setStyle( const char *name ) = 0;
      };

      // Abstract class for services to implement to provide an adapter
      // that shows information about a specific dimension from the service's
      // ServiceDimensionsModel in a Qt user interface component.
      class ServiceDimensionInfoModel : public QAbstractTableModel
      {
        public:
          ServiceDimensionInfoModel()
          {
          }

          virtual ~ServiceDimensionInfoModel()
          {
          }

          // Tells the adapter to show information about the dimension
          // identified by the given model index from the service's ServiceDimensionsModel.
          virtual void setSelectedDimension( const QModelIndex &layer ) = 0;
      };

      // Abstract class for services to implement to provide an adapter
      // that shows information about all the dimensions available on a specific layer
      // from the service's ServiceDimensionsModel in a Qt user interface component.
      class ServiceDimensionsModel : public QAbstractTableModel
      {
        public:
          ServiceDimensionsModel()
          {
          }

          virtual ~ServiceDimensionsModel()
          {
          }

          // This method will return true if all dimensions have a value set
          virtual bool configurationValid() const = 0;

          // Installs type-specific delegates for editing of dimensions in specific formats (e.g. dates)
          virtual void setDelegates( QAbstractItemView *view ) = 0;

          // Returns the model index for the dimension with the given name
          virtual QModelIndex findDimensionByName( const QString &dimensionName ) const = 0;

          // Retrieves the possible values defined by the service for the dimension identified by index.
          // The return value states whether only values in the list are valid or if user-defined values are also allowed
          // for the dimension.
          virtual bool getPossibleValues( const QModelIndex &index, std::vector< const char* > &values ) const = 0;
      };

      // Abstract class used to provide a common interface to query data from individual layers
      // within a particular service without having to identify the specific type of service
      // being used
      class ServiceLayer
      {
        public:
          ServiceLayer()
          {
          }
          virtual ~ServiceLayer()
          {
          }

          // Returns the name of the layer as it should appear in the service's tree
          virtual const char *displayName() const = 0;

          // Returns the names of the available dimensions available on this layer
          virtual void getDimensionNames( std::vector< std::string > &dimensionNames ) const = 0;

          // Returns the units of the dimension with the given name
          virtual const char* getDimensionUnits( const std::string &name ) const = 0;

          // Sets the named style on the layer
          virtual bool setStyle( const char *styleName ) = 0;

          // Returns the available styles for the layer, not including the currently set style (if any)
          virtual void getStyleNames( std::vector< std::string > &styleNames ) const = 0;

          // Returns the extent in TMCs of the layer. This is used for the 'zoom to layer' function.
          virtual bool getTMCExtent( TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const = 0;
      };

      Service();
      virtual ~Service();

      // Loads the service at the given URL and starts the callback sequence in a background thread
      virtual void loadService( const char *address ) = 0;

      // Signals the callback sequence to advance to the next step. This is called by the UI when the
      // user advances through pages in the service wizard.
      void advanceConnectionSequence();

      // Signals the callback sequence to cancel and exit. This is called by the UI when the user
      // cancels the service wizard.
      void cancelSequence();

      // Makes this service's internal data layer use the coordinate properties from the given layer,
      // so that the two data layers will appear correctly aligned in the view.
      void useTransformParametersFromLayer( TSLDataLayer *coordinateProvidingLayer );

      // Returns service-specific adapters for mapping information about the service to the UI.
      virtual ServiceLayerModel* getServiceLayerModel() = 0;
      virtual ServiceLayerInfoModel* getServiceLayerInfoModel() = 0;
      virtual ServiceDimensionsModel* getDimensionsModel() = 0;
      virtual ServiceDimensionInfoModel* getDimensionInfoModel() = 0;
      virtual ServiceLayerStylesModel* getServiceLayerStyleModel() = 0;
      virtual ServiceLayerPreview* getLayerPreviewHelper() = 0;

      // Functions to retrieve display information about the service and its contents
      virtual const char* getServiceDisplayName() const = 0;

      // Returns the URL as passed to loadService
      const std::string& getServiceURL() const;

      // Returns a list of the layers that are currently displayed on this service
      virtual void getVisibleLayers( std::vector< ServiceLayer* > &layerNames ) = 0;

      // Returns a list of layers that are not currently visible and are compatible with the 
      // current coordinate system of the service.
      virtual void getPotentialLayersForDisplay( std::vector< std::string > &layerNames ) = 0;

      virtual ServiceLayer* getLayerForToken( void* token ) = 0;

      // Returns the data layer for this service
      virtual TSLDataLayer* dataLayer() = 0;

      // Changes the draw order of layers in this service.
      virtual void updateLayerVisibilityOrder( int originalIndex, int newIndex ) = 0;

      // Changes the visibility for the named layer, and returns a token corresponding to the layer
      virtual void* setLayerVisibility( const char *layerName, bool visible ) = 0;
      virtual void setLayerVisibility( ServiceLayer *layer, bool visible ) = 0;

      // Changes the cache size of the enclosed data layer in the service
      virtual void setCacheSize( int size ) = 0;

      // Returns the CRS string that identifies the coordinate system the
      // service is using.
      const std::string& activeCRS() const;

      // The name of this service inside the MapLink drawing surface
      const std::string& drawingSurfaceName() const;
      void setDrawingSurfaceName( const char *name );

      // Returns the type of this service
      ServiceTypeEnum type() const;

      void pushCallbackObject( ServiceActionCallback *callbacks );
      void popCallbackObject();

      // These will only return data at a specific point in the service callback sequence
      virtual size_t numCoordSystemChoices() = 0;
      virtual const char** coodinateSystemChoices() = 0;
      virtual void setCoordinateSystemChoice( int choice ) = 0;

    protected:
      std::stack< ServiceActionCallback* > m_callbacks;

      ServiceTypeEnum m_type;

      std::string m_activeCRS;
      std::string m_url;

      std::string m_drawingSurfaceName;

      // Fixed transform parameters to use when more than one service is loaded to ensure
      // all layers use a compatible coordinate system, and thus the data appears in the correct place
      double m_muShiftX;
      double m_muShiftY;
      double m_tmcPerMU;
      bool m_useFixedTransformParameters;

      // Use Qt threading primitives so this code will compile with compilers that do not support
      // C++11 threads.
      QMutex m_mutex;
      QWaitCondition m_waitCond;
      bool m_loadCancelled;
  };

  inline const std::string& Service::activeCRS() const
  {
    return m_activeCRS;
  }

  inline const std::string& Service::getServiceURL() const
  {
    return m_url;
  }

  inline const std::string& Service::drawingSurfaceName() const
  {
    return m_drawingSurfaceName;
  }

  inline ServiceTypeEnum Service::type() const
  {
    return m_type;
  }
};
#endif
