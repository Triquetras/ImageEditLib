#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef DEBUG
#define DEBUG 0
#endif

namespace ImageArch {

typedef unsigned char byte;
typedef unsigned int  u32;
typedef unsigned int  uint;
typedef unsigned long ulong;

#define UNUSED(x) (void)x

// class Shader;

struct Vec2 {
    int x, y;
    Vec2(int x_, int y_) { x = x_; y = y_; }
    Vec2() { x = y = 0; }
};

//inline Vec2 operator*(const Vec2& lhs, const float rhs)              { return Vec2(lhs.x * rhs, lhs.y * rhs); }
//inline Vec2 operator/(const Vec2& lhs, const float rhs)              { return Vec2(lhs.x / rhs, lhs.y / rhs); }
inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs)            { return Vec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs)            { return Vec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline Vec2 operator*(const Vec2& lhs, const Vec2& rhs)            { return Vec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline Vec2 operator/(const Vec2& lhs, const Vec2& rhs)            { return Vec2(lhs.x / rhs.x, lhs.y / rhs.y); }
inline Vec2& operator*=(Vec2& lhs, const float rhs)                  { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
inline Vec2& operator/=(Vec2& lhs, const float rhs)                  { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
inline Vec2& operator+=(Vec2& lhs, const Vec2& rhs)                { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
inline Vec2& operator-=(Vec2& lhs, const Vec2& rhs)                { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
inline Vec2& operator*=(Vec2& lhs, const Vec2& rhs)                { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
inline Vec2& operator/=(Vec2& lhs, const Vec2& rhs)                { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
inline bool operator==(Vec2& lhs, const Vec2& rhs)                { return ((lhs.x == rhs.x) && (lhs.y == rhs.y)); }
inline bool operator!=(Vec2& lhs, const Vec2& rhs)                { return ((lhs.x != rhs.x) || (lhs.y != rhs.y)); }

union Color{
    u32 color;
    byte pixel[4];
};

} //ns
