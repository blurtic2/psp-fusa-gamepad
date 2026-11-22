	.set noreorder

#include "pspstub.s"

/* Kernel imports that PSPSDK does not provide.
   NIDs and library attributes match the original v0.3 binary. */

	STUB_START "SystemCtrlForKernel",0x00090000,0x00010005
	STUB_FUNC  0x159AF5CC,sctrlHENFindFunction
	STUB_END

	STUB_START "sceLed_driver",0x00010011,0x00010005
	STUB_FUNC  0xEA24BE03,sceLedSetMode
	STUB_END
