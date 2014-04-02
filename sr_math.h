// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_MATH_H_
#define _SR_MATH_H_

// 3D vector
struct vec3_t
{
	float x, y, z;
};

vec3_t srVec3(float x, float y, float z);
float srDot(vec3_t *a, vec3_t *b);
void srCross(vec3_t *a, vec3_t *b, vec3_t *out);

struct vec4_t
{
	float x, y, z, w;
};

vec4_t srVec4(float x, float y, float z, float w);

struct mat4_t
{
	float m[4][4];
};

#endif