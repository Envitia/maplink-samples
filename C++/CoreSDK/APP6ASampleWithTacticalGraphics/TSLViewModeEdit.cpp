
#include "stdafx.h"
#include "TSLViewModeedit.h"
#include "OpMgr.h"

bool TSLViewModeEdit::OnMouseMove(TSLViewModeButton button, long x, long y, bool shift, bool control)
{
  TSLButtonType editorButton ;

  switch ( button )
  {
    case TSLViewModeNone   : editorButton = TSLButtonNone ;   break ;
    case TSLViewModeLeft   : editorButton = TSLButtonLeft;    break ;
    case TSLViewModeMiddle : editorButton = TSLButtonCentre ; break ;
    case TSLViewModeRight  : editorButton = TSLButtonRight ;  break ;
    default                : editorButton = TSLButtonNone ;   break ;
  }
  sendLocator( x, y, editorButton, TSLLocatorMotion, shift, control ) ;
  return false ;
}

bool TSLViewModeEdit::OnRButtonUp(long x, long y, bool shift, bool control)
{
  sendLocator( x, y, TSLButtonRight, TSLLocatorRelease, shift, control ) ;
  return false ;
}

bool TSLViewModeEdit::OnMButtonUp(long x, long y, bool shift, bool control)
{
  sendLocator( x, y, TSLButtonCentre, TSLLocatorRelease, shift, control ) ;
  return false ;
}

bool TSLViewModeEdit::OnLButtonUp(long x, long y, bool shift, bool control)
{
  sendLocator( x, y, TSLButtonLeft, TSLLocatorRelease, shift, control ) ;
  return false ;
}

bool TSLViewModeEdit::OnRButtonDown(long x, long y, bool shift, bool control)
{
  sendLocator( x, y, TSLButtonRight, TSLLocatorPress, shift, control ) ;
  return false ;
}

bool TSLViewModeEdit::OnMButtonDown(long x, long y, bool shift, bool control)
{
  sendLocator( x, y, TSLButtonCentre, TSLLocatorPress, shift, control ) ;
  return false ;
}

bool TSLViewModeEdit::OnLButtonDown(long x, long y, bool shift, bool control)
{
  sendLocator( x, y, TSLButtonLeft, TSLLocatorPress, shift, control ) ;
  return false ;
}

TSLViewModeEdit::~TSLViewModeEdit()
{
  COpMgr::setEditor( NULL) ;
  if ( m_editor )
  {
    m_editor->destroy() ;
    m_editor = 0 ;
  }

}

TSLViewModeEdit::TSLViewModeEdit(TSLViewModeInterface* iViewMode, unsigned long modeID )
  : TSLViewMode( iViewMode, modeID )
{
  m_editor = new TSLEditor( &m_requestHandler ) ;
  if ( !m_editor )
    return ;

  TSLSimpleString filename;
  TSLProfileHelper::getDefaultFilename(filename);

  if ( !m_editor->initialise(filename.empty() ? "" : filename.c_str() , "editor" ) )
  {
    m_editor->destroy() ;
    m_editor = 0;
    return ;
  }

  TSLAllOperations::add( m_editor ) ;

  m_editor->attach( m_drawingSurface ) ;
  m_editor->dataChanged( ) ;

  m_editor->reset() ;

  // The OpMgr will add all custom operations
  COpMgr::setEditor( m_editor ) ;
}

void TSLViewModeEdit::sendLocator(long x, long y, TSLButtonType button, TSLLocatorType locator, bool shift, bool control)
{
  if ( m_editor && m_allowEdit )
  {
    TSLLocatorEvent event ;

    m_drawingSurface->DUToTMC( x, y, &event.m_x, &event.m_y ) ;
 
    event.m_buttonType  = button ;
    event.m_locatorType = locator ;

    if ( shift && control )
      event.m_modifiers = TSLModifierShiftAndControl ;
    else if ( shift )
      event.m_modifiers = TSLModifierShift ;
    else if ( control )
      event.m_modifiers = TSLModifierControl ;
    else
      event.m_modifiers = TSLModifierNone ;

    m_editor->locator( event ) ;
  }
}

void TSLViewModeEdit::allowEdit(bool sts)
{
  m_allowEdit = sts ;
}

const char * TSLViewModeEdit::queryPrompt( )
{
  return m_allowEdit ? m_requestHandler.queryPrompt() : "" ;
}


