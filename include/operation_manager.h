#pragma once
#include "common.h"
#include <vector>

namespace ImageArch {

struct BaseOperator;
class OperationSet;
class OperationManager {
public:
    OperationManager();
    ~OperationManager();

    BaseOperator* GetOperation(char const* opName);
    // BaseOperator* GetOperation(byte guid[16]);

    void AddOperationSet(OperationSet* opearationSet);

private:
    std::vector<OperationSet*> _operationSet;
};

} // ns
