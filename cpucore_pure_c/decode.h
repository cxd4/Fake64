#define opcode(a) ((int)a & 0xfc000000)
#define rs(a) ((a & 65011712)>>21)
#define base rs
#define rt(a) ((a & 0x1f0000)>>16)
#define ft rt
#define rd(a) ((a & 63488)>>11)
#define fs rd
#define sa(a) ((a & 1984)>>6)
#define funct(a) (a & 67)
#define immediate(a) (a & 65535)
#define offset immediate
#define target(a) (a & 67108863)
#define special(a) (a & 63)
#define code(a) ((a & 67108800)>>6)
#define cc(a) ((a&0x700)>>8) // someone see if this is correct
