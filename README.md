# FuSa GAMEPAD

**English** · [한국어](README.ko.md) · [日本語](README.ja.md)

> **About this fork**
>
> The upstream repository only contains the v0.2 source, while the released binary is v0.3.
> This fork reconstructs the v0.3 behaviour in source (config.ini, dead zone, digital mode,
> LED indication, "FuSa GamePad" USB identity) and adds two latency fixes:
>
> * Interrupt endpoint: `wMaxPacketSize` 3/4 → 8 (one packet per report) and
>   `bInterval` 10 → 1. At USB high speed the old value meant a 64 ms polling interval.
> * Controller sampling: `sceCtrlSetSamplingCycle(0)` (VSYNC, 60 Hz) → `5555` µs (~180 Hz).
>
> `SCREEN` is also accepted in config.ini (v0.3 ignored it).
>
> **Build** with [pspdev](https://github.com/pspdev/pspdev) (Linux / WSL / macOS): `./build.sh`,
> then copy `bin/FusaGamePad` to `<your_psp>:/PSP/GAME/`.

Perhaps a simpliest way to turn your psp into a powerful PC gamepad (and yes PS3 too)

* Just launch this tiny homebrew 
* Connect your psp via USB cable to PC (PS3)
* And you are ready to go :)

### Installation

Just put FusaGamePad folder into <your_psp>:/PSP/GAME folder

If you are using PHAT psp you should put it into <your_psp>:/PSP/GAMEXXX folder

(where XXX means your FW version - f.e. GAME380 for CFW 3.80)

### Compatibilty

Any psp (Phat,S&L,3K) running CFW 3.80 and newer

This gamepad was designed as usual HID device, so that means you don't need to install any drivers. It's compatible with all modern OSes (Windows, MacOs, Linux etc)

It also compatible with PS3 console.

v0.4 of this fork was tested on a PSP-3000 with 6.61 PRO-C Infinity, connected straight to a Windows PC by USB cable. Connecting through a USB hub did not work on that setup.

### What kind of GAMEPAD will I get?

4 axises: (X/Y/Z/Rz) (Analog Joystick)

8-directional POV: (Digital Pad)

And 12 buttons:
| Buttons | PSP |
| --- | --- |
| Button 1 | CROSS |
| Button 2 | CIRCLE |
| Button 3 | SQUARE |
| Button 4 | TRIANGLE |
| Button 5 | L-TRIGGER |
| Button 6 | R-TRIGGER |
| Button 7 | SELECT |
| Button 8 | START |
| Button 9 | HOME |
| Button 10 | VOLDOWN |
| Button 11 | VOLUP |
| Button 12 | DISPLAY |

### How to use:

This version allows yo to map buttons your own way.

Open CONFIG.INI with any text editor software.

**Available buttons names are:**

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

AxiSwitcher serves to switch between X/Y and Z/Rz axes (Left and Right stick)

*Note: the Memory Stick LED lights up while you hold AxiSwitcher.*

**DigitalSwitcher**

DigitalSwitcher serves to switch between Analog and Digital modes.

Analog mode: psp's joystick controls axes and directional buttons control POV

Digital mode: psp's joystick controls POV and directional buttons control axes

*Note: the WLAN LED lights up while Digital mode is on.*

**DeadZone**

Use it to adjust joystick's sensivity.

Allowed values: 1-127

**How to enter the menu?**

Disconnect USB cable if it's connected and press HOME button.

Now you can view current button configuration.

To Exit menu: press HOME or (O)

To Exit the application: press (X)

### Links

* Homepage: [foosa.do.am](http://foosa.do.am)

## Donate

* **[Ko-fi/Paypal](https://ko-fi.com/andymandev)**
* **[Patreon](https://patreon.com/andy_man)**
* **[Boosty](https://boosty.to/andy_man/donate)**
* **[YandexMoney](https://yoomoney.ru/to/410011555252085)**
* **Bitcoin**: 39VaMnFqCQo751mvDc3M7ADVty71q2tWDm 
* **TronLink**: TSYe254HXFapEwAAbZFkTHGewezZmnCKnU

### Developers

hnaves - USB driver research

Forb - help with ReportDescriptor

Andy_maN (c) 2010

-= FuSa TeaM =-

### Versions

***
**v0.4** (this fork)
* Much lower input latency: USB polling interval 64 ms → 125 µs, one packet per report
* Controller sampling 60 Hz → ~180 Hz
* v0.3 rebuilt from source; `SCREEN` accepted in config.ini
***
**v0.3**
* Led indicating
* Adjustable configuration (via config.ini)
* Deadzone support
* Digital/Analog modes
* No conflicts with drivers ("PSP type D" => "FuSa GamePad")
***
**v0.2**
* 4-Axises (w switcher) 
* 8-directional POV
* 12 Buttons
***
**v0.1**
* Initial release
* Plug'n'Play
* Does not requires any drivers


