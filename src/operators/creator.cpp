#include <creator.h>
#include <utils.h>
#include <draw_helper.h>
#include <layer.h>
#include <representer.h>
#include <iostream>
#include <layerset.h>

namespace ImageArch {

CreateLine::CreateLine(): BaseOperatorImpl(E_OPERATOR_CREATE_LINE) {
    _params.push_back(Param{ E_PARAM_COLOR, true,  "Color", { .iVal = static_cast<int>(_colorParam) }});
    _params.push_back(Param{ E_PARAM_THICKNESS, true,  "Thickness", { .dVal = static_cast<double>(_thickness) }});
    _params.push_back(Param{ E_PARAM_LAYER, false, "", { .userData = (uintptr_t)_layer}});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)&_pos1 }});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)&_pos2 }});

    opName = OPERATOR_CREATE_LINE;
    displayName = "Line";

    _repr = new GuiRepresenter((BaseOperator*)this);
}
CreateLine::~CreateLine() { if (nullptr != _repr) { delete _repr; _repr = nullptr; } }

bool CreateLine::GL_Show(Shader *shader, uint rectLayer) {
    Vec2 pos1 = *((Vec2*)(_params[3].payload.userData));
    Vec2 pos2 = *((Vec2*)(_params[4].payload.userData));
    Color color = { .color = *((u32*)_params[0].payload.userData) };
    _layer = (Layer*)GetParameter(E_PARAM_LAYER)->payload.userData;
    DrawLine(_layer, pos1, pos2, color.pixel);
    return true;
}


CreateImage::CreateImage() : BaseOperatorImpl(E_OPERATOR_CREATE_TEXTURE) {
    _params.push_back(Param{ E_PARAM_WIDTH, false, "", { .userData = (uintptr_t)0 }});
    _params.push_back(Param{ E_PARAM_HEIGHT, false, "", { .userData = (uintptr_t)0 }});
    _params.push_back(Param{ E_PARAM_IMAGE, false, "", { .userData = (uintptr_t)nullptr }});
    _params.push_back(Param{ E_PARAM_IMAGE_SIZE, false, "",  { .userData = (uintptr_t)0 }});
    _params.push_back(Param{ E_PARAM_LAYER, false, "", { .userData = (uintptr_t)nullptr}});
    _params.push_back(Param{ E_PARAM_WIDTH2, false, "", { .userData = (uintptr_t)0 }});
    _params.push_back(Param{ E_PARAM_HEIGHT2, false, "", { .userData = (uintptr_t)0 }});
    _params.push_back(Param{ E_PARAM_REPRESENTER_COORD, false, "", { .userData = (uintptr_t)nullptr }});

    opName = OPERATOR_CREATE_TEXTURE;
    displayName = "Image";

    _repr = new GuiRepresenter(reinterpret_cast<BaseOperator*>(this));

    _tex = 0;
}

bool CreateImage::GL_Show(Shader *shader, uint rectLayer)
{
    UNUSED(shader);
    if (_tex == 0)
    {
        _imageData = (byte*)GetParameter(E_PARAM_IMAGE)->payload.userData;
        _imageSize = (uint)GetParameter(E_PARAM_IMAGE_SIZE)->payload.userData;
        _width = (uint)GetParameter(E_PARAM_WIDTH)->payload.userData;
        _height = (uint)GetParameter(E_PARAM_HEIGHT)->payload.userData;
        _layer = (Layer*)GetParameter(E_PARAM_LAYER)->payload.userData;
        auto adjustedWidth = (uint)GetParameter(E_PARAM_WIDTH2)->payload.userData;
        auto adjustedHeight = (uint)GetParameter(E_PARAM_HEIGHT2)->payload.userData;

        if (adjustedWidth == 0) adjustedWidth = _width;
        if (adjustedHeight == 0) adjustedHeight = _height;


        std::cout << "CreateImage::GL_Show" << std::endl;
        std::cout << "width " << _width << std::endl;
        std::cout << "height " << _height << std::endl;
        std::cout << "image data ptr " << reinterpret_cast<void*>(_imageData) << std::endl;
        std::cout << "image size " << _imageSize << std::endl;
        std::cout << "layer ptr " << _layer << std::endl;

        glGenTextures(1, &(_tex));
        glBindTexture(GL_TEXTURE_2D, _tex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(_width), static_cast<int>(_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, _imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        _layer->SetSize(adjustedWidth, adjustedHeight);
    }

    glBindVertexArray(rectLayer);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindTexture ( GL_TEXTURE_2D, 0 );

    return true;
}

} // ns
