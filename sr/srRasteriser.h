// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRRASTERISER_H_
#define _SRRASTERISER_H_

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
void _srCreateRasteriser();

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

// =====================================
// Vertex Structure
// =====================================

// Vertex primitive types
enum
{
  SR_VERT_POSITION  = 0x0001,
  SR_VERT_NORMAL    = 0x0002,
  SR_VERT_COLOUR    = 0x0004,
  SR_VERT_UV        = 0x0008
};

// Vertex declaration struct
typedef struct
{
    uint32_t type, offset;
} srVertexDecl;

/// Sets the current vertex layout
void srSetVertexLayout(srVertexDecl vertexDecl[]);

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

#endif
