#include "representer.h"
#include "layerset.h"
#include "param.h"
#include "base_operator.h"
#include <cmath>
#include <shader.h>
#include <creator.h>
#include <move.h>
#include <iostream>
#include <font.h>

namespace ImageArch {

GuiRepresenter::GuiRepresenter(BaseOperator* op) :
    _lbClicked(false), _lbMove(false), _moveRect(0), state(DISABLE), _op(op)
{
    CalculateCoordOffset();
    //    std::cout << "GuiRepresenter " << op->opName << " " << op->displayName << " " << op->type << " " << (void*)op << std::endl;
}

// Interactable
void GuiRepresenter::MouseMove(LayerSet* canvas, int x, int y)
{
    UNUSED(canvas);
    switch (state)
    {
    case INIT:
        pos1.x = x;
        pos1.y = y;
        if (_op != nullptr)
            _op->SetParameter(_coordsOffset, (uintptr_t)&pos1, false);
        state = MOVE;
        break;
    case MOVE:
        pos2.x = x;
        pos2.y = y;
        if (_op != nullptr)
            _op->SetParameter(_coordsOffset+1, (uintptr_t)&pos2, false);
        break;
    case DISABLE:
        break;
    }
}

void GuiRepresenter::Char(LayerSet *canvas, unsigned int codepoint)
{
    UNUSED(canvas);
    UNUSED(codepoint);
    if (_op->type != E_OPERATOR_TEXT)
        return;
    _buffer += static_cast<wchar_t>(codepoint);

    _op->SetParameter(E_PARAM_TEXT_DATA, (uintptr_t)&_buffer);
}

void GuiRepresenter::SpecKey(LayerSet *canvas, unsigned int codepoint)
{
    UNUSED(canvas);
    UNUSED(codepoint);
    if (_op->type != E_OPERATOR_TEXT)
        return;
    switch (codepoint)
    {
    case 0:
        if (_buffer.length() > 0)
        _buffer.pop_back();
        break;
    case 1:
        _buffer += '\n';
        break;
    case 2:
        if (_buffer.length() > 0)
        _buffer.pop_back();
        break;
    }
    _op->SetParameter(E_PARAM_TEXT_DATA, (uintptr_t)&_buffer);
}

void GuiRepresenter::PrimaryMouseButtonDown(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super)
{
    state = INIT;
    UNUSED(canvas);
    SpecKey(shift, ctrl, alt, super);

}

void GuiRepresenter::PrimaryMouseButtonUp(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super)
{
    state = DISABLE;
    UNUSED(canvas);
    SpecKey(shift, ctrl, alt, super);

}

void GuiRepresenter::SecondaryMouseButtonDown(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super)
{
    UNUSED(canvas);
    SpecKey(shift, ctrl, alt, super);
}

void GuiRepresenter::SecondaryMouseButtonUp(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super)
{
    UNUSED(canvas);
    SpecKey(shift, ctrl, alt, super);
}

bool GuiRepresenter::GL_Show(Shader *shader, uint rectLayer)
{
    UNUSED(shader);
    UNUSED(rectLayer);
    //    if (_active)
    //    {
    Layer *_layer = reinterpret_cast<Layer*>(_op->GetParameter(E_PARAM_LAYER)->payload.userData);

    if (_layer != nullptr)
    {
        if (_moveRect == 0)
        {
            glGenTextures(1, &(_moveRect));
            glBindTexture(GL_TEXTURE_2D, _moveRect);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        }

        if (size != _layer->Size())
        {
            size = _layer->Size();
            uint _width = static_cast<uint>(size.x);
            uint _height = static_cast<uint>(size.y);

            ulong size = _width * _height * 4;
            byte *moveRect = new byte[size];
            memset(moveRect, 0, size);

            for (ulong y = 0; y < _height; ++y)
            {
                for (ulong x  = 0; x < _width; ++x)
                {
                    if (x < 1 || y < 1 || y > _height - 2 || x > _width - 2)
                    {
                        auto p = 4 * (y * _width + x);

                        if (
                                (x < 1 || x > _width - 2 || x % 10 > 4) &&
                                (y < 1 || y > _height - 2 || y % 10 > 4)
                                )
                        {
                            memset(moveRect + p, 255, 3);
                        }

                        memset(moveRect + p + 3, 255, 1);
                    }
                }
            }

            glBindTexture(GL_TEXTURE_2D, _moveRect);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, moveRect);
            glBindTexture(GL_TEXTURE_2D, 0);
            delete [] moveRect;
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f * _layer->Pos().x, 1.0f * _layer->Pos().y, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * _layer->Size().x, 0.5f * _layer->Size().y, 0.0f)); // перемещаем точку начала поворота в центр прямоугольника
        model = glm::rotate(model, glm::radians(1.0f * _layer->Angle()), glm::vec3(0.0f, 0.0f, 1.0f)); // затем производим поворот
        model = glm::translate(model, glm::vec3(-0.5f * _layer->Size().x, -0.5f * _layer->Size().y, 0.0f));// возвращаем главную точку в верхний левый угол
        shader->setMat4("layer", glm::scale(model, glm::vec3(1.0f * _layer->Size().x, 1.0f * _layer->Size().y, 0.0f )));

        glBindVertexArray(rectLayer);
        glBindTexture(GL_TEXTURE_2D, _moveRect);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //    }

    return true;
}

void GuiRepresenter::CalculateCoordOffset()
{
    int size;
    auto p = _op->GetParameters(size);
    int idx = 0;
    for (; idx < size; ++idx) {
        if (p[idx].type == E_PARAM_REPRESENTER_COORD) break;
    }
    _coordsOffset = idx;
}

void GuiRepresenter::SpecKey(bool shift, bool ctrl, bool alt, bool super)
{
    _op->SetParameter(E_PARAM_SHIFT_PRESSED, (uintptr_t)shift);
    _op->SetParameter(E_PARAM_CTRL_PRESSED, (uintptr_t)ctrl);
    _op->SetParameter(E_PARAM_ALT_PRESSED, (uintptr_t)alt);
    _op->SetParameter(E_PARAM_SUPER_PRESSED, (uintptr_t)super);
}

} // ns
