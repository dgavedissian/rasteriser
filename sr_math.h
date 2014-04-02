// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_MATH_H_
#define _SR_MATH_H_

// 3D vector
typedef struct
{
	float x, y, z;
} vec3_t;

vec3_t srVec3(float x, float y, float z);
float srDot(vec3_t *a, vec3_t *b);
void srCross(vec3_t *a, vec3_t *b, vec3_t *out);

typedef struct
{
	float x, y, z, w;
} vec4_t;

vec4_t srVec4(float x, float y, float z, float w);

typedef struct
{
	float m[4][4];
} mat4_t;

#endif