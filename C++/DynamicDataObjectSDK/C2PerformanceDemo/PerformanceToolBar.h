
#ifndef PerformanceToolBar_h
#define PerformanceToolBar_h

class PerformanceToolBar : public CToolBar
{
public:
  void DrawFrameRate(double currentFPS, double averageFPS, bool clear);
};

#endif