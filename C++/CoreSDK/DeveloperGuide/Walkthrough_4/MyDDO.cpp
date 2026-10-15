#include "stdafx.h"
#include ".\myddo.h"
#include ".\mydo.h"

MyDDO::MyDDO(void)
{
}

MyDDO::~MyDDO(void)
{
}

TSLDisplayObject * MyDDO::instantiateDO(TSLDisplayType key, int dsID) const
{
  MyDO * displayObject = new MyDO ;

  return displayObject ;
}
