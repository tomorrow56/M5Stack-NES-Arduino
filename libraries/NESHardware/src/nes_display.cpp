#include "nes_display.h"

void nes_display_init(void) {
  M5.Display.fillScreen(BLACK);
  M5.Display.setRotation(1);
  M5.Display.setTextColor(WHITE);
}

void nes_display_write_frame(const uint16_t x, const uint16_t y, 
                              const uint16_t width, const uint16_t height, 
                              const uint8_t *data[]) {
  static uint16_t *line_buffer = NULL;
  
  if (line_buffer == NULL) {
    line_buffer = (uint16_t*)malloc(NES_SCREEN_WIDTH * 2);
    if (line_buffer == NULL) {
      Serial.println("Failed to allocate line buffer");
      return;
    }
  }

  int scale_x = 320 / NES_SCREEN_WIDTH;
  int scale_y = 240 / NES_SCREEN_HEIGHT;
  int offset_x = (320 - (NES_SCREEN_WIDTH * scale_x)) / 2;
  int offset_y = (240 - (NES_SCREEN_HEIGHT * scale_y)) / 2;

  for (uint16_t i = 0; i < height; i++) {
    const uint8_t *line = data[i];
    
    for (uint16_t j = 0; j < width; j++) {
      uint8_t r = line[j * 3 + 0];
      uint8_t g = line[j * 3 + 1];
      uint8_t b = line[j * 3 + 2];
      
      line_buffer[j] = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
    
    int scaled_y = offset_y + ((y + i) * scale_y);
    for (int sy = 0; sy < scale_y; sy++) {
      M5.Display.pushImage(offset_x + (x * scale_x), scaled_y + sy, 
                           width * scale_x, 1, line_buffer);
    }
  }
}
