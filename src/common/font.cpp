#include "font.h"

namespace ImageArch {

Font::Font (uint *textureID) :
    _textureID(textureID)
  , face(0)
{
    FT_Init_FreeType( &ft_library );
}

void Font::setFont(const char *ttf_file)
{
    if (face != 0)
    {
        FT_Done_Face(face);
    }
    FT_New_Face ( ft_library, ttf_file, 0, &face );
}

void Font::getTexture (std::wstring *es,
        int &width,
        int &height,
        int fontSize,
        int align,
        int vert,
        int space,
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a)
{
    FT_Set_Pixel_Sizes ( face, 0, fontSize );
    FT_Glyph glyph;

    int w = 0;
    unsigned int h = 0;
    unsigned int maxh = 0;
    unsigned int toprow = 0;
    /* эта функция возвращает сколько символов в широкой строке, если например в строке
         * будут три буквы iаф, то функция вернет три символа. */
    int len = es->length();

    if (es == nullptr || face == nullptr)
    {
        return;
    }

    /* первое что я придумал это посчитать какую текстуру вообще надо создать, но для этого
         * мне пришлось создать каждый символ и узнать его ширину. Так я вижу полную картину. Знаю
         * какой массив создать */
    for ( int i = 0; i < len; i++ ) {
        /* итак получаем символ */
        wchar_t charcode = es->at(i);
        /* далее идут стандартные операции для создания bitmap символа */
        FT_Load_Char ( face, charcode, FT_LOAD_RENDER );

        FT_UInt glyph_index = FT_Get_Char_Index ( face, charcode );
        FT_Load_Glyph ( face, glyph_index, FT_LOAD_DEFAULT );
        FT_Render_Glyph ( face->glyph, FT_RENDER_MODE_NORMAL );
        FT_Get_Glyph ( face->glyph, &glyph );

        FT_Glyph_To_Bitmap ( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
        FT_Bitmap bitmap = bitmap_glyph->bitmap;
        /* теперь надо узнать ширину символа */
        w += bitmap.width;

        /* узнать разницу высоты шрифта и отступа от верха. */
        int resize = bitmap.rows > bitmap_glyph->top ? bitmap.rows - bitmap_glyph->top : bitmap_glyph->top - bitmap.rows;
        /* теперь высота значиться как высота символа плюс отступ */
        if ( h < bitmap.rows + resize ) h = bitmap.rows + resize;
        /* здесь надо знать самую большую высоту символа */
        if ( toprow < bitmap.rows ) toprow = bitmap.rows;
        if ( maxh < bitmap.rows + bitmap_glyph->top ) maxh = bitmap.rows + bitmap_glyph->top;

        /* если символ равен пробелу, то увеличить w на столько пикселей, сколько задали при
                 * инициализации */
        if ( charcode == ' ' ) w += space;
        /* если встретился символ 'новая строка'
                 * то увеличить высоту включив туда вертикальный отступ и максимальную высоту */
        if ( charcode == '\n' ) {
            h += vert + maxh;
            FT_Done_Glyph ( glyph );
            continue;
        }
        /* это расстояние между шрифтом, если align равен одному пикселю, то увеличиться на один */
        w += align;

        FT_Done_Glyph ( glyph );
    }

    /* теперь можно создать подготовительный двухмерный массив,
         * он включает размер всего текста в пикселях */
    if ( h <= 0 ) h = maxh;
    uint8_t im[h][w];
    /* заполню нулями массив */
    memset ( &im[0][0], 0, w * h * sizeof ( uint8_t ) );

    int ih = 0;
    int iw = 0;
    int posy = 0;
    int topy = 0;
    int maxwidth = 0;
    for ( int i = 0; i < len; i++ )
    {
        wchar_t charcode = es->at(i);
        FT_Load_Char ( face, charcode, FT_LOAD_RENDER );
        FT_UInt glyph_index = FT_Get_Char_Index ( face, charcode );

        FT_Load_Glyph ( face, glyph_index, FT_LOAD_DEFAULT );
        FT_Render_Glyph ( face->glyph, FT_RENDER_MODE_NORMAL );
        FT_Get_Glyph ( face->glyph, &glyph );

        FT_Glyph_To_Bitmap ( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
        FT_Bitmap bitmap = bitmap_glyph->bitmap;

        /* получить отступ символа от верха */
        posy = bitmap_glyph->top;
        /* это математика наверное, немогу объяснить как я тут высчитал */
        posy = bitmap.rows - posy;
        topy = toprow - bitmap.rows;

        /* если новая строка, то ih - это высота от верха, то есть сверху это ноль,
                 * ниже увеличивается */
        if ( charcode == '\n' )
        {
            ih += maxh;
            iw = 0;
            FT_Done_Glyph ( glyph );
            continue;
        }
        for ( unsigned int y = 0, i = 0; y < bitmap.rows; y++ ) {
            for ( unsigned int x = 0; x < bitmap.width; x++, i++ ) {
                if ( ( ih + posy + y + topy ) > h ) {
                    if ( posy < 0 ) posy = abs ( posy );
                }

                /* здесь заполняется в нужное место один компонент цвета
                                 * пока массив из одного компонента gray, потом его перенесем в альфа канал */

                uint index = ih + posy + y + topy; //TODO: Костыль, но вроде работает, нужно тестировать
                if (index > 0) index -= 1;
                im [ index ] [ iw + x ] = bitmap.buffer[i];
            }
        }
        /* увеличиваем ширину */
        iw += bitmap.width;
        /* увеличиваем расстояние между символами */
        iw += align;
        if ( maxwidth < iw ) maxwidth = iw;

        if ( charcode == ' ' ) {
            iw += space;
        }

        FT_Done_Glyph ( glyph );

    }

    iw = maxwidth;
    width = iw;
    height = h;

    unsigned int size = width * height;
    /* а вот это уже будущая текстура */
    uint8_t *image_data = new uint8_t [ size * 4 ];
    /* заполняет белым цветом всю текстуру */
    memset ( image_data, 255, size * 4 * sizeof ( uint8_t ) );
    for ( unsigned int i = 0, y = 0; i < size; y++ ) {
        for ( int x = 0; x < width; x++, i++ ) {
            /* сюда помещаем из нашего массива значение в альфа канал */
            image_data[ 4 * i + 3] = im [ y ][ x ];
            /* сюда цвет текста */
            image_data[ 4 * i + 0] = r;
            image_data[ 4 * i + 1] = g;
            image_data[ 4 * i + 2] = b;
        }
    }

    /* стандартные действия для заполнения текстуры */

    glBindTexture ( GL_TEXTURE_2D, *_textureID );
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );
    glBindTexture(GL_TEXTURE_2D, 0);

    //    glGenTextures ( 1, &textureID );
    //    glBindTexture ( GL_TEXTURE_2D, _textureID );
    //    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    //    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    //    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    //    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );

    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    /* и удалить текстуру, она уже загружена в буфер и image_data больше не требуется. */
    delete[] image_data;
}

} // ns
