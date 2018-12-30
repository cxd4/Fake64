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

int dCPU_SPECIAL_SLL(void);
int dCPU_SPECIAL_SRL(void);
int dCPU_SPECIAL_SRA(void);
int dCPU_SPECIAL_SLLV(void);
int dCPU_SPECIAL_SRLV(void);
int dCPU_SPECIAL_JR(void);
int dCPU_SPECIAL_JALR(void);
int dCPU_SPECIAL_BREAK(void);
int dCPU_SPECIAL_MFHI(void);
int dCPU_SPECIAL_MTHI(void);
int dCPU_SPECIAL_MFLO(void);
int dCPU_SPECIAL_MTLO(void);
int dCPU_SPECIAL_DSLLV(void);
int dCPU_SPECIAL_DSRLV(void);
int dCPU_SPECIAL_DSRAV(void);
int dCPU_SPECIAL_MULT(void);
int dCPU_SPECIAL_MULTU(void);
int dCPU_SPECIAL_DIV(void);
int dCPU_SPECIAL_DIVU(void);
int dCPU_SPECIAL_DMULTU(void);
int dCPU_SPECIAL_DDIV(void);
int dCPU_SPECIAL_DDIVU(void);
int dCPU_SPECIAL_ADD(void);
int dCPU_SPECIAL_ADDU(void);
int dCPU_SPECIAL_SUB(void);
int dCPU_SPECIAL_SUBU(void);
int dCPU_SPECIAL_AND(void);
int dCPU_SPECIAL_OR(void);
int dCPU_SPECIAL_XOR(void);
int dCPU_SPECIAL_NOR(void);
int dCPU_SPECIAL_SLT(void);
int dCPU_SPECIAL_SLTU(void);
int dCPU_SPECIAL_DADDU(void);
int dCPU_SPECIAL_DSLL32(void);
int dCPU_SPECIAL_DSRA32(void);

int dCPU_REGIMM_BLTZ(void);
int dCPU_REGIMM_BGEZ(void);
int dCPU_REGIMM_BLTZL(void);
int dCPU_REGIMM_BGEZL(void);
int dCPU_REGIMM_BGEZAL(void);

int dCPU_COP0_MOVE(void);
int dCPU_COP0_TLBWI(void);
int dCPU_COP0_TLBP(void);
int dCPU_COP0_TLBR(void);
int dCPU_COP0_ERET(void);

int dCPU_COP1_S(void);
int dCPU_COP1_D(void);
int dCPU_COP1_W(void);
int dCPU_COP1_L(void);
int dCPU_COP1_CFC1(void);
int dCPU_COP1_CTC1(void);

int dCPU_SPECIAL(void);
int dCPU_REGIMM(void);
int dCPU_J(void);
int dCPU_JAL(void);
int dCPU_BEQ(void);
int dCPU_BNE(void);
int dCPU_BLEZ(void);
int dCPU_BGTZ(void);
int dCPU_ADDI(void);
int dCPU_ADDIU(void);
int dCPU_SLTI(void);
int dCPU_SLTIU(void);
int dCPU_ANDI(void);
int dCPU_ORI(void);
int dCPU_XORI(void);
int dCPU_LUI(void);
int dCPU_COP0(void);
int dCPU_COP1(void);
int dCPU_COP2(void);
int dCPU_COP1X(void);
int dCPU_BEQL(void);
int dCPU_BNEL(void);
int dCPU_BLEZL(void);
int dCPU_BGTZL(void);
int dCPU_DADDI(void);
int dCPU_DADDIU(void);
int dCPU_LB(void);
int dCPU_LH(void);
int dCPU_LWL(void);
int dCPU_LW(void);
int dCPU_LBU(void);
int dCPU_LHU(void);
int dCPU_LWR(void);
int dCPU_SB(void);
int dCPU_SH(void);
int dCPU_SWL(void);
int dCPU_SW(void);
int dCPU_SWR(void);
int dCPU_COP0_CACHE(void);
int dCPU_LWC1(void);
int dCPU_LWC2(void);
int dCPU_LDC1(void);
int dCPU_LD(void);
int dCPU_SWC1(void);
int dCPU_SWC2(void);
int dCPU_SDC1(void);
int dCPU_SD(void);
int dCPU_UNIMPLEMENTED(void);
int dCPU_SPECIAL_UNIMPLEMENTED(void);
int dCPU_REGIMM_UNIMPLEMENTED(void);
int dCPU_COP0_UNIMPLEMENTED(void);
int dCPU_COP1_UNIMPLEMENTED(void);
int dCPU_COP1_CTC1(void);
int dCPU_COP1_MTC1(void);
int dCPU_COP1_DIVS(void);
int dCPU_COP1_MULS(void);
int dCPU_COP1_CVTSW(void);
int dCPU_COP1_ADDS(void);
int dCPU_COP1_CVTDS(void);
int dCPU_COP1_TRUNCWD(void);
int dCPU_COP1_ADDD(void);
int dCPU_COP1_MFC1(void);
int dCPU_COP1_DIVS(void);
int dCPU_COP1_ADDS(void);
int dCPU_COP1_ADDD(void);
int eCPU_COP1_SUBS(void);
int eCPU_SPECIAL_SLL(void);
int eCPU_SPECIAL_SRL(void);
int eCPU_SPECIAL_SRA(void);
int eCPU_SPECIAL_SLLV(void);
int eCPU_SPECIAL_SRLV(void);
int eCPU_SPECIAL_JR(void);
int eCPU_SPECIAL_JALR(void);
int eCPU_SPECIAL_BREAK(void);
int eCPU_SPECIAL_MFHI(void);
int eCPU_SPECIAL_MTHI(void);
int eCPU_SPECIAL_MFLO(void);
int eCPU_SPECIAL_MTLO(void);
int eCPU_SPECIAL_DSLLV(void);
int eCPU_SPECIAL_DSRLV(void);
int eCPU_SPECIAL_DSRAV(void);
int eCPU_SPECIAL_MULT(void);
int eCPU_SPECIAL_MULTU(void);
int eCPU_SPECIAL_DIV(void);
int eCPU_SPECIAL_DIVU(void);
int eCPU_SPECIAL_DMULTU(void);
int eCPU_SPECIAL_DDIV(void);
int eCPU_SPECIAL_DDIVU(void);
int eCPU_SPECIAL_ADD(void);
int eCPU_SPECIAL_ADDU(void);
int eCPU_SPECIAL_SUB(void);
int eCPU_SPECIAL_SUBU(void);
int eCPU_SPECIAL_AND(void);
int eCPU_SPECIAL_OR(void);
int eCPU_SPECIAL_XOR(void);
int eCPU_SPECIAL_NOR(void);
int eCPU_SPECIAL_SLT(void);
int eCPU_SPECIAL_SLTU(void);
int eCPU_SPECIAL_DADDU(void);
int eCPU_SPECIAL_DSLL32(void);
int eCPU_SPECIAL_DSRA32(void);

int eCPU_REGIMM_BLTZ(void);
int eCPU_REGIMM_BGEZ(void);
int eCPU_REGIMM_BLTZL(void);
int eCPU_REGIMM_BGEZL(void);
int eCPU_REGIMM_BGEZAL(void);

int eCPU_COP0_MOVE(void);
int eCPU_COP0_TLBWI(void);
int eCPU_COP0_TLBP(void);
int eCPU_COP0_TLBR(void);
int eCPU_COP0_ERET(void);

int eCPU_COP1_S(void);
int eCPU_COP1_D(void);
int eCPU_COP1_L(void);
int eCPU_COP1_W(void);
int eCPU_COP1_CFC1(void);
int eCPU_COP1_CTC1(void);
int eCPU_COP1_MTC1(void);
int eCPU_COP1_DIVS(void);
int eCPU_COP1_MULS(void);
int eCPU_COP1_CVTSW(void);
int eCPU_COP1_ADDS(void);
int eCPU_COP1_CVTDS(void);
int eCPU_COP1_TRUNCWD(void);
int eCPU_COP1_ADDD(void);
int eCPU_COP1_MFC1(void);
int eCPU_COP1_CVTWS(void);

int eCPU_SPECIAL(void);
int eCPU_REGIMM(void);
int eCPU_J(void);
int eCPU_JAL(void);
int eCPU_BEQ(void);
int eCPU_BNE(void);
int eCPU_BLEZ(void);
int eCPU_BGTZ(void);
int eCPU_ADDI(void);
int eCPU_ADDIU(void);
int eCPU_SLTI(void);
int eCPU_SLTIU(void);
int eCPU_ANDI(void);
int eCPU_ORI(void);
int eCPU_XORI(void);
int eCPU_LUI(void);
int eCPU_COP0(void);
int eCPU_COP1(void);
int eCPU_COP2(void);
int eCPU_COP1X(void);
int eCPU_BEQL(void);
int eCPU_BNEL(void);
int eCPU_BLEZL(void);
int eCPU_BGTZL(void);
int eCPU_DADDI(void);
int eCPU_DADDIU(void);
int eCPU_LB(void);
int eCPU_LH(void);
int eCPU_LWL(void);
int eCPU_LW(void);
int eCPU_LBU(void);
int eCPU_LHU(void);
int eCPU_LWR(void);
int eCPU_SB(void);
int eCPU_SH(void);
int eCPU_SWL(void);
int eCPU_SW(void);
int eCPU_SWR(void);
int eCPU_COP0_CACHE(void);
int eCPU_LWC1(void);
int eCPU_LWC2(void);
int eCPU_LDC1(void);
int eCPU_LD(void);
int eCPU_SWC1(void);
int eCPU_SWC2(void);
int eCPU_SDC1(void);
int eCPU_SD(void);
int eCPU_UNIMPLEMENTED(void);
int eCPU_SPECIAL_UNIMPLEMENTED(void);
int eCPU_REGIMM_UNIMPLEMENTED(void);
int eCPU_COP0_UNIMPLEMENTED(void);
int eCPU_COP1_UNIMPLEMENTED(void);

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
