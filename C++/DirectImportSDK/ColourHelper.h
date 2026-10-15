#pragma once

#include "tslrenderingattributes.h"
#include <random>

class ColourHelper
{
public:
  static int getRandomColour()
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randomHue(0.0, 359.0);
    std::uniform_int_distribution<> randomSaturation(100, 240);
    std::uniform_int_distribution<> randomValue(200, 240);

    int r, g, b;
    hsv2rgb(randomHue(gen), randomSaturation(gen), randomValue(gen), r, g, b);

    return TSLComposeRGB(r, g, b);

  }

  static void setRandomColour(TSLRenderingAttributes & attribs)
  {
    int col = getRandomColour();
    attribs.m_edgeColour = col;
    attribs.m_fillColour = col; // Leave exterior edge colour as default
    attribs.m_symbolColour = col;
    attribs.m_textColour = col;
  }

private:

  static void hsv2rgb(double h, int  s, int  v, int & r, int & g, int & b)
  {
    double hh, ff;
    int    p, q, t;
    long   i;

    if (s <= 0)
    {
      r = v;
      g = v;
      b = v;
      return;
    }

    hh = h;
    if (hh >= 360.0)
      hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = (int)(v * (255.0 - s) / 255.0);
    q = (int)(v * (255.0 - (s * ff)) / 255.0);
    t = (int)(v * (255.0 - (s * (1.0 - ff))) / 255.0);

    switch (i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;

    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
    default:
      r = v;
      g = p;
      b = q;
      break;
    }
  }
};

