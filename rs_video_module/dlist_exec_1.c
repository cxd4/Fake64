#ifdef WIN32
	#include <windows.h>
#endif

#include <general.h>
#include "rcp_state.h"
#include "rdpcommand.h"
#include <module.h>
#include "render.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "config_options.h"

extern struct module_info* modules;
extern void do_displaylist(uint8*, int);

#if 0
#define CALL_glMaterialfv
#endif
void wrong_turn_on_lights()
{
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
#ifdef CALL_glMaterialfv
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
#endif

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

#ifdef CALL_glMaterialfv
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
#endif
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

#if 0
#define CALL_glLightModelfv
#endif
void turn_on_lights()
{
#ifdef CALL_glLightModelfv
	GLfloat ambient[4] = {0.0, 0.0, 0.0, 1.0};
#endif
	GLfloat pos[4] = {1.0, 1.0, 1.0, 0.0};
	GLfloat spec[4] = {1.0, 0.0, 0.0, 1.0};
	GLfloat mat_spec[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shin[] = {50.0};
	int l, i;

	l = get_numlight();

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shin);

	for (i = 0; i < l; i++) {
		printf("LIGHT: %i\n", i);
	//	pos[0] = ((float)rcpst.lights[i].dir[0])/ 255.0;
	//	pos[1] = ((float)rcpst.lights[i].dir[1])/ 255.0;
	//	pos[2] = ((float)rcpst.lights[i].dir[2])/ 255.0;
//		spec[0] = ((float)rcpst.lights[i].col[0])/ 255.0;
//		spec[1] = ((float)rcpst.lights[i].col[1])/ 255.0;
//		spec[2] = ((float)rcpst.lights[i].col[2])/ 255.0;
		glLightfv(GL_LIGHT1+i, GL_POSITION, pos);
		glLightfv(GL_LIGHT1+i, GL_SPECULAR, spec);
		glLightfv(GL_LIGHT1+i, GL_DIFFUSE, spec);

		glEnable(GL_LIGHT1+i);
	}
#ifdef CALL_glLightModelfv
	ambient[0] = ((float)rcpst.ambient.col[0]) / 255.0;
	ambient[1] = ((float)rcpst.ambient.col[1]) / 255.0;
	ambient[2] = ((float)rcpst.ambient.col[2]) / 255.0;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
#endif
}

void perform_moveword(rdp_command cmd) {

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
//			turn_on_lights();
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
		default:
			printd(D_RCP, D_WARN, "Teach me how to handle index: %#.2x\n", G_MOVEWORD_P1(cmd));
			test_and_kill_rcp();
			break;
	}
}

void perform_movemem(rdp_command cmd) {

	uint32 addr;
	uint32 *raddr;
	rdp_command temp;
	int ind;

	addr = translate_dma_address(G_DL_S(cmd));
	raddr = (uint32*)(modules->cpu->translate_addr_f(addr));

	switch(G_DL_P(cmd)) {
		case 0x80:
			memcpy(&rcpst.view, raddr, G_DL_L(cmd));
			break;
		case 0x86:
			temp.b32[0] = *raddr;
			rcpst.ambient.col[0] = temp.b8[1];
			rcpst.ambient.col[1] = temp.b8[2];
			rcpst.ambient.col[2] = temp.b8[3];
//			turn_on_lights();
			break;
		case 0x88:
			temp.b32[0] = *raddr;
			temp.b32[1] = *(raddr+2);
			ind = ((G_DL_P(cmd)-0x86) >> 1);
			rcpst.lights[ind].col[0] = temp.b8[1];
			rcpst.lights[ind].col[1] = temp.b8[2];
			rcpst.lights[ind].col[2] = temp.b8[3];
			rcpst.lights[ind].dir[0] = temp.b8[5];
			rcpst.lights[ind].dir[1] = temp.b8[6];
			rcpst.lights[ind].dir[2] = temp.b8[7];
//			turn_on_lights();
			break;
		default:
			printd(D_RCP, D_DEBUG, "Unknown index: %i\n", G_DL_P(cmd));
			test_and_kill_rcp();
	}
}

void perform_dl(rdp_command cmd, int depth) {

	uint32 dlist;
	uint8* rdlist;

	if (G_DL_P(cmd) != 0x00) {
		printd(D_RCP, D_EMERG, "NOPUSH requested!!\n");
	}

	dlist = translate_dma_address(G_DL_S(cmd));
	rdlist = (uint8*)(modules->cpu->translate_addr_f(dlist));

	printd(D_RCP, D_DEBUG, "DMA Address translate: %x -> %x\n", dlist, (int)rdlist);

	do_displaylist(rdlist, depth+1);
	
}

void perform_setscissor(rdp_command cmd) {

	if (G_SETSCISSOR_MODE(cmd) != 0x00) {
		printd(D_RCP, D_WARN, "Interlaced scissors requested!\n");
	} else {
		rcpst.scissors.ulx = G_SETSCISSOR_ULX(cmd);
		rcpst.scissors.uly = G_SETSCISSOR_ULY(cmd);
		rcpst.scissors.lrx = G_SETSCISSOR_LRX(cmd);
		rcpst.scissors.lry = G_SETSCISSOR_LRY(cmd);
			// We'll save them and use them for our viewport
			// or whatever when we render later
	}
}

void perform_setcimg(rdp_command cmd) {

	rcpst.cimg.fmt = G_SETCIMG_F(cmd);
	rcpst.cimg.size = G_SETCIMG_S(cmd);
	rcpst.cimg.width = G_SETCIMG_W(cmd);
	rcpst.cimg.i = G_SETCIMG_I(cmd);
	rcpst.cimg.ri = (uint8*)(modules->cpu->translate_addr_f(rcpst.cimg.i));

}

void perform_setzimg(rdp_command cmd) {

        rcpst.zimg.fmt = G_SETCIMG_F(cmd);
        rcpst.zimg.size = G_SETCIMG_S(cmd);
        rcpst.zimg.width = G_SETCIMG_W(cmd);
        rcpst.zimg.i = G_SETCIMG_I(cmd);
        rcpst.zimg.ri = (uint8*)(modules->cpu->translate_addr_f(rcpst.zimg.i));

}

void perform_settimg(rdp_command cmd) {

        rcpst.timg.fmt = G_SETCIMG_F(cmd);
        rcpst.timg.size = G_SETCIMG_S(cmd);
        rcpst.timg.width = G_SETCIMG_W(cmd);
        rcpst.timg.i = G_SETCIMG_I(cmd);
        rcpst.timg.ri = (uint8*)(modules->cpu->translate_addr_f(rcpst.timg.i));

}

void perform_setfillcolor(rdp_command cmd) {

	rcpst.fillc.r = G_SETFILLCOLOR_R(cmd);
	rcpst.fillc.g = G_SETFILLCOLOR_G(cmd);
	rcpst.fillc.b = G_SETFILLCOLOR_B(cmd);
	rcpst.fillc.a = G_SETFILLCOLOR_A(cmd);

}

void perform_fillrect(rdp_command cmd) {

	dodrawrect(G_FILLRECT_ULX(cmd), G_FILLRECT_ULY(cmd), G_FILLRECT_LRX(cmd), G_FILLRECT_LRY(cmd));

}

void perform_rdppipesync(rdp_command cmd) {

	SDL_GL_SwapBuffers();

}

void perform_mtx(rdp_command cmd) {

	uint32 addr;
	uint16* mtx;
	float m[16];

	addr = translate_dma_address(G_DL_S(cmd));
	mtx = (uint16*)(modules->cpu->translate_addr_f(addr));

	get_mtx(mtx, m);
	dump_mtx(m);

	if (G_DL_P(cmd) & G_MTX_PROJECTION) {
		glMatrixMode(GL_PROJECTION);
//		printd(D_RDP, D_DEBUG, "PROJECTION\n");

	} else {
		glMatrixMode(GL_MODELVIEW);
//		printd(D_RDP, D_DEBUG, "MODELVIEW\n");
	}

	if (G_DL_P(cmd) & G_MTX_PUSH) {
		glPushMatrix();
//		printd(D_RDP, D_DEBUG, "PUSH\n");
	}

//	if (!(G_DL_P(cmd) & G_MTX_PROJECTION)) {
	if (G_DL_P(cmd) & G_MTX_LOAD) {
//		glLoadIdentity();
//		glTranslatef(0.0, 100.0, 0.0);
		glLoadMatrixf(m);
//		printd(D_RDP, D_DEBUG, "LOAD\n");
	} else {
		glMultMatrixf(m);
//		printd(D_RDP, D_DEBUG, "MULT\n");
	}
//	}

	glMatrixMode(GL_MODELVIEW);

}

void perform_vtx(rdp_command cmd) {

	int wtc, i;
	uint16 *ptr, *dest;

	wtc = G_VTX_L(cmd) >> 1; // This is going to be *really* fast
	ptr = (uint16*)(modules->cpu->translate_addr_f(translate_dma_address(G_DL_S(cmd))));
	dest = (uint16*)(&rcpst.vtx[G_VTX_V0(cmd)]);

	for (i = 0; i < wtc; i++) {
		dest[i] = ptr[i^0x1];
	}

/*
// Can't make the cool code work so here's something safer:

	rdp_command temp, temp2;
	uint64* ptr;
	int i;

	ptr = (uint64*)(modules->cpu->translate_addr_f(translate_dma_address(G_DL_S(cmd))));

	for (i = 0; i < G_VTX_NFAST(cmd); i++) {
		temp.b64 = ptr[(2*i)];
		temp2.b64 = ptr[(2*i)+1];
		rcpst.vtx[G_VTX_V0(cmd)+i].pos[0] = temp.b16[1];
		rcpst.vtx[G_VTX_V0(cmd)+i].pos[1] = temp.b16[0];
		rcpst.vtx[G_VTX_V0(cmd)+i].pos[2] = temp.b16[3];
	} */

	dump_vtx(G_VTX_V0(cmd));
}

void perform_tri1(rdp_command cmd) {

	dodrawtri(G_TRI1_V0(cmd), G_TRI1_V1(cmd), G_TRI1_V2(cmd));

}

void perform_spritebase(rdp_command cmd) {

	rdp_command temp; // Not really a command, but it's a useful union
	uint64* data;
	uint32 addr;

	addr = translate_dma_address(G_DL_S(cmd));
	data = (uint64*)(modules->cpu->translate_addr_f(addr));

	temp.b64 = data[0];
	rcpst.sprite.src = temp.b32[0];
	rcpst.sprite.r_src = (void*)(modules->cpu->translate_addr_f(translate_dma_address(rcpst.sprite.src)));
	rcpst.sprite.src_ci = temp.b32[1];

	temp.b64 = data[1];
	rcpst.sprite.src_w = temp.b16[1];
	rcpst.sprite.sprite_w = temp.b16[0];
	rcpst.sprite.sprite_h = temp.b16[3];
	rcpst.sprite.src_type = temp.b8[5];
	rcpst.sprite.src_bits = temp.b8[4];

	temp.b64 = data[2];
	rcpst.sprite.scale_x = temp.b16[1];
	rcpst.sprite.scale_y = temp.b16[0];
	rcpst.sprite.flip_x = temp.b8[4]; // Not sure about anything from 
	rcpst.sprite.flip_y = temp.b8[5]; // here down

	temp.b64 = data[3];
	rcpst.sprite.src_ox = temp.b16[0];
	rcpst.sprite.src_oy = temp.b16[1];
	rcpst.sprite.dest_ox = temp.b16[0];
	rcpst.sprite.dest_oy = temp.b16[3];

	printd(D_RCP, D_DEBUG, "Sprite: %#.8x %ix%i type: %i depth: %i\n", rcpst.sprite.src, rcpst.sprite.sprite_w, rcpst.sprite.sprite_h, rcpst.sprite.src_type, rcpst.sprite.src_bits);

	printd(D_RCP, D_DEBUG, "%x:%x\n", rcpst.sprite.scale_x, rcpst.sprite.scale_y);

	dodrawsprite();

}

void perform_cleargeometrymode(rdp_command cmd) {

	rcpst.geometry_mode = 0;
}

void perform_texture(rdp_command cmd) {

	if(G_TEXTURE_ON(cmd)) {
		printd(D_RCP, D_WARN, "Textures not supported!\n");
		test_and_kill_rcp();
	}
}

void perform_setgeometrymode(rdp_command cmd) {

	rcpst.geometry_mode = G_GEOMETRYMODE_MODE(cmd);

	if (rcpst.geometry_mode & 0x00000001) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	if (rcpst.geometry_mode & 0x00000002) {
		printd(D_RCP, D_WARN, "Textures enabled!\n");
		test_and_kill_rcp();
	}

	if (rcpst.geometry_mode & 0x00000200) {
		glShadeModel(GL_SMOOTH);
	} else {
		glShadeModel(GL_FLAT);
	}

	switch(rcpst.geometry_mode & 0x00003000) {
		case 0x0000:
			glDisable(GL_CULL_FACE);
			break;
		case 0x1000:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		case 0x2000:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case 0x3000:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			break;
	}

	if (rcpst.geometry_mode & 0x00ff0000) {
		printd(D_RCP, D_WARN, "Unhandled geometry mode settings: %#x\n", rcpst.geometry_mode);
		test_and_kill_rcp();
	}

}

void perform_setothermode_l(rdp_command cmd) {

	switch(G_OTHERMODE_SHIFT(cmd)) {
		case 0x00:
			printd(D_RCP, D_WARN, "Alpha compare set (Ignored)\n");
			break;
		default:
			printd(D_RCP, D_WARN, "Unknown othermode\n");
			test_and_kill_rcp();
			break;
	}
}

void perform_setothermode_h(rdp_command cmd) {

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
			printd(D_RCP, D_WARN, "Texture filter set (Ignored)\n");
			break;
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
			printd(D_RCP, D_WARN, "Texture detail set (Ignored)\n");
			break;
		case 0x13:
			if (!G_OTHERMODE_DATA(cmd)) {
				printd(D_RCP, D_WARN, "Non-perspective tex?\n");
				test_and_kill_rcp();
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
