#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <general.h>
#include "memory.h"
#include <module.h>
#include "rcp_state.h"
#include "rdpcommand.h"
#include "config_options.h"

uint8 spack[320*240][4];

uint32* get_sprite(struct sprite_cfg*);

void reset_stacks() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (float)win_width, 0.0, (float)win_height, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void
dodrawrect(int x1, int y1, int x2, int y2)
{
	uint16 source;
	int i;

	// Multiply by a scale factor in here

//	x1 = x1 << 1;
//	x2 = x2 << 1;
//	y1 = y1 << 1;
//	y2 = y2 << 1;
        for (i = 0; i < rcpst.sprite.sprite_w * rcpst.sprite.sprite_h; i++) {
        	source = ((uint16 *)rcpst.sprite.r_src)[i ^ HALFWORD_ADDRESS_SWAP];
		spack[i][0] = ((source & 0xF800) >> 11) * 8;
		spack[i][1] = ((source & 0x07C0) >>  6) * 8;
		spack[i][2] = ((source & 0x003E) >>  1) * 8;
		spack[i][3] = ((source & 0x0001) >>  0) * 255;
	}
	if (!(x1 || y1) && (x2 = 319) && (y2 = 219)) {

		glClearColor((float)rcpst.fillc.r, (float)rcpst.fillc.g, (float)rcpst.fillc.b, (float)rcpst.fillc.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

//	glRecti(x1, y1, x2, y2);

}

void dodrawtri(int v0, int v1, int v2) {

//	gluLookAt(0.0, 0.0, -500.0, -122.0, -132.0, 245.0, 0.0, 1.0, 0.0);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-500.0, 500.0, -500.0, 500.0, -500.0, 500.0);

	glColor3f(1.0, 1.0, 1.0);

/*	glBegin(GL_POLYGON);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd(); */

//	printd(D_RDP, D_DEBUG, "Triangle:\n");
//	printd(D_RDP, D_DEBUG, "(%hi,%hi,%hi),(%hi,%hi,%hi),(%hi,%hi,%hi)\n", rcpst.vtx[v0].pos[0], rcpst.vtx[v0].pos[1], rcpst.vtx[v0].pos[2], rcpst.vtx[v1].pos[0], rcpst.vtx[v1].pos[1], rcpst.vtx[v1].pos[2], rcpst.vtx[v2].pos[0], rcpst.vtx[v2].pos[1], rcpst.vtx[v2].pos[2]); 

	glBegin(GL_TRIANGLES);
		glColor4ub(rcpst.vtx[v0].col[1], rcpst.vtx[v0].col[0], rcpst.vtx[v0].col[3], rcpst.vtx[v0].col[2]);
		glVertex3s(rcpst.vtx[v0].pos[0], rcpst.vtx[v0].pos[1], rcpst.vtx[v0].pos[2]);
		glNormal3b(rcpst.vtx[v0].col[1], rcpst.vtx[v0].col[0], rcpst.vtx[v0].col[3]);
                glColor4ub(rcpst.vtx[v1].col[1], rcpst.vtx[v1].col[0], rcpst.vtx[v1].col[3], rcpst.vtx[v1].col[2]);
		glVertex3s(rcpst.vtx[v1].pos[0], rcpst.vtx[v1].pos[1], rcpst.vtx[v1].pos[2]);
                glColor4ub(rcpst.vtx[v2].col[1], rcpst.vtx[v2].col[0], rcpst.vtx[v2].col[3], rcpst.vtx[v2].col[2]);
		glVertex3s(rcpst.vtx[v2].pos[0], rcpst.vtx[v2].pos[1], rcpst.vtx[v2].pos[2]);
	glEnd(); 

}

void dodrawsprite() {

	uint32* unpacked;
	int ypos;

	printd(D_RDP, D_DEBUG, "CALLED\n");

//	glPixelZoom(1.0, -1.0);

	glDisable(GL_DEPTH_TEST);

	ypos = win_height - (int)((rcpst.sprite.dest_oy >> 2) * win_scaley);
	ypos -= (rcpst.sprite.sprite_h * win_scaley);

	glRasterPos2i((int)((rcpst.sprite.dest_ox >> 2) * win_scalex), ypos);

	printd(D_RDP, D_DEBUG, "SETUP\n");

	unpacked = get_sprite(&rcpst.sprite);

	printd(D_RCP, D_DEBUG, "UNPACKED\n");

	glDrawPixels(rcpst.sprite.sprite_w * win_scalex, rcpst.sprite.sprite_h * win_scaley, GL_RGBA, GL_UNSIGNED_BYTE, unpacked);

	printd(D_RCP, D_DEBUG, "RENDERED\n");

}

void
old_dodrawsprite(void)
{
	uint16 source;
	int i;

#if 0
	glPixelZoom(
		(float)((float)rcpst.sprite.scale_x * 2.0),
		(float)((float)rcpst.sprite.scale_y * 2.0)
	);
#endif

	for (i = 0; i < rcpst.sprite.sprite_w * rcpst.sprite.sprite_h; i++) {
		source = ((uint16 *)rcpst.sprite.r_src)[i ^ HALFWORD_ADDRESS_SWAP];
		spack[i][0] = ((source & 0xf800) >> 11) * 8;
		spack[i][1] = ((source & 0x07c0) >>  6) * 8;
		spack[i][2] = ((source & 0x003e) >>  1) * 8;
		spack[i][3] = ((source & 0x0001) >>  0) * 255;
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0.0, 640.0, 0.0, 480.0, -1.0, 1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	printf("WOOWOO: (%i, %i)\n", (rcpst.sprite.dest_ox>>1), (479 - (rcpst.sprite.dest_oy >> 1)));
	glRasterPos2i(0 + (rcpst.sprite.dest_ox >> 1), 479 - (rcpst.sprite.dest_oy >> 1));
	glPixelZoom(2.0, -2.0);
	glDrawPixels(rcpst.sprite.sprite_w, rcpst.sprite.sprite_h, GL_RGBA, GL_UNSIGNED_BYTE, spack);

}
