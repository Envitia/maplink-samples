// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYDYNAMICRENDERERPLUGIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYDYNAMICRENDERERPLUGIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MYDYNAMICRENDERERPLUGIN_EXPORTS
#define MYDYNAMICRENDERERPLUGIN_API __declspec(dllexport)
#else
#define MYDYNAMICRENDERERPLUGIN_API __declspec(dllimport)
#endif

extern "C"
{
  MYDYNAMICRENDERERPLUGIN_API void initialiseDynamicRenderer( );
}