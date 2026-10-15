/****************************************************************************
    Copyright (c) 2000-2017 by Envitia Group PLC
=============================================================================
MODULE          : 
PACKAGE         : MapLink
SCCS            : 
AUTHOR(s)       : D.Dixon
=============================================================================
DESCRIPTION     : 
****************************************************************************/
#ifndef _APP6ATRACK_H_
#define _APP6ATRACK_H_

#include "Track.h"

class APP6ATrack : public CTrack  
{
  public:
    APP6ATrack( TSLAPP6AHelper* helper, HOSTILE_TYPE hostility, int type, int app6aType );
    APP6ATrack( const APP6ATrack & other );
    virtual ~APP6ATrack();

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ; 
  
    TSLEntitySet* getAsEntitySet ();

    //const TSLEntitySet *m_symbol;
    int m_type;

    virtual void hostility( HOSTILE_TYPE hostility ) ;
    virtual HOSTILE_TYPE  hostility()     { return m_hostility ; }

    void setAnnotationLevel ( AnnotationLevelEnum level );

    bool getAsSymbol(HBITMAP bitmap);

private:
    // Create the APP6a symbol helper
    TSLAPP6AHelper* m_symbolHelper;
    static int typeToAPP6ASymbol ( int type );
    static int hostilityToAPP6AHostility ( HOSTILE_TYPE hostility );
    int m_app6aType;
    TSLAPP6ASymbol m_symbol;
    TSLEntitySet * m_symbolAsEntSet;
    bool m_entitySetDirty;

};

#endif

