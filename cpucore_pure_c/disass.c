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
extern char* cop1_names[];
extern uint32 op;

FILE *disfd;  // where disassemble messages go

/** print the string representation of a register.
    params:	registertype, register number*/
#define ANSI_BASIC "\033[0;37m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_PURPLE "\033[0;35m"
#define ANSI_YELLOW "\033[1;33m"
#define ANSI_CYAN	"\033[0;36m"
#define ANSI_RED "\033[0;31m"
#define ANSI_DRED "\033[1;31m"
#define ANSI_DGREEN "\033[1;32m"

void reg_info(reg_type rt, int rn) {

	fprintf(disfd, "%s$",ANSI_BASIC);
	switch(rt) {
		case R_NUM:
			fprintf(disfd, "%s0x%.8x%s     ", ANSI_PURPLE,rn,ANSI_BASIC);
			break;
		case R_GPR:
			fprintf(disfd, "%s%s%s[%s0x%.8x%s]%s ", ANSI_RED, reg_names[rn],
				 	ANSI_CYAN, ANSI_GREEN, (uint32)reg.gpr[rn], ANSI_CYAN, 
					ANSI_BASIC);
			break;
		case R_GPR1:
			fprintf(disfd, "%s%s%s[%s0x%.8x%s]%s ", ANSI_DRED, 
					cop1_names[rn], ANSI_CYAN, ANSI_DGREEN, reg.gpr1[rn], 
					ANSI_CYAN, ANSI_BASIC);
			break;
		default:
			break;
	}
}

/** print the text representation of a register with offset.
    params: registertype, registernumber, offset
 */
void reg_offset(reg_type rt, int rn, uint32 offset) {

	switch(rt) {
		case R_GPR:
			fprintf(disfd, "%s$%s0x%.8x%s(%s$%s%s%s)(=%s0x%.8x%s)%s",ANSI_BASIC,
					ANSI_PURPLE, offset, ANSI_CYAN, ANSI_BASIC, ANSI_RED, 
					reg_names[rn], ANSI_CYAN,ANSI_DRED, 
					(uint32)(reg.gpr[rn]+(int16)offset), ANSI_CYAN, ANSI_BASIC);
			break;
		default:
			break;
	}
}

// print a load/store opcode
void lsopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rt(op));
	reg_offset(R_GPR,base(op),immediate(op));
	fputc('\n',disfd);
}

// print ALU Immediate Instructions
void aluiopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rt(op));
	reg_info(R_GPR,rs(op));
	reg_info(R_NUM,immediate(op));
	fputc('\n',disfd);
}

// print 3 operand type instructions
void totiopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rd(op));
	reg_info(R_GPR,rs(op));
	reg_info(R_GPR,rt(op));
	fputc('\n',disfd);
}

// print shift instructions
void shiftopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rd(op));
	reg_info(R_GPR,rt(op));
	if(name[0]!='v')
		reg_info(R_NUM,sa(op));
	else
		reg_info(R_GPR,rs(op));
	fputc('\n',disfd);
}

// print multiply/divide instuction
void mdopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	if((name[2]=='l'&&name[3]=='o') || (
				name[2]=='h'&&name[3]=='i')) {
		if(name[1]=='f')
			reg_info(R_GPR,rd(op));
		else
			reg_info(R_GPR,rs(op));
	} else {
	  reg_info(R_GPR,rs(op));
	  reg_info(R_GPR,rt(op));
	}
	fputc('\n',disfd);
}

void jumpopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	reg_info(R_NUM,target(op)<<2);
	fputc('\n',disfd);
}

void branchopcode(char *name) {
	fputs(name,disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rs(op));
	if(!strncmp(name,"bne",3)||!strncmp(name,"beq",3)) {
		reg_info(R_GPR,rt(op));
	}
	reg_info(R_NUM,offset(op));	
	fputc('\n',disfd);
}

void printjr() {
	fputs("jr",disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rs(op));
	fputc('\n',disfd);
}

void printjalr() {
	fputs("jalr",disfd);
	fputc('\t',disfd);
	reg_info(R_GPR,rs(op));
	reg_info(R_GPR,rd(op));
	fputc('\n',disfd);
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
  fprintf(disfd,"Unimplemented cop1 opcode 0x%x (disasm) %d,%d\n",op,base(op),special(op));
//  return -4;
}

void dCPU_LUI(void) {
	aluiopcode("lui");
//	disop2("lui", R_GPR, rt(op), R_NUM, immediate(op));
// fprintf(disfd,"lui     $%s $0x%x\n", reg_names[rt(op)], immediate(op));
}

void dCPU_ORI(void) {
	aluiopcode("ori");
// disop3("ori", R_GPR,rs(op),R_GPR,rt(op),R_NUM,immediate(op));
// fprintf(disfd,"ori	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

void dCPU_ADDIU(void) {
	aluiopcode("addiu");
	//disop3("addiu", R_GPR, rt(op), R_GPR, rs(op), R_NUM, immediate(op));
// fprintf(disfd,"addiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_BEQ(void) {
	branchopcode("beq");
//	disop3("disfd",R_GPR, rs(op)
// fprintf(disfd,"beq	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)], immediate(op));
}

void dCPU_SPECIAL(void) {
 dcpu_special[special(op)]();
}

void dCPU_SPECIAL_SLL(void) {
//	disop3("sll", R_GPR, rd(op), R_GPR, rt(op), R_NUM, sa(op));
	shiftopcode("sll");
// fprintf(disfd,"sll	$%s $%s $0x%x\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

void dCPU_SPECIAL_SLTU(void) {
	totiopcode("sltu");
// fprintf(disfd,"sltu	$%s $%s $%s\n", reg_names[rs(op)], reg_names[rt(op)], reg_names[rd(op)]);
}

void dCPU_SPECIAL_BREAK(void) {
 fprintf(disfd,"break	$0x%x\n", code(op));
}

void dCPU_SPECIAL_ADDU(void) {
 totiopcode("addu");
 //fprintf(disfd,"addu    $%s $%s $%s\n", reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_JR(void) {
	printjr();
// fprintf(disfd,"jr      $%s\n",reg_names[rs(op)]);
}

void dCPU_SPECIAL_MULT(void) {
	mdopcode("mult");
	// fprintf(disfd,"mult    $%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_MFLO(void) {
 mdopcode("mflo");
 //fprintf(disfd,"mflo    $%s\n",reg_names[rd(op)]);
}

void dCPU_SPECIAL_SUB(void) {
 totiopcode("sub");
 //fprintf(disfd,"sub     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_AND(void) {
	totiopcode("and");
// fprintf(disfd,"and     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_SRA(void) {
	shiftopcode("sra");
 //fprintf(disfd,"sra     $%s $%s $0x%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

void dCPU_SPECIAL_SUBU(void) {
	totiopcode("subu");
// fprintf(disfd,"subu    $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_OR(void) {
	totiopcode("or");
// fprintf(disfd,"or	$%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_SRL(void) {
	shiftopcode("srl");
// fprintf(disfd,"srl	$%s $%s $%x\n",reg_names[rd(op)],reg_names[rt(op)],sa(op));
}

void dCPU_SPECIAL_SLT(void) {
	totiopcode("slt");
// fprintf(disfd,"slt	$%s $%s $%s\n",reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_JALR(void) {
	printjalr();
// fprintf(disfd,"jalr    $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)]);
}

void dCPU_SPECIAL_MFHI(void) {
	mdopcode("mfhi");
// fprintf(disfd,"mfhi	$%s\n",reg_names[rd(op)]);
}

void dCPU_SPECIAL_XOR(void) {
	totiopcode("xor");
// fprintf(disfd,"xor     $%s $%s $%s\n",reg_names[rd(op)],reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_DIV(void) {
	mdopcode("div");
	// fprintf(disfd,"div	$%s $%s\n",reg_names[rs(op)],reg_names[rt(op)]);
}

void dCPU_SPECIAL_MTLO(void) {
	mdopcode("mtlo");
//	fprintf(disfd,"mtlo	$%s\n", reg_names[rs(op)]);
}

void dCPU_SPECIAL_MTHI(void) {
	mdopcode("mthi");
//	fprintf(disfd,"mthi	$%s\n", reg_names[rs(op)]);
}

void dCPU_SPECIAL_DSRLV(void) {
	shiftopcode("dsrlv");
	//fprintf(disfd,"dsrlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DSLL32(void) {
	shiftopcode("dsll32");
	// fprintf(disfd,"dsll32	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

void dCPU_SPECIAL_DSRA32(void) {
	shiftopcode("dsra32");
	// fprintf(disfd,"dsra32 	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], sa(op));
}

void dCPU_SPECIAL_DDIVU(void) {
	mdopcode("ddivu");
// fprintf(disfd,"ddivu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DSLLV(void) {
	shiftopcode("dsllv");
// fprintf(disfd,"dsllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_DDIV(void) {
	mdopcode("ddivu");
// fprintf(disfd,"ddiv	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DMULTU(void) {
	mdopcode("dmultu");
// fprintf(disfd,"dmultu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DADDU(void) {
	totiopcode("daddu");
// fprintf(disfd,"daddu	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DSRAV(void) {
	shiftopcode("dsrav");
// fprintf(disfd,"dsrav	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_SLLV(void) {
	shiftopcode("sllv");
// fprintf(disfd,"sllv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_NOR(void) {
	totiopcode("nor");
// fprintf(disfd,"nor	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_DIVU(void) {
	mdopcode("divu");
 //fprintf(disfd,"divu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_SRLV(void) {
	shiftopcode("srlv");
// fprintf(disfd,"srlv	$%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_SRAV(void) {
	shiftopcode("srav");
// fprintf(disfd,"srav    $%s $%s $%s\n", reg_names[rd(op)], reg_names[rt(op)], reg_names[rs(op)]);
}

void dCPU_SPECIAL_ADD(void) {
	totiopcode("add");
// fprintf(disfd,"add	 $%s $%s $%s\n", reg_names[rd(op)], reg_names[rs(op)], reg_names[rt(op)]);
}

void dCPU_SPECIAL_MULTU(void) {
	mdopcode("multu");
// fprintf(disfd,"multu	$%s $%s\n", reg_names[rs(op)], reg_names[rt(op)]);
}
				
void dCPU_BNE(void) {
	branchopcode("bne"); 
	//disop3("bne", R_GPR, rs(op), R_GPR, rt(op), R_NUM, immediate(op));
// fprintf(disfd,"bne     $%s(%d) $%s(%d) $0x%x\n", reg_names[rs(op)],rs(op), reg_names[rt(op)],rt(op),immediate(op));
}

void dCPU_SW(void) {
	lsopcode("sw");
//	disop3m("sw", R_GPR, rt(op), R_OFF, immediate(op), R_GPR, rs(op));
// fprintf(disfd,"sw      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_JAL(void) {
	jumpopcode("jal");
// fprintf(disfd,"jal     $0x%x\n",target(op)<<2);
}

void dCPU_J(void) {
	jumpopcode("j");
// fprintf(disfd,"j       $0x%x\n",target(op)<<2);
}

void dCPU_LW(void) {
	lsopcode("lw");
//	disop3m("lw", R_GPR, rt(op), R_OFF, immediate(op), R_GPR, rs(op));
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
	aluiopcode("slti");
// fprintf(disfd,"slti    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

void dCPU_BNEL(void) {
	branchopcode("bnel");
// fprintf(disfd,"bnel    $%s $%s $0x%x\n",reg_names[rs(op)],reg_names[rt(op)],immediate(op));
}

void dCPU_SB(void) {
	lsopcode("sb");
// fprintf(disfd,"sb      $%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)],immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SLTIU(void) {
	aluiopcode("sltiu");
// fprintf(disfd,"sltiu   $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

void dCPU_LBU(void) {
	lsopcode("lbu");
// fprintf(disfd,"lbu     $%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}




void dCPU_REGIMM(void) {
 dcpu_regimm[rt(op)]();
}

void dCPU_REGIMM_BGEZ(void) {
	branchopcode("bgez");
// fprintf(disfd,"bgez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BLTZ(void) {
	branchopcode("bltz");
// fprintf(disfd,"bltz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BLTZL(void) {
	branchopcode("bltzl");
	// fprintf(disfd,"bltzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BGEZL(void) {
	branchopcode("bgezl");
// fprintf(disfd,"bgezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_REGIMM_BGEZAL(void) {
	branchopcode("bgezal");
// fprintf(disfd,"bgezal	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}			
	
void dCPU_ANDI(void) {
	aluiopcode("andi");
// fprintf(disfd,"andi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_SH(void) {
	lsopcode("sh");
// fprintf(disfd,"sh	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op),reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LHU(void) {
	lsopcode("lhu");
// fprintf(disfd,"lhu	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_XORI(void) {
	aluiopcode("xori");
// fprintf(disfd,"xori    $%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rt(op)],immediate(op));
}

void dCPU_SWL(void) {
	lsopcode("swl");
// fprintf(disfd,"swl	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_BLEZ(void) {
	branchopcode("blez");
// fprintf(disfd,"blez    $%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_COP0(void) {
  dcpu_cop0[special(op)]();
}

void dCPU_SD(void) {
	lsopcode("sd");
// fprintf(disfd,"sd	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LD(void) {
	lsopcode("ld");
// fprintf(disfd,"ld	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SDC1(void) {
//	disop3m("sdc1", R_GPR1, ft(op), R_OFF, immediate(op), R_GPR, rs(op));
 fprintf(disfd,"sdc1	$f%i $0x%x($%s)\n", ft(op), immediate(op), reg_names[rs(op)]);	
}

void dCPU_ADDI(void) {
	aluiopcode("addi");
// fprintf(disfd,"addi	$%s $%s $0x%x\n",reg_names[rt(op)],reg_names[rs(op)],immediate(op));
}

void dCPU_LDC1(void) {
 fprintf(disfd,"ldc1	$f%i $0x%x($%s)\n", ft(op), immediate(op), reg_names[rs(op)]);
}

void dCPU_DADDIU(void) {
	aluiopcode("daddiu");
// fprintf(disfd,"daddiu	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_LH(void) {
	lsopcode("lh");
// fprintf(disfd,"lh	$%s $0x%x($%s)(=0x%x)\n",reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}	

void dCPU_BGTZ(void) {
	branchopcode("bgtz");
// fprintf(disfd,"bgtz	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_LB(void) {
	lsopcode("lb");
// fprintf(disfd,"lb	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SWC1(void) {
	//disop3m("swc1", R_GPR1, ft(op), R_OFF, immediate(op), R_GPR, rs(op));
 fprintf(disfd,"swc1	$f%i $0x%x($%s)\n", ft(op), immediate(op), reg_names[rs(op)]);
}

void dCPU_BEQL(void) {
	branchopcode("beql");
// fprintf(disfd,"beql	$%s $%s $0x%x\n", reg_names[rs(op)], reg_names[rt(op)],  immediate(op));
}

void dCPU_LWL(void) {
	lsopcode("lwl");
// fprintf(disfd,"lwl	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_LWR(void) {
	lsopcode("lwr");
// fprintf(disfd,"lwr	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_SWR(void) {
	lsopcode("swr");
// fprintf(disfd,"swr	$%s $0x%x($%s)(=0x%x)\n", reg_names[rt(op)], immediate(op), reg_names[rs(op)],(int16)immediate(op)+reg.gpr[rs(op)]);
}

void dCPU_BLEZL(void) {
	branchopcode("blezl");
// fprintf(disfd,"blezl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
}

void dCPU_BGTZL(void) {
	branchopcode("bgtzl");
// fprintf(disfd,"bgtzl	$%s $0x%x\n",reg_names[rs(op)],immediate(op));
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
	aluiopcode("daddi");
// fprintf(disfd,"daddi	$%s $%s $0x%x\n", reg_names[rt(op)], reg_names[rs(op)], immediate(op));
}

void dCPU_COP0_MOVE(void) {
 if(base(op))
   // MTC0
	//disop2("mtc0", R_GPR, rt(op), R_NUM, rd(op));
    fprintf(disfd,"mtc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
 else //MFC0
//	disop2("mfc0", R_GPR, rt(op), R_NUM, rd(op));
   fprintf(disfd,"mfc0 $%s $0x%x\n",reg_names[rt(op)],rd(op));
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
