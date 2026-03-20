#include "nes_input.h"
#include <M5Unified.h>

void nes_input_init(void) {
}

int nes_input_read(void) {
  int buttons = 0xFFFF;
  
  M5.update();
  
  if (M5.BtnA.isPressed()) {
    buttons &= ~NES_BTN_A;
  }
  
  if (M5.BtnB.isPressed()) {
    buttons &= ~NES_BTN_B;
  }
  
  if (M5.BtnC.isPressed()) {
    buttons &= ~NES_BTN_START;
  }
  
  return buttons;
}
