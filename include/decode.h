#define opcode(a) ((int)a & 0xfc000000)
#define rs(a) ((a & 0x3E00000)>>21)
#define base rs
#define fmt rs
#define rt(a) ((a & 0x1f0000)>>16)
#define ft rt
#define rd(a) ((a & 0xF800)>>11)
#define fs rd
#define sa(a) ((a & 0x7C0)>>6)
#define fd sa
#define funct(a) (a & 63) // rs noticed this (was 67)
#define immediate(a) (a & 0xFFFF)
#define offset immediate
#define target(a) (a & 0x3FFFFFF)
#define special(a) (a & 63)
#define code(a) ((a & 0x3FFFFC0)>>6)
#define cc(a) ((a&0x700)>>8) // someone see if this is correct

//#define VPN2(a) ((a&0xFFFFFFE000)>>13)
#define VPN2(a) ((a&0xFFFFE000)>>13)
#define GBIT(a) ((a&0x1))
#define ASID(a) ((a&0xFF))
//#define MASK(a) ((a&0x1FFE000)>>13)
