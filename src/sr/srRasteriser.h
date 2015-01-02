// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRRASTERISER_H
#define SRRASTERISER_H

// Render states
enum
{
  SR_WIREFRAME = 0,
  SR_RENDER_STATE_COUNT
};

// Primitive types
enum
{
  SR_POINT_LIST,
  SR_LINE_LIST,
  SR_LINE_STRIP,
  SR_TRIANGLE_LIST,
  SR_TRIANGLE_STRIP
};

// Vertex attributes
enum
{
  SR_VERT_POSITION,
  SR_VERT_NORMAL,
  SR_VERT_COLOUR,
  SR_VERT_UV
};

/// Internal: Set up the rasteriser for rendering
void _srCreateRasteriser(uint width, uint height);

/// Internal: Clean up memory allocated by the rasteriser
void _srDestroyRasteriser();

/// Sets a render state which will be used for future render operations
///
/// @param state The render state to change
/// @param value What value the render state should be set to
void srSetRenderState(uint state, uint value);

// Vertex layout
typedef struct
{
  srEnum type;
  srSize size;
} srVertexAttribute;

// Vertex array
typedef struct
{
  srVertexAttribute* inLayout;
  srVertexAttribute* outLayout;
  srSize inLayoutCount;
  srSize outLayoutCount;

  float* vertexData;
  srSize inVertexSize;
  srSize outVertexSize;
  srSize vertexCount;
} srVertexArray;

/// Create vertex array
///
/// @param out Vertex array object to initialise
/// @param inLayout Array of vertex attribute objects to specify the data layout
/// @param attribCount Number of vertex attribute objects
/// @param vertexData Vertex data
/// @param vertexCount Number of vertices
void srCreateVertexArray(
    srVertexArray* out,
    srVertexAttribute inLayout[],
    srSize inLayoutCount,
    srVertexAttribute outLayout[],
    srSize outLayoutCount,
    float* vertexData,
    srSize vertexCount);

/// Vertex Shader Function
/// The parameters are the input and output data respectively. The layout of
/// the data matches the vertex attribute layouts given in srCreateVertexArray
typedef void (*srVertexShaderFunc)(float*, float*);

/// Fragment Shader Function
/// The parameters are the vertex shader output and output colour respectively.
/// The layout of the vertex shader output matches the output vertex attribute
/// layout given in srCreateVertexArray. The output colour is an array of 4
/// floats representing RGBA.
typedef void (*srFragmentShaderFunc)(float*, float*);

/// Set shader
///
/// @param vs Vertex shader function
/// @param fs Fragment shader function
void srSetShader(srVertexShaderFunc vs, srFragmentShaderFunc fs);

/// Draw vertex array
///
/// @param type The type of primitive to draw
/// @param vao The vertex array object to draw
void srDrawVertexArray(srEnum type, srVertexArray* vao);

#endif /* SRRASTERISER_H */
