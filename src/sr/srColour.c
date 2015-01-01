// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srColour.h"

srColour srRGBA(float r, float g, float b, float a)
{
  srColour out = {r, g, b, a};
  return out;
}

srColour srRGB(float r, float g, float b)
{
  return srRGBA(r, g, b, 1.0f);
}

srColour srColourFromHex(uint32_t hex)
{
  float scale = 1.0f / 255.0f;
  srColour out;
  out.r = (float)SR_HEX_GETR(hex) * scale;
  out.g = (float)SR_HEX_GETG(hex) * scale;
  out.b = (float)SR_HEX_GETB(hex) * scale;
  out.a = (float)SR_HEX_GETA(hex) * scale;
  return out;
}

uint32_t srColourToHex(srColour* colour)
{
  int r = (int)(colour->r * 255.0f);
  int g = (int)(colour->g * 255.0f);
  int b = (int)(colour->b * 255.0f);
  int a = (int)(colour->a * 255.0f);
  return SR_HEX_RGBA(r, g, b, a);
}

srColour* srColourMix(srColour* out, srColour* a, srColour* b, float x)
{
  out->r = SR_LERP(a->r, b->r, x);
  out->g = SR_LERP(a->g, b->g, x);
  out->b = SR_LERP(a->b, b->b, x);
  out->a = SR_LERP(a->a, b->a, x);
  return out;
}
