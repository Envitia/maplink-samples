#pragma once


// ScreenshotDialog dialog

class ScreenshotDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ScreenshotDialog)

public:
	ScreenshotDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ScreenshotDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENSHOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
