#include "layerset.h"
#include "base_operator.h"
#include "representer.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>

namespace ImageArch {

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

// uint LayerSet::layerRect = 0;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

LayerSet::LayerSet(char const* clientShadersPath):
    _width(0),
    _height(0),
    _count(0),
    _name("new image"),
    _currentRepresenter(nullptr),
    _selectedLayer(0),
    shader(new Shader)
{
    _width = SCR_WIDTH; _height = SCR_HEIGHT;
    Init(clientShadersPath);
}

LayerSet::LayerSet(int w, int h, char const* name, char const* clientShadersPath):
    _width(w),
    _height(h),
    _count(0),
    _name(name),
    _currentRepresenter(nullptr),
    _selectedLayer(0),
    shader(new Shader)
{
    if (w == 0)
    {
        _width = SCR_WIDTH;
        _height = SCR_HEIGHT;
    }
    Init(clientShadersPath);
}


LayerSet::~LayerSet() {
    for (auto item: _layers) {
        if (nullptr != item) delete item;
        item = nullptr;
    }
    for (auto item: _operators) {
        if (nullptr != item) delete item;
        item = nullptr;
    }
}


bool LayerSet::Init(char const* clientShadersPath)
{
    float vertices[] =
    {
        // координаты         текстурные координаты
        1.f,  1.f, 0.0f,   1.0f, 1.0f, // верхняя правая вершина
        1.f,  0.f, 0.0f,   1.0f, 0.0f, // нижняя правая вершина
        0.f,  0.f, 0.0f,   0.0f, 0.0f, // нижняя левая вершина
        0.f,  1.f, 0.0f,   0.0f, 1.0f  // верхняя левая вершина
    };


    unsigned int indices[] =
    {
        0, 1, 3, // первый треугольник
        1, 2, 3  // второй треугольник
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // координатные атрибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // цветовые атрибуты
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (strlen(clientShadersPath) != 0)
        shader->init("shader.vs", "shader.fs", "", clientShadersPath);
    else
        shader->init();

    // конфигурация фреймбуфера
    // -------------------------
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // создание текстуры для прикрепляемого объекта цвета
    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);

    // создание объекта рендербуфера дла прикрепляемых объектов глубины и трафарета (сэмплирование мы не будет здесь проводить)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height); // использование одного объекта рендербуфера для буферов глубины и трафарета.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // теперь прикрепляем это дело
    // теперь, когда мы создали фреймбуфер и прикрепили все необходимые объекты, проверяет завершение формирование фреймбуфера
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //    std::cout << "_width " << _width << " _height " << _height << std::endl;

    //    byte pixel[4] = {255, 255, 255};
    //    ulong size = _width * _height * 4;
    //    byte *moveRect = new byte[size];
    //    memset(moveRect, 0, size);
    //    for (ulong y = 0; y < _height; ++y)
    //    {
    //        for (ulong x  = 0; x < _width; ++x)
    //        {
    //            auto p = (y * _width + x);
    //            for (ulong b = 0; b < 3; b++)
    //            {
    //                moveRect[p * 4 + b] += pixel[b];
    //            }
    //            moveRect[p * 4 + 3] += 0;
    //            if (x < 3 || y < 3 || y > _height - 4 || x > _width - 4)
    //            {
    //                moveRect[p * 4 + 3] += 255;
    //            }
    //        }
    //    }

    //    glGenTextures(1, &(_moveRect));
    //    glBindTexture(GL_TEXTURE_2D, _moveRect);
    //    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, moveRect);
    //    delete [] moveRect;


    std::cout << "_width " << _width << " _height " << _height << std::endl;
    return true;
}

//
// Draw in GPU
//
void LayerSet::GL_Show()
{
    if (!_layers.size())
        return;
    // рендеринг
    // ------
    // привязываем фреймбуфер и отрисовываем сцену в текстуру
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    //glEnable(GL_DEPTH_TEST); // включение режима теста глубины
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // убеждаемся, что очистили содержимое фреймбуфера
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();

    glViewport(0, 0, _width, _height);


    shader->setMat4("layerset", glm::ortho(0.0f, 1.f * _width, 0.0f, 1.0f * _height, 0.0f, 100.0f));
    glBindVertexArray(VAO);


    for (auto& layer: _layers)
    {
        layer->GL_Show(shader, VAO);
    }

    if (nullptr != _currentRepresenter)
        static_cast<GuiRepresenter*>(_currentRepresenter)->GL_Show(shader, VAO);

    // теперь снова привязывемся к фреймбуферу, заданному по умолчанию и
    //отрисовываем прямоугольник с прикрепленной цветовой текстурой фреймбуфера
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //    glDisable(GL_DEPTH_TEST); // отключаем режим теста глубины. Теперь экранный
    //    //прямоугольник не будет отсекаться в результате прохождения данного теста.
    //    // очищаем все сопутствующие буферы
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // устанавливаем цвет заливки на "белый"
    //(установите прозрачный цвет на белый (на самом деле это не обязательно, так как мы
    //все равно не сможем видеть пространство за прямоугольником)
    glClear(GL_COLOR_BUFFER_BIT);
    glCheckError();
}


Layer* LayerSet::AddLayer()
{
    _layers.push_back(new Layer(_width, _height, _count++, Vec2(0, 0)));
    return _layers.back();
}

bool LayerSet::AddOperation(BaseOperator* op) {
    _operators.push_back(op);
    CurrentLayer()->AddOperation(op);
    SetRepresenter(op->GetRepresenter());
    return true;
}

BaseOperator* LayerSet::SelectOperation(char const* opName) {
    if (nullptr == opName) return nullptr;
    auto pos = std::find_if(std::begin(_operators), std::end(_operators),[&opName](auto& v) {
        return v->opName == opName;
    });
    if (pos != std::end(_operators)) {
        return *pos;
    } else {
        return nullptr;
    }
}

void LayerSet::SelectLayer(uint idx) {
    _selectedLayer = idx;
//    std::cout << "SelectLayer() " << idx << std::endl;

}
void LayerSet::SelectLayer(Layer* l)
{
    if (nullptr == l)
        return;
    auto pos = std::find_if(std::begin(_layers), std::end(_layers),[&](auto& v)
    {
        return v->Id() == l->Id();
    });
    if (pos != std::end(_layers))
    {
        _selectedLayer = std::distance(_layers.begin(), pos);
        std::cout << "Selected layer " << _selectedLayer << std::endl;
    }
    else
    {
        assert(0 && "Attempt to select unavailable layer");
    }
}

void LayerSet::RemoveCurrentLayer()
{
    std::cout << "RemoveCurrentLayer: " << _selectedLayer << std::endl;
    std::remove_if(std::begin(_layers), std::end(_layers),[&](auto& v)
    {
        return _layers[_selectedLayer]->Id() == v->Id();
    });
}

void LayerSet::RemoveLayer(int idx)
{
    _layers.erase(_layers.begin() + idx);
}

Layer* LayerSet::CurrentLayer()
{
    if (_layers.size() == 0) return nullptr;
    // std::cout << "CurrentLayer: " << _selectedLayer << std::endl;
    return _layers[_selectedLayer];
}

void LayerSet::MoveLayerUp(int idx)
{
    std::cout << "MoveLayerUp: " << idx << std::endl;
    if (idx == 0) return;
    std::iter_swap(_layers.begin() + idx, _layers.begin() + idx - 1);
    SelectLayer(idx - 1);
}

void LayerSet::MoveLayerDown(int idx)
{
    std::cout << "MoveLayerDown: " << idx << std::endl;
    if ((unsigned)idx == _layers.size() - 1) return;
    std::iter_swap(_layers.begin() + idx, _layers.begin() + idx + 1);
    SelectLayer(idx + 1);
}

int LayerSet::CurrentLayerIndex()
{
    auto pos = std::find_if(std::begin(_layers), std::end(_layers),[&](auto v) {
        return v->Id() == CurrentLayer()->Id();
    });
    if (pos != std::end(_layers)) {
        //  std::cout << "CurrentLayerIndex(): " << std::distance(_layers.begin(), pos) << std::endl;
        return std::distance(_layers.begin(), pos);
    }
    else {
        return -1;
    }
}

void LayerSet::OnMouseMove(int x, int y)
{
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->MouseMove(this, x, y);
    }
}

void LayerSet::OnChar(unsigned int codepoint)
{
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->Char(this, codepoint);
    }
}

void LayerSet::OnSpecKey(unsigned int codepoint)
{
//    std::wstring str = L"Привет мир";
//    std::wcout << str << std::endl;
//    std::wcout << "OnSpecKey " << codepoint << std::endl;
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->SpecKey(this, codepoint);
    }
}

void LayerSet::SetRepresenter(BaseRepresenter *repr)
{
    _currentRepresenter = (BaseRepresenter*)repr;
}

void LayerSet::ReleaseRepresenter()
{
    _currentRepresenter = nullptr;
}

void LayerSet::OnPrimaryMouseButtonDown(bool shift, bool ctrl, bool alt, bool super)
{
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->PrimaryMouseButtonDown(this, shift, ctrl, alt, super);
    }
}
void LayerSet::OnPrimaryMouseButtonUp(bool shift, bool ctrl, bool alt, bool super) {
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->PrimaryMouseButtonUp(this, shift, ctrl, alt, super);
    }
}
void LayerSet::OnSecondaryMouseButtonDown(bool shift, bool ctrl, bool alt, bool super) {
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->SecondaryMouseButtonDown(this, shift, ctrl, alt, super);
    }
}
void LayerSet::OnSecondaryMouseButtonUp(bool shift, bool ctrl, bool alt, bool super) {
    if (nullptr != _currentRepresenter)
    {
        _currentRepresenter->SecondaryMouseButtonUp(this, shift, ctrl, alt, super);
    }
}
//
//Return texId
//
GLuint LayerSet::GetImage()
{
    GL_Show();
    return _tex;
}

} // ns
