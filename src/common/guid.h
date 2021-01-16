#pragma once
#include <string>

namespace ImageArch {

struct GUID {
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t Data4[8];
};

void StringToGuid(std::string const& strGuid, GUID* guid);
std::string GuidToString(GUID* guid);

} // ns