#pragma once

#include "common.h"
#include "types.h"
#include <functional>
#include <string>


namespace ImageArch {

class LayerSet;
struct BaseOperator;
class Shader;
struct CallbackData {
    // TODO:
};

class BaseRepresenter {
public:
    BaseRepresenter(){}
    virtual ~BaseRepresenter(){}

    // Interactable
    virtual void MouseMove(LayerSet* canvas, int x, int y) {}
    virtual void Char(LayerSet* canvas, unsigned int codepoint) {}
    virtual void SpecKey(LayerSet* canvas, unsigned int codepoint) {}
    virtual void PrimaryMouseButtonDown(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super) {}
    virtual void PrimaryMouseButtonUp(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super) {}
    virtual void SecondaryMouseButtonDown(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super) {}
    virtual void SecondaryMouseButtonUp(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super) {}

    std::function<void(CallbackData const& data)> pfnActionCallback;
};

class GuiRepresenter : public BaseRepresenter{
public:
    GuiRepresenter(BaseOperator* op);
    virtual ~GuiRepresenter(){}

    // Interactable
    virtual void MouseMove(LayerSet* canvas, int x, int y);
    virtual void Char(LayerSet* canvas, unsigned int codepoint);
    virtual void SpecKey(LayerSet* canvas, unsigned int codepoint);
    virtual void PrimaryMouseButtonDown(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super);
    virtual void PrimaryMouseButtonUp(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super);
    virtual void SecondaryMouseButtonDown(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super);
    virtual void SecondaryMouseButtonUp(LayerSet* canvas, bool shift, bool ctrl, bool alt, bool super);

    bool GL_Show(Shader *shader, uint rectLayer);

private:
    //
    // Get offset of coords for current operator;
    //
    void CalculateCoordOffset();
    void SpecKey(bool shift, bool ctrl, bool alt, bool super);


    enum {
        DISABLE = 0,
        INIT,
        MOVE
    } STATES;

private:
    bool _active;
    Vec2 pos1, pos2, size;
    bool _lbClicked, _lbMove;
    uint _moveRect;
    int _coordsOffset;
    uint state;
    BaseOperator* _op;
    std::wstring _buffer;
};

} // ns

using TA_BaseRepresenter = ImageArch::BaseRepresenter;
using TA_GuiRepresenter = ImageArch::GuiRepresenter;
