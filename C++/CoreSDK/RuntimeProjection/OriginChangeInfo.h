// OriginChangeInfo.h: interface for the OriginChangeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORIGINCHANGEINFO_H__D5185348_7ED3_4B8E_8D20_42CD832360BD__INCLUDED_)
#define AFX_ORIGINCHANGEINFO_H__D5185348_7ED3_4B8E_8D20_42CD832360BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct OriginChangeInfo  
{
  HWND              m_mainWindow;
  double            m_latitude;
  double            m_longitude;
  double            m_range;
  TSLMapDataLayer*  m_mapLayer;
  TSLLatLongGridDataLayer* m_llDataLayer;
};


#endif // !defined(AFX_ORIGINCHANGEINFO_H__D5185348_7ED3_4B8E_8D20_42CD832360BD__INCLUDED_)
