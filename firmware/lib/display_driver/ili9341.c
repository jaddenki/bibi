#include "ili9341.h"
#include <stdlib.h>

static inline void cs_select(ili9341_t *display) {
    gpio_put(display->cs_pin, 0);
}

static inline void cs_deselect(ili9341_t *display) {
    gpio_put(display->cs_pin, 1);
}

void ili9341_write_command(ili9341_t *display, uint8_t cmd) {
    gpio_put(display->dc_pin, 0);
    cs_select(display);
    spi_write_blocking(display->spi, &cmd, 1);
    cs_deselect(display);
}

void ili9341_write_data(ili9341_t *display, uint8_t data) {
    gpio_put(display->dc_pin, 1);
    cs_select(display);
    spi_write_blocking(display->spi, &data, 1);
    cs_deselect(display);
}

static void ili9341_reset(ili9341_t *display) {
    gpio_put(display->rst_pin, 1);
    sleep_ms(5);
    gpio_put(display->rst_pin, 0);
    sleep_ms(20);
    gpio_put(display->rst_pin, 1);
    sleep_ms(150);
}

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

void ili9341_init(ili9341_t *display, spi_inst_t *spi, uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sck) {
    display->spi = spi;
    display->cs_pin = cs;
    display->dc_pin = dc;
    display->rst_pin = rst;
    display->width = ILI9341_WIDTH;
    display->height = ILI9341_HEIGHT;

    spi_init(spi, 20000000);
    spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(mosi, GPIO_FUNC_SPI);
    gpio_set_function(sck, GPIO_FUNC_SPI);
    
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);

    gpio_init(dc);
    gpio_set_dir(dc, GPIO_OUT);

    gpio_init(rst);
    gpio_set_dir(rst, GPIO_OUT);

    // ===== DMA SETUP USING DIRECT REGISTER ACCESS =====
    // Claim DMA channel 0 for SPI TX
    display->dma_tx = 0;
    dma_channel_claim(display->dma_tx);
    
    // Set write address to SPI data register (fixed, doesn't increment)
    dma_hw->ch[display->dma_tx].write_addr = (uintptr_t)&spi_get_hw(spi)->dr;
    
    // Build control register value:
    // Bit 0:     EN (enable) - leave 0 for now, set when starting transfer
    // Bits 3:2:  DATA_SIZE = 0 (DMA_SIZE_8 = byte transfers)
    // Bit 4:     INCR_READ = 1 (increment source address)
    // Bit 5:     INCR_WRITE = 0 (fixed destination - SPI DR)
    // Bits 20:15: TREQ_SEL = DREQ for SPI TX (paces transfer to SPI)
    uint dreq = spi_get_dreq(spi, true);  // Get correct DREQ for this SPI instance
    uint32_t ctrl = (DMA_SIZE_8 << 2)     // 8-bit transfers
                  | (1 << 4)              // Increment read address
                  | (0 << 5)              // Don't increment write address (SPI DR is fixed)
                  | (dreq << 15);         // DREQ paces transfer
    
    // Write to ctrl (not ctrl_trig) to configure without starting
    dma_hw->ch[display->dma_tx].ctrl_trig = ctrl;

    ili9341_reset(display);
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

    ili9341_write_command(display, ILI9341_INVON);

    ili9341_write_command(display, ILI9341_DISPON);
    sleep_ms(20);
}

void ili9341_write_data_dma(ili9341_t *display, const uint8_t *data, size_t len) {
    gpio_put(display->dc_pin, 1);
    cs_select(display);
    
    dma_hw->ch[display->dma_tx].read_addr = (uintptr_t)data;
    
    dma_hw->ch[display->dma_tx].transfer_count = len;
    
    dma_hw->ch[display->dma_tx].ctrl_trig |= (1 << 0);
    
    while (dma_hw->ch[display->dma_tx].ctrl_trig & (1 << 24)) {
        tight_loop_contents();
    }
    while (spi_is_busy(display->spi)) tight_loop_contents();
    cs_deselect(display);
}

void ili9341_fill_screen(ili9341_t *display, uint16_t color) {
    ili9341_fill_rect(display, 0, 0, display->width, display->height, color);
}

void ili9341_fill_rect(ili9341_t *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= display->width || y >= display->height) return;
    if (x + w > display->width) w = display->width - x;
    if (y + h > display->height) h = display->height - y;

    ili9341_set_address_window(display, x, y, x + w - 1, y + h - 1);

    #define DMA_BUFFER_SIZE 1024
    static uint8_t dma_buffer[DMA_BUFFER_SIZE];
    
    for (int i = 0; i < DMA_BUFFER_SIZE; i += 2) {
        dma_buffer[i] = color >> 8;
        dma_buffer[i + 1] = color & 0xFF;
    }
    
    uint32_t total_bytes = w * h * 2;
    
    gpio_put(display->dc_pin, 1);
    cs_select(display);
    
    while (total_bytes > 0) {
        uint32_t chunk = (total_bytes > DMA_BUFFER_SIZE) ? DMA_BUFFER_SIZE : total_bytes;
        dma_hw->ch[display->dma_tx].read_addr = (uintptr_t)dma_buffer;
        dma_hw->ch[display->dma_tx].transfer_count = chunk;
        dma_hw->ch[display->dma_tx].ctrl_trig |= (1 << 0);
        while (dma_hw->ch[display->dma_tx].ctrl_trig & (1 << 24)) {
            tight_loop_contents();
        }
        
        total_bytes -= chunk;
    }
    
    while (spi_is_busy(display->spi)) tight_loop_contents();
    
    cs_deselect(display);
}
