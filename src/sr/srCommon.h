// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRCOMMON_H
#define SRCOMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../kazmath/kazmath.h"

// Common types
typedef unsigned int uint;

// Core headers
#include "srColour.h"

// Hex Colour Manipulation
#define SR_HEX_RGBA(r, g, b, a) \
  (((r) & 0xFF) << 24) + (((g) & 0xFF) << 16) \
  + (((b) & 0xFF) << 8) + ((a) & 0xFF)
#define SR_HEX_GETR(hex) (((hex) >> 24) & 0xFF)
#define SR_HEX_GETG(hex) (((hex) >> 16) & 0xFF)
#define SR_HEX_GETB(hex) (((hex) >> 8) & 0xFF)
#define SR_HEX_GETA(hex) ((hex) & 0xFF)

// Maths Functions
#define SR_LERP(a, b, x) ((a) + ((b) - (a)) * (x))

// Min/Max
#define SR_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SR_MAX(a, b) ((a) < (b) ? (b) : (a))

// Boolean values
#define SR_TRUE 1
#define SR_FALSE 0

#endif /* SRCOMMON_H */
