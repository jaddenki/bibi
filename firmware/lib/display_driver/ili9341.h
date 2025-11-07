#ifndef ILI9341_H
#define ILI9341_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

// ILI9341 Commands
#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09
#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13
#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29
#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E
#define ILI9341_PTLAR      0x30
#define ILI9341_MADCTL     0x36
#define ILI9341_PIXFMT     0x3A
#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6
#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7
#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD
#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1

// Display dimensions
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320

// Display structure
typedef struct {
    spi_inst_t *spi;
    uint8_t cs_pin;
    uint8_t dc_pin;
    uint8_t rst_pin;
    uint16_t width;
    uint16_t height;
} ili9341_t;

// Function prototypes
void ili9341_init(ili9341_t *display, spi_inst_t *spi, uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sck);
void ili9341_write_command(ili9341_t *display, uint8_t cmd);
void ili9341_write_data(ili9341_t *display, uint8_t data);
void ili9341_write_data16(ili9341_t *display, uint16_t data);
void ili9341_set_address_window(ili9341_t *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ili9341_fill_screen(ili9341_t *display, uint16_t color);
void ili9341_draw_pixel(ili9341_t *display, uint16_t x, uint16_t y, uint16_t color);
void ili9341_draw_rect(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_fill_rect(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_draw_line(ili9341_t *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void ili9341_draw_circle(ili9341_t *display, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void ili9341_draw_bitmap(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap);

#endif // ILI9341_H

