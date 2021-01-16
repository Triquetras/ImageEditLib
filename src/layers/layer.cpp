#include "layer.h"
#include "base_operator.h"
#include "draw_helper.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <representer.h>
#include <shader.h>


namespace ImageArch {
static uint ID = 0;
Layer::Layer(uint w, uint h, uint z, Vec2 const& pos):
    _name("new layer"),
    _size(w, h),
    _zorder(z),
    _pos(pos),
    _visible(true),
    _angle(0)
{
    _name.resize(130);
//    std::cout << "_upleftPos x " << _upleftPos.x << " y " << _upleftPos.y << std::endl;

    // TODO: Generate ID like getHash(getNextUUID())
    _id = ID++; //reinterpret_cast<size_t>(this);
}

Layer::Layer(Layer const& other):
    _opChain(other._opChain)
{
    _size.x = other.Size().x; _size.y = other.Size().y;
    _zorder = other._zorder;
    _pos = other._pos;
    _id = other._id;
    _visible = other._visible;
    //    _size = other._size;
}

Layer::~Layer() {
    for (auto it: _opChain)
    {
        if (it != nullptr) delete it;
        it = nullptr;
    }
}

Layer& Layer::operator=(Layer&& other) {
    this->~Layer();
    return *new(this) Layer(std::move(other));
}

bool Layer::AddOperation(BaseOperator* op) {
    _opChain.push_back(op);
    return true;
}

// Drawable
bool Layer::GL_Show(Shader *shader, uint rectLayer)
{
    if (!_visible)
        return true;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f * _pos.x, 1.0f * _pos.y, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f)); // перемещаем точку начала поворота в центр прямоугольника
    model = glm::rotate(model, glm::radians(1.0f * _angle), glm::vec3(0.0f, 0.0f, 1.0f)); // затем производим поворот
    model = glm::translate(model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));// возвращаем главную точку в верхний левый угол
    shader->setMat4("layer", glm::scale(model, glm::vec3(1.0f * _size.x, 1.0f * _size.y, 0.0f )));

#if DEBUG
    static bool once = true;
    if (once) {
        std::cout << "GL_Show layer size " << _size.x << ", " << _size.y << std::endl;
        once = false;
    }
#endif

    //Перебираем операторы слоя
    for (auto& op: _opChain)
        //    for(size_t i = 0; i < _opChain.size(); i++)
    {
        op->GL_Show(shader, rectLayer);
    }
    return true;
}

uint Layer::Id() const
{
    return _id;
}

std::string Layer::Name() const
{
    return _name;
}

void Layer::SetName(const char *name)
{
    _name = name;
}

void Layer::SetPos(Vec2 Pos)
{
    _pos = Pos;
}

void Layer::SetSize(uint w, uint h)
{
    SetSize(Vec2(w, h));
}

void Layer::SetSize(Vec2 size)
{
    if (size.x > 1 && size.y > 1)
        _size = size;
    else
        _size = Vec2(1,1);
}

void Layer::SetAngle(int angle)
{
    _angle = angle;
}

const int &Layer::Angle() const
{
    return _angle;
}

const Vec2 &Layer::Size() const
{
    return _size;
}

const Vec2 &Layer::Pos() const
{
    return _pos;
}

bool Layer::Visible() const
{
    return _visible;
}

bool Layer::Order() const
{
    return _zorder;
}

void Layer::SetVisible(bool v)
{
    _visible = v;
}

} // ns
