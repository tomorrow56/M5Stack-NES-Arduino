# M5Stack NES Emulator - Arduino IDE版

ESP32-NESEMU（Nofrendo NESエミュレータ）のArduino IDE移植版です。M5Stackで任天堂ファミコン（NES）のゲームを実行できます。

## 特徴

- M5Stack対応のNESエミュレータ（M5Unified使用）
- 基本的なサウンド対応
- Arduino IDEでコンパイル可能
- SDカードから優先的にROM読み込み（SPIFFSもサポート）
- 複数ROMファイルの選択UI
- ESP32 Board Manager 3.x対応

## 必要なもの

### ハードウェア
- M5Stack Basic/Gray/Fire/Core2/CoreS3（ESP32搭載）
- microSDカード（ROM保存用、推奨）

### ソフトウェア
- Arduino IDE 1.8.x または 2.x
- ESP32 Boardsパッケージ 3.x以降
- M5Unified ライブラリ

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

#### M5Unifiedライブラリのインストール
1. 「スケッチ」→「ライブラリをインクルード」→「ライブラリを管理」
2. "M5Unified"を検索
3. "M5Unified by M5Stack"をインストール

### 2. プロジェクトのセットアップ

1. このフォルダ全体をArduinoのライブラリフォルダにコピー、または:
2. `libraries/Nofrendo`と`libraries/NESHardware`を
   Arduinoの`libraries`フォルダにコピー

Arduino librariesフォルダの場所:
- Windows: `C:\Users\[ユーザー名]\Documents\Arduino\libraries\`
- macOS: `~/Documents/Arduino/libraries/`
- Linux: `~/Arduino/libraries/`

### 3. ROMファイルの準備

#### 方法1: SDカード（推奨）
1. 合法的に入手したNES ROMファイル（.nes形式）を用意
2. microSDカードをフォーマット（FAT32）
3. ROMファイル（`.nes`）をSDカードのルートディレクトリにコピー
   - ファイル名は任意（複数可）
   - 起動時に選択メニューが表示されます
4. SDカードをM5Stackに挿入

#### 方法2: SPIFFS（オプション）
1. ROMファイルを`game.nes`にリネーム
2. `M5Stack_NES_Emulator/data/`フォルダに配置
3. SPIFFSアップロードツールでアップロード
   - [Arduino ESP32 filesystem uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases)

**重要**: 公式の任天堂ROMは著作権で保護されています。
パブリックドメインまたは自作のROMのみを使用してください。

### 4. コンパイルとアップロード

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
4. ROMを準備したSDカードを挿入して起動

## 操作方法

### ROM選択（複数ROMがある場合）
- **ボタンA**: 上に移動
- **ボタンB**: 下に移動
- **ボタンC**: 選択・決定

### ゲーム操作
M5Stackのボタン:
- **ボタンA**: NES Aボタン
- **ボタンB**: NES Bボタン
- **ボタンC**: START

**注意**: デフォルトでは十字キーは未割り当てです。
完全な操作には外部コントローラーが必要です。カスタム入力の追加方法は`docs/CUSTOM_INPUT.md`を参照してください。

## トラブルシューティング

### コンパイルエラー

#### "nofrendo.h: No such file or directory"
→ Nofrendoライブラリが正しくインストールされていません。
  `libraries/Nofrendo`フォルダをArduinoのlibrariesフォルダにコピーしてください。

#### "undefined reference to..."
→ 必要なライブラリが不足しています。M5Unifiedライブラリがインストールされているか確認してください。

#### メモリ不足エラー
→ パーティション設定を確認してください。PSRAMを有効にすることを推奨します。

### 実行時エラー

#### "ROM NOT FOUND"
→ ROMファイルが見つかりません。以下を確認してください：
  - SDカードが正しく挿入されているか
  - SDカードに`.nes`ファイルが存在するか
  - SDカードがFAT32でフォーマットされているか
  - （SPIFFSを使用する場合）`data/game.nes`がアップロードされているか

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
