#include "scale.h"
#include "representer.h"
#include <layer.h>


namespace ImageArch {

Scale::Scale(): BaseOperatorImpl(E_OPERATOR_SCALE)
  , _layer(nullptr)
  , _pos1(nullptr)
  , _pos2(nullptr)
  , _inited(false)
{
    _params.push_back(Param{ E_PARAM_LAYER, false, "", { .userData = (uintptr_t)nullptr}});
    _params.push_back(Param{ E_PARAM_SHIFT_PRESSED, false, "", { .userData = (uintptr_t)false}});
    _params.push_back(Param{ E_PARAM_CTRL_PRESSED, false, "", { .userData = (uintptr_t)false}});
    _params.push_back(Param{ E_PARAM_ALT_PRESSED, false, "", { .userData = (uintptr_t)false}});
    _params.push_back(Param{ E_PARAM_SUPER_PRESSED, false, "", { .userData = (uintptr_t)false}});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});

    _repr = new GuiRepresenter(reinterpret_cast<BaseOperator*>(this));
}

bool Scale::GL_Show(Shader *shader, uint rectLayer)
{
    UNUSED(shader);
    UNUSED(rectLayer);
    _layer = reinterpret_cast<Layer*>(_params[0].payload.userData);
    if (_layer != nullptr)
    {
        if (!_inited)
        {
            _layerSize = _layer->Size();
            _tempSize = _layerSize;
            _inited = true;
        }

        _shift =static_cast<bool>(_params[1].payload.userData);
        _pos1 = reinterpret_cast<Vec2*>(_params[5].payload.userData);
        _pos2 = reinterpret_cast<Vec2*>(_params[6].payload.userData);
        if (_pos1 != nullptr && _pos2 != nullptr)
        {
            if (*_pos2 != _lastPos2)
            {
                if (*_pos1 == _lastPos1)
                {
                    _tempSize += (*(_pos2) - _lastPos2) - ((*_pos1) - _lastPos1);
                    if (_shift)
                    {
                        float diff = static_cast<float>(_layerSize.x) / static_cast<float>(_tempSize.x);

                        _tempSize.y = static_cast<int>(_layerSize.y / diff);

                    }
                }
                _lastPos1 = *_pos1;
                _lastPos2 = *_pos2;
            }
        }

        if (_tempSize.x < 1 && _tempSize.y < 1)
            _tempSize = Vec2(1, 1);
        _layer->SetSize(_tempSize);
    }
    return true;
}

} // ns
