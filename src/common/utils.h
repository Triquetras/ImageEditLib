#pragma once
#include "GL/glew.h"
#include <map>
#include <common.h>

namespace ImageArch {
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
bool LoadTextureFromMemory(unsigned char* buffer, GLuint* out_texture, int* out_width, int* out_height, int size = 0, int image_width = -1, int image_height = -1);
void setPixelRGBABitmap(unsigned char* image_data, uint x, uint y, uint aligh, u32 color);

template <typename K, typename V>
inline bool map_contains(std::map<K, V>& map, K& key) {
    if (auto s = map.find(key); s != map.end()) return true;
    return false;
}
} // ns
