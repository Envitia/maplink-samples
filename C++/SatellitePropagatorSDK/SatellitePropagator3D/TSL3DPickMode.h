#pragma once


class TSL3DPickMode : public TSL3DInteractionMode
{
  public:
    TSL3DPickMode (long modeID,  const char* prompt = "Left button press to pick satellites");
    virtual ~TSL3DPickMode(void);
    
    virtual bool onLButtonDown (long x, long y, bool shift, bool control);
    virtual TSLCursorStyle queryCursor ();
    virtual const char* queryPrompt ();

private:
    std::string m_prompt;
};
