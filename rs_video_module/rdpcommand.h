typedef union {
	uint8 b8[8];
	uint16 b16[4];
	uint32 b32[2];
	uint64 b64;
} rdp_command;

#define RDP_CMD(a) (a.b8[3])

#define DMA_SEG(a) ((a & 0x0f000000) >> 24)
#define DMA_ADDR(a) (a & 0x00ffffff)

#define G_DL_P(a) (a.b8[2])
#define G_DL_L(a) (a.b16[0])
#define G_DL_S(a) (a.b32[1])

#define G_MOVEWORD_P0(a) (a.b8[1] | (a.b8[2] << 8))
#define G_MOVEWORD_P1(a) (a.b8[0])
#define G_MOVEWORD_DAT(a) (a.b32[1])

#define G_SETCIMG_F(a) ((a.b8[2] & 0xe0) >> 5)
#define G_SETCIMG_S(a) ((a.b8[2] & 0x18) >> 3)
#define G_SETCIMG_W(a) (a.b16[0] & 0xfff)
#define G_SETCIMG_I(a) (a.b32[1])

#define G_SETFILLCOLOR_R(a) (a.b8[7])
#define G_SETFILLCOLOR_G(a) (a.b8[6])
#define G_SETFILLCOLOR_B(a) (a.b8[5])
#define G_SETFILLCOLOR_A(a) (a.b8[4])

#define G_SPRITE2D_BASE_L(a) (a.b16[0])
#define G_SPRITE2D_BASE_S(a) (a.b32[1])

#define G_SETSCISSOR_ULX(a) (((a.b32[0] & 0x00fff000) >> 12) >> 2)
#define G_SETSCISSOR_ULY(a) ((a.b32[0] & 0x00000fff) >> 2)
#define G_SETSCISSOR_MODE(a) (a.b8[7])
#define G_SETSCISSOR_LRX(a) (((a.b32[1] & 0x00fff000) >> 12) >> 2)
#define G_SETSCISSOR_LRY(a) ((a.b32[1] & 0x00000fff) >> 2)

	// Extra shift / by 4 cos you need it and stuff

#define G_FILLRECT_ULX(a) ((a.b32[1] & 0x00ffc000) >> 14)
#define G_FILLRECT_ULY(a) ((a.b32[1] & 0x00000ffc) >> 2)
#define G_FILLRECT_LRX(a) ((a.b32[0] & 0x00ffc000) >> 14)
#define G_FILLRECT_LRY(a) ((a.b32[0] & 0x00000ffc) >> 2)

#define G_MTX_PROJECTION (0x01)
#define G_MTX_LOAD (0x02)
#define G_MTX_PUSH (0x04)

#define G_TRI1_FLAGS(a) (a.b8[7])
#define G_TRI1_V0(a) (a.b8[6] / 10)
#define G_TRI1_V1(a) (a.b8[5] / 10)
#define G_TRI1_V2(a) (a.b8[4] / 10)

#define G_VTX_N(a) ((a.b8[2] >> 4) + 1)
#define G_VTX_V0(a) (a.b8[2] & 0x0f)
#define G_VTX_L(a) (a.b16[0])
#define G_VTX_V(a) (a.b32[1])
#define G_VTX_NFAST(a) (a.b16[0] >> 4)

#define G_TEXTURE_LEVEL(a) ((a.b32[0] & 0x00003800) >> 11)
#define G_TEXTURE_TILE(a) ((a.b32[0] & 0x00000700) >> 8)
#define G_TEXTURE_ON(a) (a.b8[0])
#define G_TEXTURE_S(a) (a.b16[3])
#define G_TEXTURE_T(a) (a.b16[2])

#define G_GEOMETRYMODE_MODE(a) (a.b32[1])

#define G_OTHERMODE_SHIFT(a) (a.b8[1])
#define G_OTHERMODE_LEN(a) (a.b8[0])
#define G_OTHERMODE_DATA(a) (a.b32[1])
