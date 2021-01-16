#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ImageArch {

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    bool ret =  LoadTextureFromMemory(image_data, out_texture, out_width, out_height, 0, image_width, image_height);
    stbi_image_free(image_data);

    return ret;
}

bool LoadTextureFromMemory(unsigned char* image_data, GLuint* out_texture, int* out_width, int* out_height, int size, int image_width, int image_height) 
{

    unsigned char* data;
    if (image_width == -1 || image_height == -1) {
        data = stbi_load_from_memory(image_data, size, &image_width, &image_height, NULL, 4);
        if (data == NULL)
            return false;
    } else {
        data = image_data;
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;   
}

void setPixelRGBABitmap(unsigned char* image_data, uint x, uint y, uint aligh, u32 color)
{
    auto p = 4 * (y * aligh + x);
    image_data[p + 0] = (color >> 0 & 0xFF);
    image_data[p + 1] = (color >> 8 & 0xFF);
    image_data[p + 2] = (color >> 16 & 0xFF);
    image_data[p + 3] = (color >> 24 & 0xFF);
}

}//ns
