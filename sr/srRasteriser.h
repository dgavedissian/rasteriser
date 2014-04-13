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
	uint32_t c;
} srVertex;

/// Internal: Set up the rasteriser for rendering
void _srCreateRasteriser();

/// Internal: Clean up memory allocated by the rasteriser
void _srDestroyRasteriser();

// =====================================
// Render State
// =====================================

// Render states
#define SR_WIREFRAME 0
#define SR_RENDER_STATE_COUNT 1

/// Sets a render state which will be used for future render operations
///
/// @param state The render state to change
/// @param value What value the render state should be set to
void srSetRenderState(unsigned int state, unsigned int value);

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
// Rendering
// =====================================

#define SR_POINT_LIST 0
#define SR_LINE_LIST 1
#define SR_LINE_STRIP 2
#define SR_TRIANGLE_LIST 3
#define SR_TRIANGLE_STRIP 4

// The interface for these functions is inspired by OpenGLs legacy immedate mode
// rendering functions which don't have much of a performance hit in a software
// rasteriser due to the lack of a GPU memory transfer bottleneck experienced by
// OpenGL applications

/// Allows the rasteriser to begin accepting vertices for a new primitive.
///
/// @param primitiveType The type of primitive to draw, for example:
/// SR_TRIANGLE_STRIP
void srBegin(unsigned int primitiveType);

/// Add a vertex to the current primitive.
///
/// @param x X position of this vertex
/// @param y Y position of this vertex
/// @param z Z position of this vertex
/// @param colour The colour of this vertex
void srAddVertex(float x, float y, float z, int colour);

/// Rasterises this primitive and clears all added vertices.
void srEnd();

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