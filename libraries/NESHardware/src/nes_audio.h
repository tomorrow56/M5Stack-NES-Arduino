#ifndef _NES_AUDIO_H_
#define _NES_AUDIO_H_

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_SAMPLERATE 22100
#define DEFAULT_FRAGSIZE   128

void nes_audio_init(void);
void nes_audio_submit(void *buffer, int length);
void nes_audio_set_callback(void (*callback)(void *buffer, int length));
void nes_audio_frame(void);

#ifdef __cplusplus
}
#endif

#endif
