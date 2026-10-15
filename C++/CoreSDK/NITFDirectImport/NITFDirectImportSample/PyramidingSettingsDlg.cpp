// PyramidingSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include ".\pyramidingsettingsdlg.h"


// CPyramidingSettingsDlg dialog

IMPLEMENT_DYNAMIC(CPyramidingSettingsDlg, CDialog)
CPyramidingSettingsDlg::CPyramidingSettingsDlg(TSLRasterInterpolation & interpolation,
                                               bool & multipage,
                                               TSLRasterType & rtype,
                                               int & compressionFactor, 
                                               CWnd* pParent /*=NULL*/)
	: CDialog(CPyramidingSettingsDlg::IDD, pParent)
        , m_interpolation(interpolation)
        , m_multipage(multipage)
        , m_rtype(rtype)
        , m_compressionFactor(compressionFactor)
{
}

CPyramidingSettingsDlg::~CPyramidingSettingsDlg()
{
}

void CPyramidingSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPyramidingSettingsDlg, CDialog)
END_MESSAGE_MAP()


// CPyramidingSettingsDlg message handlers

BOOL CPyramidingSettingsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CEdit* factorEdit = (CEdit*) GetDlgItem(IDC_FACTOR);
  CString buffer;
  buffer.Format( _T("%d"), m_compressionFactor );
  factorEdit->SetWindowText(buffer);

  CComboBox* interCombo = (CComboBox*) GetDlgItem(IDC_INTERPOLATION);
  interCombo->SetCurSel((int)m_interpolation);

  CComboBox* formatCombo = (CComboBox*) GetDlgItem(IDC_RASTER_FORMAT);
  switch(m_rtype)
  {
  case TSLRasterTypeBmp:    formatCombo->SetCurSel(1); break;
  case TSLRasterTypeJpeg:   formatCombo->SetCurSel(2); break;
  case TSLRasterTypePCX:    formatCombo->SetCurSel(3); break;
  case TSLRasterTypeJpeg2K: formatCombo->SetCurSel(4); break;
  case TSLRasterTypeTiff: 
  default:
    formatCombo->SetCurSel(0); break;
  }

  CButton* multiButton = (CButton*) GetDlgItem(IDC_MPAGE);
  multiButton->SetCheck(m_multipage);

  return TRUE;  
}

void CPyramidingSettingsDlg::OnOK()
{
  CButton* multiButton = (CButton*) GetDlgItem(IDC_MPAGE);

  CEdit* factorEdit = (CEdit*) GetDlgItem(IDC_FACTOR);

  CComboBox* interCombo = (CComboBox*) GetDlgItem(IDC_INTERPOLATION);


  CComboBox* formatCombo = (CComboBox*) GetDlgItem(IDC_RASTER_FORMAT);

  CString buffer;
  factorEdit->GetWindowText(buffer);
  int compressionFactor = _ttoi(buffer);
  if ( compressionFactor < 2 || compressionFactor > 255 )
  {
    AfxMessageBox(_T("Please enter a valid compression factor"));
    return;
  }
  m_compressionFactor = compressionFactor;

  switch (formatCombo->GetCurSel())
  {
  case 1:
    {
      m_rtype = TSLRasterTypeBmp;
      break;
    }
  case 2:
    {
      m_rtype = TSLRasterTypeJpeg;
      break;
    }
  case 3:
    {
      m_rtype = TSLRasterTypePCX;
      break;
    }
  case 4:
    {
      m_rtype = TSLRasterTypeJpeg2K;
      break;
    }
  case 0:
  default:
    {
      m_rtype = TSLRasterTypeTiff;
      break;
    }
  }

  m_interpolation = (TSLRasterInterpolation)interCombo->GetCurSel();;
  m_multipage = multiButton->GetCheck();

  CDialog::OnOK();
}
