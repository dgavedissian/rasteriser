// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRCOMMON_H_
#define _SRCOMMON_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>

#include "../kazmath/kazmath.h"

// RGB <-> HEX
#define SR_RGBA(r, g, b, a) (((r) & 0xFF) << 24) + (((g) & 0xFF) << 16) + (((b) & 0xFF) << 8) + ((a) & 0xFF)
#define SR_RGB(r, g, b) SR_RGBA(r, g, b, 0xFF)
#define SR_GET_R(colour) (((colour) >> 24) & 0xFF)
#define SR_GET_G(colour) (((colour) >> 16) & 0xFF)
#define SR_GET_B(colour) (((colour) >> 8) & 0xFF)
#define SR_GET_A(colour) ((colour) & 0xFF)

// Sign function
#define SR_SGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

// Min/Max
#define SR_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SR_MAX(a, b) ((a) < (b) ? (b) : (a))

// Boolean values
#define SR_TRUE 1
#define SR_FALSE 0

#endif