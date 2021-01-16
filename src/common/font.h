#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <wchar.h>
#include <GLFW/glfw3.h>
#include <common.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace ImageArch {
class Font {
public:
    /* инициализировать библиотеку freetype и сохранить указатель на текстуру. */
    Font (uint *textureID);

    /* здесь происходит создание текстуры. Вот параметры
     * 1. Путь к шрифту
     * 2. Текст в wstring
     * 3. Указатель на результирующую ширину
     * 4. Указатель на результирующую высоту
     * 5. Размер шрифта.
     * 6. Расстояние между шрифтами по горизонтали в пикселях.
     * 7. Расстояние между шрифтами по вертикали в пикселях.
     * 8. Размер пробела в пикселях.
     * 9. Компонент цвет красный.
     * 10. Компонент цвет зеленый.
     * 11. Компонент цвет синий.
     * */

    void getTexture (
            std::wstring *es,
            int &width,
            int &height,
            int fontSize,
            int align,
            int vert,
            int space,
            uint8_t r,
            uint8_t g,
            uint8_t b,
            uint8_t a );

    void setFont(const char *ttf_file);
private:
    uint *_textureID;
    FT_Library ft_library;
    FT_Face face;
};
}
