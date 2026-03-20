#ifndef NES_DISPLAY_H
#define NES_DISPLAY_H

#include <M5Unified.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NES_SCREEN_WIDTH  256
#define NES_SCREEN_HEIGHT 240

void nes_display_init(void);
void nes_display_write_frame(const uint16_t x, const uint16_t y, 
                              const uint16_t width, const uint16_t height, 
                              const uint8_t *data[]);

#ifdef __cplusplus
}
#endif

#endif
