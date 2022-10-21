#include "Defines.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>

#if 0

#ifndef SVPNG_INC_
#define SVPNG_INC_

/*! \def SVPNG_LINKAGE
    \brief User customizable linkage for svpng() function.
    By default this macro is empty.
    User may define this macro as static for static linkage,
    and/or inline in C99/C++, etc.
*/
#ifndef SVPNG_LINKAGE
#define SVPNG_LINKAGE
#endif

/*! \def SVPNG_OUTPUT
    \brief User customizable output stream.
    By default, it uses C file descriptor and fputc() to output bytes.
    In C++, for example, user may use std::ostream or std::vector instead.
*/
#ifndef SVPNG_OUTPUT
#include <stdio.h>
#define SVPNG_OUTPUT FILE* fp
#endif

/*! \def SVPNG_PUT
    \brief Write a byte
*/
#ifndef SVPNG_PUT
#define SVPNG_PUT(u) fputc(u, fp)
#endif


/*!
    \brief Save a RGB/RGBA image in PNG format.
    \param SVPNG_OUTPUT Output stream (by default using file descriptor).
    \param w Width of the image. (<16383)
    \param h Height of the image.
    \param img Image pixel data in 24-bit RGB or 32-bit RGBA format.
    \param alpha Whether the image contains alpha channel.
*/
SVPNG_LINKAGE void svpng(SVPNG_OUTPUT, unsigned w, unsigned h, const unsigned char* img, int alpha) {
    static const unsigned t[] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        /* CRC32 Table */    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
    unsigned a = 1, b = 0, c, p = w * (alpha ? 4 : 3) + 1, x, y, i;   /* ADLER-a, ADLER-b, CRC, pitch */
#define SVPNG_U8A(ua, l) for (i = 0; i < l; i++) SVPNG_PUT((ua)[i]);
#define SVPNG_U32(u) do { SVPNG_PUT((u) >> 24); SVPNG_PUT(((u) >> 16) & 255); SVPNG_PUT(((u) >> 8) & 255); SVPNG_PUT((u) & 255); } while(0)
#define SVPNG_U8C(u) do { SVPNG_PUT(u); c ^= (u); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while(0)
#define SVPNG_U8AC(ua, l) for (i = 0; i < l; i++) SVPNG_U8C((ua)[i])
#define SVPNG_U16LC(u) do { SVPNG_U8C((u) & 255); SVPNG_U8C(((u) >> 8) & 255); } while(0)
#define SVPNG_U32C(u) do { SVPNG_U8C((u) >> 24); SVPNG_U8C(((u) >> 16) & 255); SVPNG_U8C(((u) >> 8) & 255); SVPNG_U8C((u) & 255); } while(0)
#define SVPNG_U8ADLER(u) do { SVPNG_U8C(u); a = (a + (u)) % 65521; b = (b + a) % 65521; } while(0)
#define SVPNG_BEGIN(s, l) do { SVPNG_U32(l); c = ~0U; SVPNG_U8AC(s, 4); } while(0)
#define SVPNG_END() SVPNG_U32(~c)
    SVPNG_U8A("\x89PNG\r\n\32\n", 8);           /* Magic */
    SVPNG_BEGIN("IHDR", 13);                    /* IHDR chunk { */
    SVPNG_U32C(w); SVPNG_U32C(h);               /*   Width & Height (8 bytes) */
    SVPNG_U8C(8); SVPNG_U8C(alpha ? 6 : 2);     /*   Depth=8, Color=True color with/without alpha (2 bytes) */
    SVPNG_U8AC("\0\0\0", 3);                    /*   Compression=Deflate, Filter=No, Interlace=No (3 bytes) */
    SVPNG_END();                                /* } */
    SVPNG_BEGIN("IDAT", 2 + h * (5 + p) + 4);   /* IDAT chunk { */
    SVPNG_U8AC("\x78\1", 2);                    /*   Deflate block begin (2 bytes) */
    for (y = 0; y < h; y++) {                   /*   Each horizontal line makes a block for simplicity */
        SVPNG_U8C(y == h - 1);                  /*   1 for the last block, 0 for others (1 byte) */
        SVPNG_U16LC(p); SVPNG_U16LC(~p);        /*   Size of block in little endian and its 1's complement (4 bytes) */
        SVPNG_U8ADLER(0);                       /*   No filter prefix (1 byte) */
        for (x = 0; x < p - 1; x++, img++)
            SVPNG_U8ADLER(*img);                /*   Image pixel data */
    }
    SVPNG_U32C((b << 16) | a);                  /*   Deflate block end with adler (4 bytes) */
    SVPNG_END();                                /* } */
    SVPNG_BEGIN("IEND", 0); SVPNG_END();        /* IEND chunk {} */
}

#endif /* SVPNG_INC_ */

void savePng(const char* name, uint32_t w, uint32_t h, uint8_t* data, bool haveAlpha)
{
    std::string str(name);
    str += ".png";
    FILE* fp = fopen(str.c_str(), "wb");
    if (haveAlpha)
        svpng(fp, w, h, data, 1);
    else
        svpng(fp, w, h, data, 0);
    fclose(fp);
}

std::vector<uint32_t> colorBuff;
void ccmain(const std::string& filename)
{
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file";
    }

    //colorBuff.push_back(-1);
    std::string line;
    while (getline(fin, line))
    {
        std::stringstream word(line);
        uint32_t num;
        while (word >> num)
        {
            colorBuff.push_back(num);
        }
    }
    fin.close();
}


std::vector<uint8_t> dataBuff;

void updatebuff(Canvas& canvas)
{
    std::string str("C:/Users/jinze.chen/Desktop/�½��ļ���/");
    static int f = 1;
    static int offset = 129;//31
    str += std::to_string(f * 2 + 1);
    std::cout << str  <<"   offset : "<< offset << std::endl;
    ccmain(str);

    canvas.maxFrame(376 / 2);
    uint32_t* data = &colorBuff[0];
//#pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < 88; i++)
    {
        for (int j = 0; j < 489 * 2; j++)
        {
            auto ptr = &canvas.renderBuff[(i * (canvas.width()) + j) * 4];
            int64_t color = 0xffffffff & *data;
            
            color >>= 8;
            ptr[1] = (color & 0xff) / 255.0f;
            color >>= 8;
            ptr[0] = (color & 0xff) / 255.0f;
            color >>= 8;
            ptr[2] = (color & 0xff) / 255.0f;
            ptr[3] = 1.0f;
            ++data;
        }
    }
    for (int i = 0; i < 88; i++)
    {
        for (int j = 0; j < 489 * 2; j++)
        {
            auto ptr = &canvas.renderBuff[(i * (canvas.width()) + j) * 4];
            dataBuff.push_back(ptr[0] * 255);
            dataBuff.push_back(ptr[1] * 255);
            dataBuff.push_back(ptr[2] * 255);
            dataBuff.push_back(ptr[3] * 255);
        }
    }
    savePng(str.c_str(), 489 * 2, 88, &dataBuff[0], true);
    dataBuff.clear();
    colorBuff.clear();
    ++f;
    ++offset;
}
#else
void updatebuff(Canvas& canvas)
{
    canvas.maxFrame(99999);
    static int f = 0;
#pragma omp parallel for schedule(dynamic, 8)
    for (int i = 0; i < canvas.height(); i++)
    {
        for (int j = 0; j < canvas.width(); j++)
        {
            auto ptr = &canvas.renderBuff[(i * canvas.width() + j) * 4];
            ptr[0] = 0.5f * i / (RayPrecision)canvas.width();
            ptr[1] = 0.5f * j / (RayPrecision)canvas.height();
            ptr[2] = 0.5f + 0.4f * sinf(0.001f * f);
        }
    }
    ++f;
}
#endif