# Kitchen_Timer

PICを使ったキッチンタイマーを作成します。

## TODO

---

2020/07/09

- 作業予定
  - [ ] フローチャート 修正

見る人が煩わしくないように、詳細すぎない粒度でまとめる

---

2020/07/08

- 作業予定
  - [ ] IOC割込みを使って、LEDチカ
    - [x] IOC割込みの実装
      - MCCでピン設定はすぐできた
      - RAとRBを逆に付け間違えたが、付けなおしたらちゃんと動いた
      - [x] 3つのLEDがボタン3つで制御できる
    - [x] 1秒タイマ割込みの実装
      - [x] Timer1で500msのタイマを作成
    - [x] 10msタイマ割込みの実装
    - [ ] 割込み時間 の測定
    - [ ] IOC割込みを使って、同時押しの実装
  - [ ] フローチャート 修正

---

2020/07/06

- 作業予定
  - [x] フローチャート 作成
  - [ ] レビュー後 修正

待ち処理を10msタイマ割込みを使って止まらないように実現させる

---

2020/07/02

- 作業予定
  - [ ] フローチャート 作成

- 長押しの定義
  - スイッチを押してから、1秒経つ
    - スイッチの立ち上がりから、1秒が経過する
      - スイッチの立ち上がりで、長押しフラグを立てる
      - 1秒経過するまで、長押しフラグを確認する
        - 1秒が経過したら、長押し状態をONにする
  - 要素
    - 長押しフラグ
      - 長押し状態になるまでの時間で、押され続けていることを比較する
    - 長押し状態
      - スイッチを押し続けて、1秒経ったらONになる
        - ONの挙動：
          - カウント単位が増える
        - OFFへの条件

---

2020/07/02

- 作業予定
  - [ ] コンフィギュレーターの使い方 講座
  - [ ] LCD 更新間隔 確認
  - [ ] フローチャート 作成

---

2020/07/01

- 作業予定
  - [ ] フローチャート 作成
  - [ ] LCD仕様 確認

LCDとのI2C通信は、ツールによって実装できるらしい
あとは、LCDのデータシートを読んで、各コマンドを把握できれば良いと思う
明日、先輩にツールの使い方を教えていただく予定

---

2020/06/30

- 作業予定
  - [ ] フローチャート 作成
  - [ ] LCD仕様 確認

まったくダメ、データシートのI2Cの欄が多すぎる...
翻訳でさえ時間がかかる
結局、ツールでLCD周りは実装するみたいだから
今はフローチャートやろうかな

---

2020/06/29

- 作業予定
  - [x] クラス図 作成
  - [ ] フローチャート 作成
    - [ ] main
    - [ ] Input
    - [ ] Count
    - [ ] LCD
    - [ ] Buzzer

mermaidでクラス図を作った。
フローチャートは、あまり進まず...

---

2020/06/26

- 作業予定
  - [ ] フローチャート 作成
    - [ ] main
    - [ ] Input
    - [ ] Count
    - [ ] LCD
    - [ ] Buzzer

メインが簡潔になった。
これから、他の部位も簡潔に書いていきたい。

---

2020/06/25

- 作業予定
  - [ ] フローチャート作成
  - [x] HW研修

今日は、ハードウェア研修があったため、進まなかった。
レビューの時間を1日ずらしたが、レビュー前にまた2回目の研修があるようなので、
またレビューの時間をずらす必要がある。

フローチャートを書くより、コードを書いたほうが早い気がしてきた。
ただ、ピン配置などがあるため、「今は適当に」ができない。
一応、mainの状態遷移のフローだけは書いておいた。

---

2020/06/24

- 作業予定
  - PIC16F1827の確認
    - [x] CCPタイマを使用する
    - [x] PICで1秒ずつLEDが点灯するタイマを作る
    - [ ] デバッガの確認

---

2020/06/23

- 作業予定
  - PIC16F1827の確認
    - [x] Lチカ
    - [ ] CCPタイマを使用する
    - [ ] PICで1秒ずつLEDが点灯するタイマを作る
    - [ ] デバッガの確認

---

2020/06/22

- 今日の予定
  - [x] レビューのために、資料確認
    - [x] TM
    - [x] 状態遷移マトリクス
    - [x] 状態遷移図
  - [ ] 内部クロック、タイマーの仕様調べる
  - [ ] フローチャート
    - [ ] main
    - [ ] Input
    - [ ] Count
    - [ ] LCD
    - [ ] Buzzer

思いのほか、Input周りがきつい
先に、内部クロック、割込み、タイマあたりの仕様を調べたほうが良い
また、

- ボタンを押してからの処理
- 割込み処理で1秒カウント
- LCDへ表示
あたりの流れも、まとめた方が良いかと思う

---

2020/06/19

- 今日の予定
  - [x] 状態遷移図 2/2

- 時間があったら
  - [x] トレーサビリティマトリックス
  - [ ] フローチャート

- 今日の実績
  - 状態遷移図 終了の点 削除
  - フローチャート 作業中
  - [x] トレーサビリティマトリックス 作成・完了

---

2020/06/18

- 今日の予定
  - [x] 状態遷移図 1/2

- 時間があったら
  - [ ] トレーサビリティマトリックス
  - [x] フローチャート
  - [ ] テスト仕様書

- 今日の実績
  - [x] 状態遷移図 1/2
  - [x] 状態遷移図 2/2
  - [x] 状態遷移 マトリクス 表 追加、修正
  - [x] フローチャート 作成

- 状態遷移図をPlantUMLで書こうと思ったら、テキストの位置など、調整ができなかったため、draw.ioに変更

---

2020/06/17

- [x] スケジュール表 修正
  - [x] 土日 追加
  - [x] ~~実績と予定別で管理できるようにする~~
  - [x] 終了日の挙動の不具合の修正
  - [x] 変更履歴の追加

- [x] 状態遷移マトリクス 作成

---

2020/06/16

- [x] 仕様書 修正、完成
- [x] スケジュール 作成

---

2020/06/15

- [x] 第1回 仕様書 レビュー後、仕様書 修正
- [ ] ~~設計、実装、評価 スケジュール決定~~

---

期限：2020/6/12 まで

- [x] 仕様書 作り直し
  - [x] 文字での作業
    - [x] 仕様
    - [x] リセットの動作
    - [x] 出力、LCD 表現
    - [ ] ~~許容できる誤差~~
    - [x] 機能の仕様は、機能の項目に記載する
  - [x] excelでの作業
    - [x] レイアウト
    - [x] 絵、図
    - [x] LCDでの出力 表示デザイン
- [x] 構成図
  - [x] 出入口を複数にする
  - [x] マイコンやLCDの型番を書く

---

期限：2020/6/11 まで

- [x] 仕様書 下書き 作成する
  - [x] タイトル
  - [x] 概要
  - [x] 構成
  - [x] 入出力設定
    - [x] 入力
    - [x] 出力
  - [x] キッチンタイマーの仕様

---

期限：2020/6/11 まで

- [x] 仕様書 excelで作成する
  - [x] タイトル
  - [x] 概要
  - [x] 構成
  - [x] 入出力設定
    - [x] 入力
    - [x] 出力
  - [x] キッチンタイマーの仕様

---

## 仕様書 構成

1. タイトル
2. 概要
3. 構成（ブロック図など）
4. 機能
5. 入出力設定
  5.1. 入力
  5.2. 出力
6. キッチンタイマーの仕様

## 設計 構成資料

- スケジュール
- 状態遷移マトリクス
- 状態遷移図
- トレーサビリティマトリックス
- フローチャート
- テスト仕様書
