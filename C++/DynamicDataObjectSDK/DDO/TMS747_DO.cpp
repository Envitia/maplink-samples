// TMS747_DO.cpp: implementation of the TMS747_DO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMS747_DO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define TMS_747_SYMCOL	    0x00FF9933
#define TMS_747_SYMCOL256   0x00FF0000

#define TMS_747_TXTCOL	    0x00F0F0F0
#define TMS_747_TXTCOL256   0x00F0F0F0

POINT TMS747_DO::m_outline[NP] = {  { -10,  10 },
				    {  10,  10 },
				    {   9,  -1 },
				    {   8,  -4 },
				    {   6,  -7 },
				    {   4,  -9 },
				    {   1, -10 },
				    {  -1, -10 },
				    {  -4,  -9 },
				    {  -6,  -7 },
				    {  -8,  -4 },
				    {  -9,  -1 }
				 };


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMS747_DO::TMS747_DO()
{
}

TMS747_DO::~TMS747_DO()
{
}

ColourPair TMS747_DO::getColours(bool col256) const
{
  return ( col256 ? ColourPair( TMS_747_SYMCOL256, TMS_747_TXTCOL256 ) : ColourPair( TMS_747_SYMCOL, TMS_747_TXTCOL ) );
}
  
bool TMS747_DO::getSymbolDetails(POINT*& pts, int& num, long& tx, long& ty) const
{
  pts = m_outline;
  num = NP;

  return true;
}


