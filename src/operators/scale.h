#pragma once
#include "base_operator_impl.h"
#include "types.h"
#include "common.h"

namespace ImageArch {

class Layer;
class Scale : public BaseOperatorImpl {
public:
    Scale();

private:
    Layer* _layer;
    Vec2 *_pos1, *_pos2, _layerSize, _lastPos1, _lastPos2, _tempSize;
    bool _inited;
    bool _shift, _ctrl, _alt, _super;

    // BaseOperator interface
public:
    bool GL_Show(Shader *shader, uint rectLayer);
};

} // ns
