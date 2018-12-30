#include <general.h>
#include "decode.h"
#include "opcodes.h"
#include "registers.h"

extern struct cpu_reg reg;
extern cpu_instruction dcpu_instr[64];
extern cpu_instruction dcpu_special[64];
extern cpu_instruction dcpu_regimm[32];
extern cpu_instruction dcpu_cop0[8];
extern cpu_instruction dcpu_cop1[32];
extern cpu_instruction dcpu_cop1_s[64];
extern cpu_instruction dcpu_cop1_d[64];
extern cpu_instruction dcpu_cop1_wl[64];

extern char* reg_names[];

int dCPU_UNIMPLEMENTED(uint32 op){
  fprintf(stderr,"Unimplemented opcode (disasm) %d\n",opcode(op));
  return -1;
}

int dCPU_SPECIAL_UNIMPLEMENTED(uint32 op){
  fprintf(stderr,"Unimplemented special opcode (disasm) %d\n",special(op));
  return -2;
}

int dCPU_REGIMM_UNIMPLEMENTED(uint32 op){
  fprintf(stderr,"Unimplemented regimm opcode (disasm) %d\n",reg_names[rt(op)]);
  return -3;
}

int dCPU_COP0_UNIMPLEMENTED(uint32 op){
  fprintf(stderr,"Unimplemented cop0 opcode (disasm) %d\n",special(op));
  return -4;
}

int dCPU_COP1_UNIMPLEMENTED(uint32 op){
  fprintf(stderr,"Unimplemented cop1 opcode (disasm) %d,%d\n",base(op),special(op));
  return -4;
}

int dCPU_LUI(uint32 op) {
 printf("lui     $%s $0x%x\n", reg_names[rt(op)], immediate(op));
}

int dCPU_ORI(uint32 op) {
 printf("ori	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

int dCPU_ADDIU(uint32 op) {
 printf("addiu	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

int dCPU_BEQ(uint32 op) {
 printf("beq	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

int dCPU_SPECIAL(uint32 op) {
 dcpu_special[special(op)](op);
}

int dCPU_SPECIAL_SLL(uint32 op) {
 printf("sll	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], sa(op));
}

int dCPU_SPECIAL_SLTU(uint32 op) {
 printf("sltu	$%s $%s $%s\n", reg_names[rs(op)], reg_names[rt(op)], reg_names[rd(op)]);
}

int dCPU_SPECIAL_BREAK(uint32 op) {
 printf("break	$0x%x\n", code(op));
}

int dCPU_SPECIAL_ADDU(uint32 op) {
 printf("addu    $%s $%s $%s\n", reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_JR(uint32 op) {
 printf("jr      $%s\n",reg_names[rs(op)]);
}

int dCPU_SPECIAL_MULT(uint32 op) {
 printf("mult    $%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_MFLO(uint32 op) {
 printf("mflo    $%s\n",reg_names[rd(op)]);
}

int dCPU_SPECIAL_SUB(uint32 op) {
 printf("sub     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_AND(uint32 op) {
 printf("and     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_SRA(uint32 op) {
 printf("sra     $%s $%s $0x%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

int dCPU_SPECIAL_SUBU(uint32 op) {
 printf("subu    $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_OR(uint32 op) {
 printf("or	$%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_SRL(uint32 op) {
 printf("srl	$%s $%s $%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

int dCPU_SPECIAL_SLT(uint32 op) {
 printf("slt	$%s $%s $%s\n",reg_names[rs(op)], reg_names[rt(op)], reg_names[rd(op)]);
}

int dCPU_SPECIAL_JALR(uint32 op) {
 printf("jalr    $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)]);
}

int dCPU_SPECIAL_MFHI(uint32 op) {
 printf("mfhi	$%s\n",reg_names[rd(op)]);
}

int dCPU_SPECIAL_XOR(uint32 op) {
 printf("xor     $%s $%s_$%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_DIV(uint32 op) {
 printf("div	$%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_MTLO(uint32 op) {
 printf("mtlo	$%s\n", reg_names[rs(op)]);
}

int dCPU_SPECIAL_MTHI(uint32 op) {
 printf("mthi	$%s\n", reg_names[rs(op)]);
}

int dCPU_SPECIAL_DSRLV(uint32 op) {
 printf("dsrlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DSLL32(uint32 op) {
 printf("dsll32	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

int dCPU_SPECIAL_DSRA32(uint32 op) {
 printf("dsra32 	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

int dCPU_SPECIAL_DDIVU(uint32 op) {
 printf("ddivu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DSLLV(uint32 op) {
 printf("dsllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_DDIV(uint32 op) {
 printf("ddiv	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DMULTU(uint32 op) {
 printf("dmultu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DADDU(uint32 op) {
 printf("daddu	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DSRAV(uint32 op) {
 printf("dsrav	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_SLLV(uint32 op) {
 printf("sllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_NOR(uint32 op) {
 printf("nor	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DIVU(uint32 op) {
 printf("divu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_SRLV(uint32 op) {
 printf("srlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_ADD(uint32 op) {
 printf("add	 $%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_MULTU(uint32 op) {
 printf("multu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}
				
int dCPU_BNE(uint32 op) {
 printf("bne     $%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)],immediate(op));
}

int dCPU_SW(uint32 op) {
 printf("sw      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_JAL(uint32 op) {
 printf("jal     $0x%x\n",target(op)<<2);
}

int dCPU_J(uint32 op) {
 printf("j       $0x%x\n",target(op)<<2);
}

int dCPU_LW(uint32 op) {
 printf("lw	$%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_LWC1(uint32 op) {
 // eek co-processor stuff
 printf("lwc1    $%s $0x%x($%s)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)]);
}

int dCPU_COP1(uint32 op) {
  dcpu_cop1[base(op)](op);
}

int dCPU_COP1_S(uint32 op) {
  dcpu_cop1_s[special(op)](op);
}

int dCPU_COP1_D(uint32 op) {
  dcpu_cop1_d[special(op)](op);
}

int dCPU_COP1_WL(uint32 op) {
  dcpu_cop1_wl[special(op)](op);
}

int dCPU_SLTI(uint32 op) {
 printf("slti    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

int dCPU_BNEL(uint32 op) {
 printf("bnel    $%s $%s $0x%x\n",reg_names[rs(op)],reg_names[rt(op)],immediate(op));
}

int dCPU_SB(uint32 op) {
 printf("sb      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_SLTIU(uint32 op) {
 printf("sltiu   $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

int dCPU_LBU(uint32 op) {
 printf("lbu     $%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_REGIMM(uint32 op) {
 dcpu_regimm[rt(op)](op);
}

int dCPU_REGIMM_BGEZ(uint32 op) {
 printf("bgez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BLTZ(uint32 op) {
 printf("bltz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BLTZL(uint32 op) {
 printf("bltzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BGEZL(uint32 op) {
 printf("bgezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BGEZAL(uint32 op) {
 printf("bgezal	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}			
	
int dCPU_ANDI(uint32 op) {
 printf("andi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_SH(uint32 op) {
 printf("sh	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op),reg_names[rs(op)]);
}

int dCPU_LHU(uint32 op) {
 printf("lhu	$0x%x($%s) $%s\n", immediate(op), reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_XORI(uint32 op) {
 printf("xori    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rt(op)],immediate(op));
}

int dCPU_SWL(uint32 op) {
 printf("swl	$%s %0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_BLEZ(uint32 op) {
 printf("blez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_COP0(uint32 op) {
  dcpu_cop0[special(op)](op);
}

int dCPU_SD(uint32 op) {
 printf("sd	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_LD(uint32 op) {
 printf("ld	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_SDC1(uint32 op) {
 printf("sdc1	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);	
}

int dCPU_ADDI(uint32 op) {
 printf("addi	$%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

int dCPU_LDC1(uint32 op) {
 printf("ldc1	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_DADDIU(uint32 op) {
 printf("daddiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_LH(uint32 op) {
 printf("lh	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}	

int dCPU_BGTZ(uint32 op) {
 printf("bgtz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_LB(uint32 op) {
 printf("lb	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_SWC1(uint32 op) {
 printf("swc1	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_BEQL(uint32 op) {
 printf("beql	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)],  immediate(op));
}

int dCPU_LWL(uint32 op) {
 printf("lwl	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_LWR(uint32 op) {
 printf("lwr	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_SWR(uint32 op) {
 printf("swr	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_BLEZL(uint32 op) {
 printf("blezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_BGTZL(uint32 op) {
 printf("bgtzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_COP0_CACHE(uint32 op) {
 // eek more!
 printf("cop0_cache	 $%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_COP1X(uint32 op) {
 printf("unimplemented cop1x instruction\n");
}		

int dCPU_SWC2(uint32 op) {
 printf("swc2 	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}
	
int dCPU_LWC2(uint32 op) {
 printf("lwc2	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_COP2(uint32 op) {
 // eek coproc stuff
 printf("cop2	$0x%x\n",target(op));
}

int dCPU_DADDI(uint32 op) {
 printf("daddi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_COP0_MOVE(uint32 op) {
 if(base(op))
   // MTC0
    printf("mtc0 $%s $0x%x\n",reg_names[rt(op)],rd(op)); 
 else //MFC0
   printf("mfc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
}

int dCPU_COP0_TLBWI(uint32 op) {
 printf("tlbwi\n");
}

int dCPU_COP1_CFC1(uint32 op) {
  printf("cfc1 $%s $%s\n",reg_names[rt(op)],reg_names[rd(op)]);
}

int dCPU_COP1_CTC1(uint32 op) {
  printf("ctc1 $%d $%d\n",reg_names[rt(op)],fs(op));
}

int dCPU_COP1_C_F(uint32 op) {
  printf("C.F $%s $%s $%s\n",cc(op),fs(op),ft(op));
}
