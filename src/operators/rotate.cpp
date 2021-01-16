#include "rotate.h"
#include "representer.h"
#include <layer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ImageArch {

Rotate::Rotate(): BaseOperatorImpl(E_OPERATOR_ROTATE)
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

bool Rotate::GL_Show(Shader *shader, uint rectLayer)
{
    UNUSED(shader);
    UNUSED(rectLayer);
    _layer = reinterpret_cast<Layer*>(_params[0].payload.userData);
    if (_layer != nullptr)
    {
        if (!_inited)
        {
            _layerAngle = _layer->Angle();
            _tempAngle = _layerAngle;
            _inited = true;
            std::cout << "Scale Init" << std::endl;
            std::cout << "_tempAngle " << _tempAngle << std::endl;
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
                    Vec2 tempPos = (*(_pos2) - _lastPos2) - ((*_pos1) - _lastPos1);

                    double tempAngle = 0;
                    if (abs(tempPos.x) > abs(tempPos.y))
                    {
                        tempAngle = -1 * sin(glm::radians((float)tempPos.x));
                    }
                    else
                    {
                        tempAngle = sin(glm::radians((float)tempPos.y));
                    }
                    tempAngle = round(10 * tempAngle);

                    _tempAngle += ((tempAngle > 360 || tempAngle < -360) ? 0 : tempAngle);
                }
                _lastPos1 = *_pos1;
                _lastPos2 = *_pos2;
            }

        }

        _layer->SetAngle(_tempAngle);
    }
    return true;
}
} // ns
