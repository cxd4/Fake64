#define         EAX       0x0
#define         AX        EAX
#define         AL        EAX
#define         ECX       0x1
#define         CX        ECX
#define         CL        ECX
#define         EDX       0x2
#define         DX        EDX
#define         DL        EDX
#define         EBX       0x3
#define         BX        EBX
#define         BL        EBX
#define         ESP       0x4
#define         SP        ESP
#define         AH        ESP
#define         EBP       0x5
#define         BP        EBP
#define         CH        EBP
#define         ESI       0x6
#define         SI        ESI
#define         DH        ESI
#define         EDI       0x7
#define         DI        EDI
#define         BH        EDI

  // Condition codes
#define          CC_O          0x0
#define          CC_NO         0x1
#define          CC_B          0x2
#define          CC_NAE        CC_B
#define		 CC_C          CC_B       
#define          CC_NB         0x3
#define          CC_NC         0x3
#define          CC_AE         CC_NB
#define          CC_E          0x4
#define          CC_Z          CC_E
#define          CC_NE         0x5
#define          CC_NZ         CC_NE
#define          CC_BE         0x6
#define          CC_NA         CC_BE
#define          CC_NBE        0x7
#define          CC_A          CC_NBE
#define          CC_S          0x8
#define          CC_NS         0x9
#define          CC_P          0xA
#define          CC_PE         CC_P
#define          CC_NP         0xB
#define          CC_PO         CC_NP
#define          CC_L          0xC
#define          CC_NGE        CC_L
#define          CC_NL         0xD
#define          CC_GE         CC_NL
#define          CC_LE         0xE
#define          CC_NG         CC_LE
#define          CC_NLE        0xF
#define          CC_G          CC_NLE

void W8(uint8 value);
void W16(uint16 value);
void W32(uint32 value);
void CALL(uint32 addr);
void PUSH_REG(uint8 reg);
void POP_REG(uint8 reg);
void PUSH_Imm32(uint32 value);
void PUSH_Imm8(uint8 value);
void CDQ(void);
void CALL_Reg(uint8 reg);
void MOV_RegToReg(uint8 reg1,uint8 reg2);
void MOV_RegToMem(uint8 reg, void *mem);
void MOV_MemToReg(uint8 reg, void *mem);
void MOV_Imm32ToReg(uint8 reg, uint32 data);
void MOV_Imm32ToMem(void *mem, uint32 data);
void CMP_RegWithReg(uint8 reg1,uint8 reg2);
void CMP_RegWithImm(uint8 reg, uint32 data);
void TEST_RegForReg(uint8 reg1, uint8 reg2);
void SETcc(uint8 ConditionCode, uint8 reg);
void Jcc(uint8 ConditionCode, uint8 offset);
void JMP_SHORT(uint8 offset);
void JMP_FAR(uint32 addr);
void AND(uint8 reg1,uint8 reg2);
void ANDI(uint8 reg,uint32 data);
void OR(uint8 reg1,uint8 reg2);
void ORI(uint8 reg,uint32 data);
void XOR(uint8 reg1,uint8 reg2);
void XORI(uint8 reg,uint32 data);
void RET(void);
void ADD(uint8 reg1,uint8 reg2);
void ADDI(uint8 reg, uint32 data);
void SUB(uint8 reg1,uint8 reg2);
void SUBI(uint8 reg, uint8 data);
void BT32_RegWithImm(void *addr,uint8 data);
void MOV_Imm8ToMem(void *mem, uint8 data);
void AND_Imm32ToMem(void *addr,uint32 data);

