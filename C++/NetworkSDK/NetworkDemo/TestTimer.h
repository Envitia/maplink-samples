#pragma once

class TestTimer
{
public:

  TestTimer(void)
    : m_freq  ( 0 ),
      m_begin ( 0 ),
      m_end   ( 0 )
  {
    QueryPerformanceFrequency( (LARGE_INTEGER*)&m_freq );
  }

  ~TestTimer(void)
  {
  }

  void begin()
  {
    QueryPerformanceCounter( (LARGE_INTEGER*)&m_begin );
  }

  void end()
  {
    QueryPerformanceCounter( (LARGE_INTEGER*)&m_end );
  }

  double interval() const
  {
    return (double)(m_end-m_begin)/m_freq;
  }

private:
  __int64 m_freq;

  __int64 m_begin;
  __int64 m_end;
};


