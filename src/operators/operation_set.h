#pragma once
#include "common.h"
#include <map>
#include <string>

namespace ImageArch {

struct BaseOperator;
using OpLoader = BaseOperator*(*)();
using OperationLoaderNameSet = std::map<std::string, OpLoader>;
using OperationLoaderUuidSet = std::map<byte[16], OpLoader>;

class OperationSet {
public:
    OperationSet();
    ~OperationSet();

    bool RegisterOperation(char const* opName, OpLoader loader);
    //bool RegisterOperation(byte guid[16], OpLoader loader);

    inline bool RegisterOperations(OperationLoaderNameSet const& opset) {
        _opSetName.insert(opset.begin(), opset.end());
        return true;
    }

    BaseOperator* CreateOperation(char const* opName);
    //BaseOperation* CreateOperation(byte guid[16]);

private:
    OperationLoaderNameSet _opSetName;
    OperationLoaderUuidSet _opSetUuid;
};

} // ns