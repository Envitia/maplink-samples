// App6aAircraftDO.h: interface for the App6aAircraftDO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_App6aAircraftDO_H__56D381D0_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_App6aAircraftDO_H__56D381D0_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class App6aAircraftDO : public TSLDisplayObject  
{
public:
  App6aAircraftDO(TSLAPP6ASymbol::HostilityEnum hostility,
                  TSLAPP6ASymbol::TypeEnum type,
                  long symbolKey);

  virtual ~App6aAircraftDO();

  virtual bool draw(TSLRenderingInterface *d_surface, TSLEnvelope *d_extent);
protected:

  TSLEntitySet* m_symbolEntitySet;
  int m_px, m_py;
  // The APP6a symbol helper
  static TSLAPP6AHelper* m_symbolHelper;
  // Refence count so we know when to delete the helper
  static int m_referenceCount;
};

#endif // !defined(AFX_App6aApp6aAircraftDO_H__56D381D0_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
