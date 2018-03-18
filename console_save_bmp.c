#include <stdio.h>
#include <stdint.h>
#include "jagcore.h"
#include "jaglib.h"

static void Write32Le(uint32_t value,FILE *f)
{
    uint8_t buffer[4];
    uint8_t *p=(uint8_t *)&value;
    buffer[0]=p[3];
    buffer[1]=p[2];
    buffer[2]=p[1];
    buffer[3]=p[0];
    fwrite(buffer,1,4,f);
}

static uint32_t Rgb16ToRgb32(uint16_t value)
{
    uint8_t r = value >> 11; // 5 Bits
    uint8_t b = (value >> 6) & 0x1f;
    uint8_t g = value & 0x3f;
    uint32_t result = (uint32_t)((r << 19) | (g << 10) | (b << 3)); // ggf. | 0x00070307
    return result;
}

static uint8_t _bmp_header[]={
    0x42,0x4d,0x36,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x04,0x00,0x00,0x28,0x00,
    0x00,0x00,0x40,0x01,0x00,0x00,0x38,0xff,0xff,0xff,0x01,0x00,0x08,0x00,0x00,0x00,
    0x00,0x00,0x00,0xfa,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00
};

void jag_console_save_bmp(const char *fname)
{
    FILE *f=fopen(fname,"w");
    if (f)
    {
        fwrite(_bmp_header,1,54,f);
        // Color-Table
        for (uint16_t counter=0;counter<256;++counter)
        {
            Write32Le(Rgb16ToRgb32(*(CLUT + counter)),f);
        }

        // Content
        fwrite(jag_vidmem,CONSOLE_BMP_WIDTH,CONSOLE_BMP_HEIGHT,f);

        fclose(f);
    }
}