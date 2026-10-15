#include "stdafx.h"
#include ".\mydo.h"

MyDO::MyDO(void)
{
  setPixSize( -25, -25, 25, 25 ) ;
}

MyDO::~MyDO(void)
{
}  

bool MyDO::draw( TSLRenderingInterface *ri, TSLEnvelope *extent )
{
  // 1023 = filled circle with cross, 181=red in std config files
  ri->setupSymbolAttributes( 1023, 181, 50, TSLDimensionUnitsPixels ) ;
  ri->drawSymbol( position() ) ;
  return true ;
}

