// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRRASTERISER_H
#define SRRASTERISER_H

// Struct used for storing vertex data
// TODO: this should be user defined and specified to the sr library using some
// sort of vertex declaration structure.
typedef struct
{
	kmVec3 p;
	kmVec3 n;
	srColour c;
} srVertex;

/// Internal: Set up the rasteriser for rendering
void _srCreateRasteriser(uint width, uint height);

/// Internal: Clean up memory allocated by the rasteriser
void _srDestroyRasteriser();

// =====================================
// Render State
// =====================================

// Render states
enum
{
  SR_WIREFRAME = 0,
  SR_LIGHTING,
  SR_RENDER_STATE_COUNT
};

/// Sets a render state which will be used for future render operations
///
/// @param state The render state to change
/// @param value What value the render state should be set to
void srSetRenderState(uint state, uint value);

/// Updates the model-view matrix to be used for future render operations
///
/// @param matrix The matrix to use
void srSetModelViewMatrix(kmMat4* matrix);

/// Returns the pointer to the current model-view matrix. This is useful to
/// avoid a redundant copy using srSetModelViewMatrix at the expense of some
/// readability, for example:
///
/// kmMat4Multiply(srGetModelViewMatrix(), &view, &model);
///
/// @return Pointer to the model-view matrix
kmMat4* srGetModelViewMatrix();

/// Updates the projection matrix to be used for future render operations
///
/// @param matrix The matrix to use
void srSetProjectionMatrix(kmMat4* matrix);

/// Returns the pointer to the current projection matrix.
///
/// @return Pointer to the current projection matrix
/// @see srGetModelViewMatrix
kmMat4* srGetProjectionMatrix();

///////////////////////////////////////////////////////////////////////////////
// Vertex Layout
///////////////////////////////////////////////////////////////////////////////

// Vertex primitive types
enum
{
  SR_VERT_POSITION,
  SR_VERT_NORMAL,
  SR_VERT_COLOUR,
  SR_VERT_UV
};

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

// =====================================
// Primitives
// =====================================

// Primitive types
enum
{
  SR_POINT_LIST = 0,
  SR_LINE_LIST,
  SR_LINE_STRIP,
  SR_TRIANGLE_LIST,
  SR_TRIANGLE_STRIP
};

// The interface for these functions is inspired by OpenGLs legacy immedate mode
// rendering functions which don't have much of a performance hit in a software
// rasteriser due to the lack of a GPU memory transfer bottleneck experienced by
// OpenGL applications

/// Allows the rasteriser to begin accepting vertices for a new primitive.
///
/// @param primitiveType The type of primitive to draw, for example:
/// SR_TRIANGLE_STRIP
void srBegin(uint primitiveType);

/// Add an unlighted vertex to the current primitive.
///
/// @param x X position of the vertex
/// @param y Y position of the vertex
/// @param z Z position of the vertex
/// @param colour The colour of the vertex
void srAddVertex(float x, float y, float z, srColour colour);

/// Add a lit vertex to the current primitive.
///
/// @param x X position of the vertex
/// @param y Y position of the vertex
/// @param z Z position of the vertex
/// @param nx X direction of the vertex normal
/// @param ny Y direction of the vertex normal
/// @param nz Z direction of the vertex normal
/// @param colour The colour of the vertex
//void srAddVertex(float x, float y, float z, float nx, float ny, float nz, srColour colour);

/// Rasterises this primitive and clears all added vertices.
void srEnd();

// =====================================
// Lighting
// =====================================

// Light Types
enum
{
  SR_LIGHT_NONE = 0,
  SR_LIGHT_DIRECTIONAL,
  SR_LIGHT_POINT,
  SR_LIGHT_SPOT
};

/// Enables a directional light
///
/// @param id The id of the light to enable
/// @param direction The direction of this light
void srEnableDirectionalLight(uint id, kmVec3* direction);

/// Disable a light
///
/// @param id The id of the light to disable
void srDisableLight(uint id);

// =====================================
// Drawing
// =====================================

/// Draws a line between two vertices. The vertices must be in device
/// coordinates which the context will accept.
///
/// @param a First vertex
/// @param b Second vertex
/// @see srDrawPixel
void srDrawLine(srVertex* a, srVertex* b);

/// Draws a triangle between three vertices. The vertices must be in device
/// coordinates which the context will accept.
///
/// @param a First vertex
/// @param b Second vertex
/// @param c Third vertex
/// @see srDrawPixel
void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c);

#endif /* SRRASTERISER_H */
