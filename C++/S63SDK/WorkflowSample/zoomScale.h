#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

class CMainFrame;
class S63WorkFlowView;

// ZoomScale

class ZoomScale : public CComboBox
{
  DECLARE_DYNAMIC(ZoomScale)

public:
  ZoomScale();
  virtual ~ZoomScale();

  void setParent(S63WorkFlowView* pParent = NULL);

  int m_scale;
  bool m_scaleChanged;

  void AddScaleValues();

  void setScale(const char *buffer);
protected:
  DECLARE_MESSAGE_MAP()

  S63WorkFlowView* m_pParent;
public:
  afx_msg void OnCbnSelchange();

private:
  CMainFrame *m_mainWindow;
  bool m_currentScaleAdded;
};


