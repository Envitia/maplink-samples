/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

// This class provides utility functions for compiling OpenGL shaders

#include <QOpenGLFunctions_3_1> // THIS IS WRONG WE WANT 3_0
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::pair;

class GLHelpers : protected QOpenGLFunctions_3_1 // THIS IS WRONG WE WANT 3_0
{
public:
  class GLShader : protected QOpenGLFunctions_3_1 // THIS IS WRONG WE WANT 3_0
  {
  public:
    GLShader();
    ~GLShader();

    GLuint m_program;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
  };

  GLHelpers();

  // Compiles and links the given vertex and fragment shaders into an OpenGL program. Any vertex attributes listed in 'attributeLocations'
  // are set to the given vertex attribute index.
  static GLShader* compileShader( const char *vertexSource, const char *fragmentSource, const vector< pair< string, GLuint > > &attributeLocations );

  // Basic 4x4 matrix multiplication
  static void matrixMultiply( const GLfloat *mat1, const GLfloat *mat2, GLfloat *result );

private:
  bool compileShaderCode( GLuint shader, const char *source );
};
