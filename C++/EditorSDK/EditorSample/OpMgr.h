
#ifndef _INC_COPMGR_38F2D8330211_INCLUDED
#define _INC_COPMGR_38F2D8330211_INCLUDED

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

class CAttributePanel;

//This class handles activation of operations in the application 
//It's main purposes are to provide a layer of 
//separation betweent the TSLEditor functions and the actual 
//MFC MMI and to encapsulate the operation invocations and data structures.
//
//The methods in this class are all static.  They are called 
//by the MMI Menu and Toolbar callbacks and other classes 
//that need interaction with the TSLEditor.

class COpMgr 
{
  public:
	  //Sets the changed flag of the editor. This may be used
    //after a save to indicate that the current layer does
    //not need saving again.
    static void setChanged( bool sts ) ;
  
    //Returns the changed flag of the editor.  This may be 
    //used to request a save if the current edit layer has 
    //changed.
    static bool queryChanged();
  
    //Informs the editor that the current data layer has 
    //changed.
    //
    //This informs the editor that the current editable data 
    //layer has changed in the current drawing Surface.  This 
    //will cause the editor to reset back to the select 
    //operation, clear the select list and to request the 
    //current entity set from the data layer.
    //
    //This method should be called after client application 
    //has changed the data in the data layer, such as after a 
    //'loadData' call.
    static bool dataChanged();
  
  
    //This methods tells the COpMgr about the 
    //Attribute Panel.  This allows the Attribute Panel to be 
    //triggered to initialise itself once the editor has been 
    //created.
    static void setAttributePanel(CAttributePanel *  attributePanel);
  
    //This method returns whether the user is allowed to edit 
    //the current active pre-map layer
    //
    //It is called during the update handler for the 
    //Operation Mode user interface.
    static bool queryAllowEdit();
  
    //This method is used to indicate whether the user is 
    //allowed to edit the model, and hence whether events 
    //should be passed onto the editor.
    static void allowEdit(bool sts);
  
    //This method activates the Delete operation.
    static void deleteOp(long id);
  
    //This method stores the current prompt to be displayed.
    //It is called by the Editor Request Handler.
    static void setPrompt(const char * prompt);
  
    //This method should be to store the cursor readout 
    //string returned from the editor via the 
    //CEditorRequest::onCursorMovement() method.
    static void setCursorReadout(const char * readout );
  
    //This method stores the number entities selected, and 
    //the current select depth.
    //
    //It is called by the Editor Request Handler.
    static void setSelected(int num, int depth);
  
    //This method returns the current editor prompt that 
    //should be displayed.
    //
    //It is called during the update handler for the status 
    //bar.
    static const char * queryPrompt();
  
    //Retrieve the cursor readout string returned from the 
    //editor via the 
    //CEditorRequest::onCursorMovement() method.
    static const char * queryCursorReadout();
  
    //This method returns the current number of selected 
    //entities and the select depth.
    //
    //It is called during the update handler for the status 
    //bar.
    static int querySelected(int & depth);
  
    //This method activates the Polygon operation.  
    //The m_followMode attribute is passed to the activation.
    static void polygon(long id);
  
    //This method activates the Rectangle operation.  
    static void rectangle(long id);
  
    //This method activates the Polyline operation.  
    //The m_followMode attribute is passed to the activation.
    static void polyline(long id);

    //This method activates the Symbol (Marker) operation.
    static void symbol(long id);
  
    //This method activates the Circle operation.
    static void circle(long id);
  
    //This method activates the Text operation.
    static void text(long id);
  
    //This method activates the 'attributeset' operation to 
    //set the fill colour.  
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method.
    static void fillColour(long val);
  
    //This method activates the 'select' operation to select 
    //objects.
    static void select(long id);
  
    //This method activates the 'attributeset' operation to 
    //set the edge colour.  
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method.
    static void edgeColour(long val);
  
    //This method activates the 'attributeset' operation to 
    //set the edge style.
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method. 
    static void edgeStyle(long style);
  
    //This method activates the 'attributeset' operation to 
    //set the fill style.
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method. 
    static void fillStyle(long style);
  
    //This method activates the 'attributeset' operation to 
    //set the edge thickness.
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method. 
    static void edgeThickness(long thickness);
  
    //This method activates the 'attributeset' operation to 
    //set the text colour.  
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method.
    static void textColour(long val);

    static void textHeight(long val);

    static void textAlignment( TSLHorizontalAlignment halign, TSLVerticalAlignment valign );
    static void textBackgroundColour(int val);
    static void textBackgroundEdgeColour(int val);
    static void textBackgroundStyle(int val);
    static void textBackgroundMode(TSLTextBackgroundMode val);
  
    //This method activates the 'attributeset' operation to 
    //set the text font.
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method. 
    static void textFont(long val);
  
    //This method activates the 'attributeset' operation to 
    //set the symbol colour.  
    //
    //An instance of TSLAttributeData is passed to the 
    //activation method.
    static void symbolColour(long val);
  
    //This method sets the current constraint mode to 'equal'.
    static void equal(long id);
  
    //This method sets the current constraint mode to 'angular'.
    static void angular(long id, double value ) ;
  
    //This method sets the current constraint mode to 'distance'.
    static void distance(long id, double value ) ;
  
    //This method sets the current constraint mode to 
    //'unequal'.
    static void unequal(long id);
  
    //This method sets the current constraint mode to 
    //'horizontal'.
    static void horizontal(long id);
  
    //This method sets the current constraint mode to 
    //'vertical'.
    static void vertical(long id);
  
    //This method activates the Close operation.
  
    //This method activates the Clear operation.
    static void clear(long id);
  
    //This method activates the Copy operation.
    static void copy(long id);
  
    //This method activates the Undo method of TSLEditor.
    static void undo();
  
    //This method activates the Backup method of TSLEditor.
    static void backup();
  
    //This method activates the Select All operation.
    static void selectAll(long id);
  
    //This method activates the Select By Extent operation.
    static void selectByExtent(long id);
  
    //This method activates the Select By Polygon operation.
    static void selectByPolygon(long id);
  
    //This method activates the Move operation.
    static void move(long id);
  
    //This method activates the Set Text operation.
    static void setText( long id);
    
    //This method activates the Scale operation.
    static void scale(long id);
  
    //This method activates the Rotate operation.
    static void rotate(long id);
  
    //This method activates the Rotate90 operation.
    static void rotate90(long id);
  
    //This method activates the flip vertcial operation.
    static void flipVertical(long id);

    //This method activates the flip horizontal operation.
    static void flipHorizontal(long id);

    //This method activates the Front operation.
    static void front(long id);
  
    //This method activates the Back operation.
    static void back(long id);
  
    //This method activates the Group operation.
    static void group(long id);
  
    //This method activates the Ungroup operation.
    static void ungroup(long id);
  
    //This method activates the Add Point operation.
    static void addPoint(long id);
  
    //This method activates the Delete Point operation.
    static void deletePoint(long id);
  
    //This method activates the Move Point operation.
    static void movePoint(long id);
  
    //This method activates the Edit Feature ID operation
    static void editFeatureID( long id );
  
    //This method passes the specified attribute set to the 
    //editor.  It is used to initialise all attributes.
    static void attributeSet(TSLAttributeData * data );
   
    //This method sets the current cursor readout style
    static void cursorReadoutType( TSLCursorMovementType type, const char * format ) ;
    
    //This method returns the ID of the current operation.
    //
    //It is called during the update handler for the 
    //operation user interface.
    static long queryActiveOperation();
  
    //This method returns the current constraint mode.
    //
    //It is called during the update handler for the 
    //Constraint Mode user interface.
    static long queryConstraint();
  
    //This methods sets the internal pointer to an instance 
    //of TSLEditor.  This handle is used to invoke operations.
    static void setEditor(TSLEditor * editor);
    
    //Reset to the default select operation.
    //
    //This method resets the editor to the default select 
    //operation and clears the current select list.
    //
    //Returns true on success, otherwise false.
    static void reset( ) ;
  
  private:
    //This attribute holds a pointer to the Attribute Panel.  
    //This is used during initialisation.
    static CAttributePanel *m_attributePanel;
  
    //This attribute is used decide whether events are 
    //propagated to the editor.
    //
    //It shoud be set based upon the current Editing State - 
    //see associated State Diagram
    static bool m_allowEdit;
  
    //This is the ID of the currently active constraint.  It 
    //is usually an MFC control ID that is passed when an 
    //constraint is activated.  It may be queried in an MFC 
    //controls update handler to determine whether the 
    //controls constraint is the currently active one.
    static long m_activeConstraint;
  
    //This is the value for constraints which have one, such as 
    //angular and distance
    static double m_activeConstraintValue ;

    //This attribute holds the current default attributes 
    //that are applied to general primitive objects.
    static TSLAttributeData m_attributes;
  
    //This attribute is the current prompt that should be 
    //displayed.  It will change according to the current 
    //operation and the current substate within the operation.
    static std::string m_prompt;
  
    //This attribute stores the cursor readout string 
    //returned from the editor via the 
    //CEditorRequest::onCursorMovement() method.
    static std::string m_cursorReadout;
  
    static TSLCursorMovementType m_cursorReadoutType ;
  
    static std::string m_cursorReadoutFormat ;
  
    //This attribute is the number of entities that are 
    //currently selected.
    static int m_numSelected;
  
    //This attribute is the current select depth.  This 
    //indicates the maximum depth that the entity selection 
    //will descend into the group hierarchy.
    static int m_selectDepth;
  
    //This is the ID of the current operation.  It is usually 
    //an MFC control ID that is passed when an operation is 
    //activated.  It may be queried in an MFC controls update 
    //handler to determine whether the controls operation is 
    //the currently active one.
    static long m_activeOperation;
  
    //This attribute contains a handle to the TSLEditor 
    //instance.  This is used to activate operations.
    static TSLEditor* m_editor;   
};

#endif /* _INC_COPMGR_38F2D8330211_INCLUDED */

