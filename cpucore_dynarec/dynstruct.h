int DYNCPU_COP1_TRUNCWD(void);
int DYNCPU_COP1_ADDD(void);
int DYNCPU_COP1_CVTSW(void);
int DYNCPU_COP1_CVTDS(void);
int DYNCPU_COP1_CVTWS(void);
int DYNCPU_COP1_ADDS(void);
int DYNCPU_COP1_SUBS(void);
int DYNCPU_COP1_MULS(void);
int DYNCPU_COP1_DIVS(void);
int DYNCPU_COP1_S(void);
int DYNCPU_COP1_D(void);
int DYNCPU_COP1_W(void);
int DYNCPU_COP1_L(void);
int DYNCPU_LWC1(void);
int DYNCPU_LDC1(void);
int DYNCPU_REGIMM_BGEZ(void);
int DYNCPU_COP0_TLBP(void);
int DYNCPU_COP0_TLBR(void);
int DYNCPU_BGTZ(void);
int DYNCPU_REGIMM_BLTZ(void);
int DYNCPU_BLEZ(void);
int DYNCPU_COP1_MFC1(void);
int DYNCPU_COP1_CFC1(void);
int DYNCPU_COP1_MTC1(void);
int DYNCPU_COP1_CTC1(void);
int DYNCPU_COP1(void);
int DYNCPU_J(void);
int DYNCPU_REGIMM_BGEZAL(void);
int DYNCPU_REGIMM_BGEZL(void);
int DYNCPU_CODEBLOCK(void);
int DYNCPU_BLEZL(void);
int DYNCPU_BNEL(void);
int DYNCPU_SPECIAL_JR(void);
int DYNCPU_BEQL(void);
int DYNCPU_JAL(void);
int DYNCPU_BEQ(void);
int DYNCPU_SPECIAL_JALR(void);
int DYNCPU_COP0_CACHE(void);
int DYNCPU_BNE(void);
int DYNCPU_UNIMPLEMENTED(void);
int DYNCPU_SPECIAL_UNIMPLEMENTED(void);
int DYNCPU_REGIMM_UNIMPLEMENTED(void);
int DYNCPU_COP0_UNIMPLEMENTED(void);
int DYNCPU_COP1_UNIMPLEMENTED(void);
int DYNCPU_SPECIAL(void);
int DYNCPU_REGIMM(void);
int DYNCPU_COP0(void);
int DYNCPU_SPECIAL_SLL(void);
int DYNCPU_SPECIAL_SLLV(void);
int DYNCPU_SPECIAL_SLT(void);
int DYNCPU_SPECIAL_SLTU(void);
int DYNCPU_LUI(void);
int DYNCPU_ORI(void);
int DYNCPU_SPECIAL_ADD(void);
int DYNCPU_ADDI(void);
int DYNCPU_ADDIU(void);
int DYNCPU_SPECIAL_ADDU(void);
int DYNCPU_SH(void);
int DYNCPU_LHU(void);
int DYNCPU_SB(void);
int DYNCPU_LBU(void);
int DYNCPU_LB(void);
int DYNCPU_SD(void);
int DYNCPU_SW(void);
int DYNCPU_LW(void);
int DYNCPU_LH(void);
int DYNCPU_JAL(void);
int DYNCPU_SPECIAL_JALR(void);
int DYNCPU_SPECIAL_JR(void);
int DYNCPU_SPECIAL_OR(void);
int DYNCPU_SPECIAL_NOR(void);
int DYNCPU_ANDI(void);
int DYNCPU_SPECIAL_SUB(void);
int DYNCPU_SPECIAL_SUBU(void);
int DYNCPU_SPECIAL_SRL(void);
int DYNCPU_SPECIAL_SRLV(void);
int DYNCPU_SPECIAL_AND(void);
int DYNCPU_LD(void);
int DYNCPU_SPECIAL_MULT(void);
int DYNCPU_SPECIAL_MULTU(void);
int DYNCPU_SPECIAL_DMULTU(void);
int DYNCPU_SPECIAL_MFLO(void);
int DYNCPU_SPECIAL_MFHI(void);
int DYNCPU_SPECIAL_DSLL32(void);
int DYNCPU_SPECIAL_DSRA32(void);
int DYNCPU_SPECIAL_DDIVU(void);
int DYNCPU_SPECIAL_DIV(void);
int DYNCPU_SLTI(void);
int DYNCPU_SPECIAL_SRA(void);
int DYNCPU_SLTIU(void);
int DYNCPU_SPECIAL_MTLO(void);
int DYNCPU_SPECIAL_MTHI(void);
int DYNCPU_SPECIAL_XOR(void);
int DYNCPU_XORI(void);
int DYNCPU_COP0_MOVE(void);
int DYNCPU_COP0_TLBWI(void);
int DYNCPU_COP0_TLBP(void);
int DYNCPU_COP0_ERET(void);
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
