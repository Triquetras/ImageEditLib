#pragma once
#include "base_operator_impl.h"
#include "types.h"
#include "common.h"

namespace ImageArch {

class Layer;
class Move : public BaseOperatorImpl {
public:
    Move();

private:
    Layer* _layer;
    Vec2 *_pos1, *_pos2, _layerPos, _lastPos1, _lastPos2, _tempPos;
    bool _shift, _ctrl, _alt, _super;
    bool _inited;

    // BaseOperator interface
public:
    bool GL_Show(Shader *shader, uint rectLayer);
};

} // ns
