#ifndef _tslviewmodegrab_h_
#define _tslviewmodegrab_h_ 1

#include "tslviewmode.h"

class TSLViewModeGrab : public TSLViewMode
{
public:

	TSLViewModeGrab(TSLViewModeInterface* iViewMode, unsigned long modeID);

	//Initialises the grab operation.
	virtual bool OnLButtonDown(long x, long y, bool shift, bool control);

	//Completes the grab operation.
	virtual bool OnLButtonUp(long x, long y, bool shift, bool control);

	//If the left mouse button is pressed, calculates the 
	//offset between the new and last position and pans the 
	//map accordingly.
	virtual bool OnMouseMove(long x, long y, bool shift, bool control);
    
private:
	long m_x;

	long m_y;

	bool m_pressed;

};

#endif 

