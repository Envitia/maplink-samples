// MyCommandLine.h: interface for the MyCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCOMMANDLINE_H__EE8C1616_C056_11D4_A5BF_00104BB747B9__INCLUDED_)
#define AFX_MYCOMMANDLINE_H__EE8C1616_C056_11D4_A5BF_00104BB747B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//This class is a specialised command line parser.
//
//It is called by MFC for each argument on the command line.  
class MyCommandLine : public CCommandLineInfo
{
private:
  enum NextValue { Next_None, Next_Default, Next_MaplHome, Next_PCache };
public:

  MyCommandLine()
    : 
      m_isLocalMaplHome ( false ),
      m_next            ( Next_None ),
      m_maplHome        ( "" ),
      m_failed          ( false )
  {
  }
  bool isLocalMaplHome() const
  {
    return m_isLocalMaplHome;
  }
  string const& maplHome() const
  {
    return m_maplHome;
  }
  bool failed() const
  {
    return m_failed;
  }
  string const& persistentCacheIniFile() const
  {
    return m_persistentCacheIniFile;
  }
  public:
    //This method parses each command line argument
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

private:
  bool      m_isLocalMaplHome;
  NextValue m_next;
  string    m_maplHome;
  bool      m_failed;
  string   m_persistentCacheIniFile;
};


#endif // !defined(AFX_MYCOMMANDLINE_H__EE8C1616_C056_11D4_A5BF_00104BB747B9__INCLUDED_)
