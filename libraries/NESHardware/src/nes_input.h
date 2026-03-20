#ifndef _NES_INPUT_H_
#define _NES_INPUT_H_

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NES_BTN_A       0x0001
#define NES_BTN_B       0x0002
#define NES_BTN_SELECT  0x0004
#define NES_BTN_START   0x0008
#define NES_BTN_UP      0x0010
#define NES_BTN_DOWN    0x0020
#define NES_BTN_LEFT    0x0040
#define NES_BTN_RIGHT   0x0080

void nes_input_init(void);
int nes_input_read(void);

#ifdef __cplusplus
}
#endif

#endif
