#pragma once
#include "base_operator_impl.h"
#include "types.h"
#include "common.h"

namespace ImageArch {

class Layer;
class Ellipse : public BaseOperatorImpl {
public:
    Ellipse();


private:
    Layer* _layer;
    Vec2 *_pos1, *_pos2;
    bool _shift, _ctrl, _alt, _super;
    bool _inited;
    uint _tex;
    u32 _color;

    // BaseOperator interface
public:
    bool GL_Show(Shader *shader, uint rectLayer);
};

} // ns
