# 変更履歴

## Arduino IDE移植版 v1.0.0

### 新機能
- Arduino IDEでのコンパイルに対応
- M5Stackライブラリとの統合
- SPIFFSベースのROMローディング
- 簡素化されたプロジェクト構造

### 変更点
- ESP-IDFからArduinoフレームワークへ移植
- パーティションシステムからSPIFFSへ変更
- ビルドシステムをCMakeからArduino IDEに変更
- ハードウェア抽象化層の再実装

### ESP-IDF版からの主な違い

#### ファイル構造
- **ESP-IDF版**: component/main構造
- **Arduino版**: libraries/スケッチ構造

#### ROM管理
- **ESP-IDF版**: カスタムパーティション（esptool経由）
- **Arduino版**: SPIFFS（Sketch Data Upload経由）

#### ビルドシステム
- **ESP-IDF版**: CMake + idf.py
- **Arduino版**: Arduino IDE

#### 依存関係
- **ESP-IDF版**: ESP-IDF SDK
- **Arduino版**: Arduino ESP32 + M5Stack

### 既知の制限事項

1. **入力デバイス**: M5Stackボタンのみ（A/B/SELECT）
   - 十字キーは外部コントローラーが必要
   
2. **サウンド**: 基本的な音声のみ
   - 一部のゲームで音質が低下する可能性

3. **パフォーマンス**: 
   - 一部の複雑なゲームでフレームドロップの可能性
   - PSRAM必須

4. **マッパー対応**: 
   - 一般的なマッパー（0, 1, 2, 3, 4）はサポート
   - 特殊なマッパーは未対応の場合あり

### 互換性

#### テスト済みボード
- M5Stack Basic ✓
- M5Stack Gray ✓  
- M5Stack Fire ✓

#### 必要なArduino環境
- Arduino IDE 1.8.13+ または 2.0+
- ESP32 Board Package 2.0.0+
- M5Stack Library 0.4.0+

### 今後の予定

- [ ] 外部コントローラーサポートの改善
- [ ] セーブステート機能
- [ ] UIメニューの追加
- [ ] より多くのマッパー対応
- [ ] パフォーマンス最適化

### オリジナルプロジェクトとの関係

このプロジェクトは以下をベースにしています:
- **Nofrendo**: NESエミュレータコア（GPLv2）
- **ESP32-NESEMU**: Espressif SystemsによるESP32移植（Apache 2.0）

Arduino IDE版は独立した移植作業であり、公式サポートはありません。
