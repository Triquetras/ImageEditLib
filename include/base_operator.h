#pragma once

#include "common.h"
#include <string>

namespace ImageArch {
    
class BaseRepresenter;
struct Param;
class Shader;
class Layer;
struct BaseOperator {
    int type;
    byte guid[16];
    std::string displayName;
    std::string opName;

    BaseOperator(int type_) :
    type(type_)
    {

    }
    virtual ~BaseOperator() {}

    virtual Param* GetParameters(int& out_size) { return nullptr; }
    virtual int SetParameters(Param* params, int len) { return 0; }
    virtual char** GetParametersDescription(int& out_size) { return nullptr; }
    virtual int SetParametersDescription(Param* param, char* description) { return 0; }
    virtual Param* GetParameter(int idx, bool byType = true) { return nullptr; }
    virtual int SetParameter(int idx, uintptr_t value, bool byType = true) { return 0; }
    virtual BaseRepresenter* GetRepresenter() const { return _repr; }
    virtual bool GL_Show(Shader *shader, uint rectLayer) { return false; }

protected:
    BaseRepresenter* _repr;
};

} // ns

using TI_BaseOperator = ImageArch::BaseOperator;

