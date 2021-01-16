#pragma once

namespace ImageArch {

enum OperatorsTypes {
    E_OPERATOR_CREATE_LINE,
    E_OPERATOR_CREATE_POLYLINE,
    E_OPERATOR_CREATE_BEZIER_CURVE,
    E_OPERATOR_CREATE_RECT,
    E_OPERATOR_CREATE_ELLIPSE,
    E_OPERATOR_CREATE_TEXTURE,

    E_OPERATOR_SELECT_AREA,
    E_OPERATOR_MOVE,
    E_OPERATOR_SCALE,
    E_OPERATOR_ROTATE,
    E_OPERATOR_TEXT,

    E_LAYOUT,
    E_CREATE_NONE
};

enum Parameters {
    E_PARAM_WIDTH,
    E_PARAM_HEIGHT,
    E_PARAM_COLOR,
    E_PARAM_THICKNESS,
    E_PARAM_LINESTYLE,
    E_PARAM_ROUNDNESS,
    E_PARAM_STYLE,
    E_PARAM_IMAGE,
    E_PARAM_IMAGE_SIZE,
    E_PARAM_TEXT_FONT,
    E_PARAM_TEXT_SIZE,
    E_PARAM_TEXT_DATA,
    E_PARAM_TEXT_STYLE,
    E_PARAM_REPRESENTER_COORD,
    E_PARAM_LAYER,
    E_PARAM_SHIFT_PRESSED,
    E_PARAM_CTRL_PRESSED,
    E_PARAM_ALT_PRESSED,
    E_PARAM_SUPER_PRESSED,
    E_PARAM_WIDTH2,
    E_PARAM_HEIGHT2,

    E_PARAM_NONE
};

//
// Op names
//
constexpr char OPERATOR_CREATE_LINE[] = "TCreateLine";
constexpr char OPERATOR_CREATE_TEXTURE[] = "TCreateTexture";
constexpr char OPERATOR_SELECT_AREA[] = "TSelectArea";
constexpr char OPERATOR_MOVE[] = "TMove";
constexpr char OPERATOR_SCALE[] = "TScale";
constexpr char OPERATOR_ROTATE[] = "TRotate";
constexpr char OPERATOR_RECT[] = "TRect";
constexpr char OPERATOR_ELLIPSE[] = "TEllipse";
constexpr char OPERATOR_TEXT[] = "TText";


//
// Display op names
//
constexpr char DISPLAY_OPERATOR_CREATE_LINE[] = "CreateLine";
constexpr char DISPLAY_OPERATOR_CREATE_TEXTURE[] = "CreateTexture";
constexpr char DISPLAY_OPERATOR_SELECT_AREA[] = "SelectArea";
constexpr char DISPLAY_OPERATOR_MOVE[] = "Move";
constexpr char DISPLAY_OPERATOR_SCALE[] = "Scale";
constexpr char DISPLAY_OPERATOR_ROTATE[] = "Rotate";
constexpr char DISPLAY_OPERATOR_RECT[] = "Rect";
constexpr char DISPLAY_OPERATOR_ELLIPSE[] = "Ellipse";
constexpr char DISPLAY_OPERATOR_TEXT[] = "Text";

} // ns
