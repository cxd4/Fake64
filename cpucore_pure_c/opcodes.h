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

void dCPU_SPECIAL_SLL(void);
void dCPU_SPECIAL_SRL(void);
void dCPU_SPECIAL_SRA(void);
void dCPU_SPECIAL_SLLV(void);
void dCPU_SPECIAL_SRLV(void);
void dCPU_SPECIAL_SRAV(void);
void dCPU_SPECIAL_JR(void);
void dCPU_SPECIAL_JALR(void);
void dCPU_SPECIAL_BREAK(void);
void dCPU_SPECIAL_MFHI(void);
void dCPU_SPECIAL_MTHI(void);
void dCPU_SPECIAL_MFLO(void);
void dCPU_SPECIAL_MTLO(void);
void dCPU_SPECIAL_DSLLV(void);
void dCPU_SPECIAL_DSRLV(void);
void dCPU_SPECIAL_DSRAV(void);
void dCPU_SPECIAL_MULT(void);
void dCPU_SPECIAL_MULTU(void);
void dCPU_SPECIAL_DIV(void);
void dCPU_SPECIAL_DIVU(void);
void dCPU_SPECIAL_DMULTU(void);
void dCPU_SPECIAL_DDIV(void);
void dCPU_SPECIAL_DDIVU(void);
void dCPU_SPECIAL_ADD(void);
void dCPU_SPECIAL_ADDU(void);
void dCPU_SPECIAL_SUB(void);
void dCPU_SPECIAL_SUBU(void);
void dCPU_SPECIAL_AND(void);
void dCPU_SPECIAL_OR(void);
void dCPU_SPECIAL_XOR(void);
void dCPU_SPECIAL_NOR(void);
void dCPU_SPECIAL_SLT(void);
void dCPU_SPECIAL_SLTU(void);
void dCPU_SPECIAL_DADDU(void);
void dCPU_SPECIAL_DSLL32(void);
void dCPU_SPECIAL_DSRA32(void);

void dCPU_REGIMM_BLTZ(void);
void dCPU_REGIMM_BGEZ(void);
void dCPU_REGIMM_BLTZL(void);
void dCPU_REGIMM_BGEZL(void);
void dCPU_REGIMM_BGEZAL(void);

void dCPU_COP0_MOVE(void);
void dCPU_COP0_TLBWI(void);
void dCPU_COP0_TLBP(void);
void dCPU_COP0_TLBR(void);
void dCPU_COP0_ERET(void);

void dCPU_COP1_S(void);
void dCPU_COP1_D(void);
void dCPU_COP1_W(void);
void dCPU_COP1_L(void);
void dCPU_COP1_CFC1(void);
void dCPU_COP1_CTC1(void);

void dCPU_SPECIAL(void);
void dCPU_REGIMM(void);
void dCPU_J(void);
void dCPU_JAL(void);
void dCPU_BEQ(void);
void dCPU_BNE(void);
void dCPU_BLEZ(void);
void dCPU_BGTZ(void);
void dCPU_ADDI(void);
void dCPU_ADDIU(void);
void dCPU_SLTI(void);
void dCPU_SLTIU(void);
void dCPU_ANDI(void);
void dCPU_ORI(void);
void dCPU_XORI(void);
void dCPU_LUI(void);
void dCPU_COP0(void);
void dCPU_COP1(void);
void dCPU_COP2(void);
void dCPU_COP1X(void);
void dCPU_BEQL(void);
void dCPU_BNEL(void);
void dCPU_BLEZL(void);
void dCPU_BGTZL(void);
void dCPU_DADDI(void);
void dCPU_DADDIU(void);
void dCPU_LB(void);
void dCPU_LH(void);
void dCPU_LWL(void);
void dCPU_LW(void);
void dCPU_LBU(void);
void dCPU_LHU(void);
void dCPU_LWR(void);
void dCPU_SB(void);
void dCPU_SH(void);
void dCPU_SWL(void);
void dCPU_SW(void);
void dCPU_SWR(void);
void dCPU_COP0_CACHE(void);
void dCPU_LWC1(void);
void dCPU_LWC2(void);
void dCPU_LDC1(void);
void dCPU_LD(void);
void dCPU_SWC1(void);
void dCPU_SWC2(void);
void dCPU_SDC1(void);
void dCPU_SD(void);
void dCPU_UNIMPLEMENTED(void);
void dCPU_SPECIAL_UNIMPLEMENTED(void);
void dCPU_REGIMM_UNIMPLEMENTED(void);
void dCPU_COP0_UNIMPLEMENTED(void);
void dCPU_COP1_UNIMPLEMENTED(void);
void dCPU_COP1_CTC1(void);
void dCPU_COP1_MTC1(void);
void dCPU_COP1_DIVS(void);
void dCPU_COP1_MULS(void);
void dCPU_COP1_CVTSW(void);
void dCPU_COP1_ADDS(void);
void dCPU_COP1_CVTDS(void);
void dCPU_COP1_TRUNCWD(void);
void dCPU_COP1_ADDD(void);
void dCPU_COP1_MFC1(void);
void dCPU_COP1_DIVS(void);
void dCPU_COP1_ADDS(void);
void dCPU_COP1_ADDD(void);
void dCPU_COP1_SUBS(void);
void dCPU_COP1_MOVS(void);
void dCPU_COP1_MOVD(void);
void dCPU_COP1_TRUNCWS(void);
void dCPU_COP1_CVTWD(void);
void dCPU_COP1_CVTSD(void);
void dCPU_COP1_NEGS(void);

void eCPU_COP1_CCD(void);
void eCPU_COP1_CCS(void);
void eCPU_COP1_CCL(void);
void eCPU_COP1_CCW(void);
void eCPU_COP1_BC(void);
void eCPU_COP1_SUBD(void);
void eCPU_COP1_CVTSL(void);
void eCPU_COP1_CVTWD(void);
void eCPU_COP1_CVTLD(void);


void dCPU_COP1_CVTDW(void);
void dCPU_COP1_SUBD(void);
void dCPU_COP1_CONDITIONAL(void);
void dCPU_COP1_MULD(void);
void dCPU_COP1_BC(void);
void dCPU_COP1_DIVD(void);









void eCPU_SPECIAL_SLL(void);
void eCPU_SPECIAL_SRL(void);
void eCPU_SPECIAL_SRA(void);
void eCPU_SPECIAL_SLLV(void);
void eCPU_SPECIAL_SRLV(void);
void eCPU_SPECIAL_JR(void);
void eCPU_SPECIAL_JALR(void);
void eCPU_SPECIAL_BREAK(void);
void eCPU_SPECIAL_MFHI(void);
void eCPU_SPECIAL_MTHI(void);
void eCPU_SPECIAL_MFLO(void);
void eCPU_SPECIAL_MTLO(void);
void eCPU_SPECIAL_DSLLV(void);
void eCPU_SPECIAL_DSRLV(void);
void eCPU_SPECIAL_DSRAV(void);
void eCPU_SPECIAL_MULT(void);
void eCPU_SPECIAL_MULTU(void);
void eCPU_SPECIAL_DIV(void);
void eCPU_SPECIAL_DIVU(void);
void eCPU_SPECIAL_DMULTU(void);
void eCPU_SPECIAL_DDIV(void);
void eCPU_SPECIAL_DDIVU(void);
void eCPU_SPECIAL_ADD(void);
void eCPU_SPECIAL_ADDU(void);
void eCPU_SPECIAL_SUB(void);
void eCPU_SPECIAL_SUBU(void);
void eCPU_SPECIAL_AND(void);
void eCPU_SPECIAL_OR(void);
void eCPU_SPECIAL_XOR(void);
void eCPU_SPECIAL_NOR(void);
void eCPU_SPECIAL_SLT(void);
void eCPU_SPECIAL_SLTU(void);
void eCPU_SPECIAL_DADDU(void);
void eCPU_SPECIAL_DSLL32(void);
void eCPU_SPECIAL_DSRA32(void);
void eCPU_SPECIAL_SRAV();
void eCPU_SPECIAL_SYNC();
void eCPU_SPECIAL_TNE(void);
void eCPU_SPECIAL_DSLL(void);
void eCPU_SPECIAL_DSRL(void);
void eCPU_SPECIAL_DSRL32(void);
void eCPU_SPECIAL_SYSCALL();
void eCPU_SPECIAL_DSLLV(void);

void eCPU_REGIMM_BLTZ(void);
void eCPU_REGIMM_BGEZ(void);
void eCPU_REGIMM_BLTZL(void);
void eCPU_REGIMM_BGEZL(void);
void eCPU_REGIMM_BGEZAL(void);
void eCPU_REGIMM_BLTZL(void);

void eCPU_COP0_MOVE(void);
void eCPU_COP0_TLBWI(void);
void eCPU_COP0_TLBP(void);
void eCPU_COP0_TLBR(void);
void eCPU_COP0_ERET(void);

void eCPU_COP1_S(void);
void eCPU_COP1_D(void);
void eCPU_COP1_L(void);
void eCPU_COP1_W(void);
void eCPU_COP1_CFC1(void);
void eCPU_COP1_CTC1(void);
void eCPU_COP1_MTC1(void);
void eCPU_COP1_DIVS(void);
void eCPU_COP1_MULS(void);
void eCPU_COP1_CVTSW(void);
void eCPU_COP1_ADDS(void);
void eCPU_COP1_CVTDS(void);
void eCPU_COP1_TRUNCWD(void);
void eCPU_COP1_ADDD(void);
void eCPU_COP1_MFC1(void);
void eCPU_COP1_CVTWS(void);
void eCPU_COP1_SUBS(void);
void eCPU_COP1_MULD(void);
void eCPU_COP1_CVTSD(void);
void eCPU_COP1_MOVS(void);
void eCPU_COP1_CVTDW(void);
void eCPU_COP1_DIVD(void);
void eCPU_COP1_SQRTS(void);
void eCPU_COP1_TRUNCWS(void);
void eCPU_COP1_NEGS(void);
void eCPU_COP1_NEGD(void);
void eCPU_COP1_MOVD(void);



void eCPU_SDC1(void);

void eCPU_SPECIAL(void);
void eCPU_REGIMM(void);
void eCPU_J(void);
void eCPU_JAL(void);
void eCPU_BEQ(void);
void eCPU_BNE(void);
void eCPU_BLEZ(void);
void eCPU_BGTZ(void);
void eCPU_ADDI(void);
void eCPU_ADDIU(void);
void eCPU_SLTI(void);
void eCPU_SLTIU(void);
void eCPU_ANDI(void);
void eCPU_ORI(void);
void eCPU_XORI(void);
void eCPU_LUI(void);
void eCPU_COP0(void);
void eCPU_COP1(void);
void eCPU_COP2(void);
void eCPU_COP1X(void);
void eCPU_BEQL(void);
void eCPU_BNEL(void);
void eCPU_BLEZL(void);
void eCPU_BGTZL(void);
void eCPU_DADDI(void);
void eCPU_DADDIU(void);
void eCPU_LB(void);
void eCPU_LH(void);
void eCPU_LW(void);
void eCPU_LWU(void);
void eCPU_LBU(void);
void eCPU_LHU(void);
void eCPU_LWR(void);
void eCPU_SB(void);
void eCPU_SH(void);
void eCPU_SWL(void);
void eCPU_SW(void);
void eCPU_SWR(void);
void eCPU_COP0_CACHE(void);
void eCPU_LWC1(void);
void eCPU_LWC2(void);
void eCPU_LDC1(void);
void eCPU_LD(void);
void eCPU_SWC1(void);
void eCPU_SWC2(void);
void eCPU_SDC1(void);
void eCPU_SD(void);
void eCPU_UNIMPLEMENTED(void);
void eCPU_SPECIAL_UNIMPLEMENTED(void);
void eCPU_REGIMM_UNIMPLEMENTED(void);
void eCPU_COP0_UNIMPLEMENTED(void);
void eCPU_COP1_UNIMPLEMENTED(void);

void eCPU_LWL(void);
void eCPU_LWR(void);
void eCPU_BGTL(void);
void eCPU_SPECIAL_DDIV(void);
void eCPU_DADDIU(void);
void eCPU_DADDI(void);

void dCPU_STOLEN(void);
void eCPU_STOLEN(void);

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
#define CPU_LWU (uint32)(39 << bshift)
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
