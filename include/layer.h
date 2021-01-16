#pragma once

#include "common.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>

namespace ImageArch {

struct BaseOperator;
class Shader;
class Layer {
public:
    Layer(uint w, uint h, uint z, Vec2 const& pos);
    Layer(Layer const& other);
    Layer& operator=(Layer&& other);
    virtual ~Layer();

    bool AddOperation(BaseOperator* op);

    bool GL_Show(Shader *shader, uint rectLayer);

    uint Id() const;
    std::string Name() const;
    void SetName(char const* name);
    void SetPos(Vec2 Pos);
    void SetSize(uint w, uint h);
    void SetSize(Vec2 size);
    void SetAngle(int angle);
    int const& Angle() const;
    Vec2 const& Size() const;
    Vec2 const& Pos() const;
    bool  Visible() const;
    bool  Order() const;
    void SetVisible(bool v);

    inline std::vector<BaseOperator*>& Operators()  { return _opChain; }

private:
    std::string _name;
//    uint _width, _height;
    Vec2 _size;
    uint _zorder;
    Vec2 _pos;
    uint _id;
    bool _visible;
    int _angle;

    std::vector<BaseOperator*> _opChain;

};

} //ns

using TLayer = ImageArch::Layer;
