#ifdef WIN32
	#include <windows.h>
#endif

#include <general.h>
#include <module.h>
#include <memory.h>
#include "rdpcommand.h"
#include "config_options.h"
#include "render.h"
#include "rcp_state.h"
#include "rcp_core.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

static char* moveword_indices[] = { "G_MW_MATRIX", "G_MW_NUMLIGHT",
	"G_MW_CLIP", "G_MW_SEGMENT", "G_MW_FOG", "G_MW_LIGHTCOL",
	"G_MW_FORCEMTX", "G_MW_PERSPNORM" };

static char* movemem_indices[] = { "G_MV_VIEWPORT", "G_MV_LOOKATY",
	"G_MV_LOOKATX", "G_MV_L0", "G_MV_L1", "G_MV_L2", "G_MV_L3",
	"G_MV_L4", "G_MV_L5", "G_MV_L6", "G_MV_L7", "G_MV_TXTATT",
	"G_MV_MATRIX_1", "G_MV_MATRIX_2", "G_MV_MATRIX_3",
	"G_MV_MATRIX_4" };

extern void exec_command(rdp_command, int);

void disassemble_command(rdp_command, int);
void do_displaylist(uint8*, int);

uint32 do_crc(OSTask* task) {

	int i;
	uint32 crc = 0;
	uint32* data;

	data = (uint32*)(modules->cpu->translate_addr_f((uint32)task->ucode));

	for (i = 0; i < (task->ucode_size>>2); i++) {
		crc += *(data+i);
	}

	return crc;
}

int guess_ucode(uint32 crc) {

	switch(crc) {
		case 0x4f4903bb:
			return 0;
			break;
		default:
			printd(D_RCP, D_CRIT, "Unknown ucode!\n");
			break;
	}

	return -1;
}

void install_ucode(int ucode) {

	switch(ucode) {
		case 0:
			register_ucode00();
			break;
		default:
			printd(D_RCP, D_EMERG, "Unable to install pointers for this ucode\n");
			break;
	}
}

void auto_ucode(OSTask* task) {

	uint32 crc;
	int ucode_id;

	crc = do_crc(task);

	ucode_id = guess_ucode(crc);
	install_ucode(ucode_id);
	dlist_type = ucode_id;

	printd(D_RCP, D_INFO, "ucode %i successfully installed\n", ucode_id);
}

void rcp_command(OSTask* task) {

	if (dlist_type == -1) {
		auto_ucode(task);
	}

	reset_stacks();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rcp_core(task);

	SDL_GL_SwapBuffers();

}

void old_rcp_command(OSTask* task) {

	uint8* data;
	int num;

	num = task->data_size>>3;

	printd(D_RCP, D_DEBUG, "Data contains %i commands\n", num);

/*	for (i = 0; i < num; i++) {
		printd(D_RCP, D_DEBUG, "Command %i: %.2x %.2x %.2x %.2x - %.2x %.2x %.2x %.2x\n", i, data[(i*8)+3], data[(i*8)+2], data[(i*8)+1], data[(i*8)], data[(i*8)+7], data[(i*8)+6], data[(i*8)+5], data[(i*8)+4]);
		disassemble_command(data);
	}*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClear(GL_DEPTH_BUFFER_BIT);

	reset_stacks();

	data = (uint8*)(modules->cpu->translate_addr_f(task->data_ptr));
	do_displaylist(data, 0);
	SDL_GL_SwapBuffers();

}

void do_displaylist(uint8* data, int depth) { 

	int i, j;
	rdp_command cmd;
	uint8* cptr;

	i = 0;

	while (RDP_CMD(cmd) != 0xb8) {
		cptr = data + (i * sizeof(uint64));
		cmd.b64 = *(uint64*)cptr;
		if (test_debug(D_RCP, D_DEBUG)) {
			for (j = 0; j < depth; j++) { putchar(' '); }
			printd(D_RCP, D_DEBUG, "Command %i: %.2x %.2x %.2x %.2x - %.2x %.2x %.2x %.2x\n", i, cmd.b8[3], cmd.b8[2], cmd.b8[1], cmd.b8[0], cmd.b8[7], cmd.b8[6], cmd.b8[5], cmd.b8[4]);
			for (j = 0; j < depth; j++) { putchar(' '); }
			disassemble_command(cmd, depth);
		}
		exec_command(cmd, depth);
		i++;
	}

}

void disassemble_command(rdp_command cmd, int depth) {

	switch(RDP_CMD(cmd)) {
		case 0x00:
			printd(D_RCP, D_DEBUG, "G_SPNOOP\n");
			break;
		case 0x01:
			printd(D_RCP, D_DEBUG, "G_MTX matrix: %#.8x size: %i", G_DL_S(cmd), G_DL_L(cmd));
			if (G_DL_P(cmd) & G_MTX_PROJECTION) {printd(D_RCP, D_DEBUG, " PROJ");}
			if (G_DL_P(cmd) & G_MTX_LOAD) { printd(D_RCP, D_DEBUG, " LOAD"); }
			if (G_DL_P(cmd) & G_MTX_PUSH) { printd(D_RCP, D_DEBUG, " PUSH"); }
			printd(D_RCP, D_DEBUG, "\n");
			break;
		case 0x02:
			printd(D_RCP, D_DEBUG, "G_RESERVED0\n");
			break;
		case 0x03:
			printd(D_RCP, D_DEBUG, "G_MOVEMEM index: %s len: %i at %#.8x\n", movemem_indices[((G_DL_P(cmd)-0x80)>>1)], G_DL_L(cmd), G_DL_S(cmd));
			break;
		case 0x04:
			printd(D_RCP, D_DEBUG, "G_VTX start: %i n: %i(%i) len: %i data: %#.8x\n", G_VTX_V0(cmd), G_VTX_N(cmd), G_VTX_NFAST(cmd), G_VTX_L(cmd), G_VTX_V(cmd));
			break;
		case 0x05:
			printd(D_RCP, D_DEBUG, "G_RESERVED1\n");
			break;
		case 0x06:
			printd(D_RCP, D_DEBUG, "G_DL push: %#.2x data: %#.8x\n", G_DL_P(cmd), G_DL_S(cmd));
			break;
		case 0x07:
			printd(D_RCP, D_DEBUG, "G_RESERVED2\n");
			break;
		case 0x08:
			printd(D_RCP, D_DEBUG, "G_RESERVED3\n");
			break;
		case 0x09:
			printd(D_RCP, D_DEBUG, "G_SPRITE2D_BASE sizeof: %#x s: %#.8x\n", G_SPRITE2D_BASE_L(cmd), G_SPRITE2D_BASE_S(cmd));
			break;
		case 0xbf:
			printd(D_RCP, D_DEBUG, "G_TRI1 flags: %#x v0: %i v1: %i v2: %i\n", G_TRI1_FLAGS(cmd), G_TRI1_V0(cmd), G_TRI1_V1(cmd), G_TRI1_V2(cmd));
			break;
		case 0xbe:
			printd(D_RCP, D_DEBUG, "G_CULLDL\n");
			break;
		case 0xbd:
			printd(D_RCP, D_DEBUG, "G_POPMTX\n");
			break;
		case 0xbc:
			printd(D_RCP, D_DEBUG, "G_MOVEWORD index: %s offset: %#x data: %#.8x\n", moveword_indices[G_MOVEWORD_P1(cmd)>>1], G_MOVEWORD_P0(cmd), G_MOVEWORD_DAT(cmd));
			break;
		case 0xbb:
			printd(D_RCP, D_DEBUG, "G_TEXTURE\n");
			break;
		case 0xba:
			printd(D_RCP, D_DEBUG, "G_SETOTHERMODE_H\n");
			break;
		case 0xb9:
			printd(D_RCP, D_DEBUG, "G_SETOTHERMODE_L\n");
			break;
		case 0xb8:
			printd(D_RCP, D_DEBUG, "G_ENDDL\n");
			break;
		case 0xb7:
			printd(D_RCP, D_DEBUG, "G_SETGEOMETRYMODE\n");
			break;
		case 0xb6:
			printd(D_RCP, D_DEBUG, "G_CLEARGEOMETRYMODE\n");
			break;
		case 0xb5:
			printd(D_RCP, D_DEBUG, "G_LINE3D\n");
			break;
		case 0xb4:
			printd(D_RCP, D_DEBUG, "G_RDPHALF_1\n");
			break;
		case 0xb3:
			printd(D_RCP, D_DEBUG, "G_RDPHALF_2\n");
			break;
		case 0xc0:
			printd(D_RCP, D_DEBUG, "G_NOOP\n");
			break;
		case 0xff:
			printd(D_RCP, D_DEBUG, "G_SETCIMG fmt: %#x size: %#x width: %#x i: %#x\n", G_SETCIMG_F(cmd), G_SETCIMG_S(cmd), G_SETCIMG_W(cmd), G_SETCIMG_I(cmd));
			break;
		case 0xfe:
                        printd(D_RCP, D_DEBUG, "G_SETZIMG fmt: %#x size: %#x width: %#x i: %#x\n", G_SETCIMG_F(cmd), G_SETCIMG_S(cmd), G_SETCIMG_W(cmd), G_SETCIMG_I(cmd));
			break;
		case 0xfd:
			printd(D_RCP, D_DEBUG, "G_SETTIMG fmt: %#x size: %#x width: %#x i: %#x\n", G_SETCIMG_F(cmd), G_SETCIMG_S(cmd), G_SETCIMG_W(cmd), G_SETCIMG_I(cmd));
			break;
		case 0xfc:
			printd(D_RCP, D_DEBUG, "G_SETCOMBINE\n");
			break;
		case 0xfb:
			printd(D_RCP, D_DEBUG, "G_SETENVCOLOR\n");
			break;
		case 0xfa:
			printd(D_RCP, D_DEBUG, "G_SETPRIMCOLOR\n");
			break;
		case 0xf9:
			printd(D_RCP, D_DEBUG, "G_SETBLENDCOLOR\n");
			break;
		case 0xf8:
			printd(D_RCP, D_DEBUG, "G_SETFOGCOLOR\n");
			break;
		case 0xf7:
			printd(D_RCP, D_DEBUG, "G_SETFILLCOLOR r: %#x g: %#x b: %#x a: %#x\n", G_SETFILLCOLOR_R(cmd), G_SETFILLCOLOR_G(cmd), G_SETFILLCOLOR_B(cmd), G_SETFILLCOLOR_A(cmd));
			break;
		case 0xf6:
			printd(D_RCP, D_DEBUG, "G_FILLRECT (%i, %i) -> (%i, %i)\n", G_FILLRECT_ULX(cmd), G_FILLRECT_ULY(cmd), G_FILLRECT_LRX(cmd), G_FILLRECT_LRY(cmd));
			break;
		case 0xf5:
			printd(D_RCP, D_DEBUG, "G_SETTILE\n");
			break;
		case 0xf4:
			printd(D_RCP, D_DEBUG, "G_LOADTILE\n");
			break;
		case 0xf3:
			printd(D_RCP, D_DEBUG, "G_LOADBLOCK\n");
			break;
		case 0xf2:
			printd(D_RCP, D_DEBUG, "G_SETTILESIZE\n");
			break;
		case 0xf0:
			printd(D_RCP, D_DEBUG, "G_LOADTLUT\n");
			break;
		case 0xef:
			printd(D_RCP, D_DEBUG, "G_RDPSETOTHERMODE\n");
			break;
		case 0xee:
			printd(D_RCP, D_DEBUG, "G_SETPRIMDEPTH\n");
			break;
		case 0xed:
			printd(D_RCP, D_DEBUG, "G_SETSCISSOR (%i, %i) -> (%i, %i) mode: %x\n", G_SETSCISSOR_ULX(cmd), G_SETSCISSOR_ULY(cmd), G_SETSCISSOR_LRX(cmd), G_SETSCISSOR_LRY(cmd), G_SETSCISSOR_MODE(cmd));
			break;
		case 0xec:
			printd(D_RCP, D_DEBUG, "G_SETCONVERT\n");
			break;
		case 0xeb:
			printd(D_RCP, D_DEBUG, "G_SETKEYR\n");
			break;
		case 0xea:
			printd(D_RCP, D_DEBUG, "G_SETKEYGB\n");
			break;
		case 0xe9:
			printd(D_RCP, D_DEBUG, "G_RDPFULLSYNC\n");
			break;
		case 0xe8:
			printd(D_RCP, D_DEBUG, "G_RDPTILESYNC\n");
			break;
		case 0xe7:
			printd(D_RCP, D_DEBUG, "G_RDPPIPESYNC\n");
			break;
		case 0xe6:
			printd(D_RCP, D_DEBUG, "G_RDPLOADSYNC\n");
			break;
		case 0xe5:
			printd(D_RCP, D_DEBUG, "G_TEXRECTFLIP\n");
			break;
		case 0xe4:
			printd(D_RCP, D_DEBUG, "G_TEXRECT\n");
			break;
		case 0xc8:
			printd(D_RCP, D_DEBUG, "G_TRI_FILL\n");
			break;
		case 0xcc:
			printd(D_RCP, D_DEBUG, "G_TRI_SHADE\n");
			break;
		case 0xca:
			printd(D_RCP, D_DEBUG, "G_TRI_TXTR\n");
			break;
		case 0xce:
			printd(D_RCP, D_DEBUG, "G_TRI_SHADE_TXTR\n");
			break;
		case 0xc9:
			printd(D_RCP, D_DEBUG, "G_TRI_FILL_ZBUFF\n");
			break;
		case 0xcd:
			printd(D_RCP, D_DEBUG, "G_TRI_SHADE_ZBUFF\n");
			break;
		case 0xcb:
			printd(D_RCP, D_DEBUG, "G_TRI_TXTR_ZBUFF\n");
			break;
		case 0xcf:
			printd(D_RCP, D_DEBUG, "G_SHADE_TXTR_ZBUFF\n");
			break;
		default:
			printd(D_RCP, D_DEBUG, "Unknown command: %#.2x\n", cmd.b8[3]);
			break;
	}
}

void disassemble_somethingelse(uint8* data, int i) {

	uint8 cmd;

	cmd = data[(i*8)+3];

	switch(cmd) {
		case 0x00:
			printf("G_NOOP\n");
			break;
		case 0xf1:
			printf("G_RDPHALF_2\n");
			break;
		case 0xe3:
			printf("G_SETOTHERMODE_H\n");
			break;
		case 0xe2:
			printf("G_SETOTHERMODE_L\n");
			break;
		case 0xe1:
			printf("G_RDPHALF_1\n");
			break;
		case 0xe0:
			printf("G_SPNOOP\n");
			break;
		case 0xdf:
			printf("G_ENDDL\n");
			break;
		case 0xde:
			printf("G_DL\n");
			break;
		case 0xdd:
			printf("G_LOAD_UCODE\n");
			break;
		case 0xdc:
			printf("G_MOVEMEM\n");
			break;
		case 0xdb:
			printf("G_MOVEWORD\n");
			break;
		case 0xda:
			printf("G_MTX\n");
			break;
		case 0xd9:
			printf("G_GEOMETRYMODE\n");
			break;
		case 0xd8:
			printf("G_POPMTX\n");
			break;
		case 0xd7:
			printf("G_TEXTURE\n");
			break;
		case 0xd6:
			printf("G_DMA_IO\n");
			break;
		case 0xd5:
			printf("G_SPECIAL_1\n");
			break;
		case 0xd4:
			printf("G_SPECIAL_2\n");
			break;
		case 0xd3:
			printf("G_SPECIAL_3\n");
			break;
		case 0x01:
			printf("G_VTX\n");
			break;
		case 0x02:
			printf("G_MODIFYVTX\n");
			break;
		case 0x03:
			printf("G_CULLDL\n");
			break;
		case 0x04:
			printf("G_BRANCH_Z\n");
			break;
		case 0x05:
			printf("G_TRI1\n");
			break;
		case 0x06:
			printf("G_TRI2\n");
			break;
		case 0x07:
			printf("G_QUAD\n");
			break;
		case 0x08:
			printf("G_LINE3D\n");
			break;
		default:
			printf("Unknown command: %#.2x\n", cmd);
	}
}
