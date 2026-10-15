// MyDynamicRendererPlugin.cpp : Defines the entry point for the DLL application.
//

#include "pluginstdafx.h"
#include "MyDynamicRendererPlugin.h"
#include "mydynamicrenderer.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}

extern "C"
{
// This is an example of an exported function.
MYDYNAMICRENDERERPLUGIN_API void initialiseDynamicRenderer(void)
{
  MyDynamicRenderer * myClient = new MyDynamicRenderer ;
  TSLDynamicRendererCustom * customRenderer = new TSLDynamicRendererCustom( "myRenderer", myClient ) ;
  TSLDynamicRendererFactory::registerDynamicRenderer( customRenderer ) ;
}
}
