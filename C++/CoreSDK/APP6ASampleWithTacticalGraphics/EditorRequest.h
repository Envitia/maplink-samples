
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CEDITORREQUEST_38EE077401BE_INCLUDED
#define _INC_CEDITORREQUEST_38EE077401BE_INCLUDED

class COpMgr;

//This component handles the exchange of information between 
//the editor and the application.
//
//The TSLEditor must be paired with an object derived from 
//TSLEditorRequest.  The methods of the request object are 
//invoked by the TSLEditor component when information needs 
//to be requested from or sent to the user.
class CEditorRequest 
  : public TSLEditorRequest
{
  public:

	//Default constructor for the application specific 
	//version of the TSLEditorRequest class.
    CEditorRequest();

	//The destructor for the application specific version of 
	//the TSLEditorRequest class.
    ~CEditorRequest();

	//Display an error message.
	//
	//This method is called to request that the users 
	//application display an error message.
	//
	//Argument list description:
	//
	//message: A pointer to a string containing the error 
	//message.
	//
	//Returns true if successful, otherwise false.
    bool displayError(const char * message);

	//Display a prompt to the user.
	//
	//This method is called to request that the users 
	//application display a prompt message.
	//
	//Argument list description:
	//
	//message: A pointer to the string containing the text of 
	//user prompt.
	//
	//Returns true if successful, otherwise false.
    bool displayPrompt(const char * message);

	//Called to indicate that the current operation has been 
	//deactivated.
	//
	//This method is called by the editor to inform the users 
	//application that the current operation has been 
	//deactivated and that the default select operation is 
	//now active.
	//
	//Returns true if successful, otherwise false.
    bool onOperationDeactivated();

	//Called to indicate a change in the selection list.
	//
	//This method is called by the editor to inform the users 
	//application of the current select list status.
	//
	//Argument list description: 
	//
	//numEntities: The number of selected entities.
	//
	//depth: The select depth.
	//
	//Returns true if successful, otherwise false.
    bool onSelectionChanged(int numEntities, int depth);

	//Request user input from a dialog box.
	//
	//This method requests that the user select one of the 
	//specified choices. An empty string indicates that the 
	//button should not be shown.
	//
	//Argument list description:
	//
	//prompt: The prompt requesting a user selection.
	//
	//label1: The text label for button 1.
	//
	//label2: The text label for button 2.
	//
	//label3: The text label for button 3.
	//
	//label4: The text label for button 4.
	//
	//Returns true on success, otherwise false.
    bool requestDialog(const char * prompt, const char * label1, const char * label2, const char * label3, const char * label4);

	//Request text input from the user.
	//
	//This method is called to request that the client 
	//application ask the user to enter some text.
	//
	//The client application should called the textEntered 
	//method when the text has been entered by the user.
	//
	//prompt: A pointer to the string prompting the user for 
	//input.
	//
	//initialValue: A pointer to the initial value of the 
	//text field.
	//
	//Returns true on success, otherwise false.
    bool requestText(const char * prompt, const char * initialValue);

	//Called when cursor moves.
	//
	//This method is called to inform the user's application 
	//that the cursor has moved. The values of the arguments 
	//passed are determined by the settings specified in a 
	//call to TSLEditor::cursorMovementConfig
	//
	//Argument list description:
	//
	//readout: A pointer to a string containing cursor 
	//position text formatted as specified by 
	//cursorMovementConfig
	//
	//x: Cursor x position in coordinates as specified by 
	//cursorMovementConfig
	//
	//y: Cursor y position in coordinates as specified by 
	//cursorMovementConfig
	//
	//Returns true if successful, otherwise false.
    bool onCursorMovement( const char * readout, double x, double y ) const ;

	//Returns the prompt that the current operation is attempting to display
    const char * queryPrompt( ) ;

  private:

    std::string m_prompt ;
};

#endif

