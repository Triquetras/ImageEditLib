#include "base_operator_impl.h"
#include "types.h"
#include <map>
#include <vector>
#include <cstring>
#include <iostream>

namespace ImageArch {

extern std::map<int, std::vector<char const*>> OpNames;

BaseOperatorImpl::BaseOperatorImpl(int type) : BaseOperator(type) {
    InitSettings();
}

BaseOperatorImpl::~BaseOperatorImpl() 
{
    for (auto& param: _params) {
        switch (param.type)
        {
        case E_PARAM_REPRESENTER_COORD:
            delete (Vec2*)param.payload.userData;
            param.payload.userData = 0;
            break;
        
        default:
            break;
        }
    }
}

Param* BaseOperatorImpl::GetParameters(int& size) {
    size = _params.size();
    return _params.data();
}
int BaseOperatorImpl::SetParameters(Param* params, int len) {
    _params.assign(params, params + len);
    return 0;
}
char** BaseOperatorImpl::GetParametersDescription(int& size) {
    size = _params.size();
    return nullptr;
}
int BaseOperatorImpl::SetParametersDescription(Param* param, char* description) {
    return 0;
}

Param* BaseOperatorImpl::GetParameter(int idx, bool byType) {
    if (byType) {
        for (auto& p: _params) {
            if (p.type == idx) return &p;
        }
        return nullptr;
    } else {
        return &_params[idx];
    }
}
int BaseOperatorImpl::SetParameter(int idx, uintptr_t value, bool byType) {
    Param* p = nullptr;
    if (byType) {
        for (auto& pp: _params) {
            if (pp.type == idx) {
                p = &pp;
                break;
            }
        }
        if (p == nullptr) return 1;
    } else {
        p = &_params[idx];
    }

    // std::cout << "SetParam to type " << idx << ": " << value << std::endl;

    p->payload.userData = value;
    return 0;
}

void BaseOperatorImpl::InitSettings() {
    StringToGuid(OpNames[type][0], &_Guid.guid);
    memcpy(guid, _Guid.guidBytes, 16);
    opName = OpNames[type][1];
    displayName = OpNames[type][2];
}

} // ns
