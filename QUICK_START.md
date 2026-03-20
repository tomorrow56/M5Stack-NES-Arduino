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
2. スケッチ → ライブラリをインクルード → .ZIP形式のライブラリをインストール
3. M5Stackライブラリをインストール

### 2. ボード設定（2分）

Arduino IDEで:
- ボード: **M5Stack-Core-ESP32**
- Upload Speed: **115200**
- Flash Size: **16MB (128Mb)**
- Partition Scheme: **Default 4MB with spiffs**
- PSRAM: **Enabled**

### 3. ROMファイルの準備（3分）

1. NES ROM（.nes）を`M5Stack_NES_Emulator/data/`に配置
2. ファイル名を`game.nes`に変更

### 4. アップロード（5分）

1. スケッチをコンパイル・アップロード
2. ツール → ESP32 Sketch Data Upload でROMをアップロード
3. M5Stackを再起動

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
| コンパイルエラー | ライブラリ未インストール | M5Stackライブラリをインストール |
| ROM NOT FOUND | SPIFFSにROMなし | ESP32 Sketch Data Uploadを実行 |
| 画面真っ暗 | 無効なROM | 別のROMファイルを試す |
| メモリ不足 | PSRAM未有効 | ボード設定でPSRAM有効化 |

## 推奨ROMサイズ

- 最小: 40KB
- 最大: 512KB
- 最適: 128KB～256KB

大きすぎるROMは動作が不安定になる可能性があります。

## 次のステップ

動作確認ができたら:
1. 他のROMを試す
2. 外部コントローラーを接続（オプション）
3. 設定をカスタマイズ

詳細は`README.md`と`COMPILE_GUIDE.md`を参照してください。
