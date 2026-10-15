#pragma once

///////////////////////////////////////////////////////////
// Example  showing how to create a custom Dynamic Renderer
///////////////////////////////////////////////////////////
class MyDynamicRenderer :
  public TSLClientCustomDynamicRenderer
{
  public:
    // This enum is returned by the 'type' method.  It is defined like 
    // this since it is a portable way of defining a class-scoped constant value
    // that can be used if required in a switch statement.
    // It could be defined as 'static const int typeValue = 1000' ; but this isn't
    // portable to some older compilers such as Visual Studio 6 or HP-UX aCC.
    // Note that the number is > 999, since the range 0-999 is reserved for MapLink
    enum { typeValue = 1000 } ;

    // Constructor - the default one.
    MyDynamicRenderer(void);

    // Copy Constructor - used by the clone method.
    MyDynamicRenderer( const MyDynamicRenderer & other );

    // Destructor - ensure virtual so that it can be triggered
    // when the renderer is destroyed through the base class pointer.
    virtual ~MyDynamicRenderer(void);

    // clone method.
    // =============
    // This will be called by the Dynamic Renderer Factory if this
    // renderer is registered with it and the application calls 'create'
    virtual TSLClientCustomDynamicRenderer * clone () const ;

    // setConfig method.
    // =================
    // This may be called by an application in order to allow for persistence
    // of configuration values.  The renderer should read from the provided string
    // and populate any relevant configuration.  Care should be taken to include
    // some level of versioning information so that it can be updated if necessary
    // Future releases of MapLink may use this functionality
    virtual bool setConfig (const TSLSimpleString &src) ;

    // getConfig method.
    // =================
    // This may be called by an application in order to allow for persistence
    // of configuration values.  The renderer should write any relevant configuration
    // to the the provided string.  Care should be taken to include
    // some level of versioning information so that it can be updated if necessary
    // Future releases of MapLink may use this functionality
    virtual bool getConfig (TSLSimpleString &dst) const ;

    // render method.
    // ==============
    // This is called by MapLink when a matching entity is required to be drawn.
    // By default, it will not be called for Entity Sets.  This is configurable
    // when the TSLDynamicRendererCustom object is created.
    //
    // The renderer can perform various actions:
    //
    //   Decide that the entity should not be drawn, for example the scale factor
    //   of the current display may be inappropriate.  In this case, the renderer
    //   should return TSLDynamicRendererActionIgnore.  If this is returned for 
    //   an Entity Set, then the contents of the Entity Set will not be drawn.
    //
    //   Draw the entity itself, typically by querying geometric properties and
    //   perhaps dataset attributes and then using the renderingInterface
    //   directly.  In this case, the renderer should make the relevant rendering
    //   calls and then return TSLDynamicRendererActionNone
    //   
    //   Decide that normal MapLink renderering should take place.  In this case, 
    //   the renderer should return TSLDynamicRendererActionDefault.
    virtual TSLDynamicRendererAction render (const TSLEntity *entity, TSLRenderingInterface *renderingInterface, const TSLDataHandler *dataHandler, int pass, bool *morePassesRequired, TSLFeatureID *featureID) ;

    // delayerRender method.
    // ====================
    // This method is called after rendering of entities to allow the user to draw
    // additional entites on-top of all other drawing.
    virtual bool delayedRender( TSLRenderingInterface *renderingInterface, const TSLDataHandler *dataHandler);

    // type method.
    // ============
    // This method should return a renderer specific indentifier that can be used to
    // let the application know what type this is.
    // This example will merely return 'typeValue' as defined above.
    virtual int type ( ) const ;

    // Shows how to render a polyline
    // ==============================
    void renderPolyline(const TSLPolyline * entity, TSLRenderingInterface * renderingInterface);

    // Shows how to render a polygon
    // =============================
    void renderPolygon(const TSLPolygon * entity, TSLRenderingInterface * renderingInterface);

    // Shows how to render an ellipse
    // ==============================
    void renderEllipse(const TSLEllipse * entity, TSLRenderingInterface * renderingInterface);

    // Shows how to render an arc
    // ==========================
    void renderArc(const TSLArc * entity, TSLRenderingInterface * renderingInterface);


    // Shows how to render a text object
    // =================================
    void renderText(const TSLText * entity, TSLRenderingInterface * renderingInterface);

    // Shows how to render a symbol
    // ============================
    void renderSymbol(const TSLSymbol * entity, TSLRenderingInterface * renderingInterface);

    // Shows how to render a multi-part polygon
    // ========================================
    void renderMultiPolygon( const TSLMultiPolygon * entity, TSLRenderingInterface * renderingInterface ) ;

     // Shows how to render a multi-part polyline
    // ==========================================
    void renderMultiPolyline( const TSLMultiPolyline * entity, TSLRenderingInterface * renderingInterface ) ;

    // Storage for some colours that we look up and store
    bool m_queriedColours ;
    long m_red ;
    long m_green ;
    long m_blue ;
    long m_white ;
    long m_black ;
};
