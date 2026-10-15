#pragma once
#include "tsldisplayobject.h"

class MyDO :
  public TSLDisplayObject
{
public:
  MyDO(void);
  virtual ~MyDO(void);
  virtual bool draw( TSLRenderingInterface *ri, TSLEnvelope *extent ) ;
};
