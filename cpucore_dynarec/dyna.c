#include <general.h>
#include <decode.h>
#include "registers.h"
#include <romheader.h>
#include <memory.h>
#include "dynstruct.h"
#include "x86.h"
#include "opcodes.h"

//#DEFINE DEBUG
#define COMPAT_MODE


#define JUMP_VARS uint8 *temppointer,*temppointer2;

#define JUMP_MARK temppointer=codeblockpointer; W16(0x9090);

#define JUMP_SET(x) temppointer2=codeblockpointer; codeblockpointer=temppointer; Jcc((x),(uint32)temppointer2-(uint32)codeblockpointer-2); codeblockpointer=temppointer2;

#define DELAYSLOT_VARS uint32 rpc,tempop;

#define DELAYSLOT  rpc=currentpc&0x1fffffff; rpc+=4; tempop=dynop; dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16])); if (dynop) dyna_instr[opcode(dynop)>>26](); dynop=tempop;

#define PARSE_CPUDELAY 						\
								\
MOV_MemToReg(EAX,&reg.CPUdelay);				\
CMP_RegWithImm(EAX,1);						\
								\
JUMP_MARK							\
DELAYSLOT							\
ADD_ImmToMem(&COP0_COUNT,count+2);                              \
MOV_MemToReg(EAX,&reg.CPUdelayPC);				\
MOV_RegToMem(EAX,&reg.pc);					\
MOV_Imm32ToMem(&reg.CPUdelay,0);				\
RET();								\
JUMP_SET(CC_NE)							\
								\
MOV_MemToReg(EAX,&reg.CPUdelay);				\
CMP_RegWithImm(EAX,2);						\
								\
JUMP_MARK							\
ADD_ImmToMem(&COP0_COUNT,count+1);                                \
MOV_MemToReg(EAX,&reg.CPUdelayPC);				\
MOV_RegToMem(EAX,&reg.pc);					\
MOV_Imm32ToMem(&reg.CPUdelay,0);                                \
RET();								\
JUMP_SET(CC_NE)							\
jumped=1;							\
ADD_ImmToMem(&COP0_COUNT,count+1);				\
currentpc+=8; 							\
MOV_Imm32ToMem(&reg.pc,currentpc);				\
MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);								\
RET();


#ifdef DEBUG
#define INTERPRET(x) printf(#x"\n"); MOV_Imm32ToMem(&op,dynop); CALL((uint32)(x));
#else
#define INTERPRET(x) MOV_Imm32ToMem(&op,dynop); CALL((uint32)(x));
#endif

extern void do_a_dump();
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


void DYNCPU_UNIMPLEMENTED(void) {
  printf("Unimplemented dynarec instruction: %d\n",opcode(dynop)>>26);
  exit(-1);
}

void DYNCPU_SPECIAL_UNIMPLEMENTED(void) {
  printf("Unimplemented special dynarec instruction %d\n",special(dynop));
  exit(-1);
}

void DYNCPU_REGIMM_UNIMPLEMENTED(void) {
  printf("Unimplemented regimm dynarec instruction\n");
  exit(-1);
}

void DYNCPU_COP0_UNIMPLEMENTED(void) {
  printf("Unimplemented cop0 dynarec instruction\n");
  exit(-1);
}

void DYNCPU_COP1_UNIMPLEMENTED(void) {
  printf("Unimplemented cop1 dynarec instruction\n");
  exit(-1);
}

void DYNCPU_SPECIAL(void) { dyna_special[special(dynop)](); }
void DYNCPU_REGIMM(void) { dyna_regimm[rt(dynop)](); }
void DYNCPU_COP0(void) { dyna_cop0[special(dynop)](); }
void DYNCPU_COP1(void) { dyna_cop1[base(dynop)](); }
void DYNCPU_COP1_S(void) { dyna_cop1_s[funct(dynop)](); }
void DYNCPU_COP1_D(void) { dyna_cop1_d[funct(dynop)](); }
void DYNCPU_COP1_L(void) { dyna_cop1_l[funct(dynop)](); }
void DYNCPU_COP1_W(void) { dyna_cop1_w[funct(dynop)](); }

void DYNCPU_SPECIAL_SLL(void) { INTERPRET(eCPU_SPECIAL_SLL); }
void DYNCPU_SPECIAL_SLLV(void) { INTERPRET(eCPU_SPECIAL_SLLV); }
void DYNCPU_SPECIAL_SLT(void) { INTERPRET(eCPU_SPECIAL_SLT); }
void DYNCPU_SPECIAL_SLTU(void) { INTERPRET(eCPU_SPECIAL_SLTU); }
void DYNCPU_SPECIAL_ADDU(void) { INTERPRET(eCPU_SPECIAL_ADDU); }
void DYNCPU_SH(void) { INTERPRET(eCPU_SH); }
void DYNCPU_LHU(void) { INTERPRET(eCPU_LHU); }
void DYNCPU_SB(void) { INTERPRET(eCPU_SB); }
void DYNCPU_LBU(void) { INTERPRET(eCPU_LBU); }
void DYNCPU_LB(void) { INTERPRET(eCPU_LB); }
void DYNCPU_SD(void) { INTERPRET(eCPU_SD); }
void DYNCPU_SW(void) { INTERPRET(eCPU_SW); }
void DYNCPU_LW(void) { INTERPRET(eCPU_LW); }
void DYNCPU_LD(void) { INTERPRET(eCPU_LD); }
void DYNCPU_LH(void) { INTERPRET(eCPU_LH); }
void DYNCPU_SPECIAL_OR(void) { INTERPRET(eCPU_SPECIAL_OR); }
void DYNCPU_SPECIAL_NOR(void) { INTERPRET(eCPU_SPECIAL_NOR); }
void DYNCPU_ANDI(void) { INTERPRET(eCPU_ANDI); }
void DYNCPU_SPECIAL_SUB(void) { INTERPRET(eCPU_SPECIAL_SUB);}
void DYNCPU_SPECIAL_SUBU(void) { INTERPRET(eCPU_SPECIAL_SUBU);}
void DYNCPU_SPECIAL_SRL(void) { INTERPRET(eCPU_SPECIAL_SRL);}
void DYNCPU_SPECIAL_SRLV(void) { INTERPRET(eCPU_SPECIAL_SRLV);}
void DYNCPU_SPECIAL_AND(void) { INTERPRET(eCPU_SPECIAL_AND);}
void DYNCPU_SPECIAL_MULT(void) { INTERPRET(eCPU_SPECIAL_MULT);}
void DYNCPU_SPECIAL_MULTU(void) {  INTERPRET(eCPU_SPECIAL_MULTU);}
void DYNCPU_SPECIAL_DMULTU(void) { INTERPRET(eCPU_SPECIAL_DMULTU);}
void DYNCPU_SPECIAL_MFLO(void){ INTERPRET(eCPU_SPECIAL_MFLO);}
void DYNCPU_SPECIAL_MFHI(void){ INTERPRET(eCPU_SPECIAL_MFHI);}
void DYNCPU_SPECIAL_DSLL32(void){ INTERPRET(eCPU_SPECIAL_DSLL32);}
void DYNCPU_SPECIAL_DSRA32(void){ INTERPRET(eCPU_SPECIAL_DSRA32);}
void DYNCPU_SPECIAL_DDIVU(void){ INTERPRET(eCPU_SPECIAL_DDIVU);}
void DYNCPU_SPECIAL_DIV(void){ INTERPRET(eCPU_SPECIAL_DIV);}
void DYNCPU_SPECIAL_DIVU(void){ INTERPRET(eCPU_SPECIAL_DIVU);}
void DYNCPU_SLTI(void){ INTERPRET(eCPU_SLTI);}
void DYNCPU_SPECIAL_SRA(void){ INTERPRET(eCPU_SPECIAL_SRA);}
void DYNCPU_SLTIU(void){ INTERPRET(eCPU_SLTIU);}
void DYNCPU_SPECIAL_MTLO(void){ INTERPRET(eCPU_SPECIAL_MTLO);}
void DYNCPU_SPECIAL_MTHI(void){ INTERPRET(eCPU_SPECIAL_MTHI);}
void DYNCPU_SPECIAL_XOR(void){ INTERPRET(eCPU_SPECIAL_XOR);}
void DYNCPU_XORI(void){ INTERPRET(eCPU_XORI);}
void DYNCPU_COP0_MOVE(void) { INTERPRET(eCPU_COP0_MOVE);}
void DYNCPU_COP0_TLBWI(void) { INTERPRET(eCPU_COP0_TLBWI);}
void DYNCPU_COP0_CACHE(void) { }
void DYNCPU_COP1_MFC1(void) { INTERPRET(eCPU_COP1_MFC1);}
void DYNCPU_COP1_CFC1(void) { INTERPRET(eCPU_COP1_CFC1);}
void DYNCPU_COP1_MTC1(void) { INTERPRET(eCPU_COP1_MTC1);}
void DYNCPU_COP1_CTC1(void) { INTERPRET(eCPU_COP1_CTC1);}
void DYNCPU_COP0_TLBP(void) { INTERPRET(eCPU_COP0_TLBP);}
void DYNCPU_COP0_TLBR(void) { INTERPRET(eCPU_COP0_TLBR);}
void DYNCPU_LWC1(void) {INTERPRET(eCPU_LWC1);}
void DYNCPU_LDC1(void) {INTERPRET(eCPU_LDC1);}
void DYNCPU_COP1_CVTSW(void) {INTERPRET(eCPU_COP1_CVTSW);}
void DYNCPU_COP1_SUBS(void) {INTERPRET(eCPU_COP1_SUBS);}
void DYNCPU_COP1_ADDS(void) {INTERPRET(eCPU_COP1_ADDS);}
void DYNCPU_COP1_MULS(void) {INTERPRET(eCPU_COP1_MULS);}
void DYNCPU_COP1_DIVS(void) {INTERPRET(eCPU_COP1_DIVS);}
void DYNCPU_COP1_CVTDS(void) {INTERPRET(eCPU_COP1_CVTDS);}
void DYNCPU_COP1_CVTWS(void) {INTERPRET(eCPU_COP1_CVTWS);}
void DYNCPU_COP1_ADDD(void) {INTERPRET(eCPU_COP1_ADDD);}
void DYNCPU_COP1_TRUNCWD(void) {INTERPRET(eCPU_COP1_TRUNCWD);}
void DYNCPU_SDC1(void) {INTERPRET(eCPU_SDC1);}
void DYNCPU_SWC1(void) {INTERPRET(eCPU_SWC1);}
void DYNCPU_SWL(void) {INTERPRET(eCPU_SWL);}
void DYNCPU_SWR(void) {INTERPRET(eCPU_SWR);}
void DYNCPU_LWL(void) {INTERPRET(eCPU_LWL);}
void DYNCPU_LWR(void) {INTERPRET(eCPU_LWR);}
void DYNCPU_SPECIAL_SRAV(void) {INTERPRET(eCPU_SPECIAL_SRAV);}
void DYNCPU_COP1_SUBD(void) { INTERPRET(eCPU_COP1_SUBD); }
void DYNCPU_COP1_MOVS(void) { INTERPRET(eCPU_COP1_MOVS);}
void DYNCPU_COP1_CVTSL(void) { INTERPRET(eCPU_COP1_CVTSL);}
void DYNCPU_COP1_CVTDW(void)  { INTERPRET(eCPU_COP1_CVTDW);}
void DYNCPU_COP1_CVTWD(void) { INTERPRET(eCPU_COP1_CVTWD);} 
void DYNCPU_COP1_CVTLD(void)  { INTERPRET(eCPU_COP1_CVTLD);}
void DYNCPU_COP1_CVTSD(void) { INTERPRET(eCPU_COP1_CVTSD);}
void DYNCPU_COP1_MULD(void) { INTERPRET(eCPU_COP1_MULD);}
void DYNCPU_COP1_TRUNCWS(void) { INTERPRET(eCPU_COP1_TRUNCWS);}
void DYNCPU_COP1_NEGS(void) { INTERPRET(eCPU_COP1_NEGS);}
void DYNCPU_COP1_NEGD(void) { INTERPRET(eCPU_COP1_NEGD);}
void DYNCPU_COP1_DIVD(void) { INTERPRET(eCPU_COP1_DIVD);}
void DYNCPU_COP1_SQRTS(void) { INTERPRET(eCPU_COP1_SQRTS);}
void DYNCPU_COP1_MOVD(void) { INTERPRET(eCPU_COP1_MOVD);}
/*void DYNCPU_COP1_CF(void) { INTERPRET(eCPU_COP1_CF);}
void DYNCPU_COP1_CUN(void) { INTERPRET(eCPU_COP1_CUN);}
void DYNCPU_COP1_CEQ(void) { INTERPRET(eCPU_COP1_CEQ);}
void DYNCPU_COP1_CUEQ(void) { INTERPRET(eCPU_COP1_CUEQ);}
void DYNCPU_COP1_COLT(void) { INTERPRET(eCPU_COP1_COLT);}
void DYNCPU_COP1_CULT(void) { INTERPRET(eCPU_COP1_CULT);}
void DYNCPU_COP1_COLE(void) { INTERPRET(eCPU_COP1_COLE);}
void DYNCPU_COP1_CULE(void) { INTERPRET(eCPU_COP1_CULE);}
void DYNCPU_COP1_CNGLE(void) { INTERPRET(eCPU_COP1_CNGLE);}
void DYNCPU_COP1_CSEQ(void) { INTERPRET(eCPU_COP1_CSEQ);}
void DYNCPU_COP1_CNGL(void) { INTERPRET(eCPU_COP1_CNGL);}
void DYNCPU_COP1_CLT(void) { INTERPRET(eCPU_COP1_CLT);}
void DYNCPU_COP1_CNGE(void) { INTERPRET(eCPU_COP1_CNGE);}
void DYNCPU_COP1_CLE(void) { INTERPRET(eCPU_COP1_CLE);}
void DYNCPU_COP1_CNGT(void) { INTERPRET(eCPU_COP1_CNGT);}*/
void DYNCPU_COP1_CCS(void) { INTERPRET(eCPU_COP1_CCS);}
void DYNCPU_COP1_CCD(void) { INTERPRET(eCPU_COP1_CCD);}
void DYNCPU_COP1_CCW(void) { INTERPRET(eCPU_COP1_CCW);}
void DYNCPU_COP1_CCL(void) { INTERPRET(eCPU_COP1_CCL);}
void DYNCPU_SPECIAL_SYNC(void){}

extern int run;

void DYNCPU_SPECIAL_ADD(void) { 
#ifdef COMPAT_MODE
INTERPRET(eCPU_SPECIAL_ADD);
#else
printf("DYNA ADD\n");
MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
ADD_MemToEAX(&reg.gpr[rt(dynop)]);
MOV_RegToMem(EAX,&reg.gpr[rd(dynop)]);
CDQ();
MOV_RegToMem(EDX,((uint32)&reg.gpr[rd(dynop)])+4);
#endif
}


void DYNCPU_ADDIU(void)
{
#ifdef COMPAT_MODE
  INTERPRET(eCPU_ADDIU);
#else
        printf("DYNA ADDIU\n");
        MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
        ADDI(EAX,(int16)immediate(dynop));
        MOV_RegToMem(EAX,&reg.gpr[rt(dynop)]);
        CDQ();
        MOV_RegToMem(EDX,((uint32)&reg.gpr[rt(dynop)])+4);
#endif
}


void DYNCPU_ADDI(void)
{
#ifdef COMPAT_MODE
 INTERPRET(eCPU_ADDI);
#else
	printf("DYNA ADDI\n");
	MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
	ADDI(EAX,(int16)immediate(dynop));
	MOV_RegToMem(EAX,&reg.gpr[rt(dynop)]);
	CDQ();
	MOV_RegToMem(EDX,((uint32)&reg.gpr[rt(dynop)])+4);
#endif
}

void DYNCPU_LUI(void) { 
#ifdef COMPAT_MODE
INTERPRET(eCPU_LUI); 
#else
printf("DYNA LUI\n");
MOV_Imm32ToMem(&reg.gpr[rt(dynop)], immediate(dynop)<<16);
if (0x80000000&(immediate(dynop)<<16))
   MOV_Imm32ToMem((uint32)((uint32)&reg.gpr[rt(dynop)])+4,0xFFFFFFFF);
else
   MOV_Imm32ToMem((uint32)((uint32)&reg.gpr[rt(dynop)])+4,0x00000000);
#endif
}
void DYNCPU_ORI(void) { INTERPRET(eCPU_ORI); }

// Branches, special cases

void DYNCPU_SPECIAL_JR(void) 
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_SPECIAL_JR); 
  PARSE_CPUDELAY
}

void DYNCPU_J(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_J);
  PARSE_CPUDELAY
}

void DYNCPU_JAL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_JAL);
  PARSE_CPUDELAY
}

void DYNCPU_SPECIAL_JALR(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_SPECIAL_JALR);
  PARSE_CPUDELAY
}

void DYNCPU_REGIMM_BLTZ(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_REGIMM_BLTZ);
  PARSE_CPUDELAY
}

void DYNCPU_BEQ(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BEQ);
  PARSE_CPUDELAY
}

void DYNCPU_BEQL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BEQL);
  PARSE_CPUDELAY
}

void DYNCPU_BNEL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BNEL);
  PARSE_CPUDELAY
}

void DYNCPU_BNE(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BNE);
  PARSE_CPUDELAY
}

void DYNCPU_BLEZ(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BLEZ);
  PARSE_CPUDELAY
}

void DYNCPU_BLEZL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BLEZL);
  PARSE_CPUDELAY
}

void DYNCPU_BGTZ(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_BGTZ);
  PARSE_CPUDELAY
}

void DYNCPU_REGIMM_BGEZ(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_REGIMM_BGEZ);
  PARSE_CPUDELAY
}

void DYNCPU_REGIMM_BGEZL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_REGIMM_BGEZL);
  PARSE_CPUDELAY
}

void DYNCPU_COP0_ERET(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_COP0_ERET);
  PARSE_CPUDELAY
}

void DYNCPU_COP1_BC(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_COP1_BC);
  PARSE_CPUDELAY
}

void DYNCPU_REGIMM_BGEZAL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_REGIMM_BGEZAL);
  PARSE_CPUDELAY
}

void DYNCPU_REGIMM_BLTZL(void)
{ JUMP_VARS
  DELAYSLOT_VARS
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  INTERPRET(eCPU_REGIMM_BLTZL);
  PARSE_CPUDELAY
}

/*
void print_control(void)
{
  if (reg.cr1[31]&0x800000)
	{
		printf("conrol 0x%x\n",reg.cr1[31]);
	}
}

void DYNCPU_COP1_BC(void) {
 DELAYSLOT_VARS
 JUMP_VARS
 int t=(dynop>>16)&0x3;


INTERPRET(print_control);

  // PUT CONDITION IN EAX
  MOV_MemToReg(EAX,&reg.cr1[31]);

  W8(0xc1); // shift & and to get the right bit // OPTIMIZE THIS! :/
  W8(0xf8);
  W8(0x17);   
  W8(0x83);
  W8(0xe0);   
  W8(0x01);
/*
	switch((dynop>>16)&0x3)
	{
		case 0:  //BC1F


    	break;
		case 1: //BC1T
			break;	
		case 2:  //BC1FL
    	break;
  	case 3:
			break;
	}
*/
/* CMP_RegWithImm(EAX,0);

  JUMP_MARK

  DELAYSLOT

  ADD_ImmToMem(&COP0_COUNT,count+2);
  MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to 
  RET();

  switch(t)
   { case 0: JUMP_SET(CC_E); DELAYSLOT count++; break;
     case 1: JUMP_SET(CC_NE); DELAYSLOT count++; break;
     case 2: JUMP_SET(CC_E); break;
     case 3: JUMP_SET(CC_NE); break;
   }
	

	jumped=1;

 //run=0;
}
/* (gdb) x/40i 0x80f4120
0x80f4120:      movl   $0x46264000,0x40ca1c60
0x80f412a:      call   0x40c94538 <eCPU_COP1_ADDD>
0x80f412f:      movl   $0x44897000,0x40ca1c60
0x80f4139:      call   0x40c942e8 <eCPU_COP1_MTC1>
0x80f413e:      movl   $0x44887800,0x40ca1c60
0x80f4148:      call   0x40c942e8 <eCPU_COP1_MTC1>
0x80f414d:      mov    0x40ca1ffc,%eax
0x80f4153:      sar    $0x17,%eax
0x80f4156:      and    $0x1,%eax
0x80f4159:      cmp    $0x0,%eax
0x80f415f:      je     0x80f4173
0x80f4161:      addl   $0x7,0x40ca1dc8
0x80f4168:      movl   $0x80200688,0x40ca2000
0x80f4172:      ret
0x80f4173:      addl   $0x7,0x40ca1dc8
0x80f417a:      movl   $0x8020065c,0x40ca2000
0x80f4184:      movl   $0xffffffff,0x40ca2004
0x80f418e:      ret
*/

/*
void debug_BNE(void)
{
	if ((int32)reg.gpr[rs(dynop)]==(int32)reg.gpr[rs(dynop)] && (int64)reg.gpr[rs(dynop)]!=(int64)reg.gpr[rs(dynop)])
	{
		printf("This will break\n");
	}
}

void DYNCPU_BNE(void) {

 DELAYSLOT_VARS

 JUMP_VARS

 INTERPRET(debug_BNE);
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();

 JUMP_SET(CC_E);

 DELAYSLOT
 count++;

 jumped=1;
}

void DYNCPU_BEQ(void) {

 DELAYSLOT_VARS
 
 JUMP_VARS
 
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);

 JUMP_MARK

 DELAYSLOT 

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();

 JUMP_SET(CC_NE); // opposite of E
 
 DELAYSLOT
 count++;
 jumped=1;
}

void DYNCPU_BEQL(void) { 

 DELAYSLOT_VARS
 JUMP_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();

 JUMP_SET(CC_NE); // opposite of E
 jumped=1;
}

void DYNCPU_BNEL(void) {

 DELAYSLOT_VARS
 JUMP_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_MemToReg(EBX,&reg.gpr[rt(dynop)]);
 CMP_RegWithReg32(EAX,EBX);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();

 JUMP_SET(CC_E); // opposite of NE
 jumped=1;
}

void DYNCPU_BLEZL(void) {
 JUMP_VARS
 DELAYSLOT_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();

 JUMP_SET(CC_G); // opposite of LE
 jumped=1;
}

void DYNCPU_BLEZAL(void) {

 JUMP_VARS
 DELAYSLOT_VARS

 MOV_Imm32ToMem(&reg.gpr[31],(currentpc+8));

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();

 JUMP_SET(CC_G);

 DELAYSLOT
 count++;

 jumped=1;
}

void DYNCPU_REGIMM_BGEZAL(void) {

 JUMP_VARS
 DELAYSLOT_VARS
 
 MOV_Imm32ToMem(&reg.gpr[31],(currentpc+8));

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 
 JUMP_SET(CC_L);

 DELAYSLOT
 count++;
 jumped=1;
}

void DYNCPU_BLEZ(void) {

 JUMP_VARS
 DELAYSLOT_VARS
 
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK
 
 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 
 JUMP_SET(CC_G);

 DELAYSLOT
 count++;
 jumped=1;
}

void DYNCPU_BGTZ(void) {
 JUMP_VARS
 DELAYSLOT_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);
 
 JUMP_MARK
 
 DELAYSLOT 

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 JUMP_SET(CC_LE);
 DELAYSLOT
 count++;
 jumped=1;
}

void DYNCPU_REGIMM_BGEZ(void) {

 JUMP_VARS
 DELAYSLOT_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 JUMP_SET(CC_L);

 DELAYSLOT
 count++;
 jumped=1;
}

void DYNCPU_REGIMM_BLTZ(void) {
 DELAYSLOT_VARS
 JUMP_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 JUMP_SET(CC_GE);

 DELAYSLOT
 count++;
 jumped=1;
}
void DYNCPU_REGIMM_BLTZL(void) {
 DELAYSLOT_VARS
 JUMP_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 JUMP_SET(CC_GE);

 jumped=1;
}

void DYNCPU_REGIMM_BGEZL(void) {
 JUMP_VARS
 DELAYSLOT_VARS

 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 CMP_RegWithImm(EAX,0);

 JUMP_MARK

 DELAYSLOT

 ADD_ImmToMem(&COP0_COUNT,count+2);
 MOV_Imm32ToMem(&reg.pc,(currentpc+(((int16)immediate(dynop))<<2)+4));    // to skip delay slot
 RET();
 JUMP_SET(CC_L);
 jumped=1;
}

void DYNCPU_SPECIAL_JALR(void) {
 DELAYSLOT_VARS

 DELAYSLOT
 count++; // 1 cycle for delay slot

 MOV_Imm32ToMem(&reg.gpr[rd(dynop)],(currentpc+8));
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_RegToMem(EAX,&reg.pc);
 jumped=2;
}

void DYNCPU_SPECIAL_JR(void) {
 DELAYSLOT_VARS

 DELAYSLOT
 count++;
 MOV_MemToReg(EAX,&reg.gpr[rs(dynop)]);
 MOV_RegToMem(EAX,&reg.pc);
 jumped=2;
}

void DYNCPU_JAL(void) {
 DELAYSLOT_VARS

 DELAYSLOT
 count++; // 1 cycle for delay slot

 MOV_Imm32ToMem(&reg.gpr[31],(currentpc+8));
 MOV_Imm32ToMem(&reg.pc,(reg.pc&(0xF<<28))|((target(dynop)<<2)&0xFFFFFFF));
 jumped=2;
}
*/
void DYNCPU_CODEBLOCK(void)
{

  jumped=1;
  ADD_ImmToMem(&COP0_COUNT,count);
  MOV_Imm32ToMem(&reg.pc,currentpc);
  MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
  RET();
}
/*
void delay_kill()
{  if ((reg.pc&0xfffffff) == (target(op)<<2)) {
                printf("Killing delay loop\n");
                reg.gpr0[9] = reg.VInextInt - 1;
        }
}

void DYNCPU_J(void) {
 DELAYSLOT_VARS
 
// INTERPRET(delay_kill);
 DELAYSLOT
 count++; // 1 cycle for delay slot

 MOV_Imm32ToMem(&reg.pc,(reg.pc&(0xF<<28))|((target(dynop)<<2)&0xFFFFFFF));
 jumped=2;
}

void DYNCPU_COP0_ERET(void) {

 INTERPRET(eCPU_COP0_ERET);
 jumped=2;
}*/

