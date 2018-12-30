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

void reg_info(reg_type rt, int rn) {

	switch(rt) {
		case R_NUM:
			fprintf(disfd, "$%#x ", rn);
			break;
		case R_GPR:
			fprintf(disfd, "$%s[%#.8x] ", reg_names[rn], reg.gpr[rn]);
			break;
		default:
			break;
	}
}

void reg_offset(reg_type rt, int rn, uint32 offset) {

	switch(rt) {
		case R_GPR:
			fprintf(disfd, "$%#x($%s)(=%#.8x)", offset, reg_names[rn], reg.gpr[rn]+offset);
			break;
		default:
			break;
	}
}

void disop3(char* name, reg_type r1, int n1, reg_type r2, int n2, reg_type r3, int n3) {

	fputs(name, disfd);
	fputc('\t', disfd);
	reg_info(r1, n1);
	reg_info(r2, n2);
	reg_info(r3, n3);
	fputc('\n', disfd);
}

void disop2(char* name, reg_type r1, int n1, reg_type r2, int n2) {

	fputs(name, disfd);
	fputc('\t', disfd);
	reg_info(r1, n1);
	reg_info(r2, n2);
	fputc('\n', disfd);
}

void disop3m(char* name, reg_type r1, int n1, reg_type r2, int n2, reg_type r3, int n3) {
	fputs(name, disfd);
	fputc('\t', disfd);
	reg_info(r1, n1);
	reg_offset(r3, n3, n2);
	fputc('\n', disfd);
}

void dCPU_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented opcode (disasm) %d\n",opcode(op));
//  return -1;
}

void dCPU_SPECIAL_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented special opcode (disasm) %d\n",special(op));
//  return -2;
}

void dCPU_REGIMM_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented regimm opcode (disasm) %d\n",rt(op));
//  return -3;
}

void dCPU_COP0_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented cop0 opcode (disasm) %d\n",special(op));
//  return -4;
}

void dCPU_COP1_UNIMPLEMENTED(void){
  fprintf(disfd,"Unimplemented cop1 opcode 0x%lx (disasm) %d,%d\n",op,base(op),special(op));
//  return -4;
}

void dCPU_LUI(void) {
	disop2("lui", R_GPR, rt(op), R_NUM, immediate(op));
// fprintf(disfd,"lui     $%s $0x%x\n", reg_names[rt(op)], immediate(op));
}

void dCPU_ORI(void) {
 fprintf(disfd,"ori	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

void dCPU_ADDIU(void) {
	disop3("addiu", R_GPR, rt(op), R_GPR, rs(op), R_NUM, immediate(op));
// fprintf(disfd,"addiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_BEQ(void) {
 fprintf(disfd,"beq	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

void dCPU_SPECIAL(void) {
 dcpu_special[special(op)]();
}

void dCPU_SPECIAL_SLL(void) {
	disop3("sll", R_GPR, rd(op), R_GPR, rt(op), R_NUM, sa(op));
// fprintf(disfd,"sll	$%s $%s $0x%x\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

void dCPU_SPECIAL_SLTU(void) {
 fprintf(disfd,"sltu	$%s $%s $%s\n", reg_names[rs(op)], reg_names[rt(op)], reg_names[rd(op)]);
}

void dCPU_SPECIAL_BREAK(void) {
 fprintf(disfd,"break	$0x%x\n", code(op));
}

void dCPU_SPECIAL_ADDU(void) {
 fprintf(disfd,"addu    $%s $%s $%s\n", reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_JR(void) {
 fprintf(disfd,"jr      $%s\n",reg_names[rs(op)]);
}

void dCPU_SPECIAL_MULT(void) {
 fprintf(disfd,"mult    $%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_MFLO(void) {
 fprintf(disfd,"mflo    $%s\n",reg_names[rd(op)]);
}

void dCPU_SPECIAL_SUB(void) {
 fprintf(disfd,"sub     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_AND(void) {
 fprintf(disfd,"and     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_SRA(void) {
 fprintf(disfd,"sra     $%s $%s $0x%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

void dCPU_SPECIAL_SUBU(void) {
 fprintf(disfd,"subu    $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_OR(void) {
 fprintf(disfd,"or	$%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_SRL(void) {
 fprintf(disfd,"srl	$%s $%s $%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

void dCPU_SPECIAL_SLT(void) {
 fprintf(disfd,"slt	$%s $%s $%s\n",reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_JALR(void) {
 fprintf(disfd,"jalr    $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)]);
}

void dCPU_SPECIAL_MFHI(void) {
 fprintf(disfd,"mfhi	$%s\n",reg_names[rd(op)]);
}

void dCPU_SPECIAL_XOR(void) {
 fprintf(disfd,"xor     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_DIV(void) {
 fprintf(disfd,"div	$%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_MTLO(void) {
 fprintf(disfd,"mtlo	$%s\n", reg_names[rs(op)]);
}

void dCPU_SPECIAL_MTHI(void) {
 fprintf(disfd,"mthi	$%s\n", reg_names[rs(op)]);
}

void dCPU_SPECIAL_DSRLV(void) {
 fprintf(disfd,"dsrlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DSLL32(void) {
 fprintf(disfd,"dsll32	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

void dCPU_SPECIAL_DSRA32(void) {
 fprintf(disfd,"dsra32 	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

void dCPU_SPECIAL_DDIVU(void) {
 fprintf(disfd,"ddivu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DSLLV(void) {
 fprintf(disfd,"dsllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_DDIV(void) {
 fprintf(disfd,"ddiv	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DMULTU(void) {
 fprintf(disfd,"dmultu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DADDU(void) {
 fprintf(disfd,"daddu	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DSRAV(void) {
 fprintf(disfd,"dsrav	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_SLLV(void) {
 fprintf(disfd,"sllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_NOR(void) {
 fprintf(disfd,"nor	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DIVU(void) {
 fprintf(disfd,"divu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_SRLV(void) {
 fprintf(disfd,"srlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_SRAV(void) {
 fprintf(disfd,"srav    $%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_ADD(void) {
 fprintf(disfd,"add	 $%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_MULTU(void) {
 fprintf(disfd,"multu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}
				
void dCPU_BNE(void) {
	disop3("bne", R_GPR, rs(op), R_GPR, rt(op), R_NUM, immediate(op));
// fprintf(disfd,"bne     $%s(%d) $%s(%d) $0x%x\n", reg_names[rs(op)],rs(op), reg_names[rt(op)],rt(op),immediate(op));
}

void dCPU_SW(void) {
	disop3m("sw", R_GPR, rt(op), R_OFF, immediate(op), R_GPR, rs(op));
// fprintf(disfd,"sw      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_JAL(void) {
 fprintf(disfd,"jal     $0x%x\n",target(op)<<2);
}

void dCPU_J(void) {
 fprintf(disfd,"j       $0x%x\n",target(op)<<2);
}

void dCPU_LW(void) {
	disop3m("lw", R_GPR, rt(op), R_OFF, immediate(op), R_GPR, rs(op));
// fprintf(disfd,"lw	$%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LWC1(void) {
 // eek co-processor stuff
 fprintf(disfd,"lwc1    $f%i $0x%x($%s)\n",ft(op),immediate(op),reg_names[rs(op)]);
}

void dCPU_COP1(void) {
  dcpu_cop1[base(op)]();
}

void dCPU_COP1_S(void) {
  dcpu_cop1_s[special(op)]();
}

void dCPU_COP1_D(void) {
  dcpu_cop1_d[special(op)]();
}

void dCPU_COP1_W(void) {
  dcpu_cop1_w[special(op)]();
}

void dCPU_COP1_L(void) {
  dcpu_cop1_l[special(op)]();
}

void dCPU_SLTI(void) {
 fprintf(disfd,"slti    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

void dCPU_BNEL(void) {
 fprintf(disfd,"bnel    $%s $%s $0x%x\n",reg_names[rs(op)],reg_names[rt(op)],immediate(op));
}

void dCPU_SB(void) {
 fprintf(disfd,"sb      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SLTIU(void) {
 fprintf(disfd,"sltiu   $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

void dCPU_LBU(void) {
 fprintf(disfd,"lbu     $%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}




void dCPU_REGIMM(void) {
 dcpu_regimm[rt(op)]();
}

void dCPU_REGIMM_BGEZ(void) {
 fprintf(disfd,"bgez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BLTZ(void) {
 fprintf(disfd,"bltz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BLTZL(void) {
 fprintf(disfd,"bltzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BGEZL(void) {
 fprintf(disfd,"bgezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BGEZAL(void) {
 fprintf(disfd,"bgezal	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}			
	
void dCPU_ANDI(void) {
 fprintf(disfd,"andi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_SH(void) {
 fprintf(disfd,"sh	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LHU(void) {
 fprintf(disfd,"lhu	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_XORI(void) {
 fprintf(disfd,"xori    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rt(op)],immediate(op));
}

void dCPU_SWL(void) {
 fprintf(disfd,"swl	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_BLEZ(void) {
 fprintf(disfd,"blez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_COP0(void) {
  dcpu_cop0[special(op)]();
}

void dCPU_SD(void) {
 fprintf(disfd,"sd	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LD(void) {
 fprintf(disfd,"ld	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SDC1(void) {
 fprintf(disfd,"sdc1	$f%i $0x%x($%s)\n", ft(op), immediate(op), reg_names[rs(op)]);	
}

void dCPU_ADDI(void) {
 fprintf(disfd,"addi	$%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

void dCPU_LDC1(void) {
 fprintf(disfd,"ldc1	$f%i $0x%x($%s)\n", ft(op), immediate(op), reg_names[rs(op)]);
}

void dCPU_DADDIU(void) {
 fprintf(disfd,"daddiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_LH(void) {
 fprintf(disfd,"lh	$%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}	

void dCPU_BGTZ(void) {
 fprintf(disfd,"bgtz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_LB(void) {
 fprintf(disfd,"lb	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SWC1(void) {
 fprintf(disfd,"swc1	$f%i $0x%x($%s)\n", ft(op), immediate(op), reg_names[rs(op)]);
}

void dCPU_BEQL(void) {
 fprintf(disfd,"beql	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)],  immediate(op));
}

void dCPU_LWL(void) {
 fprintf(disfd,"lwl	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LWR(void) {
 fprintf(disfd,"lwr	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SWR(void) {
 fprintf(disfd,"swr	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_BLEZL(void) {
 fprintf(disfd,"blezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_BGTZL(void) {
 fprintf(disfd,"bgtzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_COP0_CACHE(void) {
 // eek more!
 fprintf(disfd,"cop0_cache	 $%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

void dCPU_COP1X(void) {
 fprintf(disfd,"unimplemented cop1x instruction\n");
}		

void dCPU_SWC2(void) {
 fprintf(disfd,"swc2 	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}
	
void dCPU_LWC2(void) {
 fprintf(disfd,"lwc2	$%s $0x%x($%s)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)]);
}

void dCPU_COP2(void) {
 // eek coproc stuff
 fprintf(disfd,"cop2	$0x%x\n",target(op));
}

void dCPU_DADDI(void) {
 fprintf(disfd,"daddi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_COP0_MOVE(void) {
 if(base(op))
   // MTC0
	disop2("mtc0", R_GPR, rt(op), R_NUM, rd(op));
//    fprintf(disfd,"mtc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
 else //MFC0
	disop2("mfc0", R_GPR, rt(op), R_NUM, rd(op));
//   fprintf(disfd,"mfc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
}

void dCPU_COP0_TLBWI(void) {
 fprintf(disfd,"tlbwi\n");
}

void dCPU_COP0_TLBP(void) {
 fprintf(disfd,"tlbp\n");
}

void dCPU_COP0_TLBR(void) {
 fprintf(disfd,"tlbr\n");
}

void dCPU_COP1_CFC1(void) {
  fprintf(disfd,"cfc1 $%s $%s\n",reg_names[rt(op)],reg_names[rd(op)]);
}

void dCPU_COP1_CTC1(void) {
  fprintf(disfd,"ctc1 $%s $%d\n",reg_names[rt(op)],fs(op));
}

void dCPU_COP0_ERET(void) {
  fprintf(disfd,"eret\n");
}

// 9/11
void dCPU_COP1_MTC1(void)
{
  fprintf(disfd,"mtc1 $f%d $%s\n",fs(op),reg_names[rt(op)]);
}

// 9/11
void dCPU_COP1_MFC1(void)
{
  fprintf(disfd,"mfc1 $%s $f%d\n",reg_names[rt(op)],fs(op));
}
// 9/11
void dCPU_COP1_CVTSW(void)
{
  fprintf(disfd,"cvts.w $f%d $f%d\n",fd(op),fs(op));
}

void dCPU_COP1_CVTDS(void)
{
  fprintf(disfd,"cvtd.s $f%d $f%d\n",fd(op),fs(op));
}


void dCPU_COP1_CVTDW(void)
{
  fprintf(disfd,"cvtd.w $f%d $f%d\n",fd(op),fs(op));
}

void dCPU_COP1_SUBD(void)
{
  fprintf(disfd,"sub.d $f%d $f%d $f%d\n",fd(op),fs(op),ft(op));
}


void dCPU_COP1_MULS(void)
{
  fprintf(disfd,"mul.s $f%d $f%d $f%d\n",fd(op),fs(op),ft(op));
}

void dCPU_COP1_MULD(void)
{
  fprintf(disfd,"mul.d $f%d $f%d $f%d\n",fd(op),fs(op),ft(op));
}

void dCPU_COP1_TRUNCWD(void)
{
  fprintf(disfd,"truncw.d $%s $%d\n",reg_names[fs(op)],fd(op));
}

void dCPU_COP1_DIVS(void)
{
  fprintf(disfd,"div.s $f%d $f%d $f%d\n",fd(op),fs(op),fd(op));
}

void dCPU_COP1_DIVD(void)
{
  fprintf(disfd,"div.d $f%d $f%d $f%d\n",fd(op),fs(op),fd(op));
}

void dCPU_COP1_ADDS(void)
{
  fprintf(disfd,"add.s $f%i $f%i $f%i\n",fs(op),ft(op),fd(op));
}

void dCPU_COP1_ADDD(void)
{
  fprintf(disfd,"add.d $f%d $f%d $f%d\n",fd(op),fs(op),ft(op));
}

void dCPU_COP1_SUBS(void)
{
  fprintf(disfd,"sub.s $%d $%d $%d\n",fs(op), ft(op), fd(op));
}

void dCPU_COP1_MOVS(void)
{
  fprintf(disfd,"mov.s $f%d $f%d\n",fs(op), fd(op));
}
void dCPU_COP1_MOVD(void)
{
  fprintf(disfd,"mov.d $f%d $f%d\n",fs(op), fd(op));
}



char *FPUCC[16]={	"c.f",	"c.un",		"c.eq",		"c.ueq",	"c.olt",	"c.ult",	"c.ole",	"c.ule",
								  "c.sf",	"c.ngle", "c.seq",	"c.ngl",	"c.lt",		"c.nge",	"c.le",		"c.ngt"};			

void dCPU_COP1_CONDITIONAL(void)
{
	// prefix
	if (special(op)<48 || special(op)>64)
	{
		fprintf(disfd,"unknow Conditional\n");
		return;
	}
	fprintf(disfd,"%s",FPUCC[special(op)-48]);

	// type
	switch(base(op))
	{
	case 16:
		fprintf(disfd,".s ");
		break;
	case 17:
		fprintf(disfd,".d ");
		break;
	case 20:
		fprintf(disfd,".w (invalid) ");
		break;
	case 21:
		fprintf(disfd,".l (invalid)");
		break;
	default:
		fprintf(disfd,".?? ");
		break;
	}
	fprintf(disfd,"$f%d $f%d\n",fs(op),ft(op));
}

void dCPU_COP1_BC(void)
{
	switch((op>>16)&0x3) 	// switch on type
	{
		case 0: //BC1F
		fprintf(disfd,"bc1f ");
    	break;
		case 1: //BC1T
		fprintf(disfd,"bc1t ");
    	break;
		case 2:  //BC1FL
		fprintf(disfd,"bc1fl ");
    	break;
		case 3:  //BC1TL
		fprintf(disfd,"bc1tl ");
    	break;
		default:
      printf("debug BC broken");
			exit(0);
	}
  fprintf(disfd,"$0x%x\n",target(op)<<2);
}

void dCPU_COP1_TRUNCWS(void) {
	fprintf(disfd, "trunk.w.s	$f%i $f%i\n", fd(op), fs(op));
}

void dCPU_COP1_CVTWD(void) {
	fprintf(disfd, "cvt.w.d	$f%i $f%i\n", fd(op), fs(op));
}

void dCPU_COP1_CVTSD(void) {
	fprintf(disfd, "cvt.s.d	$f%i $f%i\n", fd(op), fs(op));
}

void dCPU_COP1_NEGS(void) {
	fprintf(disfd, "negs	$f%i $f%i\n", fd(op), fs(op));
}

void dCPU_STOLEN(void) {
	fprintf(disfd, "muhahahaha!	%#x\n", immediate(op));
}
