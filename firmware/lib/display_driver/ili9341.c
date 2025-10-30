#include "ili9341.h"
#include <stdlib.h>

// Helper function to select the display
static inline void cs_select(ili9341_t *display) {
    gpio_put(display->cs_pin, 0);
}

// Helper function to deselect the display
static inline void cs_deselect(ili9341_t *display) {
    gpio_put(display->cs_pin, 1);
}

// Write a command to the display
void ili9341_write_command(ili9341_t *display, uint8_t cmd) {
    gpio_put(display->dc_pin, 0); // Command mode
    cs_select(display);
    spi_write_blocking(display->spi, &cmd, 1);
    cs_deselect(display);
}

// Write data to the display
void ili9341_write_data(ili9341_t *display, uint8_t data) {
    gpio_put(display->dc_pin, 1); // Data mode
    cs_select(display);
    spi_write_blocking(display->spi, &data, 1);
    cs_deselect(display);
}

// Write 16-bit data to the display
void ili9341_write_data16(ili9341_t *display, uint16_t data) {
    uint8_t buf[2] = {data >> 8, data & 0xFF};
    gpio_put(display->dc_pin, 1); // Data mode
    cs_select(display);
    spi_write_blocking(display->spi, buf, 2);
    cs_deselect(display);
}

// Reset the display
static void ili9341_reset(ili9341_t *display) {
    gpio_put(display->rst_pin, 1);
    sleep_ms(5);
    gpio_put(display->rst_pin, 0);
    sleep_ms(20);
    gpio_put(display->rst_pin, 1);
    sleep_ms(150);
}

// Set the address window for drawing
void ili9341_set_address_window(ili9341_t *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ili9341_write_command(display, ILI9341_CASET);
    ili9341_write_data(display, x0 >> 8);
    ili9341_write_data(display, x0 & 0xFF);
    ili9341_write_data(display, x1 >> 8);
    ili9341_write_data(display, x1 & 0xFF);

    ili9341_write_command(display, ILI9341_PASET);
    ili9341_write_data(display, y0 >> 8);
    ili9341_write_data(display, y0 & 0xFF);
    ili9341_write_data(display, y1 >> 8);
    ili9341_write_data(display, y1 & 0xFF);

    ili9341_write_command(display, ILI9341_RAMWR);
}

// Initialize the display
void ili9341_init(ili9341_t *display, spi_inst_t *spi, uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sck) {
    display->spi = spi;
    display->cs_pin = cs;
    display->dc_pin = dc;
    display->rst_pin = rst;
    display->width = ILI9341_WIDTH;
    display->height = ILI9341_HEIGHT;

    // Initialize SPI
    spi_init(spi, 20000000); // 20 MHz
    spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    // Set up GPIO pins
    gpio_set_function(mosi, GPIO_FUNC_SPI);
    gpio_set_function(sck, GPIO_FUNC_SPI);
    
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);

    gpio_init(dc);
    gpio_set_dir(dc, GPIO_OUT);

    gpio_init(rst);
    gpio_set_dir(rst, GPIO_OUT);

    // Reset display
    ili9341_reset(display);

    // Initialization sequence
    ili9341_write_command(display, 0xEF);
    ili9341_write_data(display, 0x03);
    ili9341_write_data(display, 0x80);
    ili9341_write_data(display, 0x02);

    ili9341_write_command(display, 0xCF);
    ili9341_write_data(display, 0x00);
    ili9341_write_data(display, 0xC1);
    ili9341_write_data(display, 0x30);

    ili9341_write_command(display, 0xED);
    ili9341_write_data(display, 0x64);
    ili9341_write_data(display, 0x03);
    ili9341_write_data(display, 0x12);
    ili9341_write_data(display, 0x81);

    ili9341_write_command(display, 0xE8);
    ili9341_write_data(display, 0x85);
    ili9341_write_data(display, 0x00);
    ili9341_write_data(display, 0x78);

    ili9341_write_command(display, 0xCB);
    ili9341_write_data(display, 0x39);
    ili9341_write_data(display, 0x2C);
    ili9341_write_data(display, 0x00);
    ili9341_write_data(display, 0x34);
    ili9341_write_data(display, 0x02);

    ili9341_write_command(display, 0xF7);
    ili9341_write_data(display, 0x20);

    ili9341_write_command(display, 0xEA);
    ili9341_write_data(display, 0x00);
    ili9341_write_data(display, 0x00);

    ili9341_write_command(display, ILI9341_PWCTR1);
    ili9341_write_data(display, 0x23);

    ili9341_write_command(display, ILI9341_PWCTR2);
    ili9341_write_data(display, 0x10);

    ili9341_write_command(display, ILI9341_VMCTR1);
    ili9341_write_data(display, 0x3e);
    ili9341_write_data(display, 0x28);

    ili9341_write_command(display, ILI9341_VMCTR2);
    ili9341_write_data(display, 0x86);

    ili9341_write_command(display, ILI9341_MADCTL);
    ili9341_write_data(display, 0x48);

    ili9341_write_command(display, ILI9341_PIXFMT);
    ili9341_write_data(display, 0x55);

    ili9341_write_command(display, ILI9341_FRMCTR1);
    ili9341_write_data(display, 0x00);
    ili9341_write_data(display, 0x18);

    ili9341_write_command(display, ILI9341_DFUNCTR);
    ili9341_write_data(display, 0x08);
    ili9341_write_data(display, 0x82);
    ili9341_write_data(display, 0x27);

    ili9341_write_command(display, 0xF2);
    ili9341_write_data(display, 0x00);

    ili9341_write_command(display, ILI9341_GAMMASET);
    ili9341_write_data(display, 0x01);

    ili9341_write_command(display, ILI9341_GMCTRP1);
    ili9341_write_data(display, 0x0F);
    ili9341_write_data(display, 0x31);
    ili9341_write_data(display, 0x2B);
    ili9341_write_data(display, 0x0C);
    ili9341_write_data(display, 0x0E);
    ili9341_write_data(display, 0x08);
    ili9341_write_data(display, 0x4E);
    ili9341_write_data(display, 0xF1);
    ili9341_write_data(display, 0x37);
    ili9341_write_data(display, 0x07);
    ili9341_write_data(display, 0x10);
    ili9341_write_data(display, 0x03);
    ili9341_write_data(display, 0x0E);
    ili9341_write_data(display, 0x09);
    ili9341_write_data(display, 0x00);

    ili9341_write_command(display, ILI9341_GMCTRN1);
    ili9341_write_data(display, 0x00);
    ili9341_write_data(display, 0x0E);
    ili9341_write_data(display, 0x14);
    ili9341_write_data(display, 0x03);
    ili9341_write_data(display, 0x11);
    ili9341_write_data(display, 0x07);
    ili9341_write_data(display, 0x31);
    ili9341_write_data(display, 0xC1);
    ili9341_write_data(display, 0x48);
    ili9341_write_data(display, 0x08);
    ili9341_write_data(display, 0x0F);
    ili9341_write_data(display, 0x0C);
    ili9341_write_data(display, 0x31);
    ili9341_write_data(display, 0x36);
    ili9341_write_data(display, 0x0F);

    ili9341_write_command(display, ILI9341_SLPOUT);
    sleep_ms(120);

    ili9341_write_command(display, ILI9341_DISPON);
    sleep_ms(20);
}

// Fill the entire screen with a color
void ili9341_fill_screen(ili9341_t *display, uint16_t color) {
    ili9341_fill_rect(display, 0, 0, display->width, display->height, color);
}

// Draw a single pixel
void ili9341_draw_pixel(ili9341_t *display, uint16_t x, uint16_t y, uint16_t color) {
    if (x >= display->width || y >= display->height) return;
    
    ili9341_set_address_window(display, x, y, x, y);
    ili9341_write_data16(display, color);
}

// Fill a rectangle
void ili9341_fill_rect(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= display->width || y >= display->height) return;
    if (x + w > display->width) w = display->width - x;
    if (y + h > display->height) h = display->height - y;

    ili9341_set_address_window(display, x, y, x + w - 1, y + h - 1);

    uint8_t color_hi = color >> 8;
    uint8_t color_lo = color & 0xFF;
    uint8_t buf[2] = {color_hi, color_lo};

    gpio_put(display->dc_pin, 1); // Data mode
    cs_select(display);
    
    for (uint32_t i = 0; i < w * h; i++) {
        spi_write_blocking(display->spi, buf, 2);
    }
    
    cs_deselect(display);
}

// Draw a rectangle outline
void ili9341_draw_rect(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ili9341_draw_line(display, x, y, x + w - 1, y, color);
    ili9341_draw_line(display, x + w - 1, y, x + w - 1, y + h - 1, color);
    ili9341_draw_line(display, x + w - 1, y + h - 1, x, y + h - 1, color);
    ili9341_draw_line(display, x, y + h - 1, x, y, color);
}

// Draw a line
void ili9341_draw_line(ili9341_t *display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        uint16_t temp = x0; x0 = y0; y0 = temp;
        temp = x1; x1 = y1; y1 = temp;
    }

    if (x0 > x1) {
        uint16_t temp = x0; x0 = x1; x1 = temp;
        temp = y0; y0 = y1; y1 = temp;
    }

    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) {
            ili9341_draw_pixel(display, y0, x0, color);
        } else {
            ili9341_draw_pixel(display, x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

// Draw a circle
void ili9341_draw_circle(ili9341_t *display, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ili9341_draw_pixel(display, x0, y0 + r, color);
    ili9341_draw_pixel(display, x0, y0 - r, color);
    ili9341_draw_pixel(display, x0 + r, y0, color);
    ili9341_draw_pixel(display, x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ili9341_draw_pixel(display, x0 + x, y0 + y, color);
        ili9341_draw_pixel(display, x0 - x, y0 + y, color);
        ili9341_draw_pixel(display, x0 + x, y0 - y, color);
        ili9341_draw_pixel(display, x0 - x, y0 - y, color);
        ili9341_draw_pixel(display, x0 + y, y0 + x, color);
        ili9341_draw_pixel(display, x0 - y, y0 + x, color);
        ili9341_draw_pixel(display, x0 + y, y0 - x, color);
        ili9341_draw_pixel(display, x0 - y, y0 - x, color);
    }
}

// Draw a bitmap (RGB565 format)
void ili9341_draw_bitmap(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap) {
    if (x >= display->width || y >= display->height) return;
    if (x + w > display->width) w = display->width - x;
    if (y + h > display->height) h = display->height - y;

    ili9341_set_address_window(display, x, y, x + w - 1, y + h - 1);

    gpio_put(display->dc_pin, 1); // Data mode
    cs_select(display);
    
    for (uint32_t i = 0; i < w * h; i++) {
        uint16_t color = bitmap[i];
        uint8_t buf[2] = {color >> 8, color & 0xFF};
        spi_write_blocking(display->spi, buf, 2);
    }
    
    cs_deselect(display);
}

