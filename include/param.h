#pragma once

namespace ImageArch {

struct Param {
    int type;
    bool configurable;
    char description[32];
    union {
        int iVal;
        double dVal;
        char* strVal;
        // void* userData;
        uintptr_t userData;
    } payload;
};

} //ns

using TA_PARM = ImageArch::Param;