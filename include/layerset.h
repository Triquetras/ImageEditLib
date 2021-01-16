#pragma once

#include "common.h"
#include "layer.h"
#include <string>
#include <memory>
#include <GL/glew.h>

namespace ImageArch {

class BaseRepresenter;
class Shader;
struct BaseOperator;
class LayerSet {
public:

    LayerSet(char const* clientShadersPath = "");
    LayerSet(int w, int h, char const* name, char const* clientShadersPath = "");
    ~LayerSet();

    Layer* AddLayer();
    void SelectLayer(uint idx);
    void SelectLayer(Layer* l);
    void RemoveCurrentLayer();
    void RemoveLayer(int idx);
    Layer* CurrentLayer();
    void MoveLayerUp(int idx);
    void MoveLayerDown(int idx);
    int CurrentLayerIndex();
    inline std::vector<Layer*> const& Layers() const { return _layers; }

    void SetRepresenter(BaseRepresenter* repr);
    void ReleaseRepresenter();

    //
    // Events handling
    //
    void OnMouseMove(int x, int y);
    void OnChar(unsigned int codepoint);
    void OnSpecKey(unsigned int codepoint);
    void OnPrimaryMouseButtonDown(bool shift, bool ctrl, bool alt, bool super);
    void OnPrimaryMouseButtonUp(bool shift, bool ctrl, bool alt, bool super);
    void OnSecondaryMouseButtonDown(bool shift, bool ctrl, bool alt, bool super);
    void OnSecondaryMouseButtonUp(bool shift, bool ctrl, bool alt, bool super);

    bool AddOperation(BaseOperator* op);
    BaseOperator* SelectOperation(char const* opName);
    inline Vec2 Size() { return Vec2(_width, _height); }

    inline char const* Name() const { return _name.c_str(); }
    inline void SetName(char const* name) { _name = name; }

    //
    // Draw in GPU
    //
    void GL_Show();

    //
    //Return texId
    //
    GLuint GetImage();

//    static uint layerRect;

private:
    bool Init(char const* clientShaderPath);

private:
    std::vector<Layer*> _layers;
    uint _width, _height, _count;
    std::string _name;
    std::vector<BaseOperator*> _operators;
    BaseRepresenter* _currentRepresenter;

    uint _selectedLayer;
    GLuint _tex, _moveRect;
    GLuint _framebuffer;
    unsigned int VBO, VAO, EBO;
    Shader *shader;
};


} // ns

using TLayerSet = ImageArch::LayerSet;
