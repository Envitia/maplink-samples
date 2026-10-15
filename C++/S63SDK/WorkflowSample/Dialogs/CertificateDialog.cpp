// CertificateDialog.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63/s63workflow.h"

#include "CertificateDialog.h"


// CertificateDialog dialog

IMPLEMENT_DYNAMIC(CertificateDialog, CDialog)

CertificateDialog::CertificateDialog(S63Workflow * workflow, const bool dataServerCertificate, CWnd* pParent /*=NULL*/)
	: CDialog(CertificateDialog::IDD, pParent)
        , m_workFlow(workflow)
        , m_dataServerCertificate(dataServerCertificate)
        , m_certificateInformation()
        , m_certificatePath()
        , m_expiryDate()
        , m_certificateInput()
        , m_validFrom()
        , m_validTo()
        , m_okButton()
        
{
}

CertificateDialog::~CertificateDialog()
{
  m_workFlow = 0;
}

void CertificateDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_CERTIFICATE, m_certificatePath );

  DDX_Control(pDX, IDC_CERTIFICATE, m_certificateInput);
  DDX_Control(pDX, IDC_INFORMATION, m_certificateInformation);
  DDX_Control(pDX, IDC_VALID_FROM, m_validFrom);
  DDX_Control(pDX, IDC_VALID_UNTIL, m_validTo);
  DDX_Control(pDX, IDOK, m_okButton);
}

BEGIN_MESSAGE_MAP(CertificateDialog, CDialog)

  ON_BN_CLICKED(IDC_CERTIFICATE_BROWSE, &CertificateDialog::OnBnClickedCertificateBrowse)
  ON_BN_CLICKED(IDOK, &CertificateDialog::OnBnClickedOk)
END_MESSAGE_MAP()

void CertificateDialog::OnBnClickedCertificateBrowse()
{
  CFileDialog FileDlg(true, _T(".crt"), NULL, OFN_NOCHANGEDIR, _T("Certificate Files (*.crt)|*.crt|Certificate Files (*.cer)|*.cer|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Open Certificate");

  if( FileDlg.DoModal() != IDOK )
  {
    return; 
  }
    
  m_certificatePath = FileDlg.GetPathName();

  UpdateData(FALSE);

  //see if what they entered is a correct media file
  updateCertificateInformation();
}

void CertificateDialog::updateCertificateInformation()
{
  TSLTimeType validTo;
  TSLTimeType validFrom;
  TSLSimpleString certificateInformation;

  TSLUTF8Encoder certificatePathUTF8( m_certificatePath );
  string certificatePath( certificatePathUTF8 );

  if ( m_workFlow->queryCertificate( certificatePath, validFrom, validTo, certificateInformation) )
  {
    //format the dates
    CTime validToDate(validTo);
    CTime validFromDate(validFrom);
 
    string information( certificateInformation.c_str() );

    //replace commas with new lines        
    for( size_t i = 0; i < information.length(); i++ )
    {
      if ( information[i] == ',' && information[i+1] == ' ')
      {
        information.replace( (int)i, 2, "\n");
        i+=2;
      }
    }

    m_validTo.SetWindowText(validToDate.Format(_T("%A, %B %d, %Y")));
    m_validFrom.SetWindowText(validFromDate.Format(_T("%A, %B %d, %Y")));
    m_certificateInformation.SetWindowText((LPCTSTR)TSLUTF8Decoder(information)); 
    m_expiryDate = validToDate.Format(_T("%A, %B %d, %Y"));
    m_okButton.EnableWindow(true);
  }
  else
  {
    AfxMessageBox(_T("The selected file is not a valid certificate."), MB_ICONERROR);
    m_validTo.SetWindowText(_T("-"));
    m_validFrom.SetWindowText(_T("-"));
    m_expiryDate = _T("");
    m_certificateInformation.SetWindowText(_T("-"));
    m_okButton.EnableWindow(false);
  }

}
void CertificateDialog::OnBnClickedOk()
{
  CDialog::OnOK();
}
