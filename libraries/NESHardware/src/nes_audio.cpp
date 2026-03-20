#include "nes_audio.h"
#include <driver/i2s.h>

static void (*audio_callback)(void *buffer, int length) = NULL;
static int16_t *audio_frame = NULL;

#define NES_REFRESH_RATE 60

void nes_audio_init(void) {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = DEFAULT_SAMPLERATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = DEFAULT_FRAGSIZE,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_driver_install((i2s_port_t)0, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)0, NULL);
  i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);

  audio_frame = (int16_t*)malloc(DEFAULT_SAMPLERATE / NES_REFRESH_RATE * sizeof(int16_t));
  
  Serial.println("Audio initialized");
}

void nes_audio_set_callback(void (*callback)(void *buffer, int length)) {
  audio_callback = callback;
}

void nes_audio_frame(void) {
  if (audio_callback == NULL) return;
  
  int left = DEFAULT_SAMPLERATE / NES_REFRESH_RATE;
  
  while (left > 0) {
    int n = DEFAULT_FRAGSIZE;
    if (n > left) n = left;
    
    audio_callback(audio_frame, n);
    
    for (int i = n - 1; i >= 0; i--) {
      audio_frame[i] = audio_frame[i] + 0x8000;
    }
    
    size_t bytes_written;
    i2s_write((i2s_port_t)0, (const char*)audio_frame, 2 * n, &bytes_written, portMAX_DELAY);
    
    left -= n;
  }
}
