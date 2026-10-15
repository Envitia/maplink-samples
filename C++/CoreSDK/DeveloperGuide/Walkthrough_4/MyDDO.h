#pragma once
#include "tsldynamicdataobject.h"

class MyDDO :
  public TSLDynamicDataObject
{
public:
  MyDDO(void);
  virtual ~MyDDO(void);
  virtual TSLDisplayObject * instantiateDO(TSLDisplayType key, int dsID) const ;
};
