/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

// This header file contains the GLSL source code for the shaders used by the tracks data layer.

/**********************************
 * Vertex shader for drawing tracks from a texture atlas
 **********************************/
static const char *g_trackBodyVertexShaderSource = "#version 130\n\
\n\
in vec3 vertexPositon;\n\
in vec2 clipShift;\n\
in vec3 texCoords;\n\
\n\
out vec3 atlasCoords;\n\
\n\
uniform mat4 mvpMatrix;\n\
\n\
void main()\n\
{\n\
  gl_Position = mvpMatrix * vec4( vertexPositon.xy, 0.0, 1.0 );\n\
  gl_Position.xy += clipShift;\n\
  gl_Position.zw = vec2( vertexPositon.z, 1.0 );\n\
  atlasCoords = texCoords;\n\
}\n\
";

/**********************************
 * Fragment shader for drawing tracks from a texture atlas
 **********************************/
static const char *g_trackBodyFragmentShaderSource = "#version 130\n\
\n\
uniform sampler2DArray tex0;\n\
\n\
in vec3 atlasCoords;\n\
\n\
out vec4 pixelColour;\n\
\n\
void main()\n\
{\n\
  vec4 fragment = texture( tex0, atlasCoords );\n\
  // Discard fragments fully transparent fragments to avoid writing to the depth buffer\n\
  if( fragment.a == 0.0 )\n\
    discard;\n\
  pixelColour = fragment;\n\
}\n\
";

/**********************************
 * Vertex shader for drawing track heading indicators
 **********************************/
static const char *g_trackHeadingVertexShaderSource = "#version 130\n\
\n\
in vec3 vertexPositon;\n\
in vec4 colour;\n\
\n\
out vec4 fragmentColour;\n\
\n\
uniform mat4 mvpMatrix;\n\
\n\
void main()\n\
{\n\
  gl_Position = mvpMatrix * vec4( vertexPositon.xy, 0.0, 1.0 );\n\
  gl_Position.zw = vec2( vertexPositon.z, 1.0 );\n\
  fragmentColour = colour;\n\
}\n\
";

/**********************************
 * Fragment shader for drawing track heading indicators
 **********************************/
static const char *g_trackHeadingFragmentShaderSource = "#version 130\n\
\n\
in vec4 fragmentColour;\n\
\n\
out vec4 pixelColour;\n\
\n\
void main()\n\
{\n\
  pixelColour = fragmentColour;\n\
}\n\
";

/**********************************
 * Vertex shader for drawing track history points
 **********************************/
static const char *g_trackHistoryVertexShaderSource = "#version 130\n\
\n\
in vec3 vertexPositon;\n\
in vec4 colour;\n\
\n\
out vec4 fragmentColour;\n\
\n\
uniform mat4 mvpMatrix;\n\
uniform float size;\n\
\n\
void main()\n\
{\n\
  gl_Position = mvpMatrix * vec4( vertexPositon.xy, 0.0, 1.0 );\n\
  gl_Position.zw = vec2( vertexPositon.z, 1.0 );\n\
  gl_PointSize = size;\n\
  fragmentColour = colour;\n\
}\n\
";
