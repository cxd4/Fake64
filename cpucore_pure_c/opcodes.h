#define cpu_step 4

#define bshift 26

#define CPU_SPECIAL_SLL (uint32)(0)
#define CPU_SPECIAL_
#define CPU_SPECIAL_SRL (uint32)(2)
#define CPU_SPECIAL_SRA (uint32)(3)
#define CPU_SPECIAL_SLLV (uint32)(4)
#define CPU_SPECIAL_SRLV (uint32)(6)
#define CPU_SPECIAL_JR (uint32)(8)
#define CPU_SPECIAL_JALR (uint32)(9)
#define CPU_SPECIAL_BREAK (uint32)(13)
#define CPU_SPECIAL_MFHI (uint32)(16)
#define CPU_SPECIAL_MTHI (uint32)(17)
#define CPU_SPECIAL_MFLO (uint32)(18)
#define CPU_SPECIAL_MTLO (uint32)(19)
#define CPU_SPECIAL_DSLLV (uint32)(20)
#define CPU_SPECIAL_DSRLV (uint32)(22)
#define CPU_SPECIAL_DSRAV (uint32)(23)
#define CPU_SPECIAL_MULT (uint32)(24)
#define CPU_SPECIAL_MULTU (uint32)(25)
#define CPU_SPECIAL_DIV (uint32)(26)
#define CPU_SPECIAL_DIVU (uint32)(27)
#define CPU_SPECIAL_DMULTU (uint32)(29)
#define CPU_SPECIAL_DDIV (uint32)(30)
#define CPU_SPECIAL_DDIVU (uint32)(31)
#define CPU_SPECIAL_ADD (uint32)(32)
#define CPU_SPECIAL_ADDU (uint32)(33)
#define CPU_SPECIAL_SUB (uint32)(34)
#define CPU_SPECIAL_SUBU (uint32)(35)
#define CPU_SPECIAL_AND (uint32)(36)
#define CPU_SPECIAL_OR (uint32)(37)
#define CPU_SPECIAL_XOR (uint32)(38)
#define CPU_SPECIAL_NOR (uint32)(39)
#define CPU_SPECIAL_SLT (uint32)(42)
#define CPU_SPECIAL_SLTU (uint32)(43)
#define CPU_SPECIAL_DADDU (uint32)(45)
#define CPU_SPECIAL_DSLL32 (uint32)(60)
#define CPU_SPECIAL_DSRA32 (uint32)(63)

#define CPU_REGIMM_BLTZ (uint32)(0)
#define CPU_REGIMM_BGEZ (uint32)(1) 
#define CPU_REGIMM_BLTZL (uint32)(2)
#define CPU_REGIMM_BGEZL (uint32)(3)
#define CPU_REGIMM_BGEZAL (uint32)(17)

int dCPU_SPECIAL_SLL(uint32 op);
int dCPU_SPECIAL_SRL(uint32 op);
int dCPU_SPECIAL_SRA(uint32 op);
int dCPU_SPECIAL_SLLV(uint32 op);
int dCPU_SPECIAL_SRLV(uint32 op);
int dCPU_SPECIAL_JR(uint32 op);
int dCPU_SPECIAL_JALR(uint32 op);
int dCPU_SPECIAL_BREAK(uint32 op);
int dCPU_SPECIAL_MFHI(uint32 op);
int dCPU_SPECIAL_MTHI(uint32 op);
int dCPU_SPECIAL_MFLO(uint32 op);
int dCPU_SPECIAL_MTLO(uint32 op);
int dCPU_SPECIAL_DSLLV(uint32 op);
int dCPU_SPECIAL_DSRLV(uint32 op);
int dCPU_SPECIAL_DSRAV(uint32 op);
int dCPU_SPECIAL_MULT(uint32 op);
int dCPU_SPECIAL_MULTU(uint32 op);
int dCPU_SPECIAL_DIV(uint32 op);
int dCPU_SPECIAL_DIVU(uint32 op);
int dCPU_SPECIAL_DMULTU(uint32 op);
int dCPU_SPECIAL_DDIV(uint32 op);
int dCPU_SPECIAL_DDIVU(uint32 op);
int dCPU_SPECIAL_ADD(uint32 op);
int dCPU_SPECIAL_ADDU(uint32 op);
int dCPU_SPECIAL_SUB(uint32 op);
int dCPU_SPECIAL_SUBU(uint32 op);
int dCPU_SPECIAL_AND(uint32 op);
int dCPU_SPECIAL_OR(uint32 op);
int dCPU_SPECIAL_XOR(uint32 op);
int dCPU_SPECIAL_NOR(uint32 op);
int dCPU_SPECIAL_SLT(uint32 op);
int dCPU_SPECIAL_SLTU(uint32 op);
int dCPU_SPECIAL_DADDU(uint32 op);
int dCPU_SPECIAL_DSLL32(uint32 op);
int dCPU_SPECIAL_DSRA32(uint32 op);

int dCPU_REGIMM_BLTZ(uint32 op);
int dCPU_REGIMM_BGEZ(uint32 op);
int dCPU_REGIMM_BLTZL(uint32 op);
int dCPU_REGIMM_BGEZL(uint32 op);
int dCPU_REGIMM_BGEZAL(uint32 op);

int dCPU_COP0_MOVE(uint32 op);
int dCPU_COP0_TLBWI(uint32 op);

int dCPU_COP1_S(uint32 op);
int dCPU_COP1_D(uint32 op);
int dCPU_COP1_WL(uint32 op);
int dCPU_COP1_CFC1(uint32 op);
int dCPU_COP1_CTC1(uint32 op);
int dCPU_COP1_C_F(uint32 op);

int dCPU_SPECIAL(uint32 op);
int dCPU_REGIMM(uint32 op);
int dCPU_J(uint32 op);
int dCPU_JAL(uint32 op);
int dCPU_BEQ(uint32 op);
int dCPU_BNE(uint32 op);
int dCPU_BLEZ(uint32 op);
int dCPU_BGTZ(uint32 op);
int dCPU_ADDI(uint32 op);
int dCPU_ADDIU(uint32 op);
int dCPU_SLTI(uint32 op);
int dCPU_SLTIU(uint32 op);
int dCPU_ANDI(uint32 op);
int dCPU_ORI(uint32 op);
int dCPU_XORI(uint32 op);
int dCPU_LUI(uint32 op);
int dCPU_COP0(uint32 op);
int dCPU_COP1(uint32 op);
int dCPU_COP2(uint32 op);
int dCPU_COP1X(uint32 op);
int dCPU_BEQL(uint32 op);
int dCPU_BNEL(uint32 op);
int dCPU_BLEZL(uint32 op);
int dCPU_BGTZL(uint32 op);
int dCPU_DADDI(uint32 op);
int dCPU_DADDIU(uint32 op);
int dCPU_LB(uint32 op);
int dCPU_LH(uint32 op);
int dCPU_LWL(uint32 op);
int dCPU_LW(uint32 op);
int dCPU_LBU(uint32 op);
int dCPU_LHU(uint32 op);
int dCPU_LWR(uint32 op);
int dCPU_SB(uint32 op);
int dCPU_SH(uint32 op);
int dCPU_SWL(uint32 op);
int dCPU_SW(uint32 op);
int dCPU_SWR(uint32 op);
int dCPU_COP0_CACHE(uint32 op);
int dCPU_LWC1(uint32 op);
int dCPU_LWC2(uint32 op);
int dCPU_LDC1(uint32 op);
int dCPU_LD(uint32 op);
int dCPU_SWC1(uint32 op);
int dCPU_SWC2(uint32 op);
int dCPU_SDC1(uint32 op);
int dCPU_SD(uint32 op);
int dCPU_UNIMPLEMENTED(uint32 op);
int dCPU_SPECIAL_UNIMPLEMENTED(uint32 op);
int dCPU_REGIMM_UNIMPLEMENTED(uint32 op);
int dCPU_COP0_UNIMPLEMENTED(uint32 op);
int dCPU_COP1_UNIMPLEMENTED(uint32 op);

int eCPU_SPECIAL_SLL(uint32 op);
int eCPU_SPECIAL_SRL(uint32 op);
int eCPU_SPECIAL_SRA(uint32 op);
int eCPU_SPECIAL_SLLV(uint32 op);
int eCPU_SPECIAL_SRLV(uint32 op);
int eCPU_SPECIAL_JR(uint32 op);
int eCPU_SPECIAL_JALR(uint32 op);
int eCPU_SPECIAL_BREAK(uint32 op);
int eCPU_SPECIAL_MFHI(uint32 op);
int eCPU_SPECIAL_MTHI(uint32 op);
int eCPU_SPECIAL_MFLO(uint32 op);
int eCPU_SPECIAL_MTLO(uint32 op);
int eCPU_SPECIAL_DSLLV(uint32 op);
int eCPU_SPECIAL_DSRLV(uint32 op);
int eCPU_SPECIAL_DSRAV(uint32 op);
int eCPU_SPECIAL_MULT(uint32 op);
int eCPU_SPECIAL_MULTU(uint32 op);
int eCPU_SPECIAL_DIV(uint32 op);
int eCPU_SPECIAL_DIVU(uint32 op);
int eCPU_SPECIAL_DMULTU(uint32 op);
int eCPU_SPECIAL_DDIV(uint32 op);
int eCPU_SPECIAL_DDIVU(uint32 op);
int eCPU_SPECIAL_ADD(uint32 op);
int eCPU_SPECIAL_ADDU(uint32 op);
int eCPU_SPECIAL_SUB(uint32 op);
int eCPU_SPECIAL_SUBU(uint32 op);
int eCPU_SPECIAL_AND(uint32 op);
int eCPU_SPECIAL_OR(uint32 op);
int eCPU_SPECIAL_XOR(uint32 op);
int eCPU_SPECIAL_NOR(uint32 op);
int eCPU_SPECIAL_SLT(uint32 op);
int eCPU_SPECIAL_SLTU(uint32 op);
int eCPU_SPECIAL_DADDU(uint32 op);
int eCPU_SPECIAL_DSLL32(uint32 op);
int eCPU_SPECIAL_DSRA32(uint32 op);

int eCPU_REGIMM_BLTZ(uint32 op);
int eCPU_REGIMM_BGEZ(uint32 op);
int eCPU_REGIMM_BLTZL(uint32 op);
int eCPU_REGIMM_BGEZL(uint32 op);
int eCPU_REGIMM_BGEZAL(uint32 op);

int eCPU_COP0_MOVE(uint32 op);
int eCPU_COP0_TLBWI(uint32 op);

int eCPU_COP1_S(uint32 op);
int eCPU_COP1_D(uint32 op);
int eCPU_COP1_WL(uint32 op);
int eCPU_COP1_CFC1(uint32 op);
int eCPU_COP1_CTC1(uint32 op);
int eCPU_COP1_C_F(uint32 op);

int eCPU_SPECIAL(uint32 op);
int eCPU_REGIMM(uint32 op);
int eCPU_J(uint32 op);
int eCPU_JAL(uint32 op);
int eCPU_BEQ(uint32 op);
int eCPU_BNE(uint32 op);
int eCPU_BLEZ(uint32 op);
int eCPU_BGTZ(uint32 op);
int eCPU_ADDI(uint32 op);
int eCPU_ADDIU(uint32 op);
int eCPU_SLTI(uint32 op);
int eCPU_SLTIU(uint32 op);
int eCPU_ANDI(uint32 op);
int eCPU_ORI(uint32 op);
int eCPU_XORI(uint32 op);
int eCPU_LUI(uint32 op);
int eCPU_COP0(uint32 op);
int eCPU_COP1(uint32 op);
int eCPU_COP2(uint32 op);
int eCPU_COP1X(uint32 op);
int eCPU_BEQL(uint32 op);
int eCPU_BNEL(uint32 op);
int eCPU_BLEZL(uint32 op);
int eCPU_BGTZL(uint32 op);
int eCPU_DADDI(uint32 op);
int eCPU_DADDIU(uint32 op);
int eCPU_LB(uint32 op);
int eCPU_LH(uint32 op);
int eCPU_LWL(uint32 op);
int eCPU_LW(uint32 op);
int eCPU_LBU(uint32 op);
int eCPU_LHU(uint32 op);
int eCPU_LWR(uint32 op);
int eCPU_SB(uint32 op);
int eCPU_SH(uint32 op);
int eCPU_SWL(uint32 op);
int eCPU_SW(uint32 op);
int eCPU_SWR(uint32 op);
int eCPU_COP0_CACHE(uint32 op);
int eCPU_LWC1(uint32 op);
int eCPU_LWC2(uint32 op);
int eCPU_LDC1(uint32 op);
int eCPU_LD(uint32 op);
int eCPU_SWC1(uint32 op);
int eCPU_SWC2(uint32 op);
int eCPU_SDC1(uint32 op);
int eCPU_SD(uint32 op);
int eCPU_UNIMPLEMENTED(uint32 op);
int eCPU_SPECIAL_UNIMPLEMENTED(uint32 op);
int eCPU_REGIMM_UNIMPLEMENTED(uint32 op);
int eCPU_COP0_UNIMPLEMENTED(uint32 op);
int eCPU_COP1_UNIMPLEMENTED(uint32 op);

#define CPU_SPECIAL (uint32)(0 << bshift)
#define CPU_REGIMM (uint32)(1 << bshift)
#define CPU_J (uint32)(2 << bshift)
#define CPU_JAL (uint32)(3 << bshift)
#define CPU_BEQ (uint32)(4 << bshift)
#define CPU_BNE (uint32)(5 << bshift)
#define CPU_BLEZ (uint32)(6 << bshift)
#define CPU_BGTZ (uint32)(7 << bshift)
#define CPU_ADDI (uint32)(8 << bshift)
#define CPU_ADDIU (uint32)(9 << bshift)
#define CPU_SLTI (uint32)(10 << bshift)
#define CPU_SLTIU (uint32)(11 << bshift)
#define CPU_ANDI (uint32)(12 << bshift)
#define CPU_ORI (uint32)(13 << bshift)
#define CPU_XORI (uint32)(14 << bshift)
#define CPU_LUI (uint32)(15 << bshift)
#define CPU_COP0 (uint32)(16 << bshift)
#define CPU_COP1 (uint32)(17 << bshift)
#define CPU_COP2 (uint32)(18 << bshift)
#define CPU_COP1X (uint32)(19 << bshift)
#define CPU_BEQL (uint32)(20 << bshift)
#define CPU_BNEL (uint32)(21 << bshift)
#define CPU_BLEZL (uint32)(22 << bshift)
#define CPU_BGTZL (uint32)(23 << bshift)
#define CPU_DADDI (uint32)(24 << bshift)
#define CPU_DADDIU (uint32)(25 << bshift)
#define CPU_LB (uint32)(32 << bshift)
#define CPU_LH (uint32)(33 << bshift)
#define CPU_LWL (uint32)(34 << bshift)
#define CPU_LW (uint32)(35 << bshift)
#define CPU_LBU (uint32)(36 << bshift)
#define CPU_LHU (uint32)(37 << bshift)
#define CPU_LWR (uint32)(38 << bshift)
#define CPU_SB (uint32)(40 << bshift)
#define CPU_SH (uint32)(41 << bshift)
#define CPU_SWL (uint32)(42 << bshift)
#define CPU_SW (uint32)(43 << bshift)
#define CPU_SWR (uint32)(46 << bshift)
#define CPU_COP0_CACHE (uint32)(47 << bshift)   //im a cop0 instr
//#define CPU_COP0_MOVE //hmm? 
#define CPU_LWC1 (uint32)(49 << bshift)
#define CPU_LWC2 (uint32)(50 << bshift)
#define CPU_LDC1 (uint32)(53 << bshift)
#define CPU_LD (uint32)(55 << bshift)
#define CPU_SWC1 (uint32)(57 << bshift)
#define CPU_SWC2 (uint32)(58 << bshift)
#define CPU_SDC1 (uint32)(61 << bshift)
#define CPU_SD (uint32)(63 << bshift)
