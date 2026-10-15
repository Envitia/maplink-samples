// TSLApp6aSymbolHelper.cpp: implementation of the TSLSymbolHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSLApp6aSymbolHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct BackgroundType 
{
  char * name  ;
  int    index ;
} ;

struct BackgroundType backgroundType [] =
{ 
    "AIR"       , 110000,
    "LAND_UNITS", 110010,
    "LAND_EQUIP", 110020,
    "SEASURFACE", 110030,
    "SUBSURFACE", 110040,
    "SOF"       , 110050,
		"LAND_EQUIP_INST", 110060,
    "NONE"      , 0     ,
     NULL        ,0
} ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TSLApp6aSymbolHelper::TSLApp6aSymbolHelper()
{
  m_maxSymbols = 0;
}

TSLApp6aSymbolHelper::~TSLApp6aSymbolHelper()
{

}

bool TSLApp6aSymbolHelper::Load(char *configFilename)
{
  m_maxSymbols = 0 ;

  FILE * infile = fopen (configFilename ,"r") ;

  char inputline [1001] ;

  char* inputline_copy;

  char symbol_backgroundType[100] = {"NONE"} ;
  int count = 0;

  if (!infile)
  {
      return 0 ;
  }

  fgets(inputline, 1000, infile) ;
  int num_symbols = atoi(inputline) ;

 
  while (fgets(inputline, 1000, infile))
  { 
    
    inputline[strlen(inputline)-1] = '\0' ;
    
    int tab_count = 0;
    
    inputline_copy =  _strdup( inputline );
  
  
    char seps[]   = "\t";
    char* desc_name;
    char* desc_id;
    
    desc_id   = strtok( inputline_copy, seps );
    desc_name = strtok( NULL, seps );

    if (desc_id) 
      APP6ASymbols[count].id = strdup (desc_id) ;
    
    APP6ASymbols[count].name = desc_name;   
    
    char finalnum[50];
    
    char *numpart = strtok( desc_id, "." );

    APP6ASymbols[count].depth = depth(inputline) ;
  
    strcpy( finalnum, ""   );
    
    while (numpart != NULL )
    {
       strcat( finalnum, numpart);
       numpart = strtok( NULL, "." );  
    }
    
    APP6ASymbols[count].symbol_index = count+100000;

    for (size_t i = 0; i < strlen(inputline);i++)
    {
      
      if (inputline[i] == '\t')
      {
        if (tab_count == 1)
        {
          inputline[i] = '\0' ;
          strcpy(symbol_backgroundType, &inputline[i+1]) ;
          
        }
        else 
        { 
          inputline[i] = '_' ;
          tab_count++;
        }
      } 
      else if (inputline[i] == '.') 
      {
        inputline[i] = '_';
      }
    }

    
    for (int k=0; backgroundType[k].index != 0; k++) 
    {
      if (!strcmp(backgroundType[k].name, symbol_backgroundType))
      {
         APP6ASymbols[count].background_start = backgroundType[k].index ;
      }
    }
  count++;
  }
  m_maxSymbols = count;
 
  fclose(infile);
  return true;
}

const TSLEntitySet* TSLApp6aSymbolHelper::CreateSymbol(long app6a_index, long symbolX, long symbolY, long height)
{

  // find symbol
  long symbol_index = 0;

  if (app6a_index < 0 || app6a_index >= m_maxSymbols) 
    return NULL ;
  
  symbol_index = APP6ASymbols[app6a_index].symbol_index; 
  TSLEntitySet* es;

  es = TSLEntitySet::create();
  TSLSymbol * symbol = es->createSymbol(0,symbolX,symbolY,height,0.0) ;
  symbol->setRendering(TSLRenderingAttributeSymbolColour          , 100     ) ; 
  symbol->setRendering(TSLRenderingAttributeSymbolStyle           , symbol_index   ) ; 
  symbol->setRendering(TSLRenderingAttributeSymbolMinPixelSize    , 10      ) ;
  symbol->setRendering(TSLRenderingAttributeSymbolMaxPixelSize    , 2000    ) ; 
  symbol->entityID(app6a_index);
  return es;
}

bool TSLApp6aSymbolHelper::IsApp6a(TSLEntitySet* es)
{
#ifdef XXXX
    TSLSymbol *shapeSymbol = (*es)[0]->isSymbol();
  long ss = shapeSymbol->entityID();
  long symbol_index = 0;

  for (int i=0; i< m_maxSymbols; i++)
  {
    if (APP6ASymbols[i].app6a_index == ss)
    {
      return true;
    }
  }
  return false;
#endif
  return 1 ;
}

const TSLEntitySet* TSLApp6aSymbolHelper::SetHostility(TSLEntitySet *es, int hostility)
{
  TSLEntitySet* hostility_es;
  hostility_es = TSLEntitySet::create();
  TSLSymbol *shapeSymbol = TSLSymbol::isSymbol((*es)[0]);
  TSLEntityID ss = shapeSymbol->entityID();
  const TSLCoord shapePosition = shapeSymbol->position();
  long shapeHeight = shapeSymbol->height();
  long symbol_background = 0;
  long symbol_index = 0;


  if (ss >= 0 && ss <  m_maxSymbols)
  {
      symbol_background = APP6ASymbols[ss].background_start;
      symbol_index = APP6ASymbols[ss].symbol_index;
  }

  if( symbol_background != 0)
  {
    TSLSymbol *background = hostility_es->createSymbol(0,shapePosition.x(), shapePosition.y(), shapeHeight, 0.0);  
    background->setRendering(TSLRenderingAttributeSymbolColour          , 100 ) ;
    background->setRendering(TSLRenderingAttributeSymbolStyle           , symbol_background + hostility) ; 
    background->setRendering(TSLRenderingAttributeSymbolMinPixelSize    , 10      ) ;
    background->setRendering(TSLRenderingAttributeSymbolMaxPixelSize    , 2000    ) ;   
    background->entityID(hostility);
  }

  TSLSymbol *symbol = hostility_es->createSymbol(0,shapePosition.x(),shapePosition.y(),shapeHeight,0.0) ;
  symbol->setRendering(TSLRenderingAttributeSymbolColour          , 100  ) ; 
  symbol->setRendering(TSLRenderingAttributeSymbolStyle           , symbol_index   ) ; 
  symbol->setRendering(TSLRenderingAttributeSymbolMinPixelSize    , 10      ) ;
  symbol->setRendering(TSLRenderingAttributeSymbolMaxPixelSize    , 2000    ) ; 
  symbol->entityID(ss);

  return hostility_es;
}

bool TSLApp6aSymbolHelper::GetHostility(TSLEntitySet *es, TSLEntityID *hostility)
{
  TSLSymbol *hostilitySymbol = TSLSymbol::isSymbol((*es)[1]);
  if (hostilitySymbol)
  {
    *hostility = hostilitySymbol->entityID();
    return true;
  }
  return false;
}

const TSLEntitySet* TSLApp6aSymbolHelper::SetText(TSLEntitySet *es, char *text)
{
  return NULL;
}

bool TSLApp6aSymbolHelper::GetName(TSLEntitySet *es, char **name)
{
  TSLSymbol *shapeSymbol = TSLSymbol::isSymbol((*es)[0]);
  TSLEntityID ss = shapeSymbol->entityID();

  if (ss >= 0 && ss <  m_maxSymbols)
  {
    *name = APP6ASymbols[ss].name;
    return true;
  }
  else
    return false;
}

bool TSLApp6aSymbolHelper::GetName(int index, char **name)
{
  if ((index >=0) && (index < m_maxSymbols))
  {
    *name = APP6ASymbols[index].name;
    return true;
  }
  return false;
  
}

bool TSLApp6aSymbolHelper::GetId(int index, char **id)
{
  if ((index >=0) && (index < m_maxSymbols))
  {
    *id = APP6ASymbols[index].id;
    return true;
  }
  return false;
  
}

int TSLApp6aSymbolHelper::Size()
{
  return m_maxSymbols;
}

int TSLApp6aSymbolHelper::depth(char *inputline)
{
  char * name_posn = strstr (inputline,"\t") ;
  name_posn++;
  int depth;
  
  depth = 0;
  for (size_t i = 0; i <= strlen(inputline); i++)
  {
    if (inputline[i] == '.')
      depth++ ;
  }
  depth--; // Ignore the dot for X (e.g 1.x = 0)
  return depth ; 
}

int TSLApp6aSymbolHelper::GetDepth(int index)
{
  if ((index >=0) && (index < m_maxSymbols))
  {
    return APP6ASymbols[index].depth;
    
  }
  return 0;
}
