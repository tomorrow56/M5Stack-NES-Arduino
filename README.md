# M5Stack NES Emulator - Arduino IDE版

ESP32-NESEMU（Nofrendo NESエミュレータ）のArduino IDE移植版です。M5Stackで任天堂ファミコン（NES）のゲームを実行できます。

## 特徴

- M5Stack対応のNESエミュレータ
- 基本的なサウンド対応
- Arduino IDEでコンパイル可能
- SPIFFSからROMを読み込み

## 必要なもの

### ハードウェア
- M5Stack Basic/Gray/Fire（ESP32搭載）

### ソフトウェア
- Arduino IDE 1.8.x または 2.x
- ESP32 Boardsパッケージ
- M5Stack ライブラリ

## インストール手順

### 1. Arduino IDEのセットアップ

#### ESP32ボードパッケージのインストール
1. Arduino IDEを開く
2. 「ファイル」→「環境設定」
3. 「追加のボードマネージャのURL」に以下を追加:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. 「ツール」→「ボード」→「ボードマネージャ」
5. "esp32"を検索してインストール

#### M5Stackライブラリのインストール
1. 「スケッチ」→「ライブラリをインクルード」→「ライブラリを管理」
2. "M5Stack"を検索
3. "M5Stack by M5Stack"をインストール

### 2. プロジェクトのセットアップ

1. このフォルダ全体をArduinoのライブラリフォルダにコピー、または:
2. `libraries/Nofrendo`と`libraries/NESHardware`を
   Arduinoの`libraries`フォルダにコピー

Arduino librariesフォルダの場所:
- Windows: `C:\Users\[ユーザー名]\Documents\Arduino\libraries\`
- macOS: `~/Documents/Arduino/libraries/`
- Linux: `~/Arduino/libraries/`

### 3. ROMファイルの準備

1. 合法的に入手したNES ROMファイル（.nes形式）を用意
2. ファイル名を`game.nes`に変更
3. `M5Stack_NES_Emulator/data/`フォルダに配置

**重要**: 公式の任天堂ROMは著作権で保護されています。
パブリックドメインまたは自作のROMのみを使用してください。

### 4. SPIFFSアップロードツールのインストール

Arduino IDE 1.x の場合:
1. [Arduino ESP32 filesystem uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases)をダウンロード
2. `ESP32FS`フォルダを`[Arduino]/tools/`にコピー
3. Arduino IDEを再起動

Arduino IDE 2.x の場合:
- コマンドラインツール`arduino-cli`または`esptool.py`を使用

### 5. コンパイルとアップロード

#### ボード設定
1. 「ツール」→「ボード」→「M5Stack-Core-ESP32」を選択
   （ない場合は「ESP32 Dev Module」）
2. 「ツール」→「Flash Size」→「16MB (128Mb)」
3. 「ツール」→「Partition Scheme」→「Default 4MB with spiffs」
4. 「ツール」→「PSRAM」→「Enabled」（推奨）

#### アップロード手順
1. M5StackをUSBで接続
2. 適切なシリアルポートを選択
3. スケッチをコンパイル・アップロード
4. 「ツール」→「ESP32 Sketch Data Upload」でROMをアップロード

## 操作方法

M5Stackのボタン:
- **ボタンA**: NES Aボタン
- **ボタンB**: NES Bボタン
- **ボタンC**: SELECT

**注意**: 現在のバージョンでは十字キーがM5Stackの物理ボタンでは操作できません。
完全な操作には外部コントローラー（FACES Gamepad Kit等）が必要です。

## トラブルシューティング

### コンパイルエラー

#### "nofrendo.h: No such file or directory"
→ Nofrendoライブラリが正しくインストールされていません。
  `libraries/Nofrendo`フォルダをArduinoのlibrariesフォルダにコピーしてください。

#### "undefined reference to..."
→ 必要なライブラリが不足しています。M5Stackライブラリがインストールされているか確認してください。

#### メモリ不足エラー
→ パーティション設定を確認してください。PSRAMを有効にすることを推奨します。

### 実行時エラー

#### "ROM NOT FOUND"
→ ROMファイルがSPIFFSにアップロードされていません。
  `data/game.nes`が存在し、SPIFFSにアップロードされているか確認してください。

#### 画面が真っ暗
→ ROMファイルが破損しているか、互換性がない可能性があります。
  別のROMファイルを試してください。

## 制限事項

- 一部のマッパーには未対応
- サウンドの品質は限定的
- フレームドロップが発生する可能性があります
- セーブ機能は未実装

## ライセンス

- Nofrendoエミュレータコア: GPLv2 (Matthew Conte)
- ESP32移植部分: Apache License 2.0 (Espressif Systems)
- Arduino移植部分: Apache License 2.0

## 参考リンク

- オリジナルプロジェクト: https://github.com/espressif/esp32-nesemu
- Nofrendo: https://github.com/bfirsh/nofrendo
- M5Stack: https://m5stack.com/

## 免責事項

このソフトウェアはプルーフオブコンセプトです。Espressif Systemsの公式サポートはありません。
著作権で保護されたROMファイルの使用はユーザーの責任において行ってください。
