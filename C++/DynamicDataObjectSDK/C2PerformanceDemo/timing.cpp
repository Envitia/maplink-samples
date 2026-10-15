/****************************************************************************
                Copyright (c) 2005-2017 by Envitia Group PLC.
=============================================================================
MODULE          : timing.cpp
=============================================================================
DESCRIPTION     : Code for obtaining time data.
****************************************************************************/

#include "StdAfx.h"
#include "timing.h"

long long TSLTimer::m_frequency = 0;
bool TSLTimer::m_setup = false;


long long TSLTimer::time()
{

  if (!TSLTimer::m_setup)
  {
    TSLTimer::m_setup = true;
    LARGE_INTEGER lpFrequency;
    if (!QueryPerformanceFrequency(&lpFrequency))
    {
      TSLTimer::m_frequency = 0;
    }
    else
    {
      TSLTimer::m_frequency = lpFrequency.QuadPart;
    }
  }
  if (TSLTimer::m_frequency != 0)
  {
    LARGE_INTEGER performanceCount;
    QueryPerformanceCounter(&performanceCount);
    return (long long)((performanceCount.QuadPart/(double)TSLTimer::m_frequency) * 1000);
  }
  else
  {
    return GetTickCount(); // fall back up not very accurate and a lot slower
  }

}

