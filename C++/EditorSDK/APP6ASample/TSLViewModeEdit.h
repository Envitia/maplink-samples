
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TSLVIEWMODEEDIT_38ED8C510391_INCLUDED
#define _INC_TSLVIEWMODEEDIT_38ED8C510391_INCLUDED

#include "TSLViewMode.h"
#include "EditorRequest.h"

//This is an interaction mode for handling editing.
//	
//The users application creates an instance of one these 
//classes and passes all mouse actions to it.  All events 
//received are passed to the embedded TSLEditor instance for 
//processing.
class TSLViewModeEdit 
: public TSLViewMode
{
  public:
    //This method is used to indicate whther the user is 
    //allowed to edit the model, and hence whether event 
    //should be passed onto the editor.
    void allowEdit(bool sts);
  
    //Constructor for the view mode edit class
    TSLViewModeEdit(TSLViewModeInterface* iViewMode, unsigned long modeID);
  
    //Destructor for view mode edit class
    virtual ~TSLViewModeEdit();
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnMouseMove(TSLViewModeButton button, long x, long y, bool shift, bool control);
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnRButtonUp(long x, long y, bool shift, bool control);
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnMButtonUp(long x, long y, bool shift, bool control);
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnLButtonUp(long x, long y, bool shift, bool control);
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnRButtonDown(long x, long y, bool shift, bool control);
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnMButtonDown(long x, long y, bool shift, bool control);
  
    //Locator handlers for view mode base class
    //
    //The default base class implementations all do nothing 
    //and return false.
    //
    //NOTE : On Windows NT, an mfc application automatically 
    //uses a window class that traps double click events and 
    //sends them to the On?ButtonDblClick handler.  It may be 
    //necessary to redirect these to the On?ButtonDown 
    //handler to avoid losing some interactions
    //
    //Argument list description:
    //
    //(x,y): Position of locator event, in device unit 
    //co-ordinates.
    //
    //shift: Flag to indicate whether the shift key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //control: Flag to indicate whether the control key is 
    //pressed. An interaction mode may use this to modify its 
    //behaviour.
    //
    //Returns true if the view needs to be redrawn, false 
    //otherwise. The default base class implementations all 
    //return false.
    virtual bool OnLButtonDown(long x, long y, bool shift, bool control);
  
    virtual const char * queryPrompt( ) ;
  
  private:
    //This attribute is used decide whether events are 
    //propagated to the editor.
    //
    //It shoud be set based upon the current Editing State - 
    //see associated State Diagram
    bool m_allowEdit;
   
    //Utility method to send a locator event to the embedded 
    //editor.
    void sendLocator(long x, long y, TSLButtonType button, TSLLocatorType locator, bool shift, bool control);
  
    //Handler class for feedback from the editor.
    CEditorRequest m_requestHandler;
  
    //This method contains a handle to the TSLEditor class.
    //
    //It is through this interface that the application
    //invokes LandLink editing functionality.
    TSLEditor* m_editor;
};

#endif

