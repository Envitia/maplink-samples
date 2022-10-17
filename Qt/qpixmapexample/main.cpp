#include <QApplication>
#include <QGraphicsObject>
#include <QGraphicsView>
#include <QPainter>
#include <QPaintEvent>
#include <qbitmap.h>
#include <windows.h>

#include <MapLink.h>

#include <memory>

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{
  BITMAP bmp;
  PBITMAPINFO pbmi;
  WORD    cClrBits;

  // Retrieve the bitmap color format, width, and height.  
  if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
    return nullptr;

  // Convert the color format to a count of bits.  
  cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
  if (cClrBits == 1)
    cClrBits = 1;
  else if (cClrBits <= 4)
    cClrBits = 4;
  else if (cClrBits <= 8)
    cClrBits = 8;
  else if (cClrBits <= 16)
    cClrBits = 16;
  else if (cClrBits <= 24)
    cClrBits = 24;
  else cClrBits = 32;

  // Allocate memory for the BITMAPINFO structure. (This structure  
  // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
  // data structures.)  

  if (cClrBits < 24)
  {
    pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1ULL << cClrBits));
  }
  // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 
  else
  {
    pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));
  }

  // Initialize the fields in the BITMAPINFO structure.  

  pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  pbmi->bmiHeader.biWidth = bmp.bmWidth;
  pbmi->bmiHeader.biHeight = bmp.bmHeight;
  pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
  pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
  if (cClrBits < 24)
    pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

  // If the bitmap is not compressed, set the BI_RGB flag.  
  pbmi->bmiHeader.biCompression = BI_RGB;

  // Compute the number of bytes in the array of color  
  // indices and store the result in biSizeImage.  
  // The width must be DWORD aligned unless the bitmap is RLE 
  // compressed. 
  pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * pbmi->bmiHeader.biHeight;
  // Set biClrImportant to 0, indicating that all of the  
  // device colors are important.  
  pbmi->bmiHeader.biClrImportant = 0;
  return pbmi;
}


class MyGraphicsObject : public QGraphicsObject
{
public:
  MyGraphicsObject(std::string mapPath, std::vector<std::string> overlayImagePaths) 
  {
    mapPixmap.reset(new QPixmap());

    for (auto overlayPath : overlayImagePaths)
    {
      overlayImagePixmaps.push_back(new QPixmap(overlayPath.c_str()));
    }

    screenDc = CreateDC(L"DISPLAY", nullptr, nullptr, nullptr);
    displayDc = CreateCompatibleDC(screenDc);
    
    m_drawingSurface.reset(new TSLNTSurface(displayDc, true));
    m_dataLayer.reset(new TSLMapDataLayer());

    QRect cr = boundingRect().toRect();
    bool mapLoaded = m_drawingSurface->wndResize(0, 0, cr.width(), cr.height(), false);
    mapLoaded = m_drawingSurface->reset();

    mapLoaded = m_dataLayer->loadData(mapPath.c_str());
    mapLoaded = m_drawingSurface->addDataLayer(m_dataLayer.get(), "map");

    std::pair<double, double> bottomLeft = { 0,0 };
    std::pair<double, double> topRight = { 0,0 };
    m_dataLayer->getUUExtent(&bottomLeft.first, &bottomLeft.second, &topRight.first, &topRight.second, m_drawingSurface.get());
    m_drawingSurface->resize(bottomLeft.first, bottomLeft.second, topRight.first, topRight.second, true, true);
  }

  ~MyGraphicsObject()
  {
    DeleteObject(displayDc);
    DeleteDC(screenDc);

    for (auto* overlay : overlayImagePixmaps)
    {
      delete overlay;
    }
  }

  QRectF boundingRect() const {
    const QRectF targetRect = { -299, -99, 600, 200 };
    return targetRect;
  }

  void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
  {
    QRectF widgetRect = boundingRect();
    QRect cr = boundingRect().toRect();

    // Draw the pixmaps to screen
    DrawToPixmap(m_drawingSurface.get(), mapPixmap.get(), cr);
    painter->drawPixmap(cr, *mapPixmap);
    for (auto& pixmap : overlayImagePixmaps)
    {
      painter->drawPixmap(0, 0, 30, 30, *pixmap);
    }

  }

  bool DrawToPixmap(TSLNTSurface* drawingSurface, QPixmap* pixmap, QRect& cr)
  {
	// Create a bitmap, Windows style
    HBITMAP hBitmap = CreateCompatibleBitmap(screenDc, cr.width(), cr.height());
    SelectObject(displayDc, hBitmap);

    // Size the drawing surface to the bitmaps dimensions
    drawingSurface->wndResize(0, 0, cr.width(), cr.height(), true, TSLResizeActionMaintainCentre);
	
	// Get the visible extent of the map in user units
    std::pair<double, double> bottomLeft = { 0,0 };
    std::pair<double, double> topRight = { 0,0 };
    drawingSurface->getUUExtent(&bottomLeft.first, &bottomLeft.second, &topRight.first, &topRight.second);

    //Tell the drawing surface to draw to the bitmap.
    bool result = drawingSurface->drawToHDC((TSLDeviceContext)displayDc, bottomLeft.first, bottomLeft.second, topRight.first, topRight.second, false);

    // Setup stuff we need to get the bitmap bits (Windows style)
    PBITMAPINFOHEADER pbih;  
    LPBYTE lpBits;
    PBITMAPINFO pbi = CreateBitmapInfoStruct(hBitmap);
    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	// Windows bitmaps are top-down, negating biHeight instructs Windows to read the lines in reverse
    pbi->bmiHeader.biHeight = -pbi->bmiHeader.biHeight;

    // Get the bitmap bits.
    GetDIBits(displayDc, hBitmap, 0, (WORD)cr.height(), lpBits, pbi, DIB_RGB_COLORS);

    // We now have the bitmap the right way up and in RGB 32 format. Load into a QImage...
    QImage image(lpBits, cr.width(), cr.height(), QImage::Format_RGB32);
	// ... which we then load into the pixmap.
    *pixmap = QPixmap::fromImage(image);

    // Free memory.  
    GlobalFree((HGLOBAL)lpBits);
    LocalFree(pbi);

    return result;
  }

  std::unique_ptr<QPixmap> mapPixmap;
  std::vector<QPixmap*> overlayImagePixmaps;

  std::unique_ptr<TSLNTSurface> m_drawingSurface;
  std::unique_ptr<TSLDataLayer, TSLDestroyPointer<TSLDataLayer>> m_dataLayer;

  HDC screenDc = nullptr;
  HDC displayDc = nullptr;
};

class View : public QGraphicsView {
public:
  View(QWidget* parent = 0) :
    QGraphicsView(parent)
  {
    // This brings the original paint engine alive.
    QGraphicsView::paintEngine();

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setRenderHint(QPainter::Antialiasing);
  }

  QPaintEngine* paintEngine() const Q_DECL_OVERRIDE { return 0; }

  void create()
  {
    if (!created)
    {
      TSLErrorStack::clear();
      TSLDrawingSurface::loadStandardConfig();
      TSLDrawingSurface::setupSymbols("tslsymbolsAPP6A.dat");
      TSLCoordinateSystem::loadCoordinateSystems();

      created = true;
    }

  }

  bool event(QEvent* event) Q_DECL_OVERRIDE {

    switch (event->type())
    {
    case QEvent::Create:
      create();
      break;

    case QEvent::Resize:
      if (!created)
      {
        create();
      }

      break;

    case QEvent::Paint:
    {
      break;
    }
    case QEvent::UpdateRequest:
    {
      break;
    }
    }

    return QGraphicsView::event(event);
  }

  void resizeEvent(QResizeEvent* event) {
    fitInView(0,0,event->size().width(), event->size().height(), Qt::KeepAspectRatio);
  }

private:
  bool created = false;
};


int main(int argc, char* argv[])
{
  enum Args
  {
    Arg_map = 1, // First argument is the path to a MapLink Map
    Arg_overlayImagesStart // Can then have as many paths to images as you like, they will be loaded into a pixmap and drawn over the map.
  };

  std::string mapPath = (argc >= Arg_map ? argv[Arg_map] : "");
  std::vector<std::string> imagePaths;
  for (int arg = Arg_overlayImagesStart; arg < argc; ++arg)
  {
    imagePaths.push_back(argv[arg]);
  }

  QApplication a(argc, argv);
  QGraphicsScene s;
  MyGraphicsObject* obj = new MyGraphicsObject(mapPath, imagePaths);
  s.addItem(obj);
  View view;
  view.setScene(&s);
  view.show();

  return a.exec();
}