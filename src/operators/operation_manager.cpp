#include "operation_manager.h"
#include "operation_set.h"
#include "local_operations_set.h"

namespace ImageArch {

OperationManager::OperationManager() {

   auto opset = new OperationSet();
   opset->RegisterOperations(GetLocalOperationSet());
   _operationSet.push_back(opset);
}
OperationManager::~OperationManager() {
    for (auto p: _operationSet) {
        if (p != nullptr) {
            delete p;
            p = nullptr;
        }
    }
}

BaseOperator* OperationManager::GetOperation(char const* opName) {
    for (auto  p: _operationSet) {
        BaseOperator* op = p->CreateOperation(opName);
        if (op != nullptr) return op;
    }
    return nullptr;
}

// BaseOperation* OperationManager::GetOperation(byte guid[16]) {
//     for (auto  p: _operationSet) {
//         BaseOperation* op = p->CreateOperation(guid);
//         if (op != nullptr) return op;
//     }
//     return nullptr;
// }

void OperationManager::AddOperationSet(OperationSet* opearationSet) {
    _operationSet.push_back(opearationSet);
}

} // ns
