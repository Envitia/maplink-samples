#pragma once

#include "useroperations\tsluseroperation.h"
#include "useroperations\tsluseroperationrequest.h"

////////////////////////////////////////
// Complex arrow with a tail
////////////////////////////////////////
class CTacticalGraphicComplexArrowOperation :
  public TSLUserOperation
{
  enum MyState
  {
    SelectingTipOfArrowHead,
    SelectingBackOfArrowHead,
    SelectingTailOfArrow,
    SelectingSizeOfArrowHead
  } ;

public:
  CTacticalGraphicComplexArrowOperation(void);
  virtual ~CTacticalGraphicComplexArrowOperation(void);

    virtual void requestHandler (TSLUserOperationRequest* handler)
    {
      m_request = handler ;
    }
    virtual int activate (void* data);

    virtual int locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

    virtual int done( ) ;

    virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) = 0;

    // Set the echo and prompt according to the current state
    // Separated out so it could be used in case we implemented 'backup'
    void setEchoAndPrompt( ) ; 

  protected:

    TSLUserOperationRequest * m_request ;
    MyState m_state ;

    TSLCoord         m_tipOfArrowHead ;
    TSLCoord         m_backOfArrowHead ;
    vector<TSLCoord> m_tailOfArrow ;
    TSLCoord         m_sizeOfArrowHead ;

    vector<TSLCoord> m_primitiveEchoLine ;
    TSLEchoCoord     m_dynamicEchoLine[ 3 ] ;
};

////////////////////////////////////////
// Simple arrow with a tail
////////////////////////////////////////
class CTacticalGraphicSimpleArrowWithTailOperation :
  public TSLUserOperation
{
  enum MyState
  {
    SelectingTip,
    SelectingSecondPoint,
    SelectingTailPoints
  } ;

public:
  CTacticalGraphicSimpleArrowWithTailOperation(void);
  virtual ~CTacticalGraphicSimpleArrowWithTailOperation(void);

    virtual void requestHandler (TSLUserOperationRequest* handler)
    {
      m_request = handler ;
    }
    virtual int activate (void* data);

    virtual int locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

    virtual int done( ) ;

    virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) = 0;

    // Set the echo and prompt according to the current state
    // Separated out so it could be used in case we implemented 'backup'
    void setEchoAndPrompt( ) ; 

  protected:

    TSLUserOperationRequest * m_request ;
    MyState m_state ;

    TSLCoord         m_tipOfArrow ;
    vector<TSLCoord> m_tailOfArrow ;

    vector<TSLCoord> m_primitiveEchoLine ;
    TSLEchoCoord     m_dynamicEchoLine[ 3 ] ;
};

////////////////////////////////////////
// Simple two point
////////////////////////////////////////
class CTacticalGraphicTwoPointOperation :
  public TSLUserOperation
{
  enum MyState
  {
    SelectingFirstPoint,
    SelectingSecondPoint
  } ;

public:
  CTacticalGraphicTwoPointOperation(const char * firstPrompt, const char * secondPrompt );
  virtual ~CTacticalGraphicTwoPointOperation(void);

    virtual void requestHandler (TSLUserOperationRequest* handler)
    {
      m_request = handler ;
    }
    virtual int activate (void* data);

    virtual int locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

    virtual int done( ) ;

    virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) = 0;

    // Set the echo and prompt according to the current state
    // Separated out so it could be used in case we implemented 'backup'
    void setEchoAndPrompt( ) ; 

  protected:

    TSLUserOperationRequest * m_request ;
    MyState m_state ;

    TSLCoord         m_firstPoint ;
    TSLCoord         m_secondPoint ;
    TSLEchoCoord     m_dynamicEchoLine[ 2 ] ;

  private:
    const char * m_firstPrompt ;
    const char * m_secondPrompt ;
};

////////////////////////////////////////
// Simple three point with point echo
////////////////////////////////////////
class CTacticalGraphicThreePointOperation :
  public TSLUserOperation
{
  enum MyState
  {
    SelectingFirstPoint,
    SelectingSecondPoint,
    SelectingThirdPoint
  } ;

public:
  CTacticalGraphicThreePointOperation(const char * firstPrompt, const char * secondPrompt, const char * thirdPrompt );
  virtual ~CTacticalGraphicThreePointOperation(void);

    virtual void requestHandler (TSLUserOperationRequest* handler)
    {
      m_request = handler ;
    }
    virtual int activate (void* data);

    virtual int locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

    virtual int done( ) ;

    virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) = 0;

    // Set the echo and prompt according to the current state
    // Separated out so it could be used in case we implemented 'backup'
    void setEchoAndPrompt( ) ; 

  protected:

    TSLUserOperationRequest * m_request ;
    MyState m_state ;

    TSLCoord         m_points[3] ;

  private:
    const char * m_firstPrompt ;
    const char * m_secondPrompt ;
    const char * m_thirdPrompt ;
};


////////////////////////////////////////
// 3-point arrow with a tip and back section
////////////////////////////////////////
class CTacticalGraphicArrowWithBackOperation :
  public TSLUserOperation
{
  enum MyState
  {
    SelectingTipOfArrow,
    SelectingLeftOfBack,
    SelectingRightOfBack
  } ;

public:
  CTacticalGraphicArrowWithBackOperation(void);
  virtual ~CTacticalGraphicArrowWithBackOperation(void);

    virtual void requestHandler (TSLUserOperationRequest* handler)
    {
      m_request = handler ;
    }
    virtual int activate (void* data);

    virtual int locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

    virtual int done( ) ;

    virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) = 0;

    // Set the echo and prompt according to the current state
    // Separated out so it could be used in case we implemented 'backup'
    void setEchoAndPrompt( ) ; 

  protected:

    TSLUserOperationRequest * m_request ;
    MyState m_state ;

    TSLCoord         m_tipOfArrow ;
    TSLCoord         m_backLeft ;
    TSLCoord         m_backRight ;

    TSLEchoCoord     m_dynamicEchoLine[ 3 ] ;
};

////////////////////////////////////////
// 4-point Two arrows with tips and back section
////////////////////////////////////////
class CTacticalGraphicTwoArrowsWithBackOperation :
  public TSLUserOperation
{
  enum MyState
  {
    SelectingTipOfArrow1,
    SelectingLeftOfBack,
    SelectingTipOfArrow2,
    SelectingRightOfBack
  } ;

public:
  CTacticalGraphicTwoArrowsWithBackOperation(void);
  virtual ~CTacticalGraphicTwoArrowsWithBackOperation(void);

    virtual void requestHandler (TSLUserOperationRequest* handler)
    {
      m_request = handler ;
    }
    virtual int activate (void* data);

    virtual int locator (TSLTMC x, TSLTMC y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

    virtual int done( ) ;

    virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) = 0;

    // Set the echo and prompt according to the current state
    // Separated out so it could be used in case we implemented 'backup'
    void setEchoAndPrompt( ) ; 

  protected:

    TSLUserOperationRequest * m_request ;
    MyState m_state ;

    TSLCoord         m_tipOfArrow1 ;
    TSLCoord         m_backLeft ;
    TSLCoord         m_tipOfArrow2 ;
    TSLCoord         m_backRight ;

    TSLEchoCoord     m_dynamicEchoLine[ 4 ] ;
};

/////////////////////////////////////////////////////////////////////////////////////
//  Now the specific operations
////////////////////////////////////////////////////////////////////////////////////
class CFriendlyMainAttackAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CFriendlyMainAttackAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CFriendlyMainAttackAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

///////

class CFriendlySupportingAttackAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CFriendlySupportingAttackAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CFriendlySupportingAttackAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

///////

class CFriendlyPlannedAttackAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CFriendlyPlannedAttackAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CFriendlyPlannedAttackAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

///////

class CFeintAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CFeintAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CFeintAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

///////

class CCounterAttackAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CCounterAttackAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CCounterAttackAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CEnemyConfirmedAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CEnemyConfirmedAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CEnemyConfirmedAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CEnemySuspectedAxisOperation :
  public CTacticalGraphicComplexArrowOperation
{
public:
  CEnemySuspectedAxisOperation(void) : CTacticalGraphicComplexArrowOperation() { }
  virtual ~CEnemySuspectedAxisOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CAttackByFirePositionOperation :
  public CTacticalGraphicArrowWithBackOperation
{
public:
  CAttackByFirePositionOperation(void) : CTacticalGraphicArrowWithBackOperation() { }
  virtual ~CAttackByFirePositionOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CSurpriseAttackOperation :
  public CTacticalGraphicArrowWithBackOperation
{
public:
  CSurpriseAttackOperation(void) : CTacticalGraphicArrowWithBackOperation() { }
  virtual ~CSurpriseAttackOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CSupportByFirePositionOperation :
  public CTacticalGraphicTwoArrowsWithBackOperation
{
public:
  CSupportByFirePositionOperation(void) : CTacticalGraphicTwoArrowsWithBackOperation() { }
  virtual ~CSupportByFirePositionOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CConvoyMovingOperation :
  public CTacticalGraphicTwoPointOperation
{
public:
  CConvoyMovingOperation(void) : CTacticalGraphicTwoPointOperation( "Select front of convoy", "Second rear of convoy" ) { }
  virtual ~CConvoyMovingOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CConvoyHaltedOperation :
  public CTacticalGraphicTwoPointOperation
{
public:
  CConvoyHaltedOperation(void) : CTacticalGraphicTwoPointOperation( "Select front of convoy", "Second rear of convoy" )  { }
  virtual ~CConvoyHaltedOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CFollowAndAssumeOperation :
  public CTacticalGraphicSimpleArrowWithTailOperation
{
public:
  CFollowAndAssumeOperation(void) : CTacticalGraphicSimpleArrowWithTailOperation() { }
  virtual ~CFollowAndAssumeOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CFollowAndSupportOperation :
  public CTacticalGraphicSimpleArrowWithTailOperation
{
public:
  CFollowAndSupportOperation(void) : CTacticalGraphicSimpleArrowWithTailOperation() { }
  virtual ~CFollowAndSupportOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CFriendlyMainAttackDirectionOperation :
  public CTacticalGraphicSimpleArrowWithTailOperation
{
public:
  CFriendlyMainAttackDirectionOperation(void) : CTacticalGraphicSimpleArrowWithTailOperation() { }
  virtual ~CFriendlyMainAttackDirectionOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;
////////

class CFriendlySupportingAttackDirectionOperation :
  public CTacticalGraphicSimpleArrowWithTailOperation
{
public:
  CFriendlySupportingAttackDirectionOperation(void) : CTacticalGraphicSimpleArrowWithTailOperation() { }
  virtual ~CFriendlySupportingAttackDirectionOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////////////

class CFriendlyPlannedAttackDirectionOperation :
  public CTacticalGraphicSimpleArrowWithTailOperation
{
public:
  CFriendlyPlannedAttackDirectionOperation(void) : CTacticalGraphicSimpleArrowWithTailOperation() { }
  virtual ~CFriendlyPlannedAttackDirectionOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CFeintDirectionOperation :
  public CTacticalGraphicSimpleArrowWithTailOperation
{
public:
  CFeintDirectionOperation(void) : CTacticalGraphicSimpleArrowWithTailOperation() { }
  virtual ~CFeintDirectionOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CContainOperation :
  public CTacticalGraphicTwoPointOperation
{
public:
  CContainOperation(void) 
    : CTacticalGraphicTwoPointOperation( "Select centre of containment area", "Select start point on boundary of clockwise semicircle" ) { }
  virtual ~CContainOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CBlockOperation :
  public CTacticalGraphicThreePointOperation
{
public:
  CBlockOperation(void) 
    : CTacticalGraphicThreePointOperation( "Select first endpoint of obstacle", "Select second endpoint of obstacle", "Select limit of enemy advance" ) { }
  virtual ~CBlockOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CDisruptOperation :
  public CTacticalGraphicThreePointOperation
{
public:
  CDisruptOperation(void) 
    : CTacticalGraphicThreePointOperation( "Select first endpoint of disruption", "Select second endpoint of disruption", "Select tip of longest arrow" ) { }
  virtual ~CDisruptOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CBypassEasyOperation :
  public CTacticalGraphicThreePointOperation
{
public:
  CBypassEasyOperation(void) 
    : CTacticalGraphicThreePointOperation( "Select tip of first arrow", "Select tip of second arrow", "Select centre of rear line of graphic" ) { }
  virtual ~CBypassEasyOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CBypassDifficultOperation :
  public CTacticalGraphicThreePointOperation
{
public:
  CBypassDifficultOperation(void) 
    : CTacticalGraphicThreePointOperation( "Select tip of first arrow", "Select tip of second arrow", "Select centre of rear line of graphic" ) { }
  virtual ~CBypassDifficultOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;

////////

class CBypassImpossibleOperation :
  public CTacticalGraphicThreePointOperation
{
public:
  CBypassImpossibleOperation(void) 
    : CTacticalGraphicThreePointOperation( "Select tip of first arrow", "Select tip of second arrow", "Select centre of rear line of graphic" ) { }
  virtual ~CBypassImpossibleOperation(void) { }

  virtual TSLEntity * createSpecificGraphic( const TSLCoordSet * coords ) ;
} ;
