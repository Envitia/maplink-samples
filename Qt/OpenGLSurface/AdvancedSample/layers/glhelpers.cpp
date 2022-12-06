/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "glhelpers.h"
#include <QMessageBox>

GLHelpers::GLShader::GLShader()
  : m_program(0)
  , m_vertexShader(0)
  , m_fragmentShader(0)
{
  initializeOpenGLFunctions();
}

GLHelpers::GLShader::~GLShader()
{
  glDetachShader( m_program, m_vertexShader );
  glDetachShader( m_program, m_fragmentShader );
  glDeleteShader( m_vertexShader );
  glDeleteShader( m_fragmentShader );
  glDeleteProgram( m_program );
}

GLHelpers::GLHelpers()
{
  initializeOpenGLFunctions();
}

GLHelpers::GLShader* GLHelpers::compileShader( const char *vertexSource, const char *fragmentSource, const vector< pair< string, GLuint > > &attributeLocations )
{
  GLHelpers helper; // Provides access to OpenGL functions

  GLShader *shader = new GLShader();
  shader->m_program = helper.glCreateProgram();
  shader->m_vertexShader = helper.glCreateShader( GL_VERTEX_SHADER );
  shader->m_fragmentShader = helper.glCreateShader( GL_FRAGMENT_SHADER );

  if( !helper.compileShaderCode( shader->m_vertexShader, vertexSource ) ||
      !helper.compileShaderCode( shader->m_fragmentShader, fragmentSource ) )
  {
    delete shader;
    return NULL;
  }

  helper.glAttachShader( shader->m_program, shader->m_vertexShader );
  helper.glAttachShader( shader->m_program, shader->m_fragmentShader );

  for( size_t i = 0; i < attributeLocations.size(); ++i )
  {
    helper.glBindAttribLocation( shader->m_program, attributeLocations[i].second, attributeLocations[i].first.c_str() );
  }

  helper.glLinkProgram( shader->m_program );
  GLint programLinkSuccess = GL_FALSE;
  helper.glGetProgramiv( shader->m_program, GL_LINK_STATUS, &programLinkSuccess );

  GLint logLength = 0;
  helper.glGetProgramiv( shader->m_program, GL_INFO_LOG_LENGTH, &logLength );
  if( logLength > 0 )
  {
    GLchar *programLog = new GLchar[logLength];
    programLog[0] = '\0';
    helper.glGetProgramInfoLog( shader->m_program, logLength, NULL, programLog );

    // Some implementations return a log that is just a NULL terminator - disregard these
    if( programLog[0] != '\0' )
    {
      QMessageBox::critical( NULL, "Error: Failed to link shader", QString(programLog) );
    }
    delete[] programLog;
  }

  if( !programLinkSuccess )
  {
    delete shader;
    return NULL;
  }

  return shader;
}

void GLHelpers::matrixMultiply( const GLfloat *mat1, const GLfloat *mat2, GLfloat *result )
{
  for( size_t col = 0; col < 16; col += 4 )
  {
    for( size_t row = 0; row < 4; ++row )
    {
      result[row + col] = ( mat1[0+row]  * mat2[0+col] ) +
                          ( mat1[4+row]  * mat2[1+col] ) +
                          ( mat1[8+row]  * mat2[2+col] ) +
                          ( mat1[12+row] * mat2[3+col] );
    }
  }
}

bool GLHelpers::compileShaderCode( GLuint shader, const char *source )
{
  glShaderSource( shader, 1, &source, NULL );
  glCompileShader( shader );

  GLint compileSuccessful = GL_FALSE;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &compileSuccessful );

  GLint logLength = 0;
  glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );

  if( logLength > 0 )
  {
    GLchar *compileLog = new GLchar[logLength];
    compileLog[0] = '\0';
    glGetShaderInfoLog( shader, logLength, NULL, compileLog );

    // Some implementations always return a log which is just a NULL terminator - disregard these
    if( compileLog[0] != '\0' )
    {
      QMessageBox::critical( NULL, "Error: Failed to compile shader", QString(compileLog) );
    }

    delete[] compileLog;
  }

  return compileSuccessful == GL_TRUE;
}
