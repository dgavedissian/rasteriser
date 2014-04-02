// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_COMMON_H_
#define _SR_COMMON_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>
#include <math.h>

// RGB <-> HEX
#define RGBA(r, g, b, a) (((r) & 0xFF) << 24) + (((g) & 0xFF) << 16) + (((b) & 0xFF) << 8) + ((a) & 0xFF)
#define RGB(r, g, b) RGBA(r, g, b, 0xFF)
#define GET_R(colour) (((colour) >> 24) & 0xFF)
#define GET_G(colour) (((colour) >> 16) & 0xFF)
#define GET_B(colour) (((colour) >> 8) & 0xFF)
#define GET_A(colour) ((colour) & 0xFF)

// Sign function
#define SGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

// Min/Max
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

// Boolean values
#define SR_TRUE 1
#define SR_FALSE 0

#endif