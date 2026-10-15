#include "stdafx.h"
#include "TacticalGraphicsOperations.h"
#include "TacticalGraphics/tacticalgraphicfactory.h"

#include "areas/attackByFirePosition.h"
#include "areas/supportByFirePosition.h"

using namespace envitia::MapLink;

/////////////////////////////////////////////
// Complex arrow - with variable sized head
/////////////////////////////////////////////
CTacticalGraphicComplexArrowOperation::CTacticalGraphicComplexArrowOperation(void)
{
  m_request = 0 ;
}

CTacticalGraphicComplexArrowOperation::~CTacticalGraphicComplexArrowOperation(void)
{
}

void CTacticalGraphicComplexArrowOperation::setEchoAndPrompt()
{
  switch( m_state )
  {
  case SelectingTipOfArrowHead:
    {
      m_request->displayPrompt( "Select tip of arrow head" ) ;
      m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingBackOfArrowHead:
    {
      m_request->displayPrompt( "Select back of the arrow head" ) ;
      m_request->setFixedEcho( TSLFixedEchoLine, &m_primitiveEchoLine[0], (int)m_primitiveEchoLine.size() ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }

  case SelectingTailOfArrow:
    {
      m_request->displayPrompt( "Select tail of arrow, RMB when finished" ) ;
      m_request->setFixedEcho( TSLFixedEchoLine, &m_primitiveEchoLine[0], (int)m_primitiveEchoLine.size() ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }

  case SelectingSizeOfArrowHead:
    {
      m_request->displayPrompt( "Select size of arrow head" ) ;
      m_request->setFixedEcho( TSLFixedEchoLine, &m_primitiveEchoLine[0], (int)m_primitiveEchoLine.size() ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }
  }
}

int CTacticalGraphicComplexArrowOperation::activate (void* data)
{
  m_state = SelectingTipOfArrowHead ;
  m_request->displayHighlights( false ) ;
  m_primitiveEchoLine.clear() ;
  m_tailOfArrow.clear() ;

  setEchoAndPrompt() ;

  return 0 ; // Already displaying the prompt I want
}

int CTacticalGraphicComplexArrowOperation::locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  // Only accept  left button press
  if ( button != TSLButtonLeft || type != TSLLocatorPress )
    return 0 ;

  switch ( m_state )
  {
  case SelectingTipOfArrowHead:
    {
      // Remember the tip of the arrow
      m_tipOfArrowHead.x( x ) ;
      m_tipOfArrowHead.y( y ) ;

      // Set up the dynamic echo for feedback when picking the back of the arrow head
      m_dynamicEchoLine[0].m_type = TSLEchoCoordFixed ;  // Fixed position in TMC
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      m_dynamicEchoLine[1].m_type = TSLEchoCoordMoving;  // Offset from the cursor position
      m_dynamicEchoLine[1].m_x = 0 ;
      m_dynamicEchoLine[1].m_y = 0 ;

      // Increment the state engine
      m_state = SelectingBackOfArrowHead ;

      setEchoAndPrompt() ;

      break ;
    }

  case SelectingBackOfArrowHead:
    {
      // Remember the tip of the arrow
      m_backOfArrowHead.x( x ) ;
      m_backOfArrowHead.y( y ) ;

      // This point is the start of the tail
      m_tailOfArrow.push_back( m_backOfArrowHead ) ;

      // Set up the primitive echo to show the baseline
      m_primitiveEchoLine.push_back( m_tipOfArrowHead ) ;
      m_primitiveEchoLine.push_back( m_backOfArrowHead ) ;

      // Update the start of the dynamic echo line - the rest stays the same
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      // Increment the state engine
      m_state = SelectingTailOfArrow ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingTailOfArrow:
    {
      m_tailOfArrow.push_back( TSLCoord( x, y ) ) ;	

      m_primitiveEchoLine.push_back( TSLCoord( x, y ) ) ;

      // Update the start of the dynamic echo line - the rest stays the same
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingSizeOfArrowHead:
    {
      // Remember the selection of the arrow head size
      m_sizeOfArrowHead.x( x ) ;
      m_sizeOfArrowHead.y( y ) ;     

      // Now construct the coordinate array as per the APP6 standard
      TSLCoordSet * coords = new TSLCoordSet ;
      coords->add( m_tipOfArrowHead ) ;
      coords->add( m_backOfArrowHead ) ;
      if ( m_tailOfArrow.size() > 1 )
        coords->addCoords( &m_tailOfArrow[1], (int)m_tailOfArrow.size()-1 ) ;
      coords->add( m_sizeOfArrowHead ) ;

      // Now create the graphic, add it to the select list, refresh the screen and complete the operation
      TSLEntity * graphic = createSpecificGraphic( coords ) ;
      coords->destroy() ;
      if ( graphic )
      {
        TSLDataLayer *dataLayer = m_request->getEditableDataLayer();
        if (dataLayer->layerType() == TSLDataLayerTypeStandardDataLayer)
        {
          TSLStandardDataLayer *stdLayer = static_cast<TSLStandardDataLayer*>(dataLayer);

          stdLayer->entitySet()->insert( graphic ) ;
          stdLayer->notifyChanged( true ) ;
        }

        // Send some feedback in the application is using that
        TSLEntityReferenceSet *set = new TSLEntityReferenceSet ;
        set->add( graphic ) ;
        m_request->feedbackCreated( set, false ) ;

        m_request->clearSelectList( false ) ;
        m_request->select( graphic ) ;
        m_request->feedbackSelected( set ) ;
        set->destroy() ;

        m_request->redrawEntityExtent( graphic, false );
      }
      m_request->deactivate( ) ;	
      break ;
    }
  }

  return 0 ;
}

int CTacticalGraphicComplexArrowOperation::done( )
{
  // If we're drawing the tail, then this signifies it's complete and move onto the next state
  if ( m_state == SelectingTailOfArrow )
  {
    m_state = SelectingSizeOfArrowHead ;
    m_dynamicEchoLine[0].m_x = m_tipOfArrowHead.x() ;
    m_dynamicEchoLine[0].m_y = m_tipOfArrowHead.y() ;    
    setEchoAndPrompt() ;
    return 0 ;
  }

  // Otherwise it's treated as aborting the operation
  m_request->deactivate( ) ;
  return 0 ;
}

///////////////////////////////////////////////////////////////////
// Now the simple two point selection operation
///////////////////////////////////////////////////////////////////
CTacticalGraphicTwoPointOperation::CTacticalGraphicTwoPointOperation( const char * prompt1, const char * prompt2)
  : m_firstPrompt( prompt1 ), m_secondPrompt( prompt2 )
{
  m_request = 0 ;
}

CTacticalGraphicTwoPointOperation::~CTacticalGraphicTwoPointOperation(void)
{
}

void CTacticalGraphicTwoPointOperation::setEchoAndPrompt()
{
  switch( m_state )
  {
  case SelectingFirstPoint:
    {
      m_request->displayPrompt( m_firstPrompt ) ;
      m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingSecondPoint:
    {
      m_request->displayPrompt( m_secondPrompt ) ;
      m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }
  }
}

int CTacticalGraphicTwoPointOperation::activate (void* data)
{
  m_state = SelectingFirstPoint ;
  m_request->displayHighlights( false ) ;

  setEchoAndPrompt() ;

  return 0 ; // Already displaying the prompt I want
}

int CTacticalGraphicTwoPointOperation::locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  // Only accept  left button press
  if ( button != TSLButtonLeft || type != TSLLocatorPress )
    return 0 ;

  switch ( m_state )
  {
  case SelectingFirstPoint:
    {
      // Remember the tip of the arrow
      m_firstPoint.x( x ) ;
      m_firstPoint.y( y ) ;

      // Set up the dynamic echo for feedback when picking the back of the arrow head
      m_dynamicEchoLine[0].m_type = TSLEchoCoordFixed ;  // Fixed position in TMC
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      m_dynamicEchoLine[1].m_type = TSLEchoCoordMoving;  // Offset from the cursor position
      m_dynamicEchoLine[1].m_x = 0 ;
      m_dynamicEchoLine[1].m_y = 0 ;

      // Increment the state engine
      m_state = SelectingSecondPoint ;

      setEchoAndPrompt() ;

      break ;
    }

  case SelectingSecondPoint:
    {
      // Remember the back of the arrow
      m_secondPoint.x( x ) ;
      m_secondPoint.y( y ) ;

      // Now construct the coordinate array as per the APP6 standard
      TSLCoordSet * coords = new TSLCoordSet ;
      coords->add( m_firstPoint ) ;
      coords->add( m_secondPoint ) ;

      // Now create the graphic, add it to the select list, refresh the screen and complete the operation
      TSLEntity * graphic = createSpecificGraphic( coords ) ;
      coords->destroy() ;
      if ( graphic )
      {
        TSLDataLayer *dataLayer = m_request->getEditableDataLayer();
        if (dataLayer->layerType() == TSLDataLayerTypeStandardDataLayer)
        {
          TSLStandardDataLayer *stdLayer = static_cast<TSLStandardDataLayer*>(dataLayer);

          stdLayer->entitySet()->insert( graphic ) ;
          stdLayer->notifyChanged( true ) ;
        }

        // Send some feedback in the application is using that
        TSLEntityReferenceSet *set = new TSLEntityReferenceSet ;
        set->add( graphic ) ;
        m_request->feedbackCreated( set, false ) ;

        m_request->clearSelectList( false ) ;
        m_request->select( graphic ) ;
        m_request->feedbackSelected( set ) ;
        set->destroy() ;

        m_request->redrawEntityExtent( graphic, false );
      }
      m_request->deactivate( ) ;
      break ;
    }
  }

  return 0 ;
}

int CTacticalGraphicTwoPointOperation::done( )
{
  // Treat as aborting the operation
  m_request->deactivate( ) ;
  return 0 ;
}

///////////////////////////////////////////////////////////////////
// Now the simple three point selection operation
///////////////////////////////////////////////////////////////////
CTacticalGraphicThreePointOperation::CTacticalGraphicThreePointOperation( const char * prompt1, const char * prompt2, const char * prompt3)
  : m_firstPrompt( prompt1 ), m_secondPrompt( prompt2 ), m_thirdPrompt( prompt3 )
{
  m_request = 0 ;
}

CTacticalGraphicThreePointOperation::~CTacticalGraphicThreePointOperation(void)
{
}

void CTacticalGraphicThreePointOperation::setEchoAndPrompt()
{
  switch( m_state )
  {
    case SelectingFirstPoint:
      {
	m_request->displayPrompt( m_firstPrompt ) ;
	m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
	break ;
      }

    case SelectingSecondPoint:
      {
	m_request->displayPrompt( m_secondPrompt ) ;
	m_request->setFixedEcho( TSLFixedEchoPoint, m_points, 1 ) ;
	break ;
      }  

    case SelectingThirdPoint:
      {
	m_request->displayPrompt( m_thirdPrompt ) ;
	m_request->setFixedEcho( TSLFixedEchoPoint, m_points, 2 ) ;
	break ;
      }
  }
}

int CTacticalGraphicThreePointOperation::activate (void* data)
{
  m_state = SelectingFirstPoint ;
  m_request->displayHighlights( false ) ;

  setEchoAndPrompt() ;

  return 0 ; // Already displaying the prompt I want
}

int CTacticalGraphicThreePointOperation::locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  // Only accept  left button press
  if ( button != TSLButtonLeft || type != TSLLocatorPress )
    return 0 ;

  switch ( m_state )
  {
  case SelectingFirstPoint:
    {
      // Remember the point
      m_points[0].x( x ) ;
      m_points[0].y( y ) ;

      // Increment the state engine
      m_state = SelectingSecondPoint ;

      setEchoAndPrompt() ;

      break ;
    }

  case SelectingSecondPoint:
    {
      // Remember the point
      m_points[1].x( x ) ;
      m_points[1].y( y ) ;

      // Increment the state engine
      m_state = SelectingThirdPoint ;

      setEchoAndPrompt() ;

      break ;
    }
  case SelectingThirdPoint:
    {
      // Remember the back of the arrow
      m_points[2].x( x ) ;
      m_points[2].y( y ) ;

      // Now construct the coordinate array as per the APP6 standard
      TSLCoordSet * coords = new TSLCoordSet ;
      coords->addCoords( m_points, 3 ) ;

      // Now create the graphic, add it to the select list, refresh the screen and complete the operation
      TSLEntity * graphic = createSpecificGraphic( coords ) ;
      coords->destroy() ;
      if ( graphic )
      {
        TSLDataLayer *dataLayer = m_request->getEditableDataLayer();
        if (dataLayer->layerType() == TSLDataLayerTypeStandardDataLayer)
        {
          TSLStandardDataLayer *stdLayer = static_cast<TSLStandardDataLayer*>(dataLayer);

          stdLayer->entitySet()->insert( graphic ) ;
          stdLayer->notifyChanged( true ) ;
        }

        // Send some feedback in the application is using that
        TSLEntityReferenceSet *set = new TSLEntityReferenceSet ;
        set->add( graphic ) ;
        m_request->feedbackCreated( set, false ) ;

        m_request->clearSelectList( false ) ;
        m_request->select( graphic ) ;
        m_request->feedbackSelected( set ) ;
        set->destroy() ;

        m_request->redrawEntityExtent( graphic, false );
      }
      m_request->deactivate( ) ;
      break ;
    }
  }

  return 0 ;
}

int CTacticalGraphicThreePointOperation::done( )
{
  // Treat as aborting the operation
  m_request->deactivate( ) ;
  return 0 ;
}

///////////////////////////////////////////////////////////////////
// Now the simple arrow with tail
///////////////////////////////////////////////////////////////////

CTacticalGraphicSimpleArrowWithTailOperation::CTacticalGraphicSimpleArrowWithTailOperation(void)
{
  m_request = 0 ;
}

CTacticalGraphicSimpleArrowWithTailOperation::~CTacticalGraphicSimpleArrowWithTailOperation(void)
{
}

void CTacticalGraphicSimpleArrowWithTailOperation::setEchoAndPrompt()
{
  switch( m_state )
  {
  case SelectingTip:
    {
      m_request->displayPrompt( "Select tip of arrow" ) ;
      m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingSecondPoint:
    {
      m_request->displayPrompt( "Select next point of arrow tail" ) ;
      m_request->setFixedEcho( TSLFixedEchoLine, &m_primitiveEchoLine[0], (int)m_primitiveEchoLine.size() ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }

  case SelectingTailPoints:
    {
      m_request->displayPrompt( "Select next point of arrow tail, RMB when finished" ) ;
      m_request->setFixedEcho( TSLFixedEchoLine, &m_primitiveEchoLine[0], (int)m_primitiveEchoLine.size() ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }
  }
}

int CTacticalGraphicSimpleArrowWithTailOperation::activate (void* data)
{
  m_state = SelectingTip ;
  m_request->displayHighlights( false ) ;
  m_primitiveEchoLine.clear() ;
  m_tailOfArrow.clear() ;

  setEchoAndPrompt() ;

  return 0 ; // Already displaying the prompt I want
}

int CTacticalGraphicSimpleArrowWithTailOperation::locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  // Only accept  left button press
  if ( button != TSLButtonLeft || type != TSLLocatorPress )
    return 0 ;

  switch ( m_state )
  {
  case SelectingTip:
    {
      // Remember the tip of the arrow
      m_tipOfArrow.x( x ) ;
      m_tipOfArrow.y( y ) ;

      // Set up the dynamic echo for feedback when picking the back of the arrow head
      m_dynamicEchoLine[0].m_type = TSLEchoCoordFixed ;  // Fixed position in TMC
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      m_dynamicEchoLine[1].m_type = TSLEchoCoordMoving;  // Offset from the cursor position
      m_dynamicEchoLine[1].m_x = 0 ;
      m_dynamicEchoLine[1].m_y = 0 ;

      // Increment the state engine
      m_state = SelectingSecondPoint ;

      setEchoAndPrompt() ;

      break ;
    }

  case SelectingSecondPoint:
    {
      // Remember the tip of the arrow
      m_tailOfArrow.push_back( TSLCoord( x, y ) ) ;	

      // Set up the primitive echo to show the baseline
      m_primitiveEchoLine.push_back( m_tipOfArrow ) ;      
      m_primitiveEchoLine.push_back( TSLCoord( x, y ) ) ;

      // Update the start of the dynamic echo line - the rest stays the same
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      // Increment the state engine
      m_state = SelectingTailPoints ;

      setEchoAndPrompt() ;
      break ;
    } 

  case SelectingTailPoints:
    {
      // Remember the tip of the arrow
      m_tailOfArrow.push_back( TSLCoord( x, y ) ) ;	

      // Set up the primitive echo to show the baseline    
      m_primitiveEchoLine.push_back( TSLCoord( x, y ) ) ;

      // Update the start of the dynamic echo line - the rest stays the same
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      // No state change

      setEchoAndPrompt() ;
      break ;
    }
  }

  return 0 ;
}

int CTacticalGraphicSimpleArrowWithTailOperation::done( )
{
  // If we're drawing the tail, then this signifies it's complete and move onto the next state
  if ( m_state == SelectingTailPoints )
  {
    // Now construct the coordinate array as per the APP6 standard
    TSLCoordSet * coords = new TSLCoordSet ;
    coords->add( m_tipOfArrow ) ;
    coords->addCoords( &m_tailOfArrow[0], (int)m_tailOfArrow.size() ) ;

    // Now create the graphic, add it to the select list, refresh the screen and complete the operation
    TSLEntity * graphic = createSpecificGraphic( coords ) ;
    coords->destroy() ;
    if ( graphic )
    {
      TSLDataLayer *dataLayer = m_request->getEditableDataLayer();
      if (dataLayer->layerType() == TSLDataLayerTypeStandardDataLayer)
      {
        TSLStandardDataLayer *stdLayer = static_cast<TSLStandardDataLayer*>(dataLayer);

        stdLayer->entitySet()->insert( graphic ) ;
        stdLayer->notifyChanged( true ) ;
      }

      // Send some feedback in the application is using that
      TSLEntityReferenceSet *set = new TSLEntityReferenceSet ;
      set->add( graphic ) ;
      m_request->feedbackCreated( set, false ) ;

      m_request->clearSelectList( false ) ;
      m_request->select( graphic ) ;
      m_request->feedbackSelected( set ) ;
      set->destroy() ;

      m_request->redrawEntityExtent( graphic, false );
    }
    m_request->deactivate( ) ;	
    return 0 ;
  }

  // Otherwise it's treated as aborting the operation
  m_request->deactivate( ) ;
  return 0 ;
}

///////////////////////////////////////////////////////////////////
// Now the three point operations for an arrow with a back of some description
///////////////////////////////////////////////////////////////////

CTacticalGraphicArrowWithBackOperation::CTacticalGraphicArrowWithBackOperation(void)
{
  m_request = 0 ;
}

CTacticalGraphicArrowWithBackOperation::~CTacticalGraphicArrowWithBackOperation(void)
{
}

void CTacticalGraphicArrowWithBackOperation::setEchoAndPrompt()
{
  switch( m_state )
  {
  case SelectingTipOfArrow:
    {
      m_request->displayPrompt( "Select tip of arrow" ) ;
      m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingLeftOfBack:
    {
      m_request->displayPrompt( "Select left endpoint of the back of the graphic" ) ;
      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow, 1 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingRightOfBack:
    {
      m_request->displayPrompt( "Select right endpoint of the back of the graphic" ) ;
      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow, 1 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }
  }
}

int CTacticalGraphicArrowWithBackOperation::activate (void* data)
{
  m_state = SelectingTipOfArrow ;
  m_request->displayHighlights( false ) ;

  setEchoAndPrompt() ;

  return 0 ; // Already displaying the prompt I want
}

int CTacticalGraphicArrowWithBackOperation::locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  // Only accept  left button press
  if ( button != TSLButtonLeft || type != TSLLocatorPress )
    return 0 ;

  switch ( m_state )
  {
  case SelectingTipOfArrow:
    {
      // Remember the tip of the arrow
      m_tipOfArrow.x( x ) ;
      m_tipOfArrow.y( y ) ;

      // Increment the state engine
      m_state = SelectingLeftOfBack ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingLeftOfBack:
    {
      // Remember the left side of the back of the graphic
      m_backLeft.x( x ) ;
      m_backLeft.y( y ) ;

      // Update the start of the dynamic echo line - the rest stays the same
      // Set up the dynamic echo for feedback when picking the back of the arrow head
      m_dynamicEchoLine[0].m_type = TSLEchoCoordFixed ;  // Fixed position in TMC
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      m_dynamicEchoLine[1].m_type = TSLEchoCoordMoving;  // Offset from the cursor position
      m_dynamicEchoLine[1].m_x = 0 ;
      m_dynamicEchoLine[1].m_y = 0 ;

      // Increment the state engine
      m_state = SelectingRightOfBack ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingRightOfBack:
    {
      // Remember the right side of the back of the graphic
      m_backRight.x(x) ;
      m_backRight.y(y) ;

      // Now construct the coordinate array as per the APP6 standard
      TSLCoordSet * coords = new TSLCoordSet ;
      coords->add( m_tipOfArrow ) ;
      coords->add( m_backLeft ) ;
      coords->add( m_backRight ) ;

      // Now create the graphic, add it to the select list, refresh the screen and complete the operation
      TSLEntity * graphic = createSpecificGraphic( coords ) ;
      coords->destroy() ;
      if ( graphic )
      {
        TSLDataLayer *dataLayer = m_request->getEditableDataLayer();
        if (dataLayer->layerType() == TSLDataLayerTypeStandardDataLayer)
        {
          TSLStandardDataLayer *stdLayer = static_cast<TSLStandardDataLayer*>(dataLayer);

          stdLayer->entitySet()->insert( graphic ) ;
          stdLayer->notifyChanged( true ) ;
        }

        // Send some feedback in the application is using that
        TSLEntityReferenceSet *set = new TSLEntityReferenceSet ;
        set->add( graphic ) ;
        m_request->feedbackCreated( set, false ) ;

        m_request->clearSelectList( false ) ;
        m_request->select( graphic ) ;
        m_request->feedbackSelected( set ) ;
        set->destroy() ;

        m_request->redrawEntityExtent( graphic, false );
      }
      m_request->deactivate( ) ;	
      break ;
    }
  }

  return 0 ;
}

int CTacticalGraphicArrowWithBackOperation::done( )
{
  // Treat as aborting the operation
  m_request->deactivate( ) ;
  return 0 ;
}

/*******************************************************/
///////////////////////////////////////////////////////////////////
// Now the four point operations for two arrows with a back of some description
///////////////////////////////////////////////////////////////////

CTacticalGraphicTwoArrowsWithBackOperation::CTacticalGraphicTwoArrowsWithBackOperation(void)
{
  m_request = 0 ;
}

CTacticalGraphicTwoArrowsWithBackOperation::~CTacticalGraphicTwoArrowsWithBackOperation(void)
{
}

void CTacticalGraphicTwoArrowsWithBackOperation::setEchoAndPrompt()
{
  switch( m_state )
  {
  case SelectingTipOfArrow1:
    {
      m_request->displayPrompt( "Select tip of first arrow PT3" ) ;
      m_request->setFixedEcho( TSLFixedEchoNone, 0, 0 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingTipOfArrow2:
    {
      m_request->displayPrompt( "Select tip of second arrow PT4" ) ;
      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow1, 1 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingLeftOfBack:
    {
      m_request->displayPrompt( "Select left endpoint of the back of the graphic PT1" ) ;
      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow1, 1 ) ;
      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow2, 1 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoNone, 0, 0, 0 ) ;
      break ;
    }

  case SelectingRightOfBack:
    {
      m_request->displayPrompt( "Select right endpoint of the back of the graphic PT2" ) ;
      //      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow1, 1 ) ;
      //      m_request->setFixedEcho( TSLFixedEchoPoint, &m_tipOfArrow2, 1 ) ;
      m_request->setDynamicEcho( TSLDynamicEchoSegments, m_dynamicEchoLine, 2, 0 ) ;
      break ;
    }
  }
}

int CTacticalGraphicTwoArrowsWithBackOperation::activate (void* data)
{
  m_state = SelectingTipOfArrow1 ;
  m_request->displayHighlights( false ) ;

  setEchoAndPrompt() ;

  return 0 ; // Already displaying the prompt I want
}

int CTacticalGraphicTwoArrowsWithBackOperation::locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier)
{
  // Only accept  left button press
  if ( button != TSLButtonLeft || type != TSLLocatorPress )
    return 0 ;

  switch ( m_state )
  {
  case SelectingTipOfArrow1:
    {
      // Remember the tip of the arrow
      m_tipOfArrow1.x( x ) ;
      m_tipOfArrow1.y( y ) ;

      // Increment the state engine
      m_state = SelectingTipOfArrow2 ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingTipOfArrow2:
    {
      // Remember the tip of the arrow
      m_tipOfArrow2.x( x ) ;
      m_tipOfArrow2.y( y ) ;

      // Increment the state engine
      m_state = SelectingLeftOfBack ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingLeftOfBack:
    {
      // Remember the left side of the back of the graphic
      m_backLeft.x( x ) ;
      m_backLeft.y( y ) ;

      // Update the start of the dynamic echo line - the rest stays the same
      // Set up the dynamic echo for feedback when picking the back of the arrow head
      m_dynamicEchoLine[0].m_type = TSLEchoCoordFixed ;  // Fixed position in TMC
      m_dynamicEchoLine[0].m_x = x ;
      m_dynamicEchoLine[0].m_y = y ;

      m_dynamicEchoLine[1].m_type = TSLEchoCoordMoving;  // Offset from the cursor position
      m_dynamicEchoLine[1].m_x = 0 ;
      m_dynamicEchoLine[1].m_y = 0 ;

      // Increment the state engine
      m_state = SelectingRightOfBack ;

      setEchoAndPrompt() ;
      break ;
    }

  case SelectingRightOfBack:
    {
      // Remember the right side of the back of the graphic
      m_backRight.x(x) ;
      m_backRight.y(y) ;

      // Now construct the coordinate array as per the APP6 standard
      TSLCoordSet * coords = new TSLCoordSet ;
      coords->add( m_backLeft, 0 ) ;
      coords->add( m_backRight, 1 ) ;
      coords->add( m_tipOfArrow1, 2 ) ;
      coords->add( m_tipOfArrow2, 3 ) ;

      // Now create the graphic, add it to the select list, refresh the screen and complete the operation
      TSLEntity * graphic = createSpecificGraphic( coords ) ;
      coords->destroy() ;
      if ( graphic )
      {
        TSLDataLayer *dataLayer = m_request->getEditableDataLayer();
        if (dataLayer->layerType() == TSLDataLayerTypeStandardDataLayer)
        {
          TSLStandardDataLayer *stdLayer = static_cast<TSLStandardDataLayer*>(dataLayer);

          stdLayer->entitySet()->insert( graphic ) ;
          stdLayer->notifyChanged( true ) ;
        }

        // Send some feedback in the application is using that
        TSLEntityReferenceSet *set = new TSLEntityReferenceSet ;
        set->add( graphic ) ;
        m_request->feedbackCreated( set, false ) ;

        m_request->clearSelectList( false ) ;
        m_request->select( graphic ) ;
        m_request->feedbackSelected( set ) ;
        set->destroy() ;

        m_request->redrawEntityExtent( graphic, false );
      }
      m_request->deactivate( ) ;	
      break ;
    }
  }

  return 0 ;
}

int CTacticalGraphicTwoArrowsWithBackOperation::done( )
{
  // Treat as aborting the operation
  m_request->deactivate( ) ;
  return 0 ;
}


/*******************************************************/

///////////////////////////////////////////////////////////////////
// Now the specific overrides to create the individual graphics
///////////////////////////////////////////////////////////////////

TSLEntity * CFriendlyMainAttackAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  // Main Attack: APP-6A(C) May 2011, Table 7-7 page 7-47
  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GFC*MOLAM-*****", *coords, NULL, 0, true);
  return arrow ;
}

TSLEntity * CFriendlySupportingAttackAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GFC*MOLAS-*****", *coords, NULL, 0, true);
  return arrow ;
}
TSLEntity * CFriendlyPlannedAttackAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GFC*MOLAO-*****", *coords, NULL, 0, true);
  return arrow ;
}
TSLEntity * CFeintAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*MDA---*****", *coords, NULL, 0, true);
  return arrow ;
}
TSLEntity * CCounterAttackAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*T*GK----*****", *coords, NULL, 0, true);
  return arrow ;
}

TSLEntity * CEnemyConfirmedAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GHC*MOLAE-*****", *coords, NULL, 0, true);
  return arrow ;
}

TSLEntity * CEnemySuspectedAxisOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GHC*MOLAT-*****", *coords, NULL, 0, true);
  return arrow ;
}

TSLEntity * CAttackByFirePositionOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet* area = TacticalGraphics::APP6CTacticalGraphicsFactory::createArea("G*C*MOOP--*****", *coords, NULL, 0);
  return area;

}

TSLEntity * CSupportByFirePositionOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet* area = TacticalGraphics::APP6CTacticalGraphicsFactory::createArea("G*C*MOOS--*****", *coords, NULL, 0);
  return area;
}

TSLEntity * CSurpriseAttackOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet* area = TacticalGraphics::APP6CTacticalGraphicsFactory::createArea("G*C*MSGA--*****", *coords, NULL, 0);
  return area;
}

TSLEntity * CConvoyMovingOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection
  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*SLCM--******", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CConvoyHaltedOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*SLCH--******", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CContainOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet* area = TacticalGraphics::APP6CTacticalGraphicsFactory::createArea("G*T*GJ----*****", *coords, NULL, 0);
  return area;
}

TSLEntity * CFollowAndAssumeOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection
  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*T*GA----*****", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CFollowAndSupportOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection
  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*T*GAS---*****", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CFriendlyMainAttackDirectionOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection
  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GFC*MOLDM-*****", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CFriendlySupportingAttackDirectionOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GFC*MOLDS-*****", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CFriendlyPlannedAttackDirectionOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("GFC*MOLDO-*****", *coords, NULL, 0, false);
  return arrow ;
}

TSLEntity * CFeintDirectionOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  TSLEntitySet *arrow = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*MDF---*****", *coords, NULL, 0, false);
  return arrow;
}

TSLEntity * CBlockOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  // Obstacle Control Measure: block
  TSLEntitySet *graphic = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*BOAEB-*****", *coords, NULL, 0, false);

  return graphic;
}

TSLEntity * CDisruptOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  // disrupt
  TSLEntitySet *graphic = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*BOAED-*****", *coords, NULL, 0, false);

  return graphic;
}


TSLEntity * CBypassEasyOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  //     * -  Obstacle Bypass Easy p7-171
  //   *  - SIDC: G*C*BYOE--*****
  TSLEntitySet *graphic = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*BYOE--*****", *coords, NULL, 0, false);

  return graphic;
}


TSLEntity * CBypassDifficultOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  //     * -  Obstacle Bypass Difficult p7-172
  //   *  - SIDC: G*C*BYOD--*****
  TSLEntitySet *graphic = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*BYOD--*****", *coords, NULL, 0, false);

  return graphic;
}


TSLEntity * CBypassImpossibleOperation::createSpecificGraphic( const TSLCoordSet * coords )
{
  // Use the stored coordinates to create the specific graphic.
  // Set the rendering attributes of the object as required
  // Then return it for selection

  //      * -  Obstacle Bypass Impossible p7-173
  //    *  - SIDC: G*C*BYOI--*****
  TSLEntitySet *graphic = TacticalGraphics::APP6CTacticalGraphicsFactory::createLine("G*C*BYOI--*****", *coords, NULL, 0, false);
  return graphic ;
}
