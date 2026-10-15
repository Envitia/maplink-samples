
#include "stdafx.h"
#include "EditorSample.h"
#include "EditorRequest.h"
#include "OpMgr.h"
#include "TSLTextEntry.h"
#include "TSLUserDialog.h"

CEditorRequest::CEditorRequest()
{
}

CEditorRequest::~CEditorRequest()
{
}

bool CEditorRequest::displayError(const char * message)
{
  AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(message) ) ;
  return true ;
}

bool CEditorRequest::displayPrompt(const char * message)
{
  m_prompt = message ;
  return true ;
}

bool CEditorRequest::onOperationDeactivated()
{
  COpMgr::select(0 ) ;
  return true ;
}

bool CEditorRequest::onSelectionChanged(int numEntities, int depth)
{
  COpMgr::setSelected( numEntities, depth ) ;
  return true ;
}

bool CEditorRequest::onCursorMovement( const char * readout, double x, double y ) const
{
  COpMgr::setCursorReadout( readout ) ;
  return true ;
}


bool CEditorRequest::requestDialog(const char * prompt, const char * label1, const char * label2, const char * label3, const char * label4)
{
  // Since the dialog is modal, we don't have to implement the cancelDialog
  TSLUserDialog dlog ;

  Int num = dlog.askForData( prompt, label1, label2, label3, label4  ) ;

  if ( num == IDCANCEL || num == IDABORT )
    m_editor->dialogEntered( -1 ) ;
  else
    m_editor->dialogEntered( num - 100 ) ;

  return true ;
}

bool CEditorRequest::requestText(const char * prompt, const char * initialValue)
{
  // Since the dialog is modal, we don't have to implement the cancelText
  TSLTextEntry dlog ;

  std::string value = initialValue ;

  bool cancelled = dlog.askForData( prompt, value ) ;

  m_editor->textEntered( value.c_str(), cancelled ) ;

  return true ;
}

const char * CEditorRequest::queryPrompt( )
{
  return m_prompt.c_str() ;
}
