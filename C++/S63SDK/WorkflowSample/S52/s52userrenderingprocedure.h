#ifndef s52userrenderingprocedure_h
#define s52userrenderingprocedure_h 1

//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "S52\tsls52userrenderingprocedure.h"

class S52UserRenderingProcedure : public TSLS52UserRenderingProcedure
{
  public:
      
    virtual bool execute (const char *procedureName, const TSLEntity* entity, const TSLDataSet*  dataset, const TSLDataHandler* dataHandler, const TSLS52StateObject* state, TSLRenderingInterface *renderingInterface);
};

#endif
