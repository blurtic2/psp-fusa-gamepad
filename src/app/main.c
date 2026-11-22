#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fusainterface.h"
#include "config.h"

PSP_MODULE_INFO("GAMEPAD", 0, 0, 2);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#define printf	pspDebugScreenPrintf

#define CONFIG_FILE    "config.ini"
#define CONFIG_SECTION "CONFIG"
#define VALUE_SIZE     50

int done = 0;
SceCtrlData pad;
GP_Config settings;
char buf[0xFF];

/* Used when config.ini is missing a key or gives an unknown value */
static const GP_Config default_settings = {
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

void waitbuttonup(int button)
{
	while ((pad.Buttons & button)) {
	fusaCtrlReadBufferPositive(&pad);
	sceKernelDelayThread(10*1000);
	}
}

#define pspDebugScreenSetX(a) pspDebugScreenSetXY(a,pspDebugScreenGetY());

void display_config();

void display_info(){

	pspDebugScreenSetXY(0,0);
	printf(" .: FuSa GAMEPAD :: v0.3 :: coded by Andy_maN :: www.foosa.do.am :.\n");
	pspDebugScreenSetXY(0,32);
	printf(" ': [         ] ::: [ %s ] :: [ AXESES: %s] :: rd.ver 1.0 :'\n",
		(pad.Buttons & settings.DigitalSwitcher) ? "DIGITAL" : "ANALOG ",
		(pad.Buttons & settings.AxisSwitcher) ? "Z/Rz" : "X/Y ");
}

void user_wanna_exit(){

	pspDebugScreenSetXY(22,30);
	printf("(X) EXIT | (O) CANCEL");

	display_config();

	int loop = 1;

	while (loop) {
	fusaCtrlReadBufferPositive(&pad);

	if (pad.Buttons & PSP_CTRL_CROSS) {
		waitbuttonup(PSP_CTRL_CROSS);
		done = 1;
		loop = 0;
		pspDebugScreenSetXY(22,30);
		printf("     Exiting...      ");
		};

	if (pad.Buttons & (PSP_CTRL_CIRCLE | PSP_CTRL_HOME)) {
	waitbuttonup((PSP_CTRL_CIRCLE | PSP_CTRL_HOME));
	loop = 0;
	pspDebugScreenSetXY(22,2);
	pspDebugScreenClear();
	display_info();
	}

	sceKernelDelayThread(10*1000);
	}


}

typedef struct {
	const char *name;  /* config.ini name    */
	const char *label; /* config screen text */
	unsigned int mask;
} ButtonName;

/* Order matches the original menu output */
static const ButtonName button_names[] = {
	{ "SELECT",   " (SELECT)", PSP_CTRL_SELECT   },
	{ "START",    " (START)",  PSP_CTRL_START    },
	{ "UP",       " UP",       PSP_CTRL_UP       },
	{ "RIGHT",    " RIGHT",    PSP_CTRL_RIGHT    },
	{ "DOWN",     " DOWN",     PSP_CTRL_DOWN     },
	{ "LEFT",     " LEFT",     PSP_CTRL_LEFT     },
	{ "LTRIGGER", " /L|",      PSP_CTRL_LTRIGGER },
	{ "RTRIGGER", " |R\\",     PSP_CTRL_RTRIGGER },
	{ "TRIANGLE", " (/\\)",    PSP_CTRL_TRIANGLE },
	{ "CIRCLE",   " (O)",      PSP_CTRL_CIRCLE   },
	{ "CROSS",    " (X)",      PSP_CTRL_CROSS    },
	{ "SQUARE",   " ([])",     PSP_CTRL_SQUARE   },
	{ "HOME",     " (HOME)",   PSP_CTRL_HOME     },
	{ "NOTE",     " (\x0d)",   PSP_CTRL_NOTE     },
	{ "SCREEN",   " ( )",      PSP_CTRL_SCREEN   },
	{ "VOLUP",    " (+)",      PSP_CTRL_VOLUP    },
	{ "VOLDOWN",  " (-)",      PSP_CTRL_VOLDOWN  },
	{ "WLAN",     " |WLAN|",   PSP_CTRL_WLAN_UP  },
};

#define BUTTON_NAME_COUNT (sizeof(button_names) / sizeof(button_names[0]))

char * button2str(unsigned int key)
{
	unsigned int i;

	if (!key) return " none";

	buf[0] = '\0';
	for (i = 0; i < BUTTON_NAME_COUNT; i++)
		if (key & button_names[i].mask) strcat(buf, button_names[i].label);
	return buf;
}

/* Returns the button mask for a config.ini name, 0 if unknown.
   Like the original, a value only has to start with the name. */
unsigned int str2button(const char *s)
{
	unsigned int i;

	for (i = 0; i < BUTTON_NAME_COUNT; i++)
		if (strncmp(s, button_names[i].name, strlen(button_names[i].name)) == 0)
			return button_names[i].mask;
	return 0;
}

void display_config(){
	int i;
	int *button = &settings.Button1;

	memset(buf, 0, sizeof(buf));

	fusaGetConfig(&settings);

	pspDebugScreenSetXY(22,4);

	printf("Axis Switcher  -%s\n",button2str(settings.AxisSwitcher));
	pspDebugScreenSetX(22);
	printf("Analog/Digital -%s\n",button2str(settings.DigitalSwitcher));
	pspDebugScreenSetX(22);
	printf("Deadzone:      - %d\n\n",settings.DeadZone);
	pspDebugScreenSetX(22);
	printf("POV RIGHT:     -%s\n",button2str(settings.POV_RX));
	pspDebugScreenSetX(22);
	printf("POV LEFT:      -%s\n",button2str(settings.POV_LX));
	pspDebugScreenSetX(22);
	printf("POV UP:        -%s\n",button2str(settings.POV_UY));
	pspDebugScreenSetX(22);
	printf("POV DOWN:      -%s\n\n",button2str(settings.POV_DY));
	pspDebugScreenSetX(22);

	for (i = 0; i < 12; i++) {
		printf("Button%2d:      -%s\n", i + 1, button2str(button[i]));
		pspDebugScreenSetX(22);
	}
}

static void load_button(const char *key, int *value)
{
	char str[VALUE_SIZE];
	unsigned int mask;

	ini_gets(CONFIG_SECTION, key, "NULL", str, sizeof(str), CONFIG_FILE);
	mask = str2button(str);
	if (mask) *value = mask;
}

void load_config(){
	char str[VALUE_SIZE];
	char key[16];
	int deadzone, i;
	int *button = &settings.Button1;

	settings = default_settings;

	load_button("AxiSwitcher", &settings.AxisSwitcher);
	load_button("DigitalSwitcher", &settings.DigitalSwitcher);

	ini_gets(CONFIG_SECTION, "DeadZone", "NULL", str, sizeof(str), CONFIG_FILE);
	deadzone = atoi(str) & 0x7F;
	if (deadzone) settings.DeadZone = deadzone;

	load_button("POV_RX", &settings.POV_RX);
	load_button("POV_LX", &settings.POV_LX);
	load_button("POV_UY", &settings.POV_UY);
	load_button("POV_DY", &settings.POV_DY);

	for (i = 0; i < 12; i++) {
		sprintf(key, "Button%d", i + 1);
		load_button(key, &button[i]);
	}
}

int main(void)
{
	pspDebugScreenInit();

	display_info();

	SceUID mod;
	mod = pspSdkLoadStartModule("usbgamepad.prx", PSP_MEMORY_PARTITION_KERNEL);

	if (mod < 0) {
	pspDebugScreenSetXY(2,3);
	printf(" Error - driver was not loaded\nExitting...");
	sceKernelDelayThread(2000*1000);
	done = 1;
	} else {
	load_config();
	fusaSetConfig(&settings);
	}

	int count = 1;

	while(!done){

		if (fusaIsConnected()) {

			/* progress stars along the bottom line before the screen turns off */
			if (count % 100 == 0) {
				pspDebugScreenSetXY(count / 100 + 6, 32);
				printf("*");
			}

			if (count > 700) fusaDisplay(0); else count++;

			pspDebugScreenSetXY(25,15);
			printf(":: CONNECTED ::\n");
		} else {

			fusaDisplay(1);

			fusaCtrlReadBufferPositive(&pad);

			pspDebugScreenSetXY(25,15);
			printf(">-CONNECTING-->\n");

			if (pad.Buttons & PSP_CTRL_HOME){
			waitbuttonup(PSP_CTRL_HOME);
			user_wanna_exit();
			}

			display_info();
			count = 0;
		}

		sceKernelDelayThread(10*1000);
	}

	sceKernelExitGame();
	return 0;
}
