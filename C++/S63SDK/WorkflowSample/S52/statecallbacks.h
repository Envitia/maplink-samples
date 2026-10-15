#ifndef _STATECALLBACKS_H_
#define _STATECALLBACKS_H_

#include <string>

class S52StateObject;

class WorkFlowCallback
{
public:
  WorkFlowCallback() {}
  virtual ~WorkFlowCallback() {}
  virtual void saveSettings(S52StateObject *) = 0;
};


class DrawingSurfaceCallback
{
public:
  DrawingSurfaceCallback() {}
  virtual ~DrawingSurfaceCallback() {}
  virtual void noDataLayerVisible(bool toggle) = 0;
  virtual void setupDynamicColours(std::string file) = 0;
};

#endif