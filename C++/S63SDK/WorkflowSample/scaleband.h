#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

class CMainFrame;
class S63WorkFlowView;

class ScaleBand : public CComboBox
{
  DECLARE_DYNAMIC(ScaleBand)

public:
  ScaleBand();
  virtual ~ScaleBand();

  void setParent(S63WorkFlowView* pParent = NULL);

  int m_scaleBand;
  bool m_scaleBandChanged;

  void AddScaleBandValues();

  void setScaleBand(const char *buffer);
protected:
  DECLARE_MESSAGE_MAP()

  S63WorkFlowView* m_pParent;
public:
  afx_msg void OnCbnSelchange();

private:
  CMainFrame *m_mainWindow;
  bool m_currentScaleBandAdded;
};


