# カスタム入力デバイスの追加方法

M5Stack NESエミュレータは、デフォルトでM5Stackの3つのボタン（A、B、C）を使用していますが、外部入力デバイス（例：Bluetoothコントローラー、GPIO接続のボタン、I2Cジョイスティックなど）を追加することができます。

## 入力システムの概要

入力処理は `libraries/NESHardware/src/nes_input.cpp` で実装されています。

### NESボタンマッピング

NESコントローラーのボタンは16ビット値で表現されます（`nes_input.h`で定義）：

```cpp
// ビットマスク（0でボタン押下、1で未押下）
#define NES_BTN_A       0x0001  // Aボタン
#define NES_BTN_B       0x0002  // Bボタン
#define NES_BTN_SELECT  0x0004  // SELECT
#define NES_BTN_START   0x0008  // START
#define NES_BTN_UP      0x0010  // 上
#define NES_BTN_DOWN    0x0020  // 下
#define NES_BTN_LEFT    0x0040  // 左
#define NES_BTN_RIGHT   0x0080  // 右
```

デフォルトのボタン割り当て：
- M5Stack ボタンA → NES A (0x0001)
- M5Stack ボタンB → NES B (0x0002)
- M5Stack ボタンC → NES START (0x0008)

## 方法1: nes_input.cppを直接編集

### ステップ1: nes_input.cppを開く

`libraries/NESHardware/src/nes_input.cpp` を編集します。

### ステップ2: カスタム入力読み取りコードを追加

```cpp
#include "nes_input.h"
#include <M5Unified.h>

// 外部デバイス用の初期化関数を追加
void nes_input_init(void) {
  // 例: GPIOピンの初期化
  pinMode(21, INPUT_PULLUP);  // Aボタン
  pinMode(22, INPUT_PULLUP);  // Bボタン
  pinMode(25, INPUT_PULLUP);  // UP
  pinMode(26, INPUT_PULLUP);  // DOWN
}

int nes_input_read(void) {
  int buttons = 0xFFFF;  // 全ボタン未押下
  
  M5.update();
  
  // M5Stack ボタン
  if (M5.BtnA.isPressed()) {
    buttons &= ~NES_BTN_A;
  }
  
  if (M5.BtnB.isPressed()) {
    buttons &= ~NES_BTN_B;
  }
  
  if (M5.BtnC.isPressed()) {
    buttons &= ~NES_BTN_START;
  }
  
  // 外部GPIO入力
  if (digitalRead(21) == LOW) {
    buttons &= ~NES_BTN_A;
  }
  
  if (digitalRead(22) == LOW) {
    buttons &= ~NES_BTN_B;
  }
  
  if (digitalRead(25) == LOW) {
    buttons &= ~NES_BTN_UP;
  }
  
  if (digitalRead(26) == LOW) {
    buttons &= ~NES_BTN_DOWN;
  }
  
  return buttons;
}
```

## 方法2: Bluetoothコントローラーの追加

### ステップ1: ESP32 Bluetooth Classic/BLEライブラリを使用

```cpp
#include "nes_input.h"
#include <M5Unified.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
static int bt_buttons = 0xFFFF;

void nes_input_init(void) {
  SerialBT.begin("M5Stack_NES");
  Serial.println("Bluetooth started, pairing as 'M5Stack_NES'");
}

// Bluetoothから受信したデータを処理
void updateBluetoothInput() {
  if (SerialBT.available()) {
    uint8_t cmd = SerialBT.read();
    
    // 簡易プロトコル例
    // 'A' = Aボタン押下
    // 'a' = Aボタン解放
    // 'U' = UP押下, 'u' = UP解放
    
    switch(cmd) {
      case 'A': bt_buttons &= ~NES_BTN_A; break;
      case 'a': bt_buttons |= NES_BTN_A; break;
      case 'B': bt_buttons &= ~NES_BTN_B; break;
      case 'b': bt_buttons |= NES_BTN_B; break;
      case 'U': bt_buttons &= ~NES_BTN_UP; break;
      case 'u': bt_buttons |= NES_BTN_UP; break;
      case 'D': bt_buttons &= ~NES_BTN_DOWN; break;
      case 'd': bt_buttons |= NES_BTN_DOWN; break;
      case 'L': bt_buttons &= ~NES_BTN_LEFT; break;
      case 'l': bt_buttons |= NES_BTN_LEFT; break;
      case 'R': bt_buttons &= ~NES_BTN_RIGHT; break;
      case 'r': bt_buttons |= NES_BTN_RIGHT; break;
      case 'S': bt_buttons &= ~NES_BTN_START; break;
      case 's': bt_buttons |= NES_BTN_START; break;
    }
  }
}

int nes_input_read(void) {
  M5.update();
  updateBluetoothInput();
  
  int buttons = bt_buttons;
  
  // M5Stackボタンも併用
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
```

## 方法3: I2Cジョイスティック（例：Adafruit Seesaw）

```cpp
#include "nes_input.h"
#include <M5Unified.h>
#include <Adafruit_Seesaw.h>

Adafruit_Seesaw joystick;

void nes_input_init(void) {
  if (!joystick.begin(0x49)) {
    Serial.println("Joystick not found!");
  } else {
    Serial.println("Joystick initialized");
  }
}

int nes_input_read(void) {
  int buttons = 0xFFFF;
  
  M5.update();
  
  // ジョイスティックから読み取り
  int x = joystick.analogRead(2);  // X軸
  int y = joystick.analogRead(3);  // Y軸
  uint32_t button_mask = joystick.digitalReadBulk();
  
  // アナログスティック → 方向キー
  if (x < 300) buttons &= ~NES_BTN_LEFT;
  if (x > 700) buttons &= ~NES_BTN_RIGHT;
  if (y < 300) buttons &= ~NES_BTN_UP;
  if (y > 700) buttons &= ~NES_BTN_DOWN;
  
  // ボタン
  if (!(button_mask & (1 << 6))) buttons &= ~NES_BTN_A;
  if (!(button_mask & (1 << 5))) buttons &= ~NES_BTN_B;
  if (!(button_mask & (1 << 0))) buttons &= ~NES_BTN_SELECT;
  if (!(button_mask & (1 << 16))) buttons &= ~NES_BTN_START;
  
  // M5Stackボタンも併用
  if (M5.BtnA.isPressed()) {
    buttons &= ~NES_BTN_A;
  }
  
  return buttons;
}
```

## 方法4: カスタムコールバック関数の実装

より柔軟な実装のため、コールバック関数を追加できます。

### nes_input.hにコールバック型を追加

```cpp
#ifndef NES_INPUT_H
#define NES_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*custom_input_callback_t)(void);

void nes_input_init(void);
int nes_input_read(void);
void nes_input_set_callback(custom_input_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif
```

### nes_input.cppに実装

```cpp
#include "nes_input.h"
#include <M5Unified.h>

static custom_input_callback_t custom_callback = NULL;

void nes_input_set_callback(custom_input_callback_t callback) {
  custom_callback = callback;
}

int nes_input_read(void) {
  int buttons = 0xFFFF;
  
  M5.update();
  
  // カスタムコールバックがあれば呼び出す
  if (custom_callback != NULL) {
    buttons = custom_callback();
  }
  
  // M5Stackボタン（カスタム入力と併用）
  if (M5.BtnA.isPressed()) {
    buttons &= ~0x0001;
  }
  
  if (M5.BtnB.isPressed()) {
    buttons &= ~0x0002;
  }
  
  if (M5.BtnC.isPressed()) {
    buttons &= ~0x0100;
  }
  
  return buttons;
}
```

### メインスケッチから使用

```cpp
#include <M5Unified.h>

extern "C" {
  #include <nes_input.h>
}

// カスタム入力読み取り関数
int my_custom_input() {
  int buttons = 0xFFFF;
  
  // 例：シリアルから読み取り
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'a') buttons &= ~0x0001;  // A
    if (cmd == 'b') buttons &= ~0x0002;  // B
    // ...
  }
  
  return buttons;
}

void setup() {
  M5.begin();
  
  // カスタム入力関数を登録
  nes_input_set_callback(my_custom_input);
  
  // エミュレータ起動
  // ...
}
```

## 方法5: ESP-NOWを使ったワイヤレスコントローラー（M5AtomPadなど）

ESP-NOWを使用すると、M5AtomなどをワイヤレスNESコントローラーとして使用できます。

参考プロジェクト: [M5AtomPad](https://github.com/tomorrow56/M5AtomPad)

### 送信側（M5Atom NESPad）

送信側は既存のM5AtomPadプロジェクトをそのまま使用できます：

```cpp
// M5Atom側（送信側）
// https://github.com/tomorrow56/M5AtomPad/blob/main/example/M5ATom_NESPad_esp-now/M5ATom_NESPad_esp-now.ino
// 
// NESパッドの入力を読み取り、ESP-NOW経由で送信
// data[0] の値:
//   0 = ボタン未押下
//   1 = NES_A
//   2 = NES_B
//   3 = NES_SELECT
//   4 = NES_START
//   5 = NES_UP
//   6 = NES_DOWN
//   7 = NES_LEFT
//   8 = NES_RIGHT
```

### 受信側（M5Stack NESエミュレータ）

#### ステップ1: nes_input.cppにESP-NOW受信処理を追加

```cpp
#include "nes_input.h"
#include <M5Unified.h>
#include <esp_now.h>
#include <WiFi.h>

static volatile uint8_t espnow_button_data = 0;

// ESP-NOW受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  if (data_len > 0) {
    espnow_button_data = data[0];
    Serial.printf("ESP-NOW received: %d\n", espnow_button_data);
  }
}

void nes_input_init(void) {
  // ESP-NOW初期化
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESP-NOW Init Success");
  } else {
    Serial.println("ESP-NOW Init Failed");
    return;
  }
  
  // 受信コールバック登録
  esp_now_register_recv_cb(OnDataRecv);
  
  Serial.println("ESP-NOW receiver initialized");
}

int nes_input_read(void) {
  int buttons = 0xFFFF;  // 全ボタン未押下
  
  M5.update();
  
  // ESP-NOWからの入力
  switch(espnow_button_data) {
    case 1:  // NES_A
      buttons &= ~NES_BTN_A;
      break;
    case 2:  // NES_B
      buttons &= ~NES_BTN_B;
      break;
    case 3:  // NES_SELECT
      buttons &= ~NES_BTN_SELECT;
      break;
    case 4:  // NES_START
      buttons &= ~NES_BTN_START;
      break;
    case 5:  // NES_UP
      buttons &= ~NES_BTN_UP;
      break;
    case 6:  // NES_DOWN
      buttons &= ~NES_BTN_DOWN;
      break;
    case 7:  // NES_LEFT
      buttons &= ~NES_BTN_LEFT;
      break;
    case 8:  // NES_RIGHT
      buttons &= ~NES_BTN_RIGHT;
      break;
    case 0:  // ボタン未押下
    default:
      // 何もしない
      break;
  }
  
  // M5Stackボタンも併用可能
  if (M5.BtnA.isPressed()) {
    buttons &= ~0x0001;  // A
  }
  
  if (M5.BtnB.isPressed()) {
    buttons &= ~0x0002;  // B
  }
  
  if (M5.BtnC.isPressed()) {
    buttons &= ~0x0100;  // START
  }
  
  return buttons;
}
```

#### ステップ2: MACアドレスの確認（オプション）

特定のM5Atomからのみ受信する場合は、MACアドレスフィルタを追加できます：

```cpp
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  // 特定のMACアドレスのみ受け入れる例
  uint8_t allowed_mac[6] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};
  
  bool is_allowed = true;
  for (int i = 0; i < 6; i++) {
    if (mac_addr[i] != allowed_mac[i]) {
      is_allowed = false;
      break;
    }
  }
  
  if (!is_allowed) {
    return;  // 許可されていないMACアドレスからの送信は無視
  }
  
  if (data_len > 0) {
    espnow_button_data = data[0];
  }
}
```

### メリット

- **ワイヤレス**: ケーブル不要
- **低遅延**: ESP-NOWは高速通信
- **M5Atomの活用**: 小型で持ちやすいコントローラーとして使用可能
- **マルチキャスト対応**: 複数のM5Stackに同時送信可能

### 注意点

- ESP-NOWとWi-Fiは同じチャンネルを使用する必要がある
- メモリ使用量が増加するため、パーティション設定に注意
- 通信範囲は約100m（障害物がない場合）

## ライブラリの更新

変更を反映するには：

1. `libraries/NESHardware/src/nes_input.cpp` を編集
2. 更新したライブラリをArduinoライブラリフォルダにコピー：

   ```bash
   cp -r "/Volumes/HPSSD500GB/CascadeProjects/M5Stack-NES-Arduino/libraries/NESHardware" \
         "/Users/masaoyamazaki/Documents/Arduino/libraries/"
   ```

3. Arduino IDEを再起動
4. 再コンパイルとアップロード

## トラブルシューティング

### 入力が反応しない
- シリアルモニタで入力値をデバッグ出力
- `Serial.printf("Buttons: 0x%04X\n", buttons);` を追加

### Bluetooth接続できない
- ESP32のメモリ不足の可能性
- パーティションスキームを確認（APP領域を増やす）

### 遅延が大きい
- `nes_input_read()` 内の処理を最適化
- ポーリング頻度を調整

## 参考情報

- [NES入力形式](https://www.nesdev.org/wiki/Controller_reading)
- [ESP32 GPIO](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)
- [M5Unified API](https://github.com/m5stack/M5Unified)
- [M5AtomPad (ESP-NOW NESパッド)](https://github.com/tomorrow56/M5AtomPad)
- [ESP-NOW プロトコル](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)
