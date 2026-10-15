
#include "stdafx.h"
#include "performancetoolbar.h"

void PerformanceToolBar::DrawFrameRate (double currentFPS, double averageFPS, bool clear)
{
  HWND hwnd = this->GetSafeHwnd();
  HDC hdc = ::GetDC(hwnd);

  //current fps rect
  RECT currentRect, averageRect;
  GetToolBarCtrl().GetItemRect(2, &currentRect);
  currentRect.bottom -= 3;
  currentRect.top += 3;
  currentRect.left += 3;
  currentRect.right -= 4;
  GetToolBarCtrl().GetItemRect(3, &averageRect);
  averageRect.bottom -= 3;
  averageRect.top += 3;
  averageRect.left += 3;
  averageRect.right -= 4;

  ::FillRect(hdc, &currentRect, (HBRUSH)GetStockObject( WHITE_BRUSH ));
  ::FillRect(hdc, &averageRect, (HBRUSH)GetStockObject( WHITE_BRUSH ));

  if ( !clear )
  {
    CString rate;
    rate.Format( _T("Cur: %.2f fps"), currentFPS);
    ::DrawText(hdc, rate.GetBuffer(), rate.GetLength(), &currentRect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_NOCLIP ); 
    rate.Format( _T("Avg: %.2f fps"), averageFPS);
    ::DrawText(hdc, rate.GetBuffer(), rate.GetLength(), &averageRect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_NOCLIP );  
  }

  ::ReleaseDC(hwnd, hdc);
}
