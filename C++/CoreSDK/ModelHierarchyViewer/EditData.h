#pragma once


// EditData dialog
#include <vector>
#include <sstream>

class TSLText;

class EditData : public CDialog
{
	DECLARE_DYNAMIC(EditData)

public:
	EditData(CWnd* pParent = NULL);   // standard constructor
	virtual ~EditData();

// Dialog Data
	enum { IDD = IDD_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedOk();

  void editControls( );

  void setEntity(TSLText* textEntity)
  {
    m_textEntity = textEntity;
  }

private:
  TSLText* m_textEntity;
  CStatic static_value, static_rotation, static_height;
  CEdit   edit_value, edit_rotation, edit_height;
};
