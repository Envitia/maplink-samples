
#include "stdafx.h"
#include "resource.h"
#include "AttributePanel.h"
#include "TSLTextEntry.h"
#include "OpMgr.h"

std::string COpMgr::m_prompt = "";

std::string COpMgr::m_cursorReadout = "";

TSLCursorMovementType  COpMgr::m_cursorReadoutType = TSLCursorMovementUUAbsolute ;

std::string  COpMgr::m_cursorReadoutFormat  = "Pos: (%g, %g)";

int COpMgr::m_numSelected = 0;

int COpMgr::m_selectDepth = 0;

long COpMgr::m_activeOperation = -1;

TSLAttributeData COpMgr::m_attributes;

TSLEditor* COpMgr::m_editor;

long COpMgr::m_activeConstraint = -1;

double COpMgr::m_activeConstraintValue = 0.0 ;

bool COpMgr::m_allowEdit = false;

CAttributePanel *COpMgr::m_attributePanel;

void COpMgr::polygon(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "polygon" ) ;
}

void COpMgr::polyline(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "polyline" ) ;
}

void COpMgr::rectangle(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "rectangle" ) ;
}

void COpMgr::symbol(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "symbol" ) ;
}

void COpMgr::circle(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "circle" ) ;
}

void COpMgr::text(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "text" ) ;
}

void COpMgr::fillColour(long val)
{
  m_attributes.m_fillColour = val ;
    
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_fillColour = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::select(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "select" ) ;
}

void COpMgr::edgeColour(long val)
{
  m_attributes.m_lineColour = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_lineColour = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::edgeStyle(long style)
{
  m_attributes.m_lineStyle     = style ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_lineStyle     = style ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::fillStyle(long style)
{
  m_attributes.m_fillStyle      = style ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_fillStyle = style ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::edgeThickness(long thickness)
{
  m_attributes.m_lineThickness      = thickness ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_lineThickness     = thickness ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textColour(long val)
{
  m_attributes.m_textColour = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textColour = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textBackgroundMode( TSLTextBackgroundMode val)
{
  m_attributes.m_textBackgroundMode = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textBackgroundMode = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textBackgroundStyle( int val)
{
  m_attributes.m_textBackgroundStyle = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textBackgroundStyle = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textBackgroundColour( int val)
{
  m_attributes.m_textBackgroundColour = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textBackgroundColour = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textBackgroundEdgeColour( int val)
{
  m_attributes.m_textBackgroundEdgeColour = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textBackgroundEdgeColour = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}


void COpMgr::textHeight(long val)
{
  m_attributes.m_textHeight = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textHeight = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textAlignment( TSLHorizontalAlignment halign, TSLVerticalAlignment valign )
{
  m_attributes.m_textHAlignment = halign ;
  m_attributes.m_textVAlignment = valign ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textHAlignment = halign ;
    attributes.m_textVAlignment = valign ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::textFont(long val)
{
  m_attributes.m_textFont = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_textFont = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::symbolColour(long val)
{
  m_attributes.m_symbolColour = val ;
  
  if ( m_editor )
  {
    TSLAttributeData attributes ;
    
    attributes.m_symbolColour = val ;
    m_editor->activate( "attributeset", &attributes ) ;
  }
}

void COpMgr::equal(long id)
{
  m_activeConstraint = id ;
  if ( m_editor )
    m_editor->constraint( TSLEditorConstraintEqual ) ;
}

void COpMgr::angular(long id, double value )
{
  m_activeConstraint = id ;
  m_activeConstraintValue = value ;
  if ( m_editor )
    m_editor->constraint( TSLEditorConstraintAngle, value ) ;
}

void COpMgr::distance(long id, double value )
{
  m_activeConstraint = id ;
  m_activeConstraintValue = value ;
  if ( m_editor )
    m_editor->constraint( TSLEditorConstraintDistance, value * USER_UNIT_SIZE ) ;
}

void COpMgr::unequal(long id)
{
  m_activeConstraint = id ;
  if ( m_editor )
    m_editor->constraint( TSLEditorConstraintNone ) ;
}

void COpMgr::horizontal(long id)
{
  m_activeConstraint = id ;
  if ( m_editor )
    m_editor->constraint( TSLEditorConstraintHorizontal ) ;
}

void COpMgr::vertical(long id)
{
  m_activeConstraint = id ;
  if ( m_editor )
    m_editor->constraint( TSLEditorConstraintVertical ) ;
}

void COpMgr::clear(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "clear" ) ;
}

void COpMgr::copy(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "copy" ) ;
}

void COpMgr::undo()
{
  if ( m_editor )
    m_editor->undo() ;
}

void COpMgr::backup()
{
  if ( m_editor )
    m_editor->backup() ;
}

void COpMgr::selectAll(long id)
{
  if ( m_editor )
    m_editor->activate( "selectall" ) ;
}

void COpMgr::selectByExtent(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "selectbyextent" ) ;
}

void COpMgr::selectByPolygon(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "selectbypolygon" ) ;
}

void COpMgr::move(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "move" ) ;
}

void COpMgr::scale(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "scale" ) ;
}

void COpMgr::rotate(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "rotate" ) ;
}

void COpMgr::rotate90(long id)
{
  if ( m_editor )
    m_editor->activate( "rotate90" ) ;
}

void COpMgr::flipVertical(long id)
{
  if ( m_editor )
    m_editor->activate( "flipVertical" ) ;
}

void COpMgr::flipHorizontal(long id)
{
  if ( m_editor )
    m_editor->activate( "flipHorizontal" ) ;
}

void COpMgr::front(long id)
{
  if ( m_editor )
    m_editor->activate( "front" ) ;
}

void COpMgr::back(long id)
{
  if ( m_editor )
    m_editor->activate( "back" ) ;
}

void COpMgr::group(long id)
{
  if ( m_editor )
    m_editor->activate( "group" ) ;
}

void COpMgr::ungroup(long id)
{
  if ( m_editor )
    m_editor->activate( "ungroup" ) ;
}

void COpMgr::addPoint(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "topologicaladdpoint" ) ;
}

void COpMgr::deletePoint(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "topologicaldeletepoint" ) ;
}

void COpMgr::movePoint(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "topologicalmovepoint" ) ;
}

void COpMgr::attributeSet(TSLAttributeData *data)
{
  // Copy over the default attributes
  if ( data->m_lineStyle >= 0 )
    m_attributes.m_lineStyle = data->m_lineStyle ;
  if ( data->m_lineColour >= 0 )
    m_attributes.m_lineColour = data->m_lineColour ;
  if ( data->m_lineThickness >= 0 )
    m_attributes.m_lineThickness = data->m_lineThickness ;
  
  if ( data->m_borderWidth >= 0 )
    m_attributes.m_borderWidth = data->m_borderWidth ;
  if ( data->m_borderColour >= 0 )
    m_attributes.m_borderColour = data->m_borderColour ;
  
  if ( data->m_fillStyle >= 0 )
    m_attributes.m_fillStyle = data->m_fillStyle ;
  if ( data->m_fillColour >= 0 )
    m_attributes.m_fillColour = data->m_fillColour ;
  
  if ( data->m_textFont>= 0 )
    m_attributes.m_textFont = data->m_textFont ;
  if ( data->m_textColour >= 0 )
    m_attributes.m_textColour = data->m_textColour ;
  if ( data->m_textBackgroundMode != TSLTextBackgroundModeUndefined )
    m_attributes.m_textBackgroundMode = data->m_textBackgroundMode ;
  if ( data->m_textBackgroundColour >= 0 )
    m_attributes.m_textBackgroundColour = data->m_textBackgroundColour ;
  if ( data->m_textBackgroundEdgeColour >= 0 )
    m_attributes.m_textBackgroundEdgeColour = data->m_textBackgroundEdgeColour ;
  if ( data->m_textBackgroundStyle >= 0 )
    m_attributes.m_textBackgroundStyle = data->m_textBackgroundStyle ;

  if ( data->m_textHeight >= 0 )
    m_attributes.m_textHeight = data->m_textHeight ;
  
  if ( data->m_symbolStyle >= 0 )
    m_attributes.m_symbolStyle = data->m_symbolStyle ;
  if ( data->m_symbolColour >= 0 )
    m_attributes.m_symbolColour = data->m_symbolColour ;
  if ( data->m_symbolSize >= 0 )
    m_attributes.m_symbolSize = data->m_symbolSize ;
  
  if ( m_editor )
    m_editor->activate( "attributeset", data ) ;
}

void COpMgr::cursorReadoutType( TSLCursorMovementType type, const char * format )
{
  m_cursorReadoutType = type ;
  m_cursorReadoutFormat = format ;
  
  if ( m_editor )
    m_editor->cursorMovementConfig( type, true, format ) ;
}

long COpMgr::queryActiveOperation()
{
  return m_activeOperation;
}

long COpMgr::queryConstraint()
{
  return m_activeConstraint;
}

void COpMgr::setEditor(TSLEditor * editor)
{
  m_editor = editor ;
  if( m_editor)
  {
    // Add user operations here
    // Must initialise here, otherwise combo boxes not initialise properly
    m_attributePanel->initialiseColours();
    m_editor->cursorMovementConfig( m_cursorReadoutType, true, m_cursorReadoutFormat.c_str() ) ;
  }
}

void COpMgr::deleteOp(long id)
{
  m_activeOperation = id ;

  if ( m_editor )
    m_editor->activate( "delete" ) ;
}

void COpMgr::setPrompt(const char * prompt)
{
  m_prompt = prompt ;
}

void COpMgr::setCursorReadout(const char * readout)
{
  m_cursorReadout = readout ;
}

void COpMgr::setSelected(int num, int depth)
{
  m_numSelected = num ;
  m_selectDepth = depth ;
}

const char * COpMgr::queryPrompt()
{
  return m_prompt.c_str() ;
}

const char * COpMgr::queryCursorReadout()
{
  return m_cursorReadout.c_str() ;
}

int COpMgr::querySelected(int & depth)
{
  depth = m_selectDepth ;
  return m_numSelected ;
}


void COpMgr::allowEdit(bool sts)
{
  m_allowEdit = sts ;
  if ( !sts )
  {
    m_editor->reset() ;
    m_activeOperation = -1 ;
  }
}

bool COpMgr::queryAllowEdit()
{
  return m_allowEdit ;
}

void COpMgr::setAttributePanel(CAttributePanel *  attributePanel)
{
  m_attributePanel = attributePanel;
}


bool COpMgr::dataChanged()
{
  if ( m_editor )
  {
    return m_editor->dataChanged();
  }
  return false;
}

bool COpMgr::queryChanged()
{
  if ( m_editor )
    return m_editor->changed();
  return false;
}

void COpMgr::setChanged( bool sts )
{
  if ( m_editor )
    m_editor->changed( sts );
}

void COpMgr::reset( )
{
  if ( m_editor )
    m_editor->reset() ;
}

void COpMgr::setText(long id)
{
  m_activeOperation = id ;
  if ( m_editor )
    m_editor->activate( "settext" ) ;
}

void COpMgr::editFeatureID(long id)
{
  if ( m_editor )
    m_editor->activate( "editFeatureID" ) ;
}
