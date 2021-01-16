#include "draw_helper.h"
#include "types.h"
#include "layer.h"
#include <cassert>
#include <cmath>

namespace ImageArch {

const byte* Pixel(Layer* layer, int x, int y) {
    // assert(layer != nullptr);
    // int pos = y * layer->Size().x + x;
    //  return &layer->_data[pos * BPP];
    return nullptr;
}

void SetPixel(Layer* layer, int x, int y, byte const pixel[]) {
    // assert(layer != nullptr);
    // int pos = y * layer->Size().x + x;
    // for (int b = 0; b < BPP; ++b) {
    //     // layer->_data[pos * BPP + b] = pixel[b];
    // }
}

void DrawLine(Layer* layer, Vec2 const& pos1, Vec2 const& pos2, byte const pixel[]) {
    // assert(layer != nullptr);
    // float w = pos2.x - pos1.x;
    // float h = pos2.y - pos1.y;
    // float ratio = h/w;
    // for (int i = 0; i < std::abs(w); i++) {
    //     SetPixel(layer,
    //         w > 0? pos1.x + i : pos1.x - i,
    //         h > 0? pos1.y + i*ratio: pos1.y - i*ratio,
    //         pixel);
    // }
}

} // ns
