#pragma once
#include "base_operator_impl.h"
#include "types.h"
#include "common.h"

namespace ImageArch {

class Layer;
class Rotate : public BaseOperatorImpl {
public:
    Rotate();

private:
    Layer* _layer;
    Vec2 *_pos1, *_pos2, _lastPos1, _lastPos2;
    int _layerAngle, _tempAngle;
    bool _shift, _ctrl, _alt, _super;
    bool _inited;
    uint _moveRect;

    // BaseOperator interface
public:
    bool GL_Show(Shader *shader, uint rectLayer);
};

} // ns
