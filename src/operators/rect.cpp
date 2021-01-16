#include "rect.h"
#include "representer.h"
#include <layer.h>

namespace ImageArch {

Rect::Rect(): BaseOperatorImpl(E_OPERATOR_CREATE_RECT)
  , _layer(nullptr)
  , _pos1(nullptr)
  , _pos2(nullptr)
  , _inited(false)
  , _tex(0)
{
    _params.push_back(Param{ E_PARAM_COLOR, true, "Color", {.iVal = (int)0xffffffff}});
    _params.push_back(Param{ E_PARAM_THICKNESS, true, "Thickness", {.dVal = 1.f}});
    _params.push_back(Param{ E_PARAM_LINESTYLE, true, "Line Style", {.iVal = 0}});
    _params.push_back(Param{ E_PARAM_ROUNDNESS, true, "Roundness", {.iVal = 0}});
    _params.push_back(Param{ E_PARAM_LAYER, false, "", { .userData = (uintptr_t)nullptr}});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});

    _repr = new GuiRepresenter(reinterpret_cast<BaseOperator*>(this));
}

bool Rect::GL_Show(Shader *shader, uint rectLayer)
{
    UNUSED(shader);
    UNUSED(rectLayer);
    _layer = reinterpret_cast<Layer*>(_params[4].payload.userData);
    if (_layer != nullptr)
    {
        if (!_inited)
        {
            _pos1 = reinterpret_cast<Vec2*>(_params[5].payload.userData);
            _pos2 = reinterpret_cast<Vec2*>(_params[6].payload.userData);
            _inited = true;
        }

        if (_tex == 0)
        {
            glGenTextures(1, &(_tex));
            glBindTexture(GL_TEXTURE_2D, _tex);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        }
        _color = static_cast<u32>(_params[0].payload.iVal);
        _pos1 = reinterpret_cast<Vec2*>(_params[5].payload.userData);
        _pos2 = reinterpret_cast<Vec2*>(_params[6].payload.userData);

        if (_pos1 != nullptr && _pos2 != nullptr)
        {
            std::cout << "Rect::GL_Show " << _pos1->x << " " << _pos1->y << " "  << _pos2->x << " "  << _pos2->y << std::endl;

            uint _width = abs(_pos2->x-_pos1->x);
            uint _height = abs(_pos2->y-_pos1->y);

            ulong size = _width * _height * 4;
            byte *moveRect = new byte[size];
            memset(moveRect, 0, size);

            for (ulong y = 0; y < _height; ++y)
            {
                for (ulong x  = 0; x < _width; ++x)
                {
                    auto p = 4 * (y * _width + x);
                    moveRect[p + 0] = (_color >> 0 & 0xFF);
                    moveRect[p + 1] = (_color >> 8 & 0xFF);
                    moveRect[p + 2] = (_color >> 16 & 0xFF);
                    moveRect[p + 3] = (_color >> 24 & 0xFF);
                }
            }

            _layer->SetPos(*_pos1);
            _layer->SetSize(_width, _height);

            glBindTexture(GL_TEXTURE_2D, _tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, moveRect);
            glBindTexture(GL_TEXTURE_2D, 0);
            delete [] moveRect;
        }

        glBindVertexArray(rectLayer);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture ( GL_TEXTURE_2D, 0 );
    }
    return true;
}
} // ns
