#include <general.h>
#include "opcodes.h"

cpu_instruction dcpu_special[64] = {
dCPU_SPECIAL_SLL,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_SRL,
dCPU_SPECIAL_SRA,
dCPU_SPECIAL_SLLV,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_SRLV,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_JR,
dCPU_SPECIAL_JALR,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_BREAK,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_MFHI,
dCPU_SPECIAL_MTHI,
dCPU_SPECIAL_MFLO,
dCPU_SPECIAL_MTLO,
dCPU_SPECIAL_DSLLV,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_DSRLV,
dCPU_SPECIAL_DSRAV,
dCPU_SPECIAL_MULT,
dCPU_SPECIAL_MULTU,
dCPU_SPECIAL_DIV,
dCPU_SPECIAL_DIVU,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_DMULTU,
dCPU_SPECIAL_DDIV,
dCPU_SPECIAL_DDIVU,
dCPU_SPECIAL_ADD,
dCPU_SPECIAL_ADDU,
dCPU_SPECIAL_SUB,
dCPU_SPECIAL_SUBU,
dCPU_SPECIAL_AND,
dCPU_SPECIAL_OR,
dCPU_SPECIAL_XOR,
dCPU_SPECIAL_NOR,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_SLT,
dCPU_SPECIAL_SLTU,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_DADDU,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_DSLL32,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_UNIMPLEMENTED,
dCPU_SPECIAL_DSRA32
};

cpu_instruction dcpu_regimm[32] =
{
dCPU_REGIMM_BLTZ,
dCPU_REGIMM_BGEZ,
dCPU_REGIMM_BLTZL,
dCPU_REGIMM_BGEZL,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_BGEZAL,        
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED,
dCPU_REGIMM_UNIMPLEMENTED
};

cpu_instruction dcpu_cop0[64] =
{
dCPU_COP0_MOVE,
dCPU_COP0_TLBR,
dCPU_COP0_TLBWI,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_TLBP,
dCPU_COP0_UNIMPLEMENTED, 
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,   
dCPU_COP0_UNIMPLEMENTED, 
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_ERET,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,  
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED,
dCPU_COP0_UNIMPLEMENTED
};

cpu_instruction dcpu_cop1[32] =
{
dCPU_COP1_MFC1,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_CFC1,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_MTC1,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_CTC1,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_S,
dCPU_COP1_D,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_W,
dCPU_COP1_L,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED
};
cpu_instruction dcpu_cop1_s[64] =
{
dCPU_COP1_ADDS,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_MULS,
dCPU_COP1_DIVS,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED
};

cpu_instruction dcpu_cop1_d[64] =
{
dCPU_COP1_ADDD,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_TRUNCWD,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED
};

cpu_instruction dcpu_instr[64] =
{ 
dCPU_SPECIAL, 
dCPU_REGIMM,
dCPU_J,
dCPU_JAL,
dCPU_BEQ,
dCPU_BNE,
dCPU_BLEZ,
dCPU_BGTZ,
dCPU_ADDI,
dCPU_ADDIU,
dCPU_SLTI,
dCPU_SLTIU,
dCPU_ANDI,
dCPU_ORI,
dCPU_XORI,
dCPU_LUI,
dCPU_COP0,
dCPU_COP1,
dCPU_COP2,
dCPU_COP1X, 
dCPU_BEQL,
dCPU_BNEL,
dCPU_BLEZL,
dCPU_BGTZL,
dCPU_DADDI,
dCPU_DADDIU,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_LB,
dCPU_LH,
dCPU_LWL,
dCPU_LW,
dCPU_LBU,
dCPU_LHU,
dCPU_LWR,
dCPU_UNIMPLEMENTED,
dCPU_SB,
dCPU_SH,
dCPU_SWL,
dCPU_SW,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_SWR,
dCPU_COP0_CACHE,
dCPU_UNIMPLEMENTED,
dCPU_LWC1,
dCPU_LWC2,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_LDC1,
dCPU_UNIMPLEMENTED,
dCPU_LD,
dCPU_UNIMPLEMENTED,
dCPU_SWC1,
dCPU_SWC2,
dCPU_UNIMPLEMENTED,
dCPU_UNIMPLEMENTED,
dCPU_SDC1,
dCPU_UNIMPLEMENTED,
dCPU_SD
};

cpu_instruction dcpu_cop1_w[64] =
{
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_CVTSW,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED
};

cpu_instruction dcpu_cop1_l[64] =
{
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED,
dCPU_COP1_UNIMPLEMENTED
};
