# FuSa GAMEPAD

[English](README.md) · **한국어** · [日本語](README.ja.md)

> **이 포크에 대해**
>
> 원본 저장소에는 v0.2 소스만 있고, 배포된 바이너리는 v0.3입니다.
> 이 포크는 v0.3의 동작(config.ini, 데드존, 디지털 모드, LED 표시, "FuSa GamePad" USB 장치 정보)을
> 소스로 재구현하고, 지연을 줄이는 수정 두 가지를 더했습니다.
>
> * 인터럽트 엔드포인트: `wMaxPacketSize` 3/4 → 8(리포트 하나를 패킷 하나로),
>   `bInterval` 10 → 1. USB 고속 연결에서 기존 값은 폴링 간격 64ms를 뜻했습니다.
> * 컨트롤러 샘플링: `sceCtrlSetSamplingCycle(0)`(VSYNC, 60Hz) → `5555`µs(약 180Hz).
>
> config.ini에서 `SCREEN`도 인식합니다(v0.3은 무시했습니다).
>
> **빌드**는 [pspdev](https://github.com/pspdev/pspdev)(Linux / WSL / macOS)에서 `./build.sh`를 실행하고,
> `bin/FusaGamePad`를 `<PSP>:/PSP/GAME/`에 복사하면 됩니다.

PSP를 강력한 PC 게임패드로 만드는 가장 간단한 방법일 겁니다(PS3에서도 됩니다).

* 이 작은 홈브류를 실행하고
* PSP를 USB 케이블로 PC(PS3)에 연결하면
* 바로 쓸 수 있습니다 :)

### 설치

FusaGamePad 폴더를 `<PSP>:/PSP/GAME` 폴더에 넣으면 됩니다.

구형 PSP(1000)에서 오래된 CFW를 쓴다면 `<PSP>:/PSP/GAMEXXX` 폴더에 넣어야 합니다.

(XXX는 펌웨어 버전입니다. 예: CFW 3.80이면 GAME380)

### 호환성

CFW 3.80 이상을 쓰는 모든 PSP(1000, 2000/슬림, 3000)

일반적인 HID 장치로 만들었기 때문에 드라이버를 설치할 필요가 없습니다. 최신 OS(Windows, macOS, Linux 등)와 모두 호환됩니다.

PS3에서도 쓸 수 있습니다.

이 포크의 v0.4는 PSP-3000, 6.61 PRO-C Infinity, Windows PC에 USB 케이블을 직접 연결한 환경에서 테스트했습니다. 이 환경에서 USB 허브를 거치면 연결되지 않았습니다.

### 어떤 게임패드가 되나요?

축 4개: X/Y/Z/Rz(아날로그 스틱)

8방향 POV(십자키)

버튼 12개(기본 설정):
| 버튼 | PSP |
| --- | --- |
| Button 1 | CROSS (×) |
| Button 2 | CIRCLE (○) |
| Button 3 | SQUARE (□) |
| Button 4 | TRIANGLE (△) |
| Button 5 | L 트리거 |
| Button 6 | R 트리거 |
| Button 7 | SELECT |
| Button 8 | START |
| Button 9 | HOME |
| Button 10 | 볼륨 − |
| Button 11 | 볼륨 + |
| Button 12 | 화면(SCREEN) |

### 사용법

버튼 배치를 원하는 대로 바꿀 수 있습니다.

`config.ini`를 아무 텍스트 편집기로 엽니다.

**쓸 수 있는 버튼 이름:**

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

누르고 있는 동안 스틱이 X/Y 축 대신 Z/Rz 축을 움직입니다(왼쪽 스틱 ↔ 오른쪽 스틱).

*참고: AxiSwitcher를 누르고 있는 동안 메모리스틱 LED가 켜집니다.*

**DigitalSwitcher**

아날로그 모드와 디지털 모드를 전환합니다.

아날로그 모드: PSP 스틱이 축을, 방향 버튼이 POV를 담당합니다.

디지털 모드: PSP 스틱이 POV를, 방향 버튼이 축을 담당합니다.

*참고: 디지털 모드인 동안 WLAN LED가 켜집니다.*

**DeadZone**

스틱 데드존(중앙에서 반응하지 않는 범위)을 조절합니다.

설정 가능한 값: 1–127

**메뉴는 어떻게 여나요?**

USB 케이블이 연결되어 있으면 뺀 다음 HOME 버튼을 누릅니다.

현재 버튼 설정을 볼 수 있습니다.

메뉴 닫기: HOME 또는 (O)

프로그램 종료: (X)

### 링크

* 원작자 홈페이지: [foosa.do.am](http://foosa.do.am)
* 원본 저장소: [andy-man/psp-fusa-gamepad](https://github.com/andy-man/psp-fusa-gamepad)

## 후원 (원작자)

* **[Ko-fi/Paypal](https://ko-fi.com/andymandev)**
* **[Patreon](https://patreon.com/andy_man)**
* **[Boosty](https://boosty.to/andy_man/donate)**
* **[YandexMoney](https://yoomoney.ru/to/410011555252085)**
* **Bitcoin**: 39VaMnFqCQo751mvDc3M7ADVty71q2tWDm
* **TronLink**: TSYe254HXFapEwAAbZFkTHGewezZmnCKnU

### 개발자

hnaves - USB 드라이버 연구

Forb - ReportDescriptor 도움

Andy_maN (c) 2010

-= FuSa TeaM =-

### 버전

***
**v0.4** (이 포크)
* 입력 지연 대폭 감소: USB 폴링 간격 64ms → 125µs, 리포트 하나를 패킷 하나로 전송
* 컨트롤러 샘플링 60Hz → 약 180Hz
* v0.3을 소스로 재구현, config.ini에서 `SCREEN` 인식
***
**v0.3**
* LED 표시
* 설정 변경 가능(config.ini)
* 데드존 지원
* 디지털/아날로그 모드
* 다른 드라이버와 충돌 없음("PSP type D" → "FuSa GamePad")
***
**v0.2**
* 축 4개(전환 버튼 포함)
* 8방향 POV
* 버튼 12개
***
**v0.1**
* 첫 릴리스
* 플러그 앤 플레이
* 드라이버 필요 없음
