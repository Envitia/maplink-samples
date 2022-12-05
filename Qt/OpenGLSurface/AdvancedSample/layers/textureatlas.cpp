/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "textureatlas.h"
#include "MapLinkOpenGLSurface.h"
#include <cassert>

using std::max;

TextureAtlas::TextureAtlas( TSLOpenGLSurface *surface )
  : m_texture(0)
  , m_stackSize(0)
  , m_currentLevel(0)
  , m_atlasDimensions(1024) // Make each level of the atlas 1024x1024
  , m_currentShelfX(0)
  , m_currentShelfY(0)
  , m_currentShelfHeight(0)
{
  initializeOpenGLFunctions();

  // Create the first layer of the atlas immediately.
  increaseStackSize( surface );
}

TextureAtlas::~TextureAtlas()
{
  glDeleteTextures( 1, &m_texture );
}

void TextureAtlas::increaseStackSize( TSLOpenGLSurface *surface )
{
  // Use the drawing surface's state tracker to ensure the OpenGL state remains consistent
  // between the application and the drawing surface
  TSLOpenGLStateTracker *stateTracker = surface->stateTracker();

  GLuint tempPBO = 0;
  GLuint oldTexture = 0;
  if( m_texture )
  {
    // Since the size of a texture array can't be changed
    // after it's created, we need to create a new one and transfer over the existing contents.
    // To avoid downloading and re-uploading the contents of the old texture, we transfer it to a temporary
    // PBO so that it can remain resident in GPU memory for the transfer.
    stateTracker->bindTexture( GL_TEXTURE0, GL_TEXTURE_2D_ARRAY, m_texture );

    GLint internalWidth = 0, internalHeight = 0;
    glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &internalWidth );
    glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &internalHeight );
    GLsizeiptr sliceSize = internalWidth * internalHeight * 4 * (m_stackSize + 1);

    glGenBuffers( 1, &tempPBO );
    stateTracker->bindBuffer( GL_PIXEL_PACK_BUFFER, tempPBO );
    glBufferData( GL_PIXEL_PACK_BUFFER, sliceSize, NULL, GL_STREAM_COPY );

    // Read the contents of the current atlas into the PBO.
    glGetTexImage( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    stateTracker->bindBuffer( GL_PIXEL_PACK_BUFFER, 0 );

    oldTexture = m_texture;
  }

  glGenTextures( 1, &m_texture );
  stateTracker->bindTexture( GL_TEXTURE0, GL_TEXTURE_2D_ARRAY, m_texture );
  glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0 );

  glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, m_atlasDimensions, m_atlasDimensions, m_stackSize+1,
                0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

  if( tempPBO != 0 )
  {
    // Copy the old levels into the new texture
    stateTracker->bindBuffer( GL_PIXEL_UNPACK_BUFFER, tempPBO );
    glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_atlasDimensions, m_atlasDimensions, m_stackSize + 1,
                     GL_RGBA, GL_UNSIGNED_BYTE, NULL );

    stateTracker->bindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    glDeleteBuffers( 1, &tempPBO );
    glDeleteTextures( 1, &oldTexture );
  }

  // Initialise the contents of the texture level to transparent black
  unsigned char *emptyBuffer = new unsigned char[m_atlasDimensions * m_atlasDimensions * 4];
  memset( emptyBuffer, 0, m_atlasDimensions * m_atlasDimensions * 4 );
  glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, m_stackSize, m_atlasDimensions, m_atlasDimensions, 1, 
                   GL_RGBA, GL_UNSIGNED_BYTE, emptyBuffer );
  delete[] emptyBuffer;

  ++m_stackSize;
}

TextureAtlas::AtlasLocation TextureAtlas::allocateSpace( TSLOpenGLSurface *surface, uint32_t width, uint32_t height )
{
  // Always reserve a 1-pixel border around entries to avoid sampling errors during draw
  uint32_t requiredWidth = width+2;
  uint32_t requiredHeight = height+2;

  while( m_currentShelfX + requiredWidth > m_atlasDimensions )
  {
    // This shelf is full, move on to the next one
    m_currentShelfX = 0;
    m_currentShelfY += m_currentShelfHeight;
  }

  if( m_currentShelfY + requiredHeight >= m_atlasDimensions )
  {
    if( m_currentLevel == m_stackSize-1 )
    {
      // This level of the atlas is full, add another level.
      increaseStackSize( surface );
    }

    m_currentShelfX = 0;
    m_currentShelfY = 0;
    ++m_currentLevel;
  }

  // Convert pixel positions to normalized texture coordinates
  AtlasLocation coords;
  coords.blX = m_currentShelfX + 1;
  coords.blY = m_currentShelfY + 1;
  coords.trX = m_currentShelfX + width + 1;
  coords.trY = m_currentShelfY + height + 1;
  coords.level = m_currentLevel;

  m_currentShelfX += requiredWidth;
  m_currentShelfHeight = max( m_currentShelfHeight, requiredHeight );

  return coords;
}

void TextureAtlas::clear( TSLOpenGLSurface *surface )
{
  // Don't recreate the texture, just clear the existing contents.
  m_currentLevel = 0;
  m_currentShelfX = 0;
  m_currentShelfY = 0;

  unsigned char *emptyBuffer = new unsigned char[m_atlasDimensions * m_atlasDimensions * 4];
  memset( emptyBuffer, 0, m_atlasDimensions * m_atlasDimensions * 4 );

  surface->stateTracker()->bindTexture( GL_TEXTURE0, GL_TEXTURE_2D_ARRAY, m_texture );

  for( uint32_t i = 0; i < m_stackSize; ++i )
  {
    glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, m_atlasDimensions, m_atlasDimensions, 1, 
                     GL_RGBA, GL_UNSIGNED_BYTE, emptyBuffer );
  }
  delete[] emptyBuffer;
}
