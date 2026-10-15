/****************************************************************************
                Copyright (c) 2005-2017 by Envitia Group PLC.
=============================================================================
MODULE          : timing.h
=============================================================================
DESCRIPTION     : Code for obtaining time data.
****************************************************************************/
#ifndef _TIMING_H_
#define _TIMING_H_


class TSLTimer
{
public:
  static long long time();

  static long long m_frequency;
  static bool m_setup;
};

#endif
