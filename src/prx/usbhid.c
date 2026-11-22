#include <pspkernel.h>
#include <pspsdk.h>
#include <psputilsforkernel.h>
#include <pspctrl.h>
#include <string.h>
#include "fusainterface.h"
#include "usbhid.h"

void sceDisplayEnable();
void sceDisplayDisable();
void sceDisplaySetBrightness( int, int );

/* stubs.S */
u32 sctrlHENFindFunction(char *modname, char *libname, u32 nid);
int sceLedSetMode(int led, int mode, void *config);

#define LED_MS   0  /* lit while AxisSwitcher is held    */
#define LED_WLAN 1  /* lit while DigitalSwitcher is held */

/* 0 = sample on VSYNC (60Hz) */
#define CTRL_SAMPLING_CYCLE 0

#define STICK_CENTER 127

struct UsbbdDeviceRequest UsbBulkinReq, UsbControlReq;
SceCtrlData pad;

int connected = 0, active = 1;
static unsigned char data[DATA_SIZE] __attribute__ ((aligned(64)));
GP_Config GPsettings = {
	PSP_CTRL_NOTE,      /* AxisSwitcher    */
	PSP_CTRL_WLAN_UP,   /* DigitalSwitcher */
	25,                 /* DeadZone        */

	PSP_CTRL_CROSS,
	PSP_CTRL_CIRCLE,
	PSP_CTRL_SQUARE,
	PSP_CTRL_TRIANGLE,
	PSP_CTRL_LTRIGGER,
	PSP_CTRL_RTRIGGER,
	PSP_CTRL_SELECT,
	PSP_CTRL_START,

	PSP_CTRL_HOME,
	PSP_CTRL_VOLDOWN,
	PSP_CTRL_VOLUP,
	PSP_CTRL_SCREEN,

	PSP_CTRL_RIGHT,
	PSP_CTRL_LEFT,
	PSP_CTRL_UP,
	PSP_CTRL_DOWN
};

PSP_MODULE_INFO ("FuSaPAD", 0x1006, 1, 1);

/*
 * sceIdStorageLookup hook.
 * The USB bus driver reads the vendor id and the manufacturer/product strings
 * from idstorage key 0x41. Patch them so the PC sees VID 0x00FF "FuSa GamePad"
 * instead of a Sony "PSP Type D" device.
 */
#define IDSTORAGE_LOOKUP_NID 0x6FE062D1
#define IDS_USB_KEY          0x41
#define IDS_USB_VID          0x000
#define IDS_USB_MANUFACTURER 0x004
#define IDS_USB_PRODUCT      0x118

#define MAKE_JUMP(f) (0x08000000 | (((u32)(f) & 0x0FFFFFFF) >> 2))

/* original first two instructions + jump back */
static u32 lookup_trampoline[4] __attribute__ ((aligned(16)));
static int (*sceIdStorageLookup_orig)(u16, u32, void *, u32) = (void *)lookup_trampoline;

static void put_utf16(unsigned char *buf, const char *s)
{
	/* buf[0..1] is the string descriptor header; characters start at 2 */
	int i;
	for (i = 0; s[i]; i++) buf[2 + i * 2] = s[i];
}

static int sceIdStorageLookup_patched(u16 key, u32 offset, void *buf, u32 len)
{
	int k1 = pspSdkSetK1(0);
	int ret = sceIdStorageLookup_orig(key, offset, buf, len);

	if (key == IDS_USB_KEY) {
		unsigned char *b = buf;
		if (offset == IDS_USB_VID) {
			b[0] = 0xFF;
			b[1] = 0x00;
		} else if (offset == IDS_USB_MANUFACTURER) {
			put_utf16(b, "FuSa");
		} else if (offset == IDS_USB_PRODUCT) {
			put_utf16(b, "FuSa GamePad");
		}
	}

	pspSdkSetK1(k1);
	return ret;
}

static u32 *find_idstorage_lookup(void)
{
	return (u32 *)sctrlHENFindFunction("sceIdStorage_Service", "sceIdStorage_driver", IDSTORAGE_LOOKUP_NID);
}

static void flush_caches(void)
{
	sceKernelDcacheWritebackAll();
	sceKernelIcacheInvalidateAll();
}

static void hook_idstorage(void)
{
	int intr = pspSdkDisableInterrupts();
	u32 *addr = find_idstorage_lookup();

	if (addr) {
		lookup_trampoline[0] = addr[0];
		lookup_trampoline[1] = addr[1];
		lookup_trampoline[2] = MAKE_JUMP(addr + 2);
		lookup_trampoline[3] = 0; /* nop (delay slot) */

		addr[0] = MAKE_JUMP(sceIdStorageLookup_patched);
		addr[1] = 0;
		flush_caches();
	}

	pspSdkEnableInterrupts(intr);
}

static void unhook_idstorage(void)
{
	int intr = pspSdkDisableInterrupts();
	u32 *addr = find_idstorage_lookup();

	if (addr) {
		addr[0] = lookup_trampoline[0];
		addr[1] = lookup_trampoline[1];
		flush_caches();
	}

	pspSdkEnableInterrupts(intr);
}

void fusaDisplay(int mode){
	if (mode) {
	sceDisplaySetBrightness( 68, 0 );
	sceDisplayEnable();
	} else {
	sceDisplaySetBrightness( 0, 0 );
	sceDisplayDisable();
	}
}

int fusaIsConnected(void)
{
	return(connected);
}

void fusaSetConfig(GP_Config * buf)
{
	memcpy(&GPsettings,buf,sizeof(GP_Config));
	return;
}

void fusaGetConfig(GP_Config * buf)
{
	memcpy(buf,&GPsettings,sizeof(GP_Config));
	return;
}

void fusaCtrlReadBufferPositive(SceCtrlData * Pad)
{

	sceCtrlReadBufferPositive(&pad, 1);
	memcpy(Pad,&pad,sizeof(pad));
	return;
}
/* Device request */
static void UsbBulkinReqDone( struct UsbbdDeviceRequest *req)
{
	req->unused = NULL;
	return;
}

void usbSendData(void *data, int size)
{
  if (!UsbBulkinReq.unused) {
		memset( &UsbBulkinReq, 0, sizeof(UsbBulkinReq) );
	  	UsbBulkinReq.endpoint = &endpoints[1];
		UsbBulkinReq.data = data;
		UsbBulkinReq.size = size;
		UsbBulkinReq.onComplete = &UsbBulkinReqDone;
		UsbBulkinReq.unused = &UsbBulkinReq;
		sceUsbbdReqSend( &UsbBulkinReq );
	}
	return;
}

void usbSendSetupPacket(void *data, int size, int length)
{
	if (!UsbControlReq.unused) {
		memset( &UsbControlReq, 0, sizeof(UsbControlReq) );
		UsbControlReq.data = data;
		UsbControlReq.size = size;
		UsbControlReq.endpoint = &endpoints[0];
		if (UsbControlReq.size > length)
		UsbControlReq.size = length;
		UsbControlReq.onComplete = &UsbBulkinReqDone;
		UsbControlReq.unused = &UsbControlReq;
		sceUsbbdReqSend (&UsbControlReq);
	}
	return;
}

static
int usb_recvctl (int arg1, int arg2, struct DeviceRequest *req)
{
  if ((req->bRequest == USB_REQ_GET_DESCRIPTOR) && ((req->wValue) == (USB_DT_REPORT << 8)) && (arg2 != -1)) {
  	  usbSendSetupPacket(ReportDescriptorGamepad,sizeof(ReportDescriptorGamepad),req->wLength);
  };

  return 0;
}

/* Alternate settings */
static
int usb_change (int interfaceNumber, int alternateSetting)
{
  return 0;
}

/* Attach callback */
static
int usb_attach (int usb_version)
{
  connected = 1;
  return 0;
}

/* Detach callback */
static
void usb_detach (void)
{
  connected = 0;
  return;
}

static
void usb_configure (int usb_version, int desc_count, struct InterfaceSettings *settings)
{
  return;
}

/* USB start function */
static
int start_func (int size, void *p)
{
  return 0;
}

/* USB stop function */
static
int stop_func (int size, void *p)
{
  return 0;
}

#define check_but(b) (((pad.Buttons & (b)) == (b)) ? 1 : 0)

/* Analog mode: snap the stick to the center inside the dead zone */
static void apply_deadzone(void)
{
	int dz = GPsettings.DeadZone;

	if (pad.Ly > STICK_CENTER - dz && pad.Ly < STICK_CENTER + dz) pad.Ly = STICK_CENTER;
	if (pad.Lx > STICK_CENTER - dz && pad.Lx < STICK_CENTER + dz) pad.Lx = STICK_CENTER;
}

/* Digital mode: the D-pad drives the axes and the stick drives the POV */
static void swap_dpad_and_stick(void)
{
	int dz = GPsettings.DeadZone;
	int lx = pad.Lx, ly = pad.Ly;
	unsigned int b = pad.Buttons;

	pad.Lx = STICK_CENTER;
	pad.Ly = STICK_CENTER;
	if (b & GPsettings.POV_UY) pad.Ly = 0;
	if (b & GPsettings.POV_RX) pad.Lx = 0xFF;
	if (b & GPsettings.POV_DY) pad.Ly = 0xFF;
	if (b & GPsettings.POV_LX) pad.Lx = 0;

	if (ly > STICK_CENTER + dz) b |= GPsettings.POV_DY; else b &= ~GPsettings.POV_DY;
	if (ly < STICK_CENTER - dz) b |= GPsettings.POV_UY; else b &= ~GPsettings.POV_UY;
	if (lx > STICK_CENTER + dz) b |= GPsettings.POV_RX; else b &= ~GPsettings.POV_RX;
	if (lx < STICK_CENTER - dz) b |= GPsettings.POV_LX; else b &= ~GPsettings.POV_LX;
	pad.Buttons = b;
}

int gamepad_int(SceSize args, void *argp)
{
	sceCtrlSetSamplingCycle(CTRL_SAMPLING_CYCLE);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	usb_start();

	while(active) {

	sceCtrlReadBufferPositive(&pad, 1);

	memset( data, 0x00, sizeof(data) );
	memset( data, 0x7F, AXIS_Rz+1 );

		if (pad.Buttons & GPsettings.DigitalSwitcher) {
			sceLedSetMode(LED_WLAN, 1, NULL);
			swap_dpad_and_stick();
		} else {
			apply_deadzone();
			sceLedSetMode(LED_WLAN, 0, NULL);
		}

		if (check_but(GPsettings.AxisSwitcher))
		{
		data[AXIS_Z] = pad.Ly;
		data[AXIS_Rz] = pad.Lx;
		sceLedSetMode(LED_MS, 1, NULL);
		} else {
		data[AXIS_X] = pad.Lx;
		data[AXIS_Y] = pad.Ly;
		sceLedSetMode(LED_MS, 0, NULL);
		};

		if ((pad.Buttons & GPsettings.POV_UY)) data[POV] = 1;
		if ((pad.Buttons & GPsettings.POV_RX)) data[POV] = 3;
		if ((pad.Buttons & GPsettings.POV_DY)) data[POV] = 5;
		if ((pad.Buttons & GPsettings.POV_LX)) data[POV] = 7;

		if ((pad.Buttons & GPsettings.POV_UY) && (pad.Buttons & GPsettings.POV_RX)) data[POV] = 2;
		if ((pad.Buttons & GPsettings.POV_RX) && (pad.Buttons & GPsettings.POV_DY)) data[POV] = 4;
		if ((pad.Buttons & GPsettings.POV_DY) && (pad.Buttons & GPsettings.POV_LX)) data[POV] = 6;
		if ((pad.Buttons & GPsettings.POV_LX) && (pad.Buttons & GPsettings.POV_UY)) data[POV] = 8;

		if (check_but(GPsettings.Button1)) data[BUTTONS_1_4] |= 0x01 << 4;
		if (check_but(GPsettings.Button2)) data[BUTTONS_1_4] |= 0x02 << 4;
		if (check_but(GPsettings.Button3)) data[BUTTONS_1_4] |= 0x04 << 4;
		if (check_but(GPsettings.Button4)) data[BUTTONS_1_4] |= 0x08 << 4;

		if (check_but(GPsettings.Button5)) data[BUTTONS_5_12] |= 0x01;
		if (check_but(GPsettings.Button6)) data[BUTTONS_5_12] |= 0x02;
		if (check_but(GPsettings.Button7)) data[BUTTONS_5_12] |= 0x04;
		if (check_but(GPsettings.Button8)) data[BUTTONS_5_12] |= 0x08;
		if (check_but(GPsettings.Button9)) data[BUTTONS_5_12] |= 0x10;
		if (check_but(GPsettings.Button10)) data[BUTTONS_5_12] |= 0x20;
		if (check_but(GPsettings.Button11)) data[BUTTONS_5_12] |= 0x40;
		if (check_but(GPsettings.Button12)) data[BUTTONS_5_12] |= 0x80;

	sceKernelDcacheWritebackRange (data, sizeof (data));

	if (sceUsbGetState () & PSP_USB_STATUS_CONNECTION_ESTABLISHED) usbSendData(data,DATA_SIZE);

	sceKernelDelayThread(4000);

	}

	usb_stop();

	return 0;
}

/* Usb start routine*/
int usb_start(void)
{
  int ret = 0;
  ret = sceUsbbdRegister (&driver);
  if (ret < 0) return ret;
  ret = sceUsbStart (PSP_USB_BUS_DRIVERNAME, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbStart ( PSP_USB_HID, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbActivate (PSP_USB_GAMEPAD_PID);
  return ret;
}

/* Usb stop */
int usb_stop (void)
{
  int ret;
  ret = sceUsbDeactivate ();
  if (ret < 0) return ret;

  ret = sceUsbStop (PSP_USB_HID, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbStop (PSP_USB_BUS_DRIVERNAME, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbbdUnregister (&driver);

  return ret;
}

int module_start (SceSize args, void *argp)
{
  hook_idstorage();

  SceUID main_thid = sceKernelCreateThread("fusapad_thread", gamepad_int, 32, 0x800, 0, NULL);
  if (main_thid >= 0) sceKernelStartThread(main_thid, args, argp);

  return 0;
}

int module_stop (SceSize args, void *argp)
{
  active = 0;
  unhook_idstorage();

  return 0;
}
