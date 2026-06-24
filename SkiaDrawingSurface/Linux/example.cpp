/****************************************************************************
        Copyright (c) 2026 by Envitia Group PLC.
=============================================================================
MODULE          : example.cpp
PACKAGE         : MapLink
DESCRIPTION     : Simple MapLink viewer using TSLSkiaSurface on Linux.
                  Renders into a pixel buffer and presents to X11 via XPutImage.
****************************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <vector>

#include "MapLink.h"
#include "tslskiasurface.h"

// -------------------------------------------------------------------------
// Constants
// -------------------------------------------------------------------------
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

static const char* MAP_LAYER_NAME = "map";

// -------------------------------------------------------------------------
// Application state
// -------------------------------------------------------------------------
static TSLSkiaSurface* g_surface = nullptr;
static TSLMapDataLayer* g_dataLayer = nullptr;
static Display* g_display = nullptr;
static Window               g_window = 0;
static GC                   g_gc = 0;
static XImage* g_ximage = nullptr;
static bool                 g_firstResize = true;

// Window visual attributes — queried after window creation
static Visual* g_visual = nullptr;
static int                  g_depth = 0;

// Pixel buffer owned here; TSLSkiaSurface renders directly into it.
// Format: BGRA 32-bit premultiplied, top-down.
static std::vector<uint8_t> g_pixels;
static void* g_pixelsPtr = nullptr;
static int                  g_width = 0;
static int                  g_height = 0;

// -------------------------------------------------------------------------
// destroyXImage — free wrapper without freeing the pixel data we own
// -------------------------------------------------------------------------
static void destroyXImage()
{
    if (g_ximage)
    {
        g_ximage->data = nullptr; // prevent XDestroyImage from freeing g_pixels
        XDestroyImage(g_ximage);
        g_ximage = nullptr;
    }
}

// -------------------------------------------------------------------------
// savePixelsToBMP — writes g_pixels to /tmp/maplink_debug.bmp
// Call after drawDU() + opaqueAlpha() to inspect buffer contents.
// -------------------------------------------------------------------------
static void savePixelsToBMP(const char* path)
{
    if (g_pixels.empty() || g_width <= 0 || g_height <= 0)
    {
        fprintf(stderr, "savePixelsToBMP: no pixel data\n");
        return;
    }

    FILE* f = fopen(path, "wb");
    if (!f) { perror(path); return; }

    const int rowBytes = g_width * 4;
    const int pixelBytes = rowBytes * g_height;
    const int fileSize = 54 + pixelBytes;

    // BMP file header (14 bytes)
    uint8_t fh[14] = {};
    fh[0] = 'B'; fh[1] = 'M';
    fh[2] = (uint8_t)(fileSize);
    fh[3] = (uint8_t)(fileSize >> 8);
    fh[4] = (uint8_t)(fileSize >> 16);
    fh[5] = (uint8_t)(fileSize >> 24);
    fh[10] = 54; // pixel data offset
    fwrite(fh, 1, 14, f);

    // DIB header (40 bytes) — BITMAPINFOHEADER
    uint8_t ih[40] = {};
    ih[0] = 40; // header size
    // width
    ih[4] = (uint8_t)(g_width);
    ih[5] = (uint8_t)(g_width >> 8);
    ih[6] = (uint8_t)(g_width >> 16);
    ih[7] = (uint8_t)(g_width >> 24);
    // height — negative = top-down
    int negH = -g_height;
    ih[8] = (uint8_t)(negH);
    ih[9] = (uint8_t)(negH >> 8);
    ih[10] = (uint8_t)(negH >> 16);
    ih[11] = (uint8_t)(negH >> 24);
    ih[12] = 1;  // planes
    ih[14] = 32; // bits per pixel
    fwrite(ih, 1, 40, f);

    // Pixel data — BMP expects BGRA, Skia gives us BGRA — write directly
    fwrite(g_pixels.data(), 1, (size_t)pixelBytes, f);

    fclose(f);
    fprintf(stderr, "Saved %dx%d pixels to %s\n", g_width, g_height, path);
}

// -------------------------------------------------------------------------
// opaqueAlpha — force all alpha bytes to 0xFF so X11 depth-32 compositing
// does not treat any pixel as transparent.
// Skia's premultiplied BGRA layout: byte 3 of each pixel is alpha.
// -------------------------------------------------------------------------
static void opaqueAlpha()
{
    if (!g_pixelsPtr) return;
    uint8_t* p = static_cast<uint8_t*>(g_pixelsPtr);  // <-- was g_pixels.data()
    uint8_t* end = p + (size_t)(g_width * g_height * 4);
    for (; p < end; p += 4)
        p[3] = 0xFF;
}

// -------------------------------------------------------------------------
// rebuildXImage — create/recreate the XImage wrapper over g_pixels
//                 Must be called AFTER g_visual and g_depth are known
// -------------------------------------------------------------------------
static void rebuildXImage()
{
    fprintf(stderr, "rebuildXImage: w=%d h=%d visual=%p depth=%d pixelsPtr=%p\n",
        g_width, g_height, (void*)g_visual, g_depth, g_pixelsPtr);

    destroyXImage();
    if (g_width <= 0 || g_height <= 0 || !g_visual || !g_pixelsPtr)
    {
        fprintf(stderr, "rebuildXImage: early return — check above values\n");
        return;
    }

    g_ximage = XCreateImage(
        g_display, g_visual,
        (unsigned int)g_depth,
        ZPixmap, 0,
        (char*)g_pixelsPtr,
        (unsigned int)g_width,
        (unsigned int)g_height,
        32, g_width * 4);

    fprintf(stderr, "rebuildXImage: ximage=%p\n", (void*)g_ximage);
}


// -------------------------------------------------------------------------
// allocatePixelBuffer — resize buffer and re-attach surface
// -------------------------------------------------------------------------
static void allocatePixelBuffer(int width, int height)
{
    if (width <= 0 || height <= 0) return;
    if (width == g_width && height == g_height) return;

    g_width = width;
    g_height = height;

    g_pixels.assign((size_t)(width * height * 4), 0);
    g_pixelsPtr = g_pixels.data();

    if (g_surface)
    {
        g_surface->attach(&g_pixelsPtr, width, height, (size_t)(width * 4));

        // Fetch live pointer after attach
        const void* skiaPixels = g_surface->getPixels();
        if (skiaPixels)
            g_pixelsPtr = const_cast<void*>(skiaPixels);
    }

    rebuildXImage();
}

// -------------------------------------------------------------------------
// cleanup
// -------------------------------------------------------------------------
static void cleanup()
{
    if (g_surface)
    {
        g_surface->removeDataLayer(MAP_LAYER_NAME);
        delete g_surface;
        g_surface = nullptr;
    }
    if (g_dataLayer)
    {
        g_dataLayer->destroy();
        g_dataLayer = nullptr;
    }
    destroyXImage();
    if (g_gc)
    {
        XFreeGC(g_display, g_gc);
        g_gc = 0;
    }
    TSLDrawingSurface::cleanup();
}

// -------------------------------------------------------------------------
// handleResize
// -------------------------------------------------------------------------
static void handleResize(int width, int height)
{
    allocatePixelBuffer(width, height);

    if (g_firstResize)
    {
        g_surface->wndResize(0, 0, width, height, false);
        g_surface->reset();
        g_firstResize = false;
    }
    else
    {
        g_surface->wndResize(0, 0, width, height, false,
            TSLResizeActionMaintainTopLeft);
    }
}

// -------------------------------------------------------------------------
// handleExpose
// -------------------------------------------------------------------------
static void handleExpose()
{
    if (!g_surface || g_width <= 0 || g_height <= 0 || !g_ximage)
    {
        fprintf(stderr, "handleExpose: early exit — surface=%p w=%d h=%d ximage=%p\n",
            (void*)g_surface, g_width, g_height, (void*)g_ximage);
        return;
    }

    if (g_firstResize)
    {
        fprintf(stderr, "handleExpose: calling wndResize+reset (%dx%d)\n", g_width, g_height);
        g_surface->wndResize(0, 0, g_width, g_height, false);
        g_surface->reset();
        g_firstResize = false;

        // Check for errors immediately after reset
        const char* err = TSLErrorStack::errorString("After reset: ");
        if (err) fprintf(stderr, "%s\n", err);
    }

    TSLDeviceUnits x1, y1, x2, y2;
    g_surface->getDUExtent(&x1, &y1, &x2, &y2);
    fprintf(stderr, "handleExpose: DU extent x1=%ld y1=%ld x2=%ld y2=%ld\n",
        (long)x1, (long)y1, (long)x2, (long)y2);

    bool drawResult = g_surface->drawDU(x1, y1, x2, y2, true);
    fprintf(stderr, "handleExpose: drawDU returned %s\n", drawResult ? "true" : "false");

    // Check error stack after draw
    const char* err = TSLErrorStack::errorString("After drawDU: ");
    if (err) fprintf(stderr, "%s\n", err);

    // Sample a few pixels to check if anything was drawn
    if (g_pixels.size() >= 4)
    {
        uint8_t* p = g_pixels.data();
        fprintf(stderr, "Pixel[0,0]   = B=%d G=%d R=%d A=%d\n", p[0], p[1], p[2], p[3]);
    }
    if ((size_t)(g_width * g_height / 2 * 4 + 3) < g_pixels.size())
    {
        uint8_t* p = g_pixels.data() + (g_width * (g_height / 2) + g_width / 2) * 4;
        fprintf(stderr, "Pixel[mid]   = B=%d G=%d R=%d A=%d\n", p[0], p[1], p[2], p[3]);
    }

    opaqueAlpha();

    static bool saved = false;
    if (!saved) { savePixelsToBMP("/tmp/maplink_debug.bmp"); saved = true; }

    opaqueAlpha();
    g_ximage->data = (char*)g_pixelsPtr;   // <-- was g_pixels.data()

    XPutImage(g_display, g_window, g_gc, g_ximage,
        0, 0, 0, 0,
        (unsigned int)g_width, (unsigned int)g_height);
    XFlush(g_display);
}

// -------------------------------------------------------------------------
// handleButtonPress
// -------------------------------------------------------------------------
static void handleButtonPress(XButtonEvent& ev)
{
    bool shiftPressed = (ev.state & ShiftMask) != 0;
    bool updated = false;

    double x = 0.0, y = 0.0;
    g_surface->DUToUU(ev.x, ev.y, &x, &y);

    switch (ev.button)
    {
    case Button1:
        if (shiftPressed)
        {
            updated = g_surface->zoom(30, true, false);
            if (updated) g_surface->pan(x, y, true);
        }
        else
            updated = g_surface->zoom(30, true, true);
        break;

    case Button2:
        updated = shiftPressed ? g_surface->reset()
            : g_surface->pan(x, y, true);
        break;

    case Button3:
        if (shiftPressed)
        {
            updated = g_surface->zoom(30, false, false);
            if (updated) g_surface->pan(x, y, true);
        }
        else
            updated = g_surface->zoom(30, false, true);
        break;
    }

    if (!updated)
        fprintf(stderr, "Coordinate space limits reached\n");
    else
        handleExpose();
}

// -------------------------------------------------------------------------
// usage
// -------------------------------------------------------------------------
static void usage(const char* appname)
{
    printf("Usage: %s [-help] mapfile.map\n", appname);
    printf("  LMB         = zoom in\n");
    printf("  RMB         = zoom out\n");
    printf("  MMB         = pan to point\n");
    printf("  Shift+LMB   = zoom in and pan\n");
    printf("  Shift+RMB   = zoom out and pan\n");
    printf("  Shift+MMB   = reset\n");
    exit(0);
}

// -------------------------------------------------------------------------
// main
// -------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    const char* filename = nullptr;

    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-help") || !strcmp(argv[i], "-h"))
            usage(argv[0]);
        else
            filename = argv[i];
    }

    if (!filename)
        usage(argv[0]);

    char resolvedname[PATH_MAX];
    if (!realpath(filename, resolvedname))
    {
        fprintf(stderr, "Unable to resolve path to '%s'\n", filename);
        return 1;
    }

    // ---- X11 setup ----
    g_display = XOpenDisplay(nullptr);
    if (!g_display)
    {
        fprintf(stderr, "Cannot open X display\n");
        return 1;
    }

    int screen = DefaultScreen(g_display);

    // Use XCreateSimpleWindow — inherits the root visual (depth 32 on
    // compositing desktops). We fix transparency by forcing alpha=0xFF
    // in opaqueAlpha() after every drawDU().
    g_window = XCreateSimpleWindow(
        g_display,
        RootWindow(g_display, screen),
        0, 0,
        (unsigned int)WINDOW_WIDTH,
        (unsigned int)WINDOW_HEIGHT,
        0,
        BlackPixel(g_display, screen),
        BlackPixel(g_display, screen));

    XStoreName(g_display, g_window, "Simple MapLink Skia Viewer");

    Atom wm_delete = XInternAtom(g_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(g_display, g_window, &wm_delete, 1);

    XSelectInput(g_display, g_window,
        ExposureMask | StructureNotifyMask | ButtonPressMask);

    XMapWindow(g_display, g_window);
    XSync(g_display, False);   // <-- ensure server processed MapWindow before querying

    {
        XWindowAttributes wa;
        XGetWindowAttributes(g_display, g_window, &wa);
        g_visual = wa.visual;
        g_depth = wa.depth;
        fprintf(stderr, "Visual=%p depth=%d\n", (void*)g_visual, g_depth);
    }

    g_gc = XCreateGC(g_display, g_window, 0, nullptr);

    // ---- MapLink setup ----
    TSLDrawingSurface::loadStandardConfig();
    TSLErrorStack::clear();

    g_dataLayer = new TSLMapDataLayer();
    if (!g_dataLayer->loadData(resolvedname))
    {
        fprintf(stderr, "Cannot load map '%s'\n", resolvedname);
        return 1;
    }

    g_width = WINDOW_WIDTH;
    g_height = WINDOW_HEIGHT;
    g_pixels.assign((size_t)(g_width * g_height * 4), 0);
    g_pixelsPtr = g_pixels.data();

    g_surface = new TSLSkiaSurface(&g_pixelsPtr, g_width, g_height, 0);

    // TODO :  Fix this, we should be able to use the pixel pointer we passed in here.
    const void* skiaPixels = g_surface->getPixels();
    fprintf(stderr, "getPixels() returned %p\n", skiaPixels);

    if (skiaPixels)
        g_pixelsPtr = const_cast<void*>(skiaPixels);
    else
        fprintf(stderr, "WARNING: getPixels() is null — surface not initialised\n");

    g_surface->addDataLayer(g_dataLayer, MAP_LAYER_NAME);
    g_surface->setOption(TSLOptionDoubleBuffered, true);

    rebuildXImage();

    const char* err = TSLErrorStack::errorString("Errors:\n");
    if (err)
    {
        fprintf(stderr, "%s\n", err);
        return 1;
    }

    rebuildXImage();

    // ---- Event loop ----
    XEvent ev;
    bool running = true;

    while (running)
    {
        XNextEvent(g_display, &ev);

        switch (ev.type)
        {
        case Expose:
            if (ev.xexpose.count == 0)
                handleExpose();
            break;

        case ConfigureNotify:
            handleResize(ev.xconfigure.width, ev.xconfigure.height);
            break;

        case ButtonPress:
            handleButtonPress(ev.xbutton);
            break;

        case ClientMessage:
            if ((Atom)ev.xclient.data.l[0] == wm_delete)
                running = false;
            break;
        }
    }

    cleanup();
    XCloseDisplay(g_display);
    return 0;
}