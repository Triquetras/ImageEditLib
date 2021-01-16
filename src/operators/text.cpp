#include "text.h"
#include "representer.h"
#include <layer.h>

namespace ImageArch {

Text::Text(): BaseOperatorImpl(E_OPERATOR_TEXT)
  , _layer(nullptr)
  , _tex(0)
  , _text_font(nullptr)
  , _text_data(nullptr)
  , _inited(false)
  , _font(new Font(&_tex))
  ,  text_font(nullptr)
{
    _params.push_back(Param{ E_PARAM_COLOR, true, "Color", {.iVal = (int)0xffffffff}});
    _params.push_back(Param{ E_PARAM_TEXT_FONT, true, "Font", {.strVal = nullptr}});
    _params.push_back(Param{ E_PARAM_TEXT_SIZE, true, "Font Size", {.iVal = 0}});
    _params.push_back(Param{ E_PARAM_TEXT_STYLE, true, "Font Style", {.iVal = 0}});
    _params.push_back(Param{ E_PARAM_TEXT_DATA, false, "", {.userData = (uintptr_t)nullptr}});
    _params.push_back(Param{ E_PARAM_LAYER, false, "", { .userData = (uintptr_t)nullptr}});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});

    _repr = new GuiRepresenter(reinterpret_cast<BaseOperator*>(this));
}

bool Text::GL_Show(Shader *shader, uint rectLayer)
{
    UNUSED(shader);
    UNUSED(rectLayer);
    _layer = reinterpret_cast<Layer*>(_params[5].payload.userData);
    if (_layer != nullptr)
    {
        if (_tex == 0)
        {
            glGenTextures ( 1, &_tex );
            glBindTexture ( GL_TEXTURE_2D, _tex );
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
            glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
            //glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glBindTexture ( GL_TEXTURE_2D, 0 );

        }

        if (!_inited || _text_data == nullptr)
        {
            _text_data = reinterpret_cast<std::wstring*>(_params[4].payload.userData);
            _inited = true;
        }

        _color = static_cast<u32>(_params[0].payload.iVal);
        _text_font = reinterpret_cast<char*>(_params[1].payload.strVal);
        _text_size = static_cast<int>(_params[2].payload.iVal);
        _text_syle = static_cast<int>(_params[3].payload.iVal);

        if (_text_data == nullptr || _text_font == nullptr)
        {
            return false;
        }

        if (text_font == nullptr || strcmp(_text_font, text_font) != 0)
        {
            _font->setFont(_text_font);
            text_font = _text_font;
        }

        int width, height;
        _font->getTexture (_text_data, width, height,
                           (_text_size > 130 ? 130 : _text_size), 2, 2, 2,
                           (_color >> 0 & 0xFF), (_color >> 8 & 0xFF), (_color >> 16 & 0xFF), (_color >> 24 & 0xFF));
        _layer->SetSize(Vec2(width, height));

        glBindVertexArray(rectLayer);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture ( GL_TEXTURE_2D, 0 );
    }
    return true;
}

} // ns
