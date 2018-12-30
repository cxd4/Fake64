#include <general.h>
#include "rcp_state.h"
#include "rdpcommand.h"
#include <module.h>
#include "config_options.h"

#include <GL/gl.h>
#include <GL/glu.h>

float frach[0x100]; // Way to avoid an fdiv in decoding the funny
float fracl[0x100]; // s15.16 format of matrices

struct rcp_state rcpst;
struct actual_state actst;

int get_numlight();
float convert_frac(short, unsigned short);

uint32 translate_dma_address(uint32 addr) {

        uint32 seg;
        uint32 ret;

        // I have no documentary evidence this is right
        // I'm 100% guessing, it just seems to work so far

        seg = DMA_SEG(addr);

        if (seg > 4) {
                printd(D_RCP, D_EMERG, "Very high segment number\n");
                ret = 0;
        } else {
                ret = rcpst.dtable.segment[seg];
        }

        ret += DMA_ADDR(addr);

        return ret;
}


void init_fdiv_tables() {

	int i;

	for (i = 0; i < 0x100; i++) {
		frach[i] = (float)i/(float)0x100;
		fracl[i] = (float)i/(float)0x10000;
	}

}

// not too sure about this at all
float convert_frac(short high, unsigned short low) {

	float ret;
	uint8 ind;

	ret = 0;

	ret += high;

	ind = ((low & 0xff00) >> 8);

	ret += frach[ind];
	ret += fracl[(low & 0xff)];

/*
	ret=high&0x7fff;
	ret+=(float)low/65536.0f;

	if (high & 0x8000)
		ret=-ret;
*/
	return ret;
}

void dump_segments() {

	int i;

	for (i = 0; i < 5; i++) {
		printd(D_RCP, D_DEBUG, "Segment %i: %#.8x\n", i, rcpst.dtable.segment[i]);
	}
}

void dump_numlight() {

	int lights;

	lights = get_numlight();

	printd(D_RCP, D_DEBUG, "Numlight: %.8x = %i lights\n", rcpst.dtable.numlight, lights);

}

int get_numlight() {

	return (((rcpst.dtable.numlight & ~(0x80000000)) >> 5) - 1);

}

void get_mtx(uint16* mtx, float* m) {

	int i;

	for (i = 0; i < 16; i++) {
		m[i] = convert_frac(mtx[(i)^0x1], mtx[(i+16)^0x1]);   // ^1 to 'byte'swap ??
	}
}

void dump_mtx(float* m) {

	printd(D_RCP, D_DEBUG, "[ %f %f %f %f ]\n", m[0], m[4], m[8], m[12]);
	printd(D_RCP, D_DEBUG, "[ %f %f %f %f ]\n", m[1], m[5], m[9], m[13]);
	printd(D_RCP, D_DEBUG, "[ %f %f %f %f ]\n", m[2], m[6], m[10], m[14]);
	printd(D_RCP, D_DEBUG, "[ %f %f %f %f ]\n", m[3], m[7], m[11], m[15]);

}

void dump_vtx(int v) {

	printd(D_RCP, D_DEBUG, "Vertex %i:\n", v);

	printd(D_RCP, D_DEBUG, "  x: %i y: %i z: %i ", rcpst.vtx[v].pos[0], rcpst.vtx[v].pos[1], rcpst.vtx[v].pos[2]);

	printd(D_RCP, D_DEBUG, "flags: %x ", rcpst.vtx[v].flags);
	printd(D_RCP, D_DEBUG, "u: %i v: %i ", rcpst.vtx[v].tc[0], rcpst.vtx[v].tc[1]);
	printd(D_RCP, D_DEBUG, "rgba: %i %i %i %i\n", rcpst.vtx[v].col[0], rcpst.vtx[v].col[1], rcpst.vtx[v].col[2], rcpst.vtx[v].col[3]);

}

void dump_scissor() {
	printd(D_RCP, D_DEBUG, "Scissors set to (%i,%i),(%i,%i)\n", rcpst.scissors.ulx, rcpst.scissors.uly, rcpst.scissors.lrx, rcpst.scissors.lry);
}

void dump_clip() {
	printd(D_RCP, D_DEBUG, "Clipping planes: rnx: %i, rny: %i, rpx: %i, rpy: %i\n", rcpst.clip.rnx, rcpst.clip.rny, rcpst.clip.rpx, rcpst.clip.rpy);
}

void dump_cimg() {
	printd(D_RCP, D_DEBUG, "CIMG fb: fmt: %i, size: %i, width: %i, address: %#x\n", rcpst.cimg.fmt, rcpst.cimg.size, rcpst.cimg.width, rcpst.cimg.i);
}

void dump_zimg() {
	printd(D_RCP, D_DEBUG, "ZIMG fb: fmt: %i, size: %i, width: %i, address: %#x\n", rcpst.zimg.fmt, rcpst.zimg.size, rcpst.zimg.width, rcpst.zimg.i);
}

void dump_timg() {
	printd(D_RCP, D_DEBUG, "TIMG fb: fmt: %i, size: %i, width: %i, address: %#x\n", rcpst.timg.fmt, rcpst.timg.size, rcpst.timg.width, rcpst.timg.i);
}

void dump_fillcolour() {
	printd(D_RCP, D_DEBUG, "FILLCOLOUR r: %i, g: %i, b: %i, a: %i\n", rcpst.fillc.r, rcpst.fillc.b, rcpst.fillc.g, rcpst.fillc.a);
}

void dump_sprite() {
	printd(D_RCP, D_DEBUG, "Sprite info: src: %#x, src_ci: %#x, src_w: %#x, sprite_w: %i, sprite_h: %i, src_type: %i, src_bits: %i\n", rcpst.sprite.src, rcpst.sprite.src_ci, rcpst.sprite.src_w, rcpst.sprite.sprite_w, rcpst.sprite.sprite_h, rcpst.sprite.src_type, rcpst.sprite.src_bits);
}

void dump_geometry() {
	printd(D_RCP, D_DEBUG, "Geometry mode: %x\n", rcpst.geometry_mode);
}

void dump_viewport() {
	int i;
	printd(D_RCP, D_DEBUG, "Viewport data:\n");
	for (i = 0; i < 4; i++) {
		printd(D_RCP, D_DEBUG, "  Scale: %i, Trans: %i\n", rcpst.view.scale[i], rcpst.view.trans[i]);
	}
}

void dump_ambient() {
	printd(D_RCP, D_DEBUG, "Ambient light: r: %i, g: %i, b: %i\n", rcpst.ambient.col[0], rcpst.ambient.col[1], rcpst.ambient.col[2]);
}

void dump_lights() {
	int i;
	for (i = 1; i < 8; i++) {
		printd(D_RCP, D_DEBUG, "Light %i: r: %i, g: %i, b: %i, dir (%i, %i, %i)\n", i, rcpst.lights[i].col[0], rcpst.lights[i].col[1], rcpst.lights[i].col[2], rcpst.lights[i].dir[0], rcpst.lights[i].dir[1], rcpst.lights[i].dir[2]);
	}
}

void dump_state() {

	int i;

	printd(D_RCP, D_DEBUG, "\n-----------\nRCP DMA Segment table:\n");
	dump_segments();

	printd(D_RCP, D_DEBUG, "\n");
	dump_numlight();

	dump_scissor();
	dump_clip();

	dump_cimg();
	dump_zimg();
	dump_timg();

	dump_fillcolour();
	dump_sprite();
	dump_geometry();

	dump_viewport();

	dump_ambient();
	dump_lights();

	printd(D_RCP, D_DEBUG, "-----------\n");

	for (i = 0; i < 64; i++) {
		dump_vtx(i);
	}

}

void test_and_kill_rcp() {

        if (rcp_break_unim) {
                dump_state();
                exit(0);
        }
}

void rcp_cleargeometrymode() {
	rcpst.geometry_mode = 0;
	actst.desynced |= GEOM_DESYNC;
}

void rcp_setgeometrymode(uint32 mode) {
	rcpst.geometry_mode = mode;
	actst.desynced |= GEOM_DESYNC;
}

void rcp_setscissors(int mode, int ulx, int uly, int lrx, int lry) {

	if (mode != 0x00) {
		printd(D_RCP, D_WARN, "Interlaced scissors requested!\n");
	} else {
		rcpst.scissors.ulx = ulx;
		rcpst.scissors.uly = uly;
		rcpst.scissors.lrx = lrx;
		rcpst.scissors.lry = lry;
	}
}

void rcp_setambient(uint8 r, uint8 g, uint8 b) {

	rcpst.ambient.col[0] = r;
	rcpst.ambient.col[1] = g;
	rcpst.ambient.col[2] = b;

	actst.desynced |= LIGHTS_DESYNC;
}

void rcp_setlight(int index, uint8 r, uint8 g, uint8 b, int8 x, int8 y, int8 z) {

	rcpst.lights[index].col[0] = r;
	rcpst.lights[index].col[1] = g;
	rcpst.lights[index].col[2] = b;

	rcpst.lights[index].dir[0] = x;
	rcpst.lights[index].dir[1] = y;
	rcpst.lights[index].dir[2] = z;

	actst.desynced |= LIGHTS_DESYNC;
}

void rcp_setcimg(int fmt, int size, int width, uint32 i) {

	rcpst.cimg.fmt = fmt;
	rcpst.cimg.size = size;
	rcpst.cimg.width = width;
	rcpst.cimg.i = i;
	rcpst.cimg.ri = (uint8*)(modules->cpu->translate_addr_f(rcpst.cimg.i));
}

void rcp_setzimg(int fmt, int size, int width, uint32 i) {

        rcpst.zimg.fmt = fmt;
        rcpst.zimg.size = size;
        rcpst.zimg.width = width;
        rcpst.zimg.i = i;
        rcpst.zimg.ri = (uint8*)(modules->cpu->translate_addr_f(rcpst.zimg.i));
}

void rcp_settimg(int fmt, int size, int width, uint32 i) {

        rcpst.timg.fmt = fmt;
        rcpst.timg.size = size;
        rcpst.timg.width = width;
        rcpst.timg.i = i;
        rcpst.timg.ri = (uint8*)(modules->cpu->translate_addr_f(rcpst.timg.i));
}

void rcp_setfillcolour(uint8 r, uint8 g, uint8 b, uint8 a) {

	rcpst.fillc.r = r;
	rcpst.fillc.g = g;
	rcpst.fillc.b = b;
	rcpst.fillc.a = a;
}

void rcp_pushmtxstack(uint32 stack) {

	glMatrixMode(stack);
	glPushMatrix();
}

void rcp_popmtxstack(uint32 stack) {

	glMatrixMode(stack);
	glPushMatrix();
}

void rcp_loadmtx(uint32 stack, float m[16]) {

	glMatrixMode(stack);
	glLoadMatrixf(m);
}

void rcp_multmtx(uint32 stack, float m[16]) {

	glMatrixMode(stack);
	glMultMatrixf(m);
}

void rcp_loadvtx(int v0, int n, int len, uint32 data) {

	int wtc, i;
	uint16 *ptr, *dest;

	wtc = len >> 1; // copying this many halfwords
	ptr = (uint16*)(modules->cpu->translate_addr_f(translate_dma_address(data))); // where we're coming from
	dest = (uint16*)(&rcpst.vtx[v0]); // where we're going to

	for (i = 0; i < wtc; i++) {
		dest[i] = ptr[i^0x1]; // whack in the data
	}

	for (i = 0; i < n; i++) {
		dump_vtx(v0+i);
	}
}
