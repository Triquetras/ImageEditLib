#pragma once

#include "base_operator_impl.h"
#include "types.h"
#include "common.h"
#include "param.h"
// #include <GL/glew.h>


namespace ImageArch {

class Layer;
class BaseRepresenter;
class GuiRepresenter;

// struct DrawableOperator: public BaseOperator, public Drawable {
//     DrawableOperator(int type, BaseRepresenter* repr): BaseOperator(type), _repr(repr) {}
//     // Drawable
//     virtual bool GL_Show() {}

// protected:
//     BaseRepresenter* _repr;
// };

struct CreateLine : public BaseOperatorImpl {

    struct Request {
        u32 color;
        float thickness;
    };

    CreateLine();
    virtual ~CreateLine();

    virtual bool GL_Show(Shader *shader, uint rectLayer);

    Layer* _layer;
    Color _color;

    //
    // Coord are always be getting from representer
    //
    Vec2 _pos1, _pos2; 


    //
    // Parameters
    //
    u32 _colorParam;
    float _thickness;
};

struct CreateImage : public BaseOperatorImpl {

    struct Request {
        int w, h;
        byte* data;
    };

    CreateImage();
    virtual ~CreateImage(){}

    virtual bool GL_Show(Shader *shader, uint rectLayer);

    byte* _imageData;
    uint _imageSize;
    uint _width, _height;
    Vec2 _pos1, _pos2;
    Layer* _layer;
    uint _tex;
};

} //ns

using TCreateLine = ImageArch::CreateLine;
