#include "nes_video_audio.h"
#include "nes_display.h"
#include "nes_audio.h"
#include "nes_input.h"

extern "C" {
#include <noftypes.h>
#include <bitmap.h>
#include <nofconfig.h>
#include <event.h>
#include <gui.h>
#include <log.h>
#include <nes.h>
#include <nes_pal.h>
#include <nesinput.h>
#include <osd.h>
}

#define DEFAULT_WIDTH  256
#define DEFAULT_HEIGHT 240

static TimerHandle_t nes_timer;
static bitmap_t *myBitmap = NULL;
static uint16_t myPalette[256];
static char fb[1];

static int vid_init(int width, int height);
int vid_set_mode(int width, int height);
void vid_set_palette(rgb_t *pal);
void vid_clear(uint8 color);
bitmap_t *vid_lock_write(void);
void vid_free_write(int num_dirties, rect_t *dirty_rects);
static void vid_custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects);

viddriver_t videoDriver = {
  "M5Stack Display Driver",
  vid_init,
  vid_shutdown,
  vid_set_mode,
  vid_set_palette,
  vid_clear,
  vid_lock_write,
  vid_free_write,
  vid_custom_blit,
  false
};

int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize) {
  Serial.printf("Timer install, freq=%d\n", frequency);
  nes_timer = xTimerCreate("nes", configTICK_RATE_HZ / frequency, pdTRUE, NULL, (TimerCallbackFunction_t)func);
  xTimerStart(nes_timer, 0);
  return 0;
}

void osd_setsound(void (*playfunc)(void *buffer, int length)) {
  nes_audio_set_callback(playfunc);
}

void osd_getsoundinfo(sndinfo_t *info) {
  info->sample_rate = DEFAULT_SAMPLERATE;
  info->bps = 16;
}

void osd_getvideoinfo(vidinfo_t *info) {
  info->default_width = DEFAULT_WIDTH;
  info->default_height = DEFAULT_HEIGHT;
  info->driver = &videoDriver;
}

void osd_togglefullscreen(int code) {
}

static int vid_init(int width, int height) {
  nes_display_init();
  return 0;
}

void vid_shutdown(void) {
  if (myBitmap) {
    bmp_destroy(&myBitmap);
  }
}

int vid_set_mode(int width, int height) {
  return 0;
}

void vid_set_palette(rgb_t *pal) {
  uint16_t c;
  for (int i = 0; i < 256; i++) {
    c = (pal[i].b >> 3) + ((pal[i].g >> 2) << 5) + ((pal[i].r >> 3) << 11);
    myPalette[i] = c;
  }
}

void vid_clear(uint8 color) {
}

bitmap_t *vid_lock_write(void) {
  myBitmap = bmp_createhw((uint8*)fb, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_WIDTH * 2);
  return myBitmap;
}

void vid_free_write(int num_dirties, rect_t *dirty_rects) {
  bmp_destroy(&myBitmap);
}

static void vid_custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects) {
  if (num_dirties <= 0) return;
  
  for (int i = 0; i < num_dirties; i++) {
    rect_t *rect = &dirty_rects[i];
    
    static const uint8_t *lines[256];
    int height = rect->h;
    if (height > 256) height = 256;
    
    for (int y = 0; y < height; y++) {
      lines[y] = bmp->line[rect->y + y] + rect->x;
    }
    
    uint8_t *rgb_data = (uint8_t*)malloc(rect->w * height * 3);
    if (rgb_data) {
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < rect->w; x++) {
          uint8_t pixel = lines[y][x];
          uint16_t color = myPalette[pixel];
          
          rgb_data[(y * rect->w + x) * 3 + 0] = ((color >> 11) & 0x1F) << 3;
          rgb_data[(y * rect->w + x) * 3 + 1] = ((color >> 5) & 0x3F) << 2;
          rgb_data[(y * rect->w + x) * 3 + 2] = (color & 0x1F) << 3;
        }
      }
      
      const uint8_t *data_ptrs[256];
      for (int y = 0; y < height; y++) {
        data_ptrs[y] = &rgb_data[y * rect->w * 3];
      }
      
      nes_display_write_frame(rect->x, rect->y, rect->w, height, data_ptrs);
      free(rgb_data);
    }
  }
  
  nes_audio_frame();
}
