#include <M5Unified.h>
#include <SD.h>
#include <SPIFFS.h>
#include <vector>

extern "C" {
  #include <nofrendo.h>
  #include <osd.h>
  #include <nes_video_audio.h>
  #include <nes_display.h>
  #include <nes_audio.h>
  #include <nes_input.h>
}

#include <nes_osd.h>

std::vector<String> nesFiles;
int selectedIndex = 0;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.begin(115200);
  Serial.println("\n\nM5Stack NES Emulator");
  Serial.println("====================");
  
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(2);
  M5.Display.println("NES Emulator");
  M5.Display.println("Loading...");
  
  if (!SD.begin()) {
    Serial.println("SD Card Mount Failed - will try SPIFFS");
    M5.Display.println("No SD Card");
  } else {
    Serial.println("SD Card mounted successfully");
    M5.Display.println("SD Card OK");
    
    File root = SD.open("/");
    if (root) {
      Serial.println("Files on SD card:");
      File file = root.openNextFile();
      while(file) {
        if (!file.isDirectory()) {
          String filename = String(file.name());
          filename.toLowerCase();
          if (filename.endsWith(".nes")) {
            nesFiles.push_back(String(file.name()));
            Serial.printf("  Found NES ROM: %s (%d bytes)\n", file.name(), file.size());
          }
        }
        file = root.openNextFile();
      }
      root.close();
    }
    
    if (nesFiles.size() > 0) {
      Serial.printf("Found %d NES ROM files\n", nesFiles.size());
      if (nesFiles.size() > 1) {
        selectedIndex = selectRomFile();
      }
      set_selected_rom(nesFiles[selectedIndex].c_str());
      Serial.printf("Selected ROM: %s\n", nesFiles[selectedIndex].c_str());
    } else {
      Serial.println("No .nes files found on SD card");
    }
  }
  
  char *romdata = osd_getromdata();
  if (romdata == NULL) {
    Serial.println("\n\n=== ROM NOT FOUND ===");
    Serial.println("Please place a NES ROM file:");
    Serial.println("Option 1: Copy game.nes to SD card root");
    Serial.println("Option 2: Upload game.nes to SPIFFS");
    
    M5.Display.fillScreen(RED);
    M5.Display.setCursor(10, 100);
    M5.Display.setTextColor(WHITE);
    M5.Display.println("ROM NOT FOUND!");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.println("Place game.nes on");
    M5.Display.println("SD card or SPIFFS");
    
    while(1) {
      delay(1000);
    }
  }
  
  Serial.println("\n\nStarting NES emulator...");
  M5.Display.fillScreen(BLACK);
  
  int ret = nofrendo_main(0, NULL);
  Serial.printf("Emulator ended with code: %d\n", ret);
}

int selectRomFile() {
  int index = 0;
  bool selected = false;
  
  while (!selected) {
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.setTextColor(WHITE);
    M5.Display.setTextSize(2);
    M5.Display.println("Select ROM:");
    M5.Display.println("");
    
    M5.Display.setTextSize(1);
    int startIdx = max(0, index - 2);
    int endIdx = min((int)nesFiles.size(), startIdx + 6);
    
    for (int i = startIdx; i < endIdx; i++) {
      if (i == index) {
        M5.Display.setTextColor(YELLOW);
        M5.Display.print("> ");
      } else {
        M5.Display.setTextColor(WHITE);
        M5.Display.print("  ");
      }
      M5.Display.println(nesFiles[i]);
    }
    
    M5.Display.setTextSize(1);
    M5.Display.setCursor(0, 220);
    M5.Display.setTextColor(GREEN);
    M5.Display.println("A:Up B:Down C:Select");
    
    M5.update();
    
    if (M5.BtnA.wasPressed()) {
      index--;
      if (index < 0) index = nesFiles.size() - 1;
      delay(200);
    }
    
    if (M5.BtnB.wasPressed()) {
      index++;
      if (index >= nesFiles.size()) index = 0;
      delay(200);
    }
    
    if (M5.BtnC.wasPressed()) {
      selected = true;
      delay(200);
    }
    
    delay(50);
  }
  
  return index;
}

void loop() {
  M5.update();
  delay(10);
}
