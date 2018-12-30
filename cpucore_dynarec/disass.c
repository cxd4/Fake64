#include <general.h>
#include <decode.h>
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
extern cpu_instruction dcpu_cop1_w[64];
extern cpu_instruction dcpu_cop1_l[64];

extern char* reg_names[];
extern uint32 op;

FILE *disfd;  // where disassemble messages go

int dCPU_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented opcode (disasm) %d\n",opcode(op));
  return -1;
}

int dCPU_SPECIAL_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented special opcode (disasm) %d\n",special(op));
  return -2;
}

int dCPU_REGIMM_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented regimm opcode (disasm) %d\n",rt(op));
  return -3;
}

int dCPU_COP0_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented cop0 opcode (disasm) %d\n",special(op));
  return -4;
}

int dCPU_COP1_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented cop1 opcode 0x%lx (disasm) %d,%d\n",op,base(op),special(op));
  return -4;
}

int dCPU_LUI(void) {
 fprintf(disfd,"lui     $%s $0x%x\n", reg_names[rt(op)], immediate(op));
}

int dCPU_ORI(void) {
 fprintf(disfd,"ori	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

int dCPU_ADDIU(void) {
 fprintf(disfd,"addiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_BEQ(void) {
 fprintf(disfd,"beq	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

int dCPU_SPECIAL(void) {
 dcpu_special[special(op)]();
}

int dCPU_SPECIAL_SLL(void) {
 fprintf(disfd,"sll	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], sa(op));
}

int dCPU_SPECIAL_SLTU(void) {
 fprintf(disfd,"sltu	$%s $%s $%s\n", reg_names[rs(op)], reg_names[rt(op)], reg_names[rd(op)]);
}

int dCPU_SPECIAL_BREAK(void) {
 fprintf(disfd,"break	$0x%x\n", code(op));
}

int dCPU_SPECIAL_ADDU(void) {
 fprintf(disfd,"addu    $%s $%s $%s\n", reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_JR(void) {
 fprintf(disfd,"jr      $%s\n",reg_names[rs(op)]);
}

int dCPU_SPECIAL_MULT(void) {
 fprintf(disfd,"mult    $%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_MFLO(void) {
 fprintf(disfd,"mflo    $%s\n",reg_names[rd(op)]);
}

int dCPU_SPECIAL_SUB(void) {
 fprintf(disfd,"sub     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_AND(void) {
 fprintf(disfd,"and     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_SRA(void) {
 fprintf(disfd,"sra     $%s $%s $0x%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

int dCPU_SPECIAL_SUBU(void) {
 fprintf(disfd,"subu    $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_OR(void) {
 fprintf(disfd,"or	$%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_SRL(void) {
 fprintf(disfd,"srl	$%s $%s $%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

int dCPU_SPECIAL_SLT(void) {
 fprintf(disfd,"slt	$%s $%s $%s\n",reg_names[rs(op)], reg_names[rt(op)], reg_names[rd(op)]);
}

int dCPU_SPECIAL_JALR(void) {
 fprintf(disfd,"jalr    $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)]);
}

int dCPU_SPECIAL_MFHI(void) {
 fprintf(disfd,"mfhi	$%s\n",reg_names[rd(op)]);
}

int dCPU_SPECIAL_XOR(void) {
 fprintf(disfd,"xor     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_DIV(void) {
 fprintf(disfd,"div	$%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

int dCPU_SPECIAL_MTLO(void) {
 fprintf(disfd,"mtlo	$%s\n", reg_names[rs(op)]);
}

int dCPU_SPECIAL_MTHI(void) {
 fprintf(disfd,"mthi	$%s\n", reg_names[rs(op)]);
}

int dCPU_SPECIAL_DSRLV(void) {
 fprintf(disfd,"dsrlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DSLL32(void) {
 fprintf(disfd,"dsll32	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

int dCPU_SPECIAL_DSRA32(void) {
 fprintf(disfd,"dsra32 	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

int dCPU_SPECIAL_DDIVU(void) {
 fprintf(disfd,"ddivu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DSLLV(void) {
 fprintf(disfd,"dsllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_DDIV(void) {
 fprintf(disfd,"ddiv	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DMULTU(void) {
 fprintf(disfd,"dmultu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DADDU(void) {
 fprintf(disfd,"daddu	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DSRAV(void) {
 fprintf(disfd,"dsrav	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_SLLV(void) {
 fprintf(disfd,"sllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_NOR(void) {
 fprintf(disfd,"nor	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_DIVU(void) {
 fprintf(disfd,"divu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_SRLV(void) {
 fprintf(disfd,"srlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

int dCPU_SPECIAL_ADD(void) {
 fprintf(disfd,"add	 $%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_SPECIAL_MULTU(void) {
 fprintf(disfd,"multu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}
				
int dCPU_BNE(void) {
 fprintf(disfd,"bne     $%s(%d) $%s(%d) $0x%x\n", reg_names[rs(op)],rs(op), reg_names[rt(op)],rt(op),immediate(op));
}

int dCPU_SW(void) {
 fprintf(disfd,"sw      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_JAL(void) {
 fprintf(disfd,"jal     $0x%x\n",target(op)<<2);
}

int dCPU_J(void) {
 fprintf(disfd,"j       $0x%x\n",target(op)<<2);
}

int dCPU_LW(void) {
 fprintf(disfd,"lw	$%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_LWC1(void) {
 // eek co-processor stuff
 fprintf(disfd,"lwc1    $%s $0x%x($%s)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)]);
}

int dCPU_COP1(void) {
  dcpu_cop1[base(op)]();
}

int dCPU_COP1_S(void) {
  dcpu_cop1_s[special(op)]();
}

int dCPU_COP1_D(void) {
  dcpu_cop1_d[special(op)]();
}

int dCPU_COP1_W(void) {
  dcpu_cop1_w[special(op)]();
}

int dCPU_COP1_L(void) {
  dcpu_cop1_l[special(op)]();
}

int dCPU_SLTI(void) {
 fprintf(disfd,"slti    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

int dCPU_BNEL(void) {
 fprintf(disfd,"bnel    $%s $%s $0x%x\n",reg_names[rs(op)],reg_names[rt(op)],immediate(op));
}

int dCPU_SB(void) {
 fprintf(disfd,"sb      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_SLTIU(void) {
 fprintf(disfd,"sltiu   $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

int dCPU_LBU(void) {
 fprintf(disfd,"lbu     $%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}




int dCPU_REGIMM(void) {
 dcpu_regimm[rt(op)]();
}

int dCPU_REGIMM_BGEZ(void) {
 fprintf(disfd,"bgez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BLTZ(void) {
 fprintf(disfd,"bltz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BLTZL(void) {
 fprintf(disfd,"bltzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BGEZL(void) {
 fprintf(disfd,"bgezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_REGIMM_BGEZAL(void) {
 fprintf(disfd,"bgezal	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}			
	
int dCPU_ANDI(void) {
 fprintf(disfd,"andi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_SH(void) {
 fprintf(disfd,"sh	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op),reg_names[rs(op)]);
}

int dCPU_LHU(void) {
 fprintf(disfd,"lhu	$0x%x($%s) $%s\n", immediate(op), reg_names[rs(op)], reg_names[rt(op)]);
}

int dCPU_XORI(void) {
 fprintf(disfd,"xori    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rt(op)],immediate(op));
}

int dCPU_SWL(void) {
 fprintf(disfd,"swl	$%s %0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_BLEZ(void) {
 fprintf(disfd,"blez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_COP0(void) {
  dcpu_cop0[special(op)]();
}

int dCPU_SD(void) {
 fprintf(disfd,"sd	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_LD(void) {
 fprintf(disfd,"ld	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

int dCPU_SDC1(void) {
 fprintf(disfd,"sdc1	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);	
}

int dCPU_ADDI(void) {
 fprintf(disfd,"addi	$%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

int dCPU_LDC1(void) {
 fprintf(disfd,"ldc1	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_DADDIU(void) {
 fprintf(disfd,"daddiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_LH(void) {
 fprintf(disfd,"lh	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}	

int dCPU_BGTZ(void) {
 fprintf(disfd,"bgtz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_LB(void) {
 fprintf(disfd,"lb	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_SWC1(void) {
 fprintf(disfd,"swc1	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_BEQL(void) {
 fprintf(disfd,"beql	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)],  immediate(op));
}

int dCPU_LWL(void) {
 fprintf(disfd,"lwl	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_LWR(void) {
 fprintf(disfd,"lwr	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_SWR(void) {
 fprintf(disfd,"swr	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_BLEZL(void) {
 fprintf(disfd,"blezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_BGTZL(void) {
 fprintf(disfd,"bgtzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

int dCPU_COP0_CACHE(void) {
 // eek more!
 fprintf(disfd,"cop0_cache	 $%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_COP1X(void) {
 fprintf(disfd,"unimplemented cop1x instruction\n");
}		

int dCPU_SWC2(void) {
 fprintf(disfd,"swc2 	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}
	
int dCPU_LWC2(void) {
 fprintf(disfd,"lwc2	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

int dCPU_COP2(void) {
 // eek coproc stuff
 fprintf(disfd,"cop2	$0x%x\n",target(op));
}

int dCPU_DADDI(void) {
 fprintf(disfd,"daddi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

int dCPU_COP0_MOVE(void) {
 if(base(op))
   // MTC0
    fprintf(disfd,"mtc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
 else //MFC0
   fprintf(disfd,"mfc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
}

int dCPU_COP0_TLBWI(void) {
 fprintf(disfd,"tlbwi\n");
}

int dCPU_COP0_TLBP(void) {
 fprintf(disfd,"tlbp\n");
}

int dCPU_COP0_TLBR(void) {
 fprintf(disfd,"tlbr\n");
}

int dCPU_COP1_CFC1(void) {
  fprintf(disfd,"cfc1 $%s $%s\n",reg_names[rt(op)],reg_names[rd(op)]);
}

int dCPU_COP1_CTC1(void) {
  fprintf(disfd,"ctc1 $%s $%d\n",reg_names[rt(op)],fs(op));
}

int dCPU_COP0_ERET(void) {
  fprintf(disfd,"eret\n");
}

// 9/11
int dCPU_COP1_MTC1(void)
{
  fprintf(disfd,"mtc1 $f%d $%s\n",rt(op),reg_names[fs(op)]);
}

// 9/11
int dCPU_COP1_MFC1(void)
{
  fprintf(disfd,"mfc1 $%s $f%d\n",reg_names[rt(op)],fs(op));
}
// 9/11
int dCPU_COP1_CVTSW(void) // TODO: add rounding support
{
  fprintf(disfd,"cvts.w $f%d $f%d\n",fd(op),fs(op));
}

int dCPU_COP1_CVTDS(void) // TODO: add rounding support
{
  fprintf(disfd,"cvtd.s $%s $%d\n",reg_names[fd(op)],fs(op));
}
// 9/11
int dCPU_COP1_MULS(void)
{
  fprintf(disfd,"mul.s $f%d $f%d $f%d\n",fd(op),fs(op),ft(op));
}

int dCPU_COP1_TRUNCWD(void)
{
  fprintf(disfd,"truncw.d $%s $%d\n",reg_names[fs(op)],fd(op));
}

// 9/11
int dCPU_COP1_DIVS(void)
{
  fprintf(disfd,"div.s $f%d $f%d $f%d\n",fd(op),fs(op),fd(op));
}

int dCPU_COP1_ADDS(void)
{
  fprintf(disfd,"add.s $%s $%d $%d\n",reg_names[fs(op)],reg_names[ft(op)],reg_names[fd(op)]);
}

int dCPU_COP1_ADDD(void)
{
  fprintf(disfd,"add.d $%s $%d $%d\n",reg_names[fs(op)],reg_names[ft(op)],reg_names[fd(op)]);
}

