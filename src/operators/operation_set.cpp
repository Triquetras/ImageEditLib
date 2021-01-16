#include "operation_set.h"
#include "common.h"
#include "utils.h"
#include <string>

namespace ImageArch {

OperationSet::OperationSet() {

}
OperationSet::~OperationSet() {

}

bool OperationSet::RegisterOperation(char const* opName, OpLoader loader) {
    _opSetName[opName] = loader;
    return true;
}

// bool OperationSet::RegisterOperation(byte guid[16], OpLoader loader) {
//     _opSetUuid[guid] = loader;
// }

BaseOperator* OperationSet::CreateOperation(char const* opName) {
    std::string s(opName);
    if (!map_contains(_opSetName, s)) return nullptr;
    OpLoader loader = _opSetName[s];
    BaseOperator* op = (*loader)();
    return op;
}

// BaseOperation* OperationSet::CreateOperation(byte guid[16]) {
//     if (!_opSetUuid.contains(guid)) return nullptr;
//     OpLoader loader = _opSetUuid[guid];
//     BaseOperation* op = loader();
//     return op;
// }

} // ns
