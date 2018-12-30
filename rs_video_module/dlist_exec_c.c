#include <general.h>
#include <module.h>
#include "rdpcommand.h"
#include "config_options.h"
#include "dlist_exec_1.h"
#include "rcp_state.h"

void exec_command(rdp_command cmd, int depth) {

	switch(RDP_CMD(cmd)) {
		case 0x00:
			printd(D_RCP, D_FATAL, "Jumped off a cliff!!\n");
			exit(0);
			break;
		case 0x01:
			perform_mtx(cmd);
			break;
		case 0x03:
			perform_movemem(cmd);
			break;
		case 0x04:
			perform_vtx(cmd);
			break;
		case 0x06:
			perform_dl(cmd, depth);
			break;
		case 0x09:
			perform_spritebase(cmd);
		case 0xb6:
			perform_cleargeometrymode(cmd);
			break;
		case 0xb7:
			perform_setgeometrymode(cmd);
			break;
		case 0xb8:
			break; // ret
		case 0xb9:
			perform_setothermode_l(cmd);
			break;
		case 0xba:
			perform_setothermode_h(cmd);
			break;
		case 0xbb:
			perform_texture(cmd);
			break;
		case 0xbc:
			perform_moveword(cmd);
			break;
		case 0xbf:
			perform_tri1(cmd);
			break;
		case 0xc0:
			break; // nop
		case 0xff:
			perform_setcimg(cmd);
			break;
		case 0xfe:
			perform_setzimg(cmd);
			break;
		case 0xfd:
			perform_settimg(cmd);
			break;
		case 0xf7:
			perform_setfillcolor(cmd);
			break;
		case 0xf6:
//			perform_fillrect(cmd);
			break;
		case 0xed:
			perform_setscissor(cmd);
			break;
		case 0xe7:
//			perform_rdppipesync(cmd);
			break;
		default:
			printd(D_RCP, D_WARN, "No method of execution specified for %#.2x\n", RDP_CMD(cmd));
			if(rcp_break_unim) {
				dump_state();
				exit(0);
			}
			break;
	}
}
