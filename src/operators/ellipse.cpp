#include "ellipse.h"
#include "representer.h"
#include <layer.h>
#include <utils.h>

namespace ImageArch {

Ellipse::Ellipse(): BaseOperatorImpl(E_OPERATOR_CREATE_ELLIPSE)
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

bool Ellipse::GL_Show(Shader *shader, uint rectLayer)
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
            uint side = static_cast<uint>(abs(_pos2->x-_pos1->x));

            uint radius = static_cast<uint>(side/2);
            radius -= radius % 2 == 0 ? 1 : 2;

            std::cout << "Rect::GL_Show " << _pos1->x << " " << _pos1->y << " "  << side << " " << radius << std::endl;
            //Rect::GL_Show 182 128 50 23
            //              243 132 124 60

            ulong size2 = side * side;
            unsigned char *ellipseBitmap = new unsigned char[size2];
            memset(ellipseBitmap, 0, size2);

            int x = 0, y = radius, gap = 0, delta = (3 - 2 * radius);
            while (y >= 0)
            {
                int tx, ty;
                tx = side/2+x;
                ty = side/2+y;
                ellipseBitmap[static_cast<uint>(ty * side + tx)] = 1;
                ellipseBitmap[static_cast<uint>((ty-1) * side + (tx-1))] = 1;

                tx = side/2+x;
                ty = side/2-y;
                ellipseBitmap[static_cast<uint>(ty * side + tx)] = 1;
                ellipseBitmap[static_cast<uint>((ty+1) * side + (tx-1))] = 1;

                tx = side/2-x;
                ty = side/2+y;
                ellipseBitmap[static_cast<uint>(ty * side + tx)] = 1;
                ellipseBitmap[static_cast<uint>((ty-1) * side + (tx+1))] = 1;

                tx = side/2-x;
                ty = side/2-y;
                ellipseBitmap[static_cast<uint>(ty * side + tx)] = 1;
                ellipseBitmap[static_cast<uint>((ty+1) * side + (tx+1))] = 1;

                gap = 2 * (delta + y) - 1;
                if (delta < 0 && gap <= 0)
                {
                    x++;
                    delta += 2 * x + 1;
                    continue;
                }
                if (delta > 0 && gap > 0)
                {
                    y--;
                    delta -= 2 * y + 1;
                    continue;
                }
                x++;
                delta += 2 * (x - y);
                y--;
            }

            ulong size = side * side * 4;
            unsigned char *colorEllipseBitmap = new unsigned char[size];
            memset(colorEllipseBitmap, 0, size);

            bool fill = true;

            for (ulong y = 0; y < side; ++y)
            {
                for (ulong x  = 0; x < side; ++x)
                {
                    if ((uint)ellipseBitmap[static_cast<uint>(y * side + x)] == 1)
                    {
//                        memset(colorEllipseBitmap + 4 * (y * side + x), 0, 3);
                        setPixelRGBABitmap(colorEllipseBitmap, x, y, side, _color);
                        if ((uint)ellipseBitmap[static_cast<uint>(y * side + x+2)] == 1)
                        {
//                            memset(colorEllipseBitmap + 4 * (y * side + x+1), 0, 3);
                            setPixelRGBABitmap(colorEllipseBitmap, x+1, y, side, _color);
                        }
                        if (fill &
                            (ellipseBitmap[static_cast<uint>(y * side + side - x-1)] == 1) ||
                            (ellipseBitmap[static_cast<uint>(y * side + side - x-2)] == 1))
                        {
                            for(uint xf = x; xf < (side - x); xf++)
                            {
//                                memset(colorEllipseBitmap + 4 * (y * side + xf), 0, 3);
                                setPixelRGBABitmap(colorEllipseBitmap, xf, y, side, _color);
                            }
                        }
                    }
                }
            }

            delete[] ellipseBitmap;

            glBindTexture(GL_TEXTURE_2D, _tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, side, side, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorEllipseBitmap);
            glBindTexture(GL_TEXTURE_2D, 0);
            delete[] colorEllipseBitmap;

            _layer->SetPos(*_pos1);
            _layer->SetSize(side, side);
        }

        glBindVertexArray(rectLayer);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture ( GL_TEXTURE_2D, 0 );
    }
    return true;
}
} // ns
