# コンパイルガイド

## Arduino IDEでのコンパイル手順（詳細版）

### ステップ1: 環境の確認

#### 必要なバージョン
- Arduino IDE: 1.8.13以降 または 2.0以降
- ESP32ボードパッケージ: 2.0.0以降推奨
- M5Stack Library: 最新版

### ステップ2: ライブラリのインストール確認

以下のコマンドでArduinoのlibrariesフォルダを確認:

```bash
# macOS/Linux
ls ~/Documents/Arduino/libraries/

# Windows
dir %USERPROFILE%\Documents\Arduino\libraries\
```

以下のフォルダが存在することを確認:
- `Nofrendo/`
- `NESHardware/`
- `M5Stack/`

### ステップ3: ボード設定の詳細

Arduino IDEで以下を設定:

```
ボード: "M5Stack-Core-ESP32" または "ESP32 Dev Module"
Upload Speed: 115200
CPU Frequency: 240MHz (推奨)
Flash Frequency: 80MHz
Flash Mode: QIO
Flash Size: 16MB (128Mb)
Partition Scheme: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"
Core Debug Level: なし (リリース時)
PSRAM: Enabled
```

### ステップ4: コンパイル時の注意事項

#### メモリ使用量
- プログラム: 約1.5MB
- RAM: 約200KB

PSRAMを有効にすることで、より安定した動作が期待できます。

#### コンパイルオプション

platformio.iniを使用する場合（上級者向け）:

```ini
[env:m5stack-core-esp32]
platform = espressif32
board = m5stack-core-esp32
framework = arduino
lib_deps = 
    m5stack/M5Stack@^0.4.0
build_flags = 
    -DBOARD_HAS_PSRAM
    -mfix-esp32-psram-cache-issue
monitor_speed = 115200
```

### ステップ5: よくあるコンパイルエラーと対処法

#### エラー1: "fatal error: M5Stack.h: No such file or directory"
**原因**: M5Stackライブラリが未インストール

**対処法**:
1. ライブラリマネージャを開く
2. "M5Stack"を検索
3. インストール

#### エラー2: "region 'dram0_0_seg' overflowed"
**原因**: プログラムサイズが大きすぎる

**対処法**:
1. Partition Schemeを変更（より大きいAPP領域を持つものを選択）
2. 不要なデバッグコードを削除
3. PSRAMを有効化

#### エラー3: Multiple libraries were found for "xxx.h"
**原因**: 競合するライブラリが複数インストールされている

**対処法**:
1. Arduino IDEの出力を確認
2. 使用されているライブラリのパスを確認
3. 不要なライブラリを削除または無効化

### ステップ6: シリアルモニタでのデバッグ

コンパイル・アップロード後:

1. シリアルモニタを開く（ボーレート: 115200）
2. 以下のようなメッセージが表示されることを確認:

```
M5Stack NES Emulator
====================
ROM file size: XXXXX bytes
ROM loaded at 0xXXXXXXXX
Header: 4e 45 53 1a
Valid NES ROM loaded

Starting NES emulator...
```

### ステップ7: パフォーマンスチューニング

より高速な動作のために:

1. **CPU周波数を最大化**: 240MHz
2. **Flash周波数を上げる**: 80MHz
3. **Flash ModeをQIO**: 最速のフラッシュアクセス
4. **コンパイル最適化**: -O2または-O3フラグを使用

### トラブルシューティング

#### コンパイルは成功するが起動しない
→ シリアルモニタでエラーメッセージを確認
→ ROMファイルがSPIFFSに正しくアップロードされているか確認

#### 画面表示が遅い/カクつく
→ CPU周波数が240MHzになっているか確認
→ PSRAMが有効になっているか確認

#### 音が出ない/ノイズが多い
→ I2S設定の確認（現在の実装では内蔵DACを使用）
→ サンプルレート設定の確認

## コマンドライン（arduino-cli）でのビルド

上級者向け:

```bash
# ボード一覧の確認
arduino-cli board list

# コンパイル
arduino-cli compile --fqbn esp32:esp32:m5stack-core-esp32 M5Stack_NES_Emulator

# アップロード
arduino-cli upload -p /dev/cu.usbserial-XXXX --fqbn esp32:esp32:m5stack-core-esp32 M5Stack_NES_Emulator

# SPIFFSのアップロード（esptool使用）
esptool.py --chip esp32 --port /dev/cu.usbserial-XXXX write_flash 0x291000 spiffs.bin
```

## 次のステップ

コンパイルが成功したら:
1. ROMファイルをSPIFFSにアップロード
2. M5Stackで動作確認
3. お気に入りのゲームをプレイ！
