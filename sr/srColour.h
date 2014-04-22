// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRCOLOUR_H_
#define _SRCOLOUR_H_

typedef struct
{
	float r, g, b, a;
} srColour;

/// Construct a colour object from red, green, blue and alpha components
///
/// @param r Red component in the range [0..1]
/// @param g Green component in the range [0..1]
/// @param b Blue component in the range [0..1]
/// @param a Alpha component in the range [0..1]
/// @return The colour object containing this colour components
srColour srRGBA(float r, float g, float b, float a);

/// Construct a colour object from red, green and blue components
///
/// @param r Red component in the range [0..1]
/// @param g Green component in the range [0..1]
/// @param b Blue component in the range [0..1]
/// @return The colour object containing this colour components
srColour srRGB(float r, float g, float b);

/// Construct a colour object from hex representation
///
/// @param hex The hexadecimal representation of a colour
/// @return The colour object describing this colour
srColour srColourFromHex(uint32_t hex);

/// Convert a colour to hex representation
///
/// @param The colour object to convert to hex
/// @return The hex representation of the colour object
uint32_t srColourToHex(srColour* colour);

/// Mix two colours together
///
/// @param out Colour to store the output of the mix operation
/// @param a First colour
/// @param b Second colour
/// @param x Interpolation factor
/// @return Simply returns the output colour pointer
srColour* srColourMix(srColour* out, srColour* a, srColour* b, float x);

#endif