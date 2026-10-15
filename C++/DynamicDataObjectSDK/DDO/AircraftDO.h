// AircraftDO.h: interface for the AircraftDO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIRCRAFTDO_H__56D381D0_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_AIRCRAFTDO_H__56D381D0_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


typedef std::pair<COLORREF,COLORREF> ColourPair;

class AircraftDO : public TSLDisplayObject  
{
public:
  AircraftDO();
  virtual ~AircraftDO();

  virtual ColourPair getColours(bool col256 = true) const = 0;
  
  virtual bool getSymbolDetails(POINT*& pts, int& num, long& tx, long& ty) const = 0;

  virtual bool draw(TSLRenderingInterface *d_surface, TSLEnvelope *d_extent);
};

#endif // !defined(AFX_AIRCRAFTDO_H__56D381D0_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
