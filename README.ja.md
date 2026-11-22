# FuSa GAMEPAD

[English](README.md) · [한국어](README.ko.md) · **日本語**

> **このフォークについて**
>
> オリジナルのリポジトリには v0.2 のソースしかなく、配布されているバイナリは v0.3 です。
> このフォークでは v0.3 の動作（config.ini、デッドゾーン、デジタルモード、LED 表示、「FuSa GamePad」の USB デバイス情報）を
> ソースとして再構築し、遅延を減らす修正を 2 つ加えました。
>
> * インタラプトエンドポイント：`wMaxPacketSize` 3/4 → 8（1 レポートを 1 パケットで）、
>   `bInterval` 10 → 1。USB ハイスピード接続では、元の値はポーリング間隔 64ms を意味していました。
> * コントローラーのサンプリング：`sceCtrlSetSamplingCycle(0)`（VSYNC、60Hz）→ `5555`µs（約 180Hz）。
>
> config.ini で `SCREEN` も使えます（v0.3 では無視されていました）。
>
> **ビルド**は [pspdev](https://github.com/pspdev/pspdev)（Linux / WSL / macOS）で `./build.sh` を実行し、
> `bin/FusaGamePad` を `<PSP>:/PSP/GAME/` にコピーしてください。

PSP を強力な PC 用ゲームパッドにする、おそらく一番簡単な方法です（PS3 でも使えます）。

* この小さな自作ソフトを起動して
* PSP を USB ケーブルで PC（PS3）に接続すれば
* すぐに使えます :)

### インストール

FusaGamePad フォルダを `<PSP>:/PSP/GAME` フォルダに入れてください。

初代 PSP（1000）で古い CFW を使っている場合は、`<PSP>:/PSP/GAMEXXX` フォルダに入れる必要があります。

（XXX はファームウェアのバージョンです。例：CFW 3.80 なら GAME380）

### 対応環境

CFW 3.80 以降が動作するすべての PSP（1000、2000/スリム、3000）

一般的な HID デバイスとして作られているため、ドライバのインストールは不要です。最近の OS（Windows、macOS、Linux など）すべてに対応しています。

PS3 でも使えます。

このフォークの v0.4 は、PSP-3000、6.61 PRO-C Infinity、Windows PC に USB ケーブルを直結した環境でテストしました。この環境では USB ハブ経由だと接続できませんでした。

### どんなゲームパッドになりますか？

4 軸：X/Y/Z/Rz（アナログスティック）

8 方向 POV（十字キー）

12 ボタン（初期設定）：
| ボタン | PSP |
| --- | --- |
| Button 1 | CROSS（×） |
| Button 2 | CIRCLE（○） |
| Button 3 | SQUARE（□） |
| Button 4 | TRIANGLE（△） |
| Button 5 | L トリガー |
| Button 6 | R トリガー |
| Button 7 | SELECT |
| Button 8 | START |
| Button 9 | HOME |
| Button 10 | 音量 − |
| Button 11 | 音量 + |
| Button 12 | 画面（SCREEN） |

### 使い方

ボタン配置を自由に変更できます。

`config.ini` を任意のテキストエディタで開きます。

**使用できるボタン名：**

* NOTE
* WLAN
* RIGHT
* LEFT
* UP
* DOWN
* CROSS
* CIRCLE
* SQUARE
* TRIANGLE
* LTRIGGER
* RTRIGGER
* SELECT
* START
* HOME
* VOLDOWN
* VOLUP
* SCREEN

**AxiSwitcher**

押している間、スティックが X/Y 軸の代わりに Z/Rz 軸を動かします（左スティック ↔ 右スティック）。

*注：AxiSwitcher を押している間、メモリースティックの LED が点灯します。*

**DigitalSwitcher**

アナログモードとデジタルモードを切り替えます。

アナログモード：PSP のスティックが軸を、方向ボタンが POV を担当します。

デジタルモード：PSP のスティックが POV を、方向ボタンが軸を担当します。

*注：デジタルモードの間、WLAN の LED が点灯します。*

**DeadZone**

スティックのデッドゾーン（中央付近で反応しない範囲）を調整します。

設定できる値：1–127

**メニューの開き方**

USB ケーブルが接続されている場合は抜いてから、HOME ボタンを押します。

現在のボタン設定を確認できます。

メニューを閉じる：HOME または (O)

アプリを終了する：(X)

### リンク

* 作者のホームページ：[foosa.do.am](http://foosa.do.am)
* オリジナルのリポジトリ：[andy-man/psp-fusa-gamepad](https://github.com/andy-man/psp-fusa-gamepad)

## 寄付（原作者）

* **[Ko-fi/Paypal](https://ko-fi.com/andymandev)**
* **[Patreon](https://patreon.com/andy_man)**
* **[Boosty](https://boosty.to/andy_man/donate)**
* **[YandexMoney](https://yoomoney.ru/to/410011555252085)**
* **Bitcoin**: 39VaMnFqCQo751mvDc3M7ADVty71q2tWDm
* **TronLink**: TSYe254HXFapEwAAbZFkTHGewezZmnCKnU

### 開発者

hnaves - USB ドライバの研究

Forb - ReportDescriptor の協力

Andy_maN (c) 2010

-= FuSa TeaM =-

### バージョン

***
**v0.4**（このフォーク）
* 入力遅延を大幅に削減：USB ポーリング間隔 64ms → 125µs、1 レポートを 1 パケットで送信
* コントローラーのサンプリング 60Hz → 約 180Hz
* v0.3 をソースとして再構築、config.ini で `SCREEN` に対応
***
**v0.3**
* LED 表示
* 設定変更が可能（config.ini）
* デッドゾーン対応
* デジタル/アナログモード
* 他のドライバと競合しない（「PSP type D」→「FuSa GamePad」）
***
**v0.2**
* 4 軸（切り替えボタン付き）
* 8 方向 POV
* 12 ボタン
***
**v0.1**
* 最初のリリース
* プラグアンドプレイ
* ドライバ不要
