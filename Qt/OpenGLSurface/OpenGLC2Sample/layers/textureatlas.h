/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <QOpenGLFunctions_3_0>
#include <tslatomic.h>

// This class implements a simple shelf-packing texture atlas. A texture atlas stores multiple different
// items in a single texture so they can be used together when drawing.
//
// This atlas uses OpenGL array textures (see http://www.opengl.org/registry/specs/EXT/texture_array.txt) 
// which are part of the core OpenGL specification since 3.0. This allows for a very large number of items
// to be stored in the atlas in a way that allows them to all be referenced as part of a single draw call.
//
// Real application will want to use a more advanced packing algorithm that makes more efficient use
// of space within the atlas when the items stored are not all of a similar size, such as Skyline packing.

class TSLOpenGLSurface;

class TextureAtlas : protected QOpenGLFunctions_3_0
{
public:
  // Defines the position of an item within the atlas
  struct AtlasLocation
  {
    uint32_t blX;
    uint32_t blY;
    uint32_t trX;
    uint32_t trY;
    uint32_t level;
  };

  TextureAtlas( TSLOpenGLSurface *surface );
  ~TextureAtlas();

  // Assigns the requested amount of space within the atlas. This must be called each time
  // a new item is to be put into the atlas
  AtlasLocation allocateSpace( TSLOpenGLSurface *surface, uint32_t width, uint32_t height );

  // Empties the texture atlas
  void clear( TSLOpenGLSurface *surface );

  // Information about the atlas required for drawing.
  GLuint textureID() const;
  uint32_t currentTextureLevel() const;
  uint32_t atlasDimensions() const;
    
private:
  void increaseStackSize( TSLOpenGLSurface *surface );

  GLuint m_texture;
  uint32_t m_stackSize;
  uint32_t m_currentLevel;
  uint32_t m_atlasDimensions;

  // Position and height of the current shelf that is being populated
  uint32_t m_currentShelfX;
  uint32_t m_currentShelfY;
  uint32_t m_currentShelfHeight;
};

inline GLuint TextureAtlas::textureID() const
{
  return m_texture;
}

inline uint32_t TextureAtlas::currentTextureLevel() const
{
  return m_currentLevel;
}

inline uint32_t TextureAtlas::atlasDimensions() const
{
  return m_atlasDimensions;
}

#endif // TEXTUREATLAS_H
