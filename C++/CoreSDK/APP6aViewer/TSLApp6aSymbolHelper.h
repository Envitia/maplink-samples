// TSLSymbolHelper.h: interface for the TSLSymbolHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLSYMBOLHELPER_H__9DE6F67D_7C2B_41BE_BBA8_246C8DA0CCB7__INCLUDED_)
#define AFX_TSLSYMBOLHELPER_H__9DE6F67D_7C2B_41BE_BBA8_246C8DA0CCB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {
    PENDING = 0,
    UNKNOWN = 1,
    ASSUMED_FRIEND = 2,
    FRIEND = 3,
    NEUTRAL = 4,
    SUSPECT = 5,
    HOSTILE = 6,
    JOKER = 7,
    FAKER = 8
};

struct SymbolProperties
{
  long symbol_index;
  char *id;
  char *name;
  long background_start;
  int  depth;

} ;

#define MAX_SYMBOLS 2000

class TSLApp6aSymbolHelper  
{

public:
	int GetDepth(int index);
	int Size();
	bool GetName(int index, char **name);
	bool GetId(int index, char **name);
	bool GetName(TSLEntitySet *es, char **name);
	const TSLEntitySet* SetText(TSLEntitySet *es, char* text);
	bool GetHostility(TSLEntitySet *es, TSLEntityID *hostility);
	const TSLEntitySet* SetHostility( TSLEntitySet *es , int hostiltiy);
	bool IsApp6a(TSLEntitySet *es );
	const TSLEntitySet* CreateSymbol(long app6a_index, long symbolX, long symbolY, long height);
	bool Load( char * app6configFilename );
	TSLApp6aSymbolHelper();
	virtual ~TSLApp6aSymbolHelper();

private:
	int depth(char * inputline);
        SymbolProperties APP6ASymbols[MAX_SYMBOLS];
        int m_maxSymbols ;        
};

#endif // !defined(AFX_TSLSYMBOLHELPER_H__9DE6F67D_7C2B_41BE_BBA8_246C8DA0CCB7__INCLUDED_)
