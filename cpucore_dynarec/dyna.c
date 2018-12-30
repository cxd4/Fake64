#include <general.h>
#include <decode.h>
#include "registers.h"
#include <romheader.h>
#include <memory.h>
#include "dynstruct.h"
#include "x86.h"
#include "opcodes.h"

//#DEFINE DEBUG

#ifdef DEBUG
#define INTERPRET(x) printf(#x"\n"); MOV_Imm32ToMem(&op,dynop); CALL((uint32)(x));
#else
#define INTERPRET(x) MOV_Imm32ToMem(&op,dynop); CALL((uint32)(x));
#endif

extern int do_a_dump();
extern cpu_instruction dyna_special[64];
extern cpu_instruction dyna_regimm[32];
extern cpu_instruction dyna_cop0[64];
extern cpu_instruction dyna_cop1[64];
extern cpu_instruction dyna_cop1_s[64];
extern cpu_instruction dyna_cop1_d[64];
extern cpu_instruction dyna_cop1_l[64];
extern cpu_instruction dyna_cop1_w[64];
extern cpu_instruction dyna_instr[64];

extern void *codeblockpointer;
extern uint32 op;
extern uint32 dynop;
extern uint32 currentpc;
extern uint32 count;
extern uint32 jumped;
extern struct cpu_reg reg;


int DYNCPU_UNIMPLEMENTED(void) {
  printf("Unimplemented dynarec instruction: %d\n",opcode(dynop)>>26);
  exit(-1);
}

int DYNCPU_SPECIAL_UNIMPLEMENTED(void) {
  printf("Unimplemented special dynarec instruction %d\n",special(dynop));
  exit(-1);
}

int DYNCPU_REGIMM_UNIMPLEMENTED(void) {
  printf("Unimplemented regimm dynarec instruction\n");
  exit(-1);
}

int DYNCPU_COP0_UNIMPLEMENTED(void) {
  printf("Unimplemented cop0 dynarec instruction\n");
  exit(-1);
}

int DYNCPU_COP1_UNIMPLEMENTED(void) {
  printf("Unimplemented cop1 dynarec instruction\n");
  exit(-1);
}

int DYNCPU_SPECIAL(void) { dyna_special[special(dynop)](); }
int DYNCPU_REGIMM(void) { dyna_regimm[rt(dynop)](); }
int DYNCPU_COP0(void) { dyna_cop0[special(dynop)](); }
int DYNCPU_COP1(void) { dyna_cop1[base(dynop)](); }
int DYNCPU_COP1_S(void) { dyna_cop1_s[funct(dynop)](); }
int DYNCPU_COP1_D(void) { dyna_cop1_d[funct(dynop)](); }
int DYNCPU_COP1_L(void) { dyna_cop1_l[funct(dynop)](); }
int DYNCPU_COP1_W(void) { dyna_cop1_w[funct(dynop)](); }

int DYNCPU_SPECIAL_SLL(void) { INTERPRET(eCPU_SPECIAL_SLL); }
int DYNCPU_SPECIAL_SLLV(void) { INTERPRET(eCPU_SPECIAL_SLLV); }
int DYNCPU_SPECIAL_SLT(void) { INTERPRET(eCPU_SPECIAL_SLT); }
int DYNCPU_SPECIAL_SLTU(void) { INTERPRET(eCPU_SPECIAL_SLTU); }
int DYNCPU_LUI(void) { INTERPRET(eCPU_LUI); }
int DYNCPU_ORI(void) { INTERPRET(eCPU_ORI); }
int DYNCPU_SPECIAL_ADD(void) { INTERPRET(eCPU_SPECIAL_ADD); }
int DYNCPU_ADDI(void) { INTERPRET(eCPU_ADDI); }
int DYNCPU_ADDIU(void) { INTERPRET(eCPU_ADDIU); }
int DYNCPU_SPECIAL_ADDU(void) { INTERPRET(eCPU_SPECIAL_ADDU); }
int DYNCPU_SH(void) { INTERPRET(eCPU_SH); }
int DYNCPU_LHU(void) { INTERPRET(eCPU_LHU); }
int DYNCPU_SB(void) { INTERPRET(eCPU_SB); }
int DYNCPU_LBU(void) { INTERPRET(eCPU_LBU); }
int DYNCPU_LB(void) { INTERPRET(eCPU_LB); }
int DYNCPU_SD(void) { INTERPRET(eCPU_SD); }
int DYNCPU_SW(void) { INTERPRET(eCPU_SW); }
int DYNCPU_LW(void) { INTERPRET(eCPU_LW); }
int DYNCPU_LD(void) { INTERPRET(eCPU_LD); }
int DYNCPU_LH(void) { INTERPRET(eCPU_LH); }
int DYNCPU_SPECIAL_OR(void) { INTERPRET(eCPU_SPECIAL_OR); }
int DYNCPU_SPECIAL_NOR(void) { INTERPRET(eCPU_SPECIAL_NOR); }
int DYNCPU_ANDI(void) { INTERPRET(eCPU_ANDI); }
int DYNCPU_SPECIAL_SUB(void) { INTERPRET(eCPU_SPECIAL_SUB);}
int DYNCPU_SPECIAL_SUBU(void) { INTERPRET(eCPU_SPECIAL_SUBU);}
int DYNCPU_SPECIAL_SRL(void) { INTERPRET(eCPU_SPECIAL_SRL);}
int DYNCPU_SPECIAL_SRLV(void) { INTERPRET(eCPU_SPECIAL_SRLV);}
int DYNCPU_SPECIAL_AND(void) { INTERPRET(eCPU_SPECIAL_AND);}
int DYNCPU_SPECIAL_MULT(void) { INTERPRET(eCPU_SPECIAL_MULT);}
int DYNCPU_SPECIAL_MULTU(void) {  INTERPRET(eCPU_SPECIAL_MULTU);}
int DYNCPU_SPECIAL_DMULTU(void) { INTERPRET(eCPU_SPECIAL_DMULTU);}
int DYNCPU_SPECIAL_MFLO(void){ INTERPRET(eCPU_SPECIAL_MFLO);}
int DYNCPU_SPECIAL_MFHI(void){ INTERPRET(eCPU_SPECIAL_MFHI);}
int DYNCPU_SPECIAL_DSLL32(void){ INTERPRET(eCPU_SPECIAL_DSLL32);}
int DYNCPU_SPECIAL_DSRA32(void){ INTERPRET(eCPU_SPECIAL_DSRA32);}
int DYNCPU_SPECIAL_DDIVU(void){ INTERPRET(eCPU_SPECIAL_DDIVU);}
int DYNCPU_SPECIAL_DIV(void){ INTERPRET(eCPU_SPECIAL_DIV);}
int DYNCPU_SLTI(void){ INTERPRET(eCPU_SLTI);}
int DYNCPU_SPECIAL_SRA(void){ INTERPRET(eCPU_SPECIAL_SRA);}
int DYNCPU_SLTIU(void){ INTERPRET(eCPU_SLTIU);}
int DYNCPU_SPECIAL_MTLO(void){ INTERPRET(eCPU_SPECIAL_MTLO);}
int DYNCPU_SPECIAL_MTHI(void){ INTERPRET(eCPU_SPECIAL_MTHI);}
int DYNCPU_SPECIAL_XOR(void){ INTERPRET(eCPU_SPECIAL_XOR);}
int DYNCPU_XORI(void){ INTERPRET(eCPU_XORI);}
int DYNCPU_COP0_MOVE(void) { INTERPRET(eCPU_COP0_MOVE);}
int DYNCPU_COP0_TLBWI(void) { INTERPRET(eCPU_COP0_TLBWI);}
int DYNCPU_COP0_CACHE(void) { }
int DYNCPU_COP1_MFC1(void) { INTERPRET(eCPU_COP1_MFC1);}
int DYNCPU_COP1_CFC1(void) { INTERPRET(eCPU_COP1_CFC1);}
int DYNCPU_COP1_MTC1(void) { INTERPRET(eCPU_COP1_MTC1);}
int DYNCPU_COP1_CTC1(void) { INTERPRET(eCPU_COP1_CTC1);}
int DYNCPU_COP0_TLBP(void) { INTERPRET(eCPU_COP0_TLBP);}
int DYNCPU_COP0_TLBR(void) { INTERPRET(eCPU_COP0_TLBR);}
int DYNCPU_LWC1(void) {INTERPRET(eCPU_LWC1);}
int DYNCPU_LDC1(void) {INTERPRET(eCPU_LDC1);}
int DYNCPU_COP1_CVTSW(void) {INTERPRET(eCPU_COP1_CVTSW);}
int DYNCPU_COP1_SUBS(void) {INTERPRET(eCPU_COP1_SUBS);}
int DYNCPU_COP1_ADDS(void) {INTERPRET(eCPU_COP1_ADDS);}
int DYNCPU_COP1_MULS(void) {INTERPRET(eCPU_COP1_MULS);}
int DYNCPU_COP1_DIVS(void) {INTERPRET(eCPU_COP1_DIVS);}
int DYNCPU_COP1_CVTDS(void) {INTERPRET(eCPU_COP1_CVTDS);}
int DYNCPU_COP1_CVTWS(void) {INTERPRET(eCPU_COP1_CVTWS);}
int DYNCPU_COP1_ADDD(void) {INTERPRET(eCPU_COP1_ADDD);}
int DYNCPU_COP1_TRUNCWD(void) {INTERPRET(eCPU_COP1_TRUNCWD);}
// Branches, special cases

int DYNCPU_BNE(void) {
 uint32 rpc;
 uint32 tempop;
 uint8 *diff,*temppointer,*temppointer2;
 
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);
 temppointer=codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=codeblockpointer;
 codeblockpointer=temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_E,0x1D+temppointer2-diff); //opposite of NE
 codeblockpointer=temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop; 
 jumped=1;
}

int DYNCPU_BEQ(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;
 
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_NE,0x1D+temppointer2-diff); //opposite of E
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop;
 jumped=1;
}

int DYNCPU_BEQL(void) { 
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_NE,0x1D+temppointer2-diff); //opposite of EQ
 codeblockpointer=(void *)temppointer2; 

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 jumped=1;
}

int DYNCPU_BNEL(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_E,0x1D+temppointer2-diff); //opposite of NE
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 jumped=1;
}

int DYNCPU_BLEZL(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_G,0x1D+temppointer2-diff); //opposite of LE (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 jumped=1;
}

int DYNCPU_BLEZAL(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_Imm32ToMem(&reg.gpr[31],(currentpc+8));
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot 
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_G,0x1D+temppointer2-diff); //opposite of LE (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop;
 jumped=1;
}

int DYNCPU_REGIMM_BGEZAL(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;
 
 MOV_Imm32ToMem(&reg.gpr[31],(currentpc+8));
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot 
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_L,0x1D+temppointer2-diff); //opposite of GE (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop;
 jumped=1;
}

int DYNCPU_BLEZ(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;
 
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_G,0x1D+temppointer2-diff); //opposite of LE (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop;
 jumped=1;
}

int DYNCPU_BGTZ(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_LE,0x1D+temppointer2-diff); //opposite of G (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop;
 jumped=1;
}

int DYNCPU_REGIMM_BGEZ(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_L,0x1D+temppointer2-diff); //opposite of GE (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 dynop=tempop;
 jumped=1;
}

int DYNCPU_REGIMM_BLTZ(void) {
 uint32 rpc;
 uint32 tempop;
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;

 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 Jcc(CC_GE,0x1D); //opposite of L
 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 jumped=1;
}

int DYNCPU_REGIMM_BGEZL(void) {
 uint32 rpc;
 uint32 tempop;
 uint32 diff,temppointer,temppointer2;

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 temppointer=(uint32)codeblockpointer;
 W8(0x90); W8(0x90); // reserve space for Jcc
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 diff=(uint32)codeblockpointer;
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 temppointer2=(uint32)codeblockpointer;
 codeblockpointer=(void *)temppointer;
 //printf("CMPcc 0x%lx 0x%lx\n regs %d %d\n",&reg.gpr[rs(dynop)],&reg.gpr[rt(dynop)],rs(dynop),rt(dynop));
 Jcc(CC_L,0x1D+temppointer2-diff); //opposite of GE (Zero)
 codeblockpointer=(void *)temppointer2;

 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 jumped=1;
}

int DYNCPU_SPECIAL_JALR(void) {
 uint32 rpc;
 uint32 tempop;
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 MOV_Imm32ToMem(&reg.gpr[rd(dynop)],(currentpc+8));
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_RegToMem(EAX,&reg.pc);
 jumped=2;
}

int DYNCPU_SPECIAL_JR(void) {
 uint32 rpc;
 uint32 tempop;
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_RegToMem(EAX,&reg.pc);
 jumped=2;
}

int DYNCPU_JAL(void) {
 uint32 rpc;
 uint32 tempop;
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 MOV_Imm32ToMem(&reg.gpr[31],(currentpc+8));
 MOV_Imm32ToMem(&reg.pc,(reg.pc&(0xF<<28))|((target(dynop)<<2)&0xFFFFFFF));
 jumped=2;
}

int DYNCPU_CODEBLOCK(void)
{

  jumped=1;
  currentpc-=8; // -4 for this instruction, -4 for "delay slot" -> makes it encode the currentpc
}

int DYNCPU_J(void) {
 uint32 rpc;
 uint32 tempop;
 rpc=currentpc&0x1fffffff;
 rpc+=4; // delay slot
 tempop=dynop;
 dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
 if (dynop) dyna_instr[opcode(dynop)>>26](); // do delay slot
 count++; // 1 cycle for delay slot
 dynop=tempop;
 MOV_Imm32ToMem(&reg.pc,(reg.pc&(0xF<<28))|((target(dynop)<<2)&0xFFFFFFF));
 jumped=2;
}

int DYNCPU_COP0_ERET(void) {

 INTERPRET(eCPU_COP0_ERET);
/* MOV_Imm8ToMem(&reg.LLbit,0);
 BT32_RegWithImm(&reg.gpr0[12],0x4);
 Jcc(CC_C,0x29);
 MOV_MemToReg(EAX,&COP0_COUNT); // fix with register caching, 32 bits is ok
 ADDI(EAX,count+1); // plus one for this instruction
 MOV_RegToMem(EAX,&COP0_COUNT);
 MOV_MemToReg(EAX,&reg.gpr0[14]); // EPC
 MOV_RegToMem(EAX,&reg.pc);
 AND_Imm32ToMem(&reg.gpr0[12],~0x4);
 RET();
 MOV_MemToReg(EAX,&reg.gpr0[30]); // ERROR_EPC
 MOV_RegToMem(EAX,&reg.pc);
 AND_Imm32ToMem(&reg.gpr0[12],~0x2);*/
 jumped=2;
}
