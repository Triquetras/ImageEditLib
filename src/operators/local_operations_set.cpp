#include "creator.h"
#include "move.h"
#include "scale.h"
#include "rotate.h"
#include "rect.h"
#include "ellipse.h"
#include "text.h"
#include "operation_set.h"
#include "types.h"
#include <map>
#include <vector>

namespace ImageArch {

extern std::map<int, std::vector<char const*>> OpNames;

BaseOperator* OperationCreateLine() {
    return new CreateLine();
}

BaseOperator* OperationCreateTexture() {
    return new CreateImage();
}

BaseOperator* OperationMove() {
    return new Move();
}

BaseOperator* OperationScale() {
    return new Scale();
}

BaseOperator* OperationRotate() {
    return new Rotate();
}

BaseOperator* OperationRect() {
    return new Rect();
}

BaseOperator* OperationEllipse() {
    return new Ellipse();
}

BaseOperator* OperationText() {
    return new Text();
}

OperationLoaderNameSet localOpSet;

OperationLoaderNameSet& GetLocalOperationSet() {

    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_LINE][0], &OperationCreateLine);
    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_LINE][1], &OperationCreateLine);

    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_TEXTURE][0], &OperationCreateTexture);
    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_TEXTURE][1], &OperationCreateTexture);

    localOpSet.emplace(OpNames[E_OPERATOR_MOVE][0], &OperationMove);
    localOpSet.emplace(OpNames[E_OPERATOR_MOVE][1], &OperationMove);

    localOpSet.emplace(OpNames[E_OPERATOR_SCALE][0], &OperationScale);
    localOpSet.emplace(OpNames[E_OPERATOR_SCALE][1], &OperationScale);

    localOpSet.emplace(OpNames[E_OPERATOR_ROTATE][0], &OperationRotate);
    localOpSet.emplace(OpNames[E_OPERATOR_ROTATE][1], &OperationRotate);

    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_RECT][0], &OperationRect);
    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_RECT][1], &OperationRect);

    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_ELLIPSE][0], &OperationEllipse);
    localOpSet.emplace(OpNames[E_OPERATOR_CREATE_ELLIPSE][1], &OperationEllipse);

    localOpSet.emplace(OpNames[E_OPERATOR_TEXT][0], &OperationText);
    localOpSet.emplace(OpNames[E_OPERATOR_TEXT][1], &OperationText);

    return localOpSet;
}

} // ns
