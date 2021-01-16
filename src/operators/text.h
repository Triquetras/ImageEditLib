#pragma once
#include "base_operator_impl.h"
#include "types.h"
#include "common.h"
#include <font.h>

namespace ImageArch {

class Layer;
class Text : public BaseOperatorImpl {
public:
    Text();
    bool GL_Show(Shader *shader, uint rectLayer);


private:
    Layer* _layer;
    uint _tex;
    u32 _color;
    char* _text_font;
    std::wstring *_text_data;
    int _text_size, _text_syle;
    bool _inited;
    Font *_font;
    char* text_font;
};

} // ns
