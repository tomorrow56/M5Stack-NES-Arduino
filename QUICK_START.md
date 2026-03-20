# クイックスタートガイド

## 最小限の手順でNESエミュレータを動かす

### 1. ライブラリのインストール（5分）

```bash
# Arduinoのlibrariesフォルダにコピー
cp -r libraries/Nofrendo ~/Documents/Arduino/libraries/
cp -r libraries/NESHardware ~/Documents/Arduino/libraries/
```

Arduino IDE経由の場合:
1. Arduino IDEを開く
2. スケッチ → ライブラリをインクルード → ライブラリを管理
3. "M5Unified"を検索してインストール

### 2. ボード設定（2分）

Arduino IDEで:
- ボード: **M5Stack-Core-ESP32**
- Upload Speed: **115200**
- Flash Size: **16MB (128Mb)**
- Partition Scheme: **Default 4MB with spiffs**
- PSRAM: **Enabled**

### 3. ROMファイルの準備（1分）

**方法1: SDカード（推奨）**
1. microSDカードをFAT32でフォーマット
2. NES ROM（.nes）をSDカードのルートにコピー
   - ファイル名は任意（複数可）
3. SDカードをM5Stackに挿入

**方法2: SPIFFS（オプション）**
1. NES ROM（.nes）を`M5Stack_NES_Emulator/data/`に配置
2. ファイル名を`game.nes`に変更
3. SPIFFS Upload Toolでアップロード

### 4. アップロード（3分）

1. M5StackをUSB接続
2. スケッチをコンパイル・アップロード
3. M5Stackを再起動（SDカード挿入済み）

### 5. 動作確認

シリアルモニタ（115200 bps）で以下が表示されればOK:

```
M5Stack NES Emulator
====================
ROM file size: XXXXX bytes
Valid NES ROM loaded
Starting NES emulator...
```

## トラブルシューティング早見表

| 症状 | 原因 | 解決方法 |
|------|------|----------|
| コンパイルエラー | ライブラリ未インストール | M5Unifiedライブラリをインストール |
| ROM NOT FOUND | SDカードなし/ROMなし | SDカードとROMファイルを確認 |
| No SD Card | SDカード未挿入 | SDカードを挿入して再起動 |
| 画面真っ暗 | 無効なROM | 別のROMファイルを試す |
| メモリ不足 | PSRAM未有効 | ボード設定でPSRAM有効化 |

## 推奨ROMサイズ

- 最小: 40KB
- 最大: 512KB
- 最適: 128KB～256KB

大きすぎるROMは動作が不安定になる可能性があります。

## 次のステップ

動作確認ができたら:
1. 複数のROMをSDカードに入れて選択機能を試す
2. カスタム入力デバイスを追加（`docs/CUSTOM_INPUT.md`参照）
3. ESP-NOW対応コントローラーの接続

詳細は`README.md`と`COMPILE_GUIDE.md`を参照してください。
