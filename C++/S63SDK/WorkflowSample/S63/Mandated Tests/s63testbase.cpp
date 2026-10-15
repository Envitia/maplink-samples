//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../../stdafx.h"
#include "s63testbase.h"

namespace S63OEMTests
{

S63TestBase::S63TestBase(const S63ConfigurationObject &configurationObject)
: m_resultsPath(configurationObject.m_resultsDirectory)
, m_configurationLocation(configurationObject.m_configurationLocation)
, m_dataLocation(configurationObject.m_testDataLocation)
, m_surface(NULL)
, m_screen(NULL)
, m_display(NULL)
, m_bitmap(NULL)
, m_oldBitmap(NULL)
, m_dsWidth(600)
, m_dsHeight(600)
{
}

S63TestBase::~S63TestBase()
{
  destroyMapLinkSurface();
}

void S63TestBase::destroyMapLinkSurface()
{
  if( m_surface )
  {
    delete m_surface;
  }

  if ( m_display )
  {
    if (m_oldBitmap)
    {
      SelectObject(m_display, m_oldBitmap);
    }
    DeleteDC( m_display );
  }
  if ( m_screen )
  {
    DeleteDC( m_screen );
  }
  if ( m_bitmap )
  {
    DeleteObject( m_bitmap );
  }

  m_surface = NULL;
  m_screen = NULL;
  m_display = NULL;
  m_bitmap = NULL;
  m_oldBitmap = NULL;
}

void S63TestBase::createMapLinkSurface()
{
  if( m_surface )
  {
    return;
  }

  m_screen = CreateDC(TSLUTF8Decoder("DISPLAY"), NULL, NULL, NULL);
  m_display = CreateCompatibleDC(m_screen);
  m_bitmap = CreateCompatibleBitmap(m_screen, m_dsWidth, m_dsHeight);
  m_oldBitmap = (HBITMAP)SelectObject(m_display, m_bitmap);
      
  // clear to red
  RECT rect = {0, 0, m_dsWidth, m_dsHeight};
  HBRUSH brush = CreateSolidBrush(RGB(0xff,0x00,0x00));
  FillRect(m_display, &rect,  brush);
  DeleteObject(brush);

  // attach bitmap
  m_surface = new TSLNTSurface(m_display, true);
  m_surface->wndResize(0, 0, m_dsWidth, m_dsHeight);

  m_surface->reset();
}

bool S63TestBase::drawToBitmap(const char* filename)
{
  if( !m_surface || !m_bitmap)
  {
    return false;
  }

  if( !m_surface->drawDU(0, 0, m_dsWidth, m_dsHeight, true) )
  {
    return false;
  }
  
  if( !writeBMP(m_bitmap, m_display, filename) )
  {
    return false;
  }

  return true;
}

bool S63TestBase::writeBMP(HBITMAP bitmap, HDC hDC, const char* filename)
{
	BITMAP bmp; 
	PBITMAPINFO pbmi; 
	WORD cClrBits; 
	HANDLE hf; // file handle 
	BITMAPFILEHEADER hdr; // bitmap file-header 
	PBITMAPINFOHEADER pbih; // bitmap info-header 
	LPBYTE lpBits; // memory pointer 
	DWORD dwTotal; // total count of bytes 
	DWORD cb; // incremental count of bytes 
	BYTE *hp; // byte pointer 
	DWORD dwTmp; 

	// create the bitmapinfo header information

	if (!GetObject( bitmap, sizeof(BITMAP), (LPSTR)&bmp))
	{
	  return false;
	}

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
	// Allocate memory for the BITMAPINFO structure.
	if (cClrBits != 24) 
	pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
	sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); 
	else 
	pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

	// Initialize the fields in the BITMAPINFO structure. 

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pbmi->bmiHeader.biWidth = bmp.bmWidth; 
	pbmi->bmiHeader.biHeight = bmp.bmHeight; 
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
	if (cClrBits < 24) 
	pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

	// If the bitmap is not compressed, set the BI_RGB flag. 
	pbmi->bmiHeader.biCompression = BI_RGB; 

	// Compute the number of bytes in the array of color 
	// indices and store the result in biSizeImage. 
	pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 * pbmi->bmiHeader.biHeight * cClrBits; 
	// Set biClrImportant to 0, indicating that all of the 
	// device colors are important. 
	pbmi->bmiHeader.biClrImportant = 0; 

	// now open file and save the data
	pbih = (PBITMAPINFOHEADER) pbmi; 
	lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits) 
  {
  	return false;
	}

	// Retrieve the color table (RGBQUAD array) and the bits 
	if (!GetDIBits(hDC, HBITMAP(bitmap), 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) 
  {
  	return false;
	}

	// Create the .BMP file. 
	hf = CreateFile(TSLUTF8Decoder(filename), GENERIC_READ | GENERIC_WRITE, (DWORD) 0, 
	NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 
	(HANDLE) NULL); 
	if (hf == INVALID_HANDLE_VALUE)
  {
	  return false;
	}
	hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M" 
	// Compute the size of the entire file. 
	hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
	pbih->biSize + pbih->biClrUsed 
	* sizeof(RGBQUAD) + pbih->biSizeImage); 
	hdr.bfReserved1 = 0; 
	hdr.bfReserved2 = 0; 

	// Compute the offset to the array of color indices. 
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
	pbih->biSize + pbih->biClrUsed 
	* sizeof (RGBQUAD); 

	// Copy the BITMAPFILEHEADER into the .BMP file. 
	if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp, NULL)) 
  {
	  return false;
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
	if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
	+ pbih->biClrUsed * sizeof (RGBQUAD), 
	(LPDWORD) &dwTmp, ( NULL)))
  {
	  return false;
	}


	// Copy the array of color indices into the .BMP file. 
	dwTotal = cb = pbih->biSizeImage; 
	hp = lpBits; 
	if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
  {
	  return false;
	}

	// Close the .BMP file. 
	if (!CloseHandle(hf))
  {
	  return false;
	}

	// Free memory. 
	GlobalFree((HGLOBAL)lpBits);
  return true;
}

TSLS63DataLayerScaleBands* S63TestBase::createScaleBands()
{
  TSLS63DataLayerScaleBands* bands = new TSLS63DataLayerScaleBands();
  double minimum = (std::numeric_limits<double>::min)();
  double maximum = (std::numeric_limits<double>::max)();
  bands->addBand(TSLS63ScaleBandOverview, minimum, maximum);
  bands->addBand(TSLS63ScaleBandGeneral, minimum, maximum);
  bands->addBand(TSLS63ScaleBandCoastal, minimum, maximum);
  bands->addBand(TSLS63ScaleBandApproach, minimum, maximum);
  bands->addBand(TSLS63ScaleBandHarbour, minimum, maximum);
  bands->addBand(TSLS63ScaleBandBerthing, minimum, maximum);
  bands->addBand(TSLS63ScaleBandOverscale, minimum, maximum);

  return bands;
}

bool S63TestBase::setupFeatureRendering(TSLDrawingSurface* surface)
  {
    if ( !surface )
      return false;

    TSLRenderingAttributes atts;
    memset(&atts, 0, sizeof(TSLRenderingAttributes));
    atts.m_borderColour = 181;
    atts.m_borderWidth = 1.0;
    atts.m_edgeThickness = 1.0;
    atts.m_edgeThicknessUnits = TSLDimensionUnitsMapUnits;
    atts.m_edgeStyle = 1;
    atts.m_edgeColour = 1;
    atts.m_exteriorEdgeColour = 1;
    atts.m_exteriorEdgeStyle = 1;
    atts.m_exteriorEdgeThickness = 1.0;
    atts.m_exteriorEdgeThicknessUnits = TSLDimensionUnitsMapUnits;

    char buffer[256];
    for ( int i = 1; i <= 504; ++i )
    {
      sprintf(buffer, "Feature%d", i);
      if ( !surface->addFeatureRendering(buffer, i) )
        return false;
      if ( !surface->setFeatureRendering(0, i, &atts) )
        return false;
    }
    
    return true;
  }

};