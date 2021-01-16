#pragma once
#include "base_operator.h"
#include "param.h"
#include "guid.h"
#include <vector>
#include <shader.h>

namespace ImageArch {

class BaseOperatorImpl :public BaseOperator {
public:
    BaseOperatorImpl(int type);
    virtual ~BaseOperatorImpl();

    virtual Param* GetParameters(int& out_size);
    virtual int SetParameters(Param* params, int len);
    virtual char** GetParametersDescription(int& out_size);
    virtual int SetParametersDescription(Param* param, char* description);
    virtual Param* GetParameter(int idx, bool byType = true);
    virtual int SetParameter(int idx, uintptr_t value, bool byType = true);

protected:
    void InitSettings();

protected:
    std::vector<Param> _params;
    union {
        byte guidBytes[16];
        GUID guid;
    } _Guid;

};

} // ns
