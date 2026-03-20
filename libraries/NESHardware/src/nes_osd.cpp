#include <Arduino.h>
#include <SD.h>
#include <SPIFFS.h>

extern "C" {
#include <noftypes.h>
#include <nofconfig.h>
#include <log.h>
#include <osd.h>
#include <nofrendo.h>
}

static char *rom_data = NULL;
static size_t rom_size = 0;
static char selected_rom_file[256] = "/game.nes";

void set_selected_rom(const char* filename) {
  if (filename[0] == '/') {
    strncpy(selected_rom_file, filename, 255);
  } else {
    snprintf(selected_rom_file, 255, "/%s", filename);
  }
  selected_rom_file[255] = '\0';
  Serial.printf("ROM file set to: %s\n", selected_rom_file);
}

char *osd_getromdata(void) {
  if (rom_data != NULL) {
    Serial.printf("Using cached ROM data@%p\n", rom_data);
    return rom_data;
  }

  Serial.printf("Trying to load ROM: %s\n", selected_rom_file);
  
  File romFile = SD.open(selected_rom_file, FILE_READ);
  if (!romFile) {
    Serial.printf("Failed to open ROM file %s on SD card\n", selected_rom_file);
    Serial.println("Trying SPIFFS...");
    
    if (!SPIFFS.begin(true)) {
      Serial.println("Failed to mount SPIFFS");
      return NULL;
    }
    
    romFile = SPIFFS.open(selected_rom_file, "r");
    if (!romFile) {
      Serial.printf("Failed to open ROM file %s on SPIFFS\n", selected_rom_file);
      Serial.println("Please place ROM on SD card or upload to SPIFFS");
      return NULL;
    }
  } else {
    Serial.printf("Loading ROM from SD card: %s\n", selected_rom_file);
  }

  rom_size = romFile.size();
  Serial.printf("ROM file size: %d bytes\n", rom_size);

  if (rom_size < 16384) {
    Serial.println("ROM file too small");
    romFile.close();
    return NULL;
  }

  rom_data = (char*)malloc(rom_size);
  if (!rom_data) {
    Serial.println("Failed to allocate memory for ROM");
    romFile.close();
    return NULL;
  }

  romFile.read((uint8_t*)rom_data, rom_size);
  romFile.close();

  Serial.printf("ROM loaded at %p\n", rom_data);
  Serial.printf("Header: %02x %02x %02x %02x\n", 
    (unsigned char)rom_data[0], (unsigned char)rom_data[1],
    (unsigned char)rom_data[2], (unsigned char)rom_data[3]);

  if (rom_data[0] != 'N' || rom_data[1] != 'E' || 
      rom_data[2] != 'S' || rom_data[3] != 0x1A) {
    Serial.println("Invalid NES ROM format!");
    free(rom_data);
    rom_data = NULL;
    return NULL;
  }

  Serial.println("Valid NES ROM loaded");
  return rom_data;
}

int osd_main(int argc, char *argv[]) {
  return main_loop("rom", system_autodetect);
}

void osd_fullname(char *fullname, const char *shortname) {
  strncpy(fullname, shortname, 256);
}

char *osd_newextension(char *string, char *ext) {
  return string;
}

int osd_makesnapname(char *filename, int len) {
  return -1;
}

int osd_init(void) {
  return 0;
}

void osd_getinput(void) {
}

void osd_getmouse(int *x, int *y, int *button) {
  *x = 0;
  *y = 0;
  *button = 0;
}
