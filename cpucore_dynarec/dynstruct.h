void DYNCPU_SPECIAL_SYNC(void);
void DYNCPU_SPECIAL_DIVU(void);
void DYNCPU_SDC1(void);
void DYNCPU_COP1_BC(void);
void DYNCPU_LWL(void);
void DYNCPU_LWR(void);
void DYNCPU_SWL(void);
void DYNCPU_SWR(void);
void DYNCPU_SWC1(void);
void DYNCPU_COP1_TRUNCWD(void);
void DYNCPU_COP1_ADDD(void);
void DYNCPU_COP1_NEGD(void);
void DYNCPU_COP1_CVTSW(void);
void DYNCPU_COP1_CVTDS(void);
void DYNCPU_COP1_CVTWS(void);
void DYNCPU_COP1_ADDS(void);
void DYNCPU_COP1_SUBS(void);
void DYNCPU_COP1_MULS(void);
void DYNCPU_COP1_DIVS(void);
void DYNCPU_COP1_S(void);
void DYNCPU_COP1_D(void);
void DYNCPU_COP1_W(void);
void DYNCPU_COP1_L(void);
void DYNCPU_LWC1(void);
void DYNCPU_LDC1(void);
void DYNCPU_REGIMM_BGEZ(void);
void DYNCPU_COP0_TLBP(void);
void DYNCPU_COP0_TLBR(void);
void DYNCPU_BGTZ(void);
void DYNCPU_REGIMM_BLTZ(void);
void DYNCPU_REGIMM_BLTZL(void);
void DYNCPU_BLEZ(void);
void DYNCPU_COP1_MFC1(void);
void DYNCPU_COP1_CFC1(void);
void DYNCPU_COP1_MTC1(void);
void DYNCPU_COP1_CTC1(void);
void DYNCPU_COP1(void);
void DYNCPU_J(void);
void DYNCPU_REGIMM_BGEZAL(void);
void DYNCPU_REGIMM_BGEZL(void);
void DYNCPU_CODEBLOCK(void);
void DYNCPU_BLEZL(void);
void DYNCPU_BNEL(void);
void DYNCPU_SPECIAL_JR(void);
void DYNCPU_BEQL(void);
void DYNCPU_JAL(void);
void DYNCPU_BEQ(void);
void DYNCPU_SPECIAL_JALR(void);
void DYNCPU_COP0_CACHE(void);
void DYNCPU_BNE(void);
void DYNCPU_UNIMPLEMENTED(void);
void DYNCPU_SPECIAL_UNIMPLEMENTED(void);
void DYNCPU_REGIMM_UNIMPLEMENTED(void);
void DYNCPU_COP0_UNIMPLEMENTED(void);
void DYNCPU_COP1_UNIMPLEMENTED(void);
void DYNCPU_SPECIAL(void);
void DYNCPU_REGIMM(void);
void DYNCPU_COP0(void);
void DYNCPU_SPECIAL_SLL(void);
void DYNCPU_SPECIAL_SLLV(void);
void DYNCPU_SPECIAL_SLT(void);
void DYNCPU_SPECIAL_SLTU(void);
void DYNCPU_LUI(void);
void DYNCPU_ORI(void);
void DYNCPU_SPECIAL_ADD(void);
void DYNCPU_ADDI(void);
void DYNCPU_ADDIU(void);
void DYNCPU_SPECIAL_ADDU(void);
void DYNCPU_SH(void);
void DYNCPU_LHU(void);
void DYNCPU_SB(void);
void DYNCPU_LBU(void);
void DYNCPU_LB(void);
void DYNCPU_SD(void);
void DYNCPU_SW(void);
void DYNCPU_LW(void);
void DYNCPU_LH(void);
void DYNCPU_JAL(void);
void DYNCPU_SPECIAL_JALR(void);
void DYNCPU_SPECIAL_JR(void);
void DYNCPU_SPECIAL_OR(void);
void DYNCPU_SPECIAL_NOR(void);
void DYNCPU_ANDI(void);
void DYNCPU_SPECIAL_SUB(void);
void DYNCPU_SPECIAL_SUBU(void);
void DYNCPU_SPECIAL_SRL(void);
void DYNCPU_SPECIAL_SRAV(void);
void DYNCPU_SPECIAL_SRLV(void);
void DYNCPU_SPECIAL_AND(void);
void DYNCPU_LD(void);
void DYNCPU_SPECIAL_MULT(void);
void DYNCPU_SPECIAL_MULTU(void);
void DYNCPU_SPECIAL_DMULTU(void);
void DYNCPU_SPECIAL_MFLO(void);
void DYNCPU_SPECIAL_MFHI(void);
void DYNCPU_SPECIAL_DSLL32(void);
void DYNCPU_SPECIAL_DSRA32(void);
void DYNCPU_SPECIAL_DDIVU(void);
void DYNCPU_SPECIAL_DIV(void);
void DYNCPU_SLTI(void);
void DYNCPU_SPECIAL_SRA(void);
void DYNCPU_SLTIU(void);
void DYNCPU_SPECIAL_MTLO(void);
void DYNCPU_SPECIAL_MTHI(void);
void DYNCPU_SPECIAL_XOR(void);
void DYNCPU_XORI(void);
void DYNCPU_COP0_MOVE(void);
void DYNCPU_COP0_TLBWI(void);
void DYNCPU_COP0_TLBP(void);
void DYNCPU_COP0_ERET(void);

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
void eCPU_REGIMM_BLTZ(void);
void eCPU_REGIMM_BGEZ(void);
void eCPU_REGIMM_BLTZL(void);
void eCPU_REGIMM_BGEZL(void);
void eCPU_REGIMM_BGEZAL(void);
void eCPU_COP0_MOVE(void);
void eCPU_COP0_TLBWI(void);
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
void eCPU_LWL(void);
void eCPU_LW(void);
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
void DYNCPU_COP1_CCS(void);
void DYNCPU_COP1_CCW(void);
void DYNCPU_COP1_CCL(void);
void DYNCPU_COP1_CVTDW(void);
void DYNCPU_COP1_CVTLD(void);
void DYNCPU_COP1_CVTSD(void);
void DYNCPU_COP1_CVTWD(void);
void DYNCPU_COP1_DIVD(void);
void DYNCPU_COP1_MOVD(void);
void DYNCPU_COP1_MOVS(void);
void DYNCPU_COP1_MULD(void);
void DYNCPU_COP1_NEGS(void);
void DYNCPU_COP1_SQRTS(void);
void DYNCPU_COP1_SUBD(void);
void DYNCPU_COP1_TRUNCWS(void);
