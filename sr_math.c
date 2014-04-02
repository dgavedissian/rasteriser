// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr_common.h"
#include "sr_math.h"

vec3_t srVec3(float x, float y, float z)
{
	vec3_t out;
	out.x = x;
	out.y = y;
	out.z = z;
	return out;
}

float srDot(vec3_t *a, vec3_t *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

void srCross(vec3_t *a, vec3_t *b, vec3_t *out)
{
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->x * b->z - a->z * b->x;
	out->z = a->x * b->y - a->y * b->x;
}

vec4_t srVec4(float x, float y, float z, float w)
{
	vec4_t out;
	out.x = x;
	out.y = y;
	out.z = z;
	out.w = w;
	return out;
}