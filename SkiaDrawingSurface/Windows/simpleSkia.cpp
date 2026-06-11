/****************************************************************************
Copyright (c) 1991-2026 Envitia Group PLC
=============================================================================
MODULE          : simple.cpp
PACKAGE         : MapLink
SCCS            :
AUTHOR(s)       : J. Robinson
=============================================================================
DESCRIPTION     : This contains a simple example of using the MapLink Skia 
drawing surface in a Windows application.
****************************************************************************/

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <string>
#include <direct.h>
#include <tchar.h>
#include <shellapi.h>
#include <vector>

#include "resource.h"

#include "MapLink.h"
#include "tslutf8decoder.h"
#include "tslutf8encoder.h"

// Comment out to enable the NT drawing surface instead.
// Useful for comparing the two surfaces.
#define SKIA 1

// Static handles to our drawing surface and data layer.

#if SKIA
static TSLSkiaSurface* drawingSurface;
#else
static TSLNTSurface* drawingSurface;
#endif

static TSLMapDataLayer* dataLayer;

// The name of our map layer. This is used when adding the data layer
// to the drawing surface and used to reference the data layer from the 
// drawing surface
static const char* MAP_LAYER_NAME = "map";

// map filename passed in
static TCHAR filename[MAX_PATH] = _T("");

// pixel buffer for rendering
void* myBuffer = nullptr;


///////////////////////////////////////////////////////////////////////////
// close - handles the Window Manager Close option (window destroy)
// Deletes all MapLink API objects and cleans up the configuration files
///////////////////////////////////////////////////////////////////////////

static void close()
{
    if (drawingSurface)
    {
        drawingSurface->removeDataLayer(MAP_LAYER_NAME);
        delete drawingSurface;
        drawingSurface = NULL;
    }

    if (dataLayer)
    {
        dataLayer->destroy();
        dataLayer = NULL;
    }

    TSLDrawingSurface::cleanup();
}

///////////////////////////////////////////////////////////////////////////
// resize - informs the drawing surface of any change in size of the window
///////////////////////////////////////////////////////////////////////////
static void resize(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    static bool initial_update = true;

    long ww = LOWORD(lParam); // width of client area 
    long wh = HIWORD(lParam); // height of client area 

    // NOTE : We will get an WM_PAINT to redraw the area, so don't bother drawing here
    if (initial_update)
    {
        // Inform the drawing surface of the initial size, then reset to display
        // the whole map.
        drawingSurface->wndResize(0, 0, ww, wh, false);
        drawingSurface->reset(false);

        initial_update = false;
    }
    else
    {
        // Inform the drawing surface of the new size of the window and ask it to
        // resize the map view accordingly, keeping the top left corner of the 
        // view anchored.
        drawingSurface->wndResize(0, 0, ww, wh, false, TSLResizeActionMaintainTopLeft);
    }
}

///////////////////////////////////////////////////////////////////////////
// press - simple input handler to allow zooming and panning around
// the map. Use shift key to provide zoom/pan and reset.
// Assumes a middle mouse button, but maps CTRL-LEFT BUTTON to be
// the same as the middle mouse button - just in case.
///////////////////////////////////////////////////////////////////////////
static void press(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    bool updated = false;

    // Map CTRL-LEFT to middle mouse button
    if (message == WM_LBUTTONDOWN && (wParam & MK_CONTROL))
        message = WM_MBUTTONDOWN;

    bool shiftPressed = (wParam & MK_SHIFT);
    long xPos = LOWORD(lParam);
    long yPos = HIWORD(lParam);

    // Get the position pressed in user units
    double x = 0.0;
    double y = 0.0;

    drawingSurface->DUToUU(xPos, yPos, &x, &y);

    if (message == WM_LBUTTONDOWN)
    {
        // Zoom in by 30%, with pan if shift is pressed.
        if (shiftPressed)
        {
            updated = drawingSurface->zoom(30, true, false);
            if (updated)
                drawingSurface->pan(x, y, true);
        }
        else
        {
            updated = drawingSurface->zoom(30, true, true);
        }
    }
    else  if (message == WM_MBUTTONDOWN)
    {
        if (shiftPressed)
        {
            // Reset to display overall map
            updated = drawingSurface->reset();
        }
        else
        {
            // Pan to point
            updated = drawingSurface->pan(x, y, true);
        }
    }
    else if (message == WM_RBUTTONDOWN)
    {
        // Zoom out by 30%, with pan if shift is pressed
        if (shiftPressed)
        {
            updated = drawingSurface->zoom(30, false, false);
            if (updated)
                drawingSurface->pan(x, y, true);
        }
        else
            updated = drawingSurface->zoom(30, false, true);
    }

    // If we haven't updated the view, then we have reached the edge of the world
    if (!updated)
        MessageBox(hwnd, _T("Coordinate space limits reached"), NULL, MB_ICONERROR);
    else
        InvalidateRect(hwnd, NULL, FALSE);
}

///////////////////////////////////////////////////////////////////////////
// draw - handles window exposures/paint messages.
// Uses the expose method to make use of double buffering for
// real window exposures.
///////////////////////////////////////////////////////////////////////////
static void draw(HWND hwnd, HDC hdc)
{
    // For simplicity, draw everything
    TSLDeviceUnits x1, y1, x2, y2;

    drawingSurface->getDUExtent(&x1, &y1, &x2, &y2);
    drawingSurface->drawDU(x1, y1, x2, y2, true);

#if SKIA
    // we own the drawing object here so we have to push to screen ourselves
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = 1920;
    bmi.bmiHeader.biHeight = -1080;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    SetDIBitsToDevice(hdc, 0, 0, 1920, 1080, 0, 0, 0, 1080,
        myBuffer, &bmi, DIB_RGB_COLORS);

#endif
}

///////////////////////////////////////////////////////////////////////////
// initialise - loads configuration files, creates drawing surface 
// bound to the window, creates a data layer, loads map into it, binds
// the two together and informs the user of any errors that occurred.
///////////////////////////////////////////////////////////////////////////
static void initialise(HWND hwnd)
{
    if (_tcslen(filename) == 0)
    {
        MessageBox(hwnd, _T("Please provide a command-line parameter indicating which map filename to display."), NULL, MB_ICONERROR);
        exit(0);
    }

    // SETUP THE DRAWING SURFACE
    // Initialise the drawing surface data files.
    TSLDrawingSurface::loadStandardConfig();

    // SETUP THE MAP DATA LAYER
    // Create a data layer and load the map
    dataLayer = new TSLMapDataLayer();

    // Find out if we need to build the full filename
    TCHAR drive[_MAX_DRIVE], path[TSL_MAX_PATH], file[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath(filename, drive, path, file, ext);

    if (!path[0])
    {
        _tgetcwd(path, TSL_MAX_PATH);
        TCHAR temp[TSL_MAX_PATH];
        _sntprintf(temp, TSL_MAX_PATH, _T("%s\\%s"), path, filename);
        _tcscpy(filename, temp);
    }

    if (!dataLayer->loadData(TSLUTF8Encoder(filename)))
    {
        MessageBox(hwnd, _T("Cannot load map"), NULL, MB_ICONERROR);
        exit(0);
    }

    // Create a drawing surface and bind the data layer to it.
#ifdef SKIA
    drawingSurface = new TSLSkiaSurface(&myBuffer, 1920, 1080, 0);
#else
    drawingSurface = new TSLNTSurface(hwnd, false);
#endif
        
    drawingSurface->addDataLayer(dataLayer, MAP_LAYER_NAME);

    // Make the drawing surface double buffered so that it looks pretty.
    drawingSurface->setOption(TSLOptionDoubleBuffered, true);

    // Check for any errors that have occurred, and display them
    const char* msg = TSLErrorStack::errorString("Error code: ");

    if (msg)
    {
        MessageBox(hwnd, (LPCTSTR)TSLUTF8Decoder(msg), NULL, MB_ICONERROR);
        exit(0);
    }
}

///////////////////////////////////////////////////////////////////////////
// WndProc - Window message handler
// Dispatches the message to an appropriate handler.
///////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        initialise(hwnd);
        return 0;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        press(hwnd, message, wParam, lParam);
        return 0;

    case WM_SIZE:
        resize(hwnd, wParam, lParam);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);
        draw(hwnd, hdc);
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        close();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////
// main - main entry point of the application. Pretty standard really.
///////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    // copy parameter to the filename
#ifdef _UNICODE
  // WinMain cannot recieve unicode arguments, so they have to be extracted manually
    int numArgs = 0;
    LPWSTR* cmdArgs = CommandLineToArgvW(GetCommandLineW(), &numArgs);
    if (numArgs > 1)
    {
        wcscpy(filename, cmdArgs[1]);
    }
    LocalFree(cmdArgs);
#else
    strcpy(filename, szCmdLine);
#endif

    // Do the usual Win32 Window Class registration, then enter the message loop
    const TCHAR szAppName[] = _T("simple");
    HWND        hwnd;
    MSG         msg;
    WNDCLASS    wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);

    hwnd = CreateWindow(szAppName, _T("Simple MapLink Viewer"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

