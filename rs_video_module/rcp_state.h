extern void dump_segments();
extern void init_fdiv_tables(void);
extern void init_sprite_cache(void);

extern float convert_frac(short, unsigned short);
extern void dump_numlight();
extern int get_numlight();
extern void get_mtx(uint16*, float*);
extern void dump_mtx(float*);
extern void dump_vtx(int);
extern void dump_state();
extern uint32 translate_dma_address(uint32);
extern void test_and_kill_rcp();
extern void rcp_cleargeometrymode();
extern void rcp_setgeometrymode(uint32);
extern void rcp_setscissors(int, int, int, int, int);
extern void rcp_setambient(uint8, uint8, uint8);
extern void rcp_setlight(int, uint8, uint8, uint8, int8, int8, int8);
extern void rcp_setcimg(int, int, int, uint32);
extern void rcp_setzimg(int, int, int, uint32);
extern void rcp_settimg(int, int, int, uint32);
extern void rcp_setfillcolour(uint8, uint8, uint8, uint8);
extern void rcp_pushmtxstack(uint32);
extern void rcp_popmtxstack(uint32);
extern void rcp_loadmtx(uint32, float[16]);
extern void rcp_multmtx(uint32, float[16]);
extern void rcp_loadvtx(int, int, int, uint32);

extern struct rcp_state rcpst;
extern struct actual_state actst;

struct dmem_table {
	uint32 matrix[16];
	uint32 numlight;
	uint32 clip[8];
	uint32 segment[5];
	uint32 fog;
	uint32 lightcol[0x3f];
	uint32 forcemtx;
	uint32 points[8];
	uint32 perspnorm;
};

struct scis_t {
	uint32 ulx;
	uint32 uly;
	uint32 lrx;
	uint32 lry;
};

struct rcpfb {
	uint32 fmt;
	uint32 size;
	uint32 width;
	uint32 i;
	uint8* ri;
};

struct rcp_colour {
	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
};

struct clip_t {
	uint32 rnx;
	uint32 rny;
	uint32 rpx;
	uint32 rpy;
};

struct movemem_t {
	int padding; // don't remove till theres something in here, rs
};

struct vertex {
	short pos[3];
	uint16 flags;
	short tc[2];
	uint8 col[4];
};

struct sprite_cfg {
	uint32 src;
	uint16* r_src;
	uint32 src_ci;
	uint16 src_w;
	uint16 sprite_w;
	uint16 sprite_h;
	uint8 src_type;
	uint8 src_bits;
	uint16 scale_x;
	uint16 scale_y;
	uint8 flip_x;
	uint8 flip_y;
	uint16 src_ox;
	uint16 src_oy;
	uint16 dest_ox;
	uint16 dest_oy;
};

struct viewport {
	int16 scale[4];
	int16 trans[4];
};

struct l_amb {
	uint8 col[3];
};

struct l_normal {
	uint8 col[3];
	int8 dir[3];
};

struct rcp_state {
	struct dmem_table dtable;
	struct movemem_t mmtable;
	struct scis_t scissors;
	struct clip_t clip;
	struct rcpfb cimg;
	struct rcpfb zimg;
	struct rcpfb timg;
	struct rcp_colour fillc;
	struct vertex vtx[64]; // Pick a number ...
	struct sprite_cfg sprite;
	uint32 geometry_mode;
	struct viewport view;
	struct l_amb ambient;
	struct l_normal lights[8];
};

#define GEOM_DESYNC (1<<0)
#define LIGHTS_DESYNC (1<<1)
#define COMBINE_DESYNC (1<<2)

struct actual_state {
	uint32 desynced;
	uint32 geometry_mode;
};
