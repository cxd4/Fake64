#include <general.h>
#include <module.h>
#include "rdpcommand.h"
#include "rcp_core.h"
#include "rcp_state.h"
#include "render.h"

#include <GL/gl.h>
#include <GL/glu.h>

static char* moveword_indices[] = { "G_MW_MATRIX", "G_MW_NUMLIGHT",
        "G_MW_CLIP", "G_MW_SEGMENT", "G_MW_FOG", "G_MW_LIGHTCOL",
        "G_MW_FORCEMTX", "G_MW_PERSPNORM" };

static char* movemem_indices[] = { "G_MV_VIEWPORT", "G_MV_LOOKATY",
        "G_MV_LOOKATX", "G_MV_L0", "G_MV_L1", "G_MV_L2", "G_MV_L3",
        "G_MV_L4", "G_MV_L5", "G_MV_L6", "G_MV_L7", "G_MV_TXTATT",
        "G_MV_MATRIX_1", "G_MV_MATRIX_2", "G_MV_MATRIX_3",
        "G_MV_MATRIX_4" };

void disassemble_all00(rdp_command cmd) {

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


void perform_moveword00(rdp_command cmd) {

        switch(G_MOVEWORD_P1(cmd)) {
                case 0x06:
                        if (G_MOVEWORD_P0(cmd) & 0x3) {
                                printd(D_RCP, D_WARN, "G_MOVEWORD says: segment not 4 bytes\n");
                        } else {
                                rcpst.dtable.segment[G_MOVEWORD_P0(cmd)>>2] = G_MOVEWORD_DAT(cmd);
                                dump_segments();
                        }
                        break;

                case 0x02:
                        rcpst.dtable.numlight = G_MOVEWORD_DAT(cmd);
                        dump_numlight();
                        break;
                case 0x04:
                        switch(G_MOVEWORD_P0(cmd)) {
                                case 0x04:
                                        rcpst.clip.rnx = G_MOVEWORD_DAT(cmd);
                                        break;
                                case 0x0c:
                                        rcpst.clip.rny = G_MOVEWORD_DAT(cmd);
                                        break;
                                case 0x14:
                                        rcpst.clip.rpx = G_MOVEWORD_DAT(cmd);
                                        break;
                                case 0x1c:
                                        rcpst.clip.rpy = G_MOVEWORD_DAT(cmd);
                                        break;
                                default:
                                        printd(D_RCP, D_ERROR, "No such clipping plane\n");
                                        break;
                        }
                        break;
		case 0x0e:
			printd(D_RCP, D_WARN, "I have absolutely no idea what G_MW_PERSPNORM is for\n");
			break;
                default:
                        printd(D_RCP, D_WARN, "Teach me how to handle index: %#.2x\n", G_MOVEWORD_P1(cmd));
                        test_and_kill_rcp();
                        break;
        }
}


void perform_cleargeometrymode00(rdp_command cmd) {
	rcp_cleargeometrymode();
}

void perform_texture00(rdp_command cmd) {

	// Ignoring textures for now ...
}

void perform_setgeometrymode00(rdp_command cmd) {
	rcp_setgeometrymode(G_DL_S(cmd));
}

void perform_setothermode_h00(rdp_command cmd) {

        switch(G_OTHERMODE_SHIFT(cmd)) {
                case 0x06:
                        printd(D_RCP, D_WARN, "RGB dither set (Ignored)\n");
                        break;
                case 0x08:
                        printd(D_RCP, D_WARN, "Combine key set (Ignored)\n");
                        break;
                case 0x09:
                        printd(D_RCP, D_WARN, "Texture convert set (Ignored)\n");
                        break;
                case 0x0c:
                        printd(D_RCP, D_WARN, "Texture filter set (Ignored)\n");                        break;
                case 0x0e:
                        if (G_OTHERMODE_DATA(cmd)) {
                                printd(D_RCP, D_WARN, "Palletted textures?\n");
                                test_and_kill_rcp();
                        }
                        break;
                case 0x10:
                        printd(D_RCP, D_WARN, "Texture LOD set (Ignored)\n");
                        break;
                case 0x11:
                        printd(D_RCP, D_WARN, "Texture detail set (Ignored)\n");                        break;
                case 0x13:
                        if (!G_OTHERMODE_DATA(cmd)) {
                                printd(D_RCP, D_WARN, "Non-perspective tex?\n");                                test_and_kill_rcp();
                        }
                        break;
                case 0x14:
                        printd(D_RCP, D_WARN, "Cycletype set (Ignored)\n");
                        break;
                case 0x17:
                        printd(D_RCP, D_WARN, "Pipeline set (Ignored)\n");
                        break;
                default:
                        printd(D_RCP, D_WARN, "Unknown othermode\n");
                        test_and_kill_rcp();
                        break;
        }
}

void perform_setscissor00(rdp_command cmd) {

	rcp_setscissors(G_SETSCISSOR_MODE(cmd), G_SETSCISSOR_ULX(cmd), G_SETSCISSOR_ULY(cmd), G_SETSCISSOR_LRX(cmd), G_SETSCISSOR_LRY(cmd));
}

void perform_setothermode_l00(rdp_command cmd) {

        switch(G_OTHERMODE_SHIFT(cmd)) {
                case 0x00:
                        printd(D_RCP, D_WARN, "Alpha compare set (Ignored)\n");
                        break;
		case 0x03:
			printd(D_RCP, D_WARN, "Render mode set (Ignored)\n");
			break;
                default:
                        printd(D_RCP, D_WARN, "Unknown othermode\n");
                        test_and_kill_rcp();
                        break;
        }
}

void perform_movemem00(rdp_command cmd) {

	uint32* raddr;

	rdp_command temp;
	int ind;

	raddr = (uint32*)(modules->cpu->translate_addr_f((uint32)translate_dma_address(G_DL_S(cmd))));

	switch(G_DL_P(cmd)) {
		case 0x80:
			// ignoring viewport
			break;
		case 0x86:
			temp.b32[0] = *raddr;
			rcp_setambient(temp.b8[1], temp.b8[2], temp.b8[3]);
			break;
		case 0x88:
			temp.b32[0] = *raddr;
			temp.b32[1] = *(raddr+2);
			ind = ((G_DL_P(cmd)-0x86) >> 1);
			rcp_setlight(ind, temp.b8[1], temp.b8[2], temp.b8[3], temp.b8[5], temp.b8[6], temp.b8[7]);
			break;
		default:
			printd(D_RCP, D_DEBUG, "Unknown index: %i\n", G_DL_P(cmd));
			test_and_kill_rcp();
			break;
	}
}

void perform_setcombine00(rdp_command cmd) {
	printd(D_RCP, D_WARN, "Ignored a combine setting\n");
}

void perform_rdppipesync00(rdp_command cmd) {
	printd(D_RCP, D_WARN, "Not pipe syncing\n");
}

void perform_setcimg00(rdp_command cmd) {
	rcp_setcimg(G_SETCIMG_F(cmd), G_SETCIMG_S(cmd), G_SETCIMG_W(cmd), G_SETCIMG_I(cmd));
}

void perform_setzimg00(rdp_command cmd) {
	rcp_setzimg(G_SETCIMG_F(cmd), G_SETCIMG_S(cmd), G_SETCIMG_W(cmd), G_SETCIMG_I(cmd));
}

void perform_settimg00(rdp_command cmd) {
	rcp_settimg(G_SETCIMG_F(cmd), G_SETCIMG_S(cmd), G_SETCIMG_W(cmd), G_SETCIMG_I(cmd));
}

void perform_setfillcolour00(rdp_command cmd) {
	rcp_setfillcolour(G_SETFILLCOLOR_R(cmd), G_SETFILLCOLOR_G(cmd), G_SETFILLCOLOR_B(cmd), G_SETFILLCOLOR_A(cmd));
}

void perform_fillrect00(rdp_command cmd) {
	// We're ignoring fillrect for now
	// It slows us down and it doesn't do much
}

void perform_noop00(rdp_command cmd) {
	// I think i'll do nothing for this one
}

void perform_settile00(rdp_command cmd) {
	// skipping more textures...
}

void perform_rdploadsync00(rdp_command cmd) {
	// Ignore these for speed
}

void perform_loadblock00(rdp_command cmd) {
	// skipping more textures...
}

void perform_settilesize00(rdp_command cmd) {
	// skipping even more textures ...
}

void perform_mtx00(rdp_command cmd) {

	uint16* mtx;
	float m[16];
	uint32 prop, stack;

	mtx = (uint16*)(modules->cpu->translate_addr_f(translate_dma_address(G_DL_S(cmd))));

	get_mtx(mtx, m);
	dump_mtx(m);

	prop = G_DL_P(cmd);

	if (prop & G_MTX_PROJECTION) {
		printd(D_RCP, D_DEBUG, "PROJ\n");
		stack = GL_PROJECTION;
	} else {
		printd(D_RCP, D_DEBUG, "MODEL\n");
		stack = GL_MODELVIEW;
	}

	if (prop & G_MTX_PUSH) {
		printd(D_RCP, D_DEBUG, "PUSH\n");
		rcp_pushmtxstack(stack);
	} 

	if (prop & G_MTX_LOAD) {
		printd(D_RCP, D_DEBUG, "LOAD\n");
		rcp_loadmtx(stack, m);
	} else {
		printd(D_RCP, D_DEBUG, "MULT\n");
		rcp_multmtx(stack, m);
	}
}

void perform_tri100(rdp_command cmd) {

	dodrawtri(G_TRI1_V0(cmd), G_TRI1_V1(cmd), G_TRI1_V2(cmd));
}

void perform_vtx00(rdp_command cmd) {

	rcp_loadvtx(G_VTX_V0(cmd), G_VTX_N(cmd), G_VTX_L(cmd), G_DL_S(cmd));
}

void perform_rdpfullsync00(rdp_command cmd) {

	// we don't care about these
}

void register_ucode00() {

	int i;

        rcp_core = (void*)basic_rcp_core;
	rcp_ecmd[0x01] = (void*)perform_mtx00;
	rcp_ecmd[0x03] = (void*)perform_movemem00;
	rcp_ecmd[0x04] = (void*)perform_vtx00;
	rcp_ecmd[0xb6] = (void*)perform_cleargeometrymode00;
	rcp_ecmd[0xb7] = (void*)perform_setgeometrymode00;
	rcp_ecmd[0xb9] = (void*)perform_setothermode_l00;
	rcp_ecmd[0xba] = (void*)perform_setothermode_h00;
	rcp_ecmd[0xbb] = (void*)perform_texture00;
        rcp_ecmd[0xbc] = (void*)perform_moveword00;
	rcp_ecmd[0xbf] = (void*)perform_tri100;
	rcp_ecmd[0xc0] = (void*)perform_noop00;
	rcp_ecmd[0xe6] = (void*)perform_rdploadsync00;
	rcp_ecmd[0xe7] = (void*)perform_rdppipesync00;
	rcp_ecmd[0xe9] = (void*)perform_rdpfullsync00;
	rcp_ecmd[0xed] = (void*)perform_setscissor00;
	rcp_ecmd[0xf2] = (void*)perform_settilesize00;
	rcp_ecmd[0xf3] = (void*)perform_loadblock00;
	rcp_ecmd[0xf5] = (void*)perform_settile00;
	rcp_ecmd[0xf6] = (void*)perform_fillrect00;
	rcp_ecmd[0xf7] = (void*)perform_setfillcolour00;
	rcp_ecmd[0xfc] = (void*)perform_setcombine00;
	rcp_ecmd[0xfd] = (void*)perform_settimg00;
	rcp_ecmd[0xfe] = (void*)perform_setzimg00;
	rcp_ecmd[0xff] = (void*)perform_setcimg00;

	for (i = 0; i < 0x100; i++) {
		rcp_dcmd[i] = (void*)disassemble_all00;
	} // this is a nice way to avoid coding 50 disassembly functions

}
