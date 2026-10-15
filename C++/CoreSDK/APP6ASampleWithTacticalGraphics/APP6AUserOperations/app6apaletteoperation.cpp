//## begin module%1.4%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.4%.codegen_version

//## begin module%403C7C770373.cm preserve=no
//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************
//## end module%403C7C770373.cm

//## begin module%403C7C770373.cp preserve=no
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
//  MODULE      : app6apaletteoperation.cpp
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 12:08:33
//  ********************************************************
//## end module%403C7C770373.cp

//## Module: APP6APaletteOperation%403C7C770373; Pseudo Package body
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6apaletteoperation.cpp

//## begin module%403C7C770373.additionalIncludes preserve=no
//## end module%403C7C770373.additionalIncludes

//## begin module%403C7C770373.includes preserve=yes
#include "stdafx.h"
#include "app6apaletteoperation.h"

// Number from messages file
#define AP_APP6A_PALETTE 300
#define AE_APP6A_PALETTE 301
#define AE_APP6A_NOTHING_SELECTED 302

// Set this definition to 1 to redirect any callbacks that this class receives from
// the properties dialog (within the palette i.e. the one displayed when you click "properties")
// to the app6apropertiesoperation. Otherwise the properties callbacks from the internal
// properties gui (only) will be handled here.
#define REDIRECT_PROPERTIES_CALLBACKS 0

//## end module%403C7C770373.includes

//## begin module%403C7C770373.additionalDeclarations preserve=yes
APP6APaletteOperation::APP6APaletteOperation(TSLAPP6ARequest*additionalPropertiesCallback )
: m_handler(0),m_dlg(0),m_additionalPropertiesCallback(additionalPropertiesCallback)
{
}
//## end module%403C7C770373.additionalDeclarations


// Class APP6APaletteOperation 

APP6APaletteOperation::APP6APaletteOperation ()
  //## begin APP6APaletteOperation::APP6APaletteOperation%403C7D8101D1.hasinit preserve=no
      : m_handler(0),  m_dlg(0),m_additionalPropertiesCallback(0)
  //## end APP6APaletteOperation::APP6APaletteOperation%403C7D8101D1.hasinit
  //## begin APP6APaletteOperation::APP6APaletteOperation%403C7D8101D1.initialization preserve=yes
  //## end APP6APaletteOperation::APP6APaletteOperation%403C7D8101D1.initialization
{
  //## begin APP6APaletteOperation::APP6APaletteOperation%403C7D8101D1.body preserve=yes
  //## end APP6APaletteOperation::APP6APaletteOperation%403C7D8101D1.body
}


APP6APaletteOperation::~APP6APaletteOperation ()
{
  //## begin APP6APaletteOperation::~APP6APaletteOperation%403C7D850082.body preserve=yes
  delete m_dlg ;
  m_dlg = 0 ;
  //## end APP6APaletteOperation::~APP6APaletteOperation%403C7D850082.body
}



//## Other Operations (implementation)
int APP6APaletteOperation::activate (void *data)
{
  //## begin APP6APaletteOperation::activate%403C7D9F012A.body preserve=yes
  
  if ( ! m_dlg )
  {

    TSLErrorStack::clear() ;

    CWnd * parent = (CWnd *) data ;
    HWND wnd = parent->GetSafeHwnd () ;
    
    // Set the callback to be this. Note that from MapLink 5.0 onwards
    // this will automatically update the internal properties window so that
    // properties callback messages are handled here.
    m_dlg = new TSLAPP6APalette ( this, false, wnd ) ;

    if ( !m_dlg->valid() )
    {
      // The create may have failed in the constructor so
      // we need to check the valid() method for this. The app
      // won't fall over, it just won't do anything.
      const char * msg = TSLErrorStack::errorString() ;
      if ( msg )
        AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg) ) ;
    }
    else 
    {
#if REDIRECT_PROPERTIES_CALLBACKS
      // To redirect callbacks set the location of the tslapp6arequest derived
      // class as below. Otherwise the callbacks will be handled here.
      if (m_additionalPropertiesCallback)
      {
        m_dlg->getProperties(m_additionalPropertiesCallback);
      }
#endif
    }
  }

  // If you want to use the decluttering facility then you need
  // to set up the feature codes for the different symbol parts.
  // By default you can set the text on or off, graphics on or
  // off and the frame on or off. If you want to declutter individual
  // symbols parts then you must set the featureID before calling
  // getSymbolAsEntitySet. DEFAULT_TEXT, DEFAULT_GRAPHIC and DEFAULT_FRAME
  // are defined in tslapp6asymbol.h.
  TSLDataLayer * layer = m_handler->getEditableDataLayer () ;
  layer->addFeatureRendering( FN_TEXT, TSLAPP6A_DEFAULT_TEXT ) ;
  layer->addFeatureRendering( FN_GRAPHIC, TSLAPP6A_DEFAULT_GRAPHIC ) ;
  layer->addFeatureRendering( FN_FRAME, TSLAPP6A_DEFAULT_FRAME ) ;

  if ( m_dlg )
    m_dlg->showWindow( true ) ;

  return AP_APP6A_PALETTE ;
  //## end APP6APaletteOperation::activate%403C7D9F012A.body
}

int APP6APaletteOperation::done ()
{
  //## begin APP6APaletteOperation::done%403C7DBD0278.body preserve=yes
  if ( m_dlg )
    m_dlg->showWindow( false ) ;

  m_handler->deactivate () ;
  
  return 0 ;
  //## end APP6APaletteOperation::done%403C7DBD0278.body
}

int APP6APaletteOperation::locator (long x, long y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  //## begin APP6APaletteOperation::locator%403C7DCF00B1.body preserve=yes
  if( type != TSLLocatorRelease || button != TSLButtonLeft )
    return 0;
  
  if ( m_dlg )
  {
    // Grap the symbol from the palette.
    TSLAPP6ASymbol tmp ;
    if ( !m_dlg->getSelectedSymbol( tmp ) )
      return AE_APP6A_NOTHING_SELECTED ;
    
    // Set up the location.
    tmp.x ( x ) ;
    tmp.y ( y ) ;
    tmp.smallText( false ) ;

    // Convert it to an entityset.
    TSLEntitySet * es = getHelper()->getSymbolAsEntitySet( &tmp ) ;

    if ( !es )
      return AE_APP6A_PALETTE ;
    
    // Note: We now own es.
    TSLEntitySet * dles = m_handler->getEntitySet () ;

    if ( !dles )
    {
      es->destroy() ;
      return AE_APP6A_PALETTE ;
    }

    if ( !dles->insert( es ) )
    {
      es->destroy() ;
      return AE_APP6A_PALETTE ;
    }

    // Update the extent.
    m_handler->updateEntityExtent ( es ) ;
    // Redraw the extent of the entity.
    m_handler->redrawEntityExtent ( es, true ) ;

  }
  return AP_APP6A_PALETTE ;

  //## end APP6APaletteOperation::locator%403C7DCF00B1.body
}

void APP6APaletteOperation::requestHandler (TSLUserOperationRequest *handler)
{
  //## begin APP6APaletteOperation::requestHandler%403C7E370092.body preserve=yes
  m_handler = handler ;
  //## end APP6APaletteOperation::requestHandler%403C7E370092.body
}

void APP6APaletteOperation::onPaletteClose (unsigned long flags)
{
  //## begin APP6APaletteOperation::onPaletteClose%403C8C61014A.body preserve=yes

  // Called when the use clicks the close cross, or the ok
  // button within the palette window. All we need to do is
  // redirect the call to the done() method.
  
  // Check that this is not the result of our the done()s showWindow() method
  // or we will get in a loop. 
  if ( flags != APP6A_SHOW_WINDOW_CLOSE )
  {
    done() ;
  }

  //## end APP6APaletteOperation::onPaletteClose%403C8C61014A.body
}

// Additional Declarations
  //## begin APP6APaletteOperation%403C7C770373.declarations preserve=yes
bool APP6APaletteOperation::onPropertiesUpdate( unsigned long flags, const TSLAPP6ASymbol* symbol )
{

    // New to MapLink 5.0 is the ability to hande the selection (combo), text(edit), clicked(buttons)
    // messages from the properties window. These can be trapped with the following code. This example
    // shows how changes in the hostility combo can be trapped here. Note that these messages
    // will always appear with the APP6A_PROPS_UPDATING flag set to true.
    if ( flags & APP6A_PROPS_UPDATING )
    {

      if ( flags & APP6A_PROPS_SELCHANGED )
      {
	// Check for the hostility field (see app6aincludes.h for all fields).
	if ( flags & APP6A_FLD_HOSTILITY )
	{
	  TSLAPP6ASymbol::HostilityEnum host = symbol->hostility() ; 
	  ; // Handle changes here...
	}
      }

      // You can also trap the KILL_FOCUS windows message. In the case of combos and buttons
      // you would be better to trap the selection changes as shown above. However, you
      // may not want to trap the selection changes in an edit control as this will change
      // on each key press. Therefore trap the kill focus.
      if ( flags & APP6A_PROPS_KILLFOCUS )
      {
	// Check for the staff comments being changes.
	if ( flags & APP6A_FLD_STAFFCOMMENTS )
	{
	  const char * staffComments = symbol->staffComments() ; 
	  ; // Handle changes here.
	}
      }

      return true ;
    }
  
    // Note that the APP6A_PROPS_OK flag is not guarateed to be unique for all
    // the field flags, but they are guarateed not to appear with an APP6A_PROPS_UPDATING
    // flag. They also always appear as a single flag thus you can use the = operator.

    if ( flags == APP6A_PROPS_OK )
    {
      // Do something
      
    }
    
    if ( flags == APP6A_PROPS_APPLY )
    {
      // Do something
    }
    
  return true ; 
}
  //## end APP6APaletteOperation%403C7C770373.declarations

//## begin module%403C7C770373.epilog preserve=yes
//## end module%403C7C770373.epilog

