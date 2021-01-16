#pragma once
#include "common.h"

namespace ImageArch {

class Layer;

static int BPP = 4; // rgba
const byte* Pixel(Layer* layer, int x, int y);
void SetPixel(Layer* layer, int x, int y, byte const pixel[]);
void DrawLine(Layer* layer, Vec2 const& pos1, Vec2 const& pos2, byte const pixel[]);


} // ns