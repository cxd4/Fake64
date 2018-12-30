#include <general.h>

//#define DEBUG

extern void *codeblockpointer;

char *regnames[] =
{
        "EAX",
        "ECX",
        "EDX",
        "EBX",
        "ESP",
        "EBP",
        "ESI",
        "EDI"
};

void W8(uint8 value)
{
    *(uint8 *)(codeblockpointer) = value;
    codeblockpointer += 1;
}
void W16(uint16 value)
{
    *(uint16 *)(codeblockpointer) = value;
    codeblockpointer += 2;
}
void W32(uint32 value)
{
    *(uint32 *)(codeblockpointer) = value;
    codeblockpointer += 4;
}

// opcodes, <op> <dest>, <src> form (when printing)
// Ported from 1964

void CALL(uint32 addr)
{ uint32 temp;

  temp = (uint32)addr - (uint32)codeblockpointer - 5;

  W8(0xe8);
  W32(temp);

#ifdef DEBUG
  printf("CALL 0x%08X\n", addr);
#endif
}

void PUSH_REG(uint8 reg)
{

  W8(0x50 | reg);

#ifdef DEBUG
  printf("PUSH %s\n", regnames[reg]);
#endif
}

void POP_REG(uint8 reg)
{
  W8(0x58 | reg);

#ifdef DEBUG
  printf("POP %s\n", regnames[reg]);
#endif 
}

void PUSH_Imm32(uint32 value)
{
  W8(0x68);
  W32(value);

#ifdef DEBUG
  printf("PUSH 0x%08X\n", value);
#endif 
}

void PUSH_Imm8(uint8 value)
{
  W8(0x6A);
  W8(value);

#ifdef DEBUG
  printf("PUSH 0x%02X\n", value);
#endif
}

void CDQ(void)
{
  W8(0x99);

#ifdef DEBUG
  printf("CDQ\n");
#endif
}

void CALL_Reg(uint8 reg)
{
  W8(0xFF);
  W8(0xD0 | reg);

#ifdef DEBUG
  printf("CALL %s\n", regnames[reg]);
#endif
}

void MOV_RegToReg(uint8 reg1,uint8 reg2)
{ 
  if(reg1==reg2) return; //simple optimization

  W8(0x8b);
  W8(0xc0 | (reg1<<3) | reg2);

#ifdef DEBUG
  printf("MOV %s, %s\n",regnames[reg1],regnames[reg2]);
#endif  
}

void MOV_RegToMem(uint8 reg, void *mem)
{
  W8(0x89);
  W8(0x05|(reg<<3));
  W32((uint32)mem);
  
#ifdef DEBUG
  printf("MOV [0x%x], %s\n",mem,regnames[reg]);
#endif
}

void MOV_MemToReg(uint8 reg, void *mem)
{
  W8(0x8B);
  W8(0x05|(reg<<3));
  W32((uint32)mem);

#ifdef DEBUG
  printf("MOV %s, [0x%x]\n",regnames[reg],mem);
#endif
}

void MOV_Imm32ToReg(uint8 reg, uint32 data)
{
  W8(0xB8|reg);
  W32(data);

#ifdef DEBUG
  printf("MOV %s, 0x%x\n",regnames[reg],data);
#endif
}

void MOV_Imm8ToMem(void *mem, uint8 data)
{
  W8(0xC6);
  W8(0x05);
  W32((uint32)mem);
  W8(data);

#ifdef DEBUG
  printf("MOV [0x%x], 0x%x\n",mem,data);
#endif
}


void MOV_Imm32ToMem(void *mem, uint32 data)
{
  W8(0xC7);
  W8(0x05);
  W32((uint32)mem);
  W32(data);

#ifdef DEBUG
  printf("MOV [0x%x], 0x%x\n",mem,data);
#endif
}

void CMP_RegWithReg32(uint8 reg1,uint8 reg2)
{
  W8(0x39);
  W8(0xC0 | (reg1<<3) | reg2);

#ifdef DEBUG
  printf("CMP %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void CMP_RegWithImm(uint8 reg, uint32 data)
{
  W8(0x81);
  W8(0xF8 | reg);
  W32(data);

#ifdef DEBUG
  printf("CMP %s, 0x%x\n",regnames[reg],data);
#endif
}

void TEST_RegForReg(uint8 reg1, uint8 reg2)
{
  W8(0x85);
  W8(0xC0 | (reg1<<3) | reg2);

#ifdef DEBUG
  printf("TEST %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void SETcc_Reg(uint8 ConditionCode, uint8 reg)
{
  W8(0x0F);
  W8(0x90 | ConditionCode);
  W8(0xC0 | reg);

#ifdef DEBUG
  printf("SET (0x%x) %s\n",ConditionCode,regnames[reg]);
#endif
}

void Jcc(uint8 ConditionCode, uint8 offset)
{
  W8(0x70 | ConditionCode);
  W8(offset);
#ifdef DEBUG
  printf("JCC (0x%x) %d\n",ConditionCode,(int8)offset);
#endif
}

void JMP_SHORT(uint8 offset)
{
  W8(0xEB);
  W8(offset);
#ifdef DEBUG
  printf("JMP %d\n",(int8)offset);
#endif
}

void JMP_FAR(uint32 addr)
{
  W8(0xFF);
  W8(0x25);
  W32(addr);
#ifdef DEBUG
  printf("JMP 0x%x\n",addr);
#endif
}

void AND(uint8 reg1,uint8 reg2)
{
  if(reg1==reg2) return; // small optimization
  W8(0x23);
  W8(0xC0 | (reg1<<3) | reg2);
#ifdef DEBUG
  printf("AND %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void ANDI(uint8 reg,uint32 data)
{ 
 W8(0x81);
 W8(0xE0|reg);
 W32(data);
#ifdef DEBUG
  printf("AND %s, 0x%x\n",regnames[reg],data);
#endif
}

void AND_Imm32ToMem(void *addr,uint32 data)
{
   W8(0x81);
   W8(0x25);
   W32((uint32)addr);
   W32(data);

#ifdef DEBUG
  printf("AND [0x%x], 0x%x\n", addr, data);
#endif 

}

void OR(uint8 reg1,uint8 reg2)
{
  if(reg1==reg2) return; // small optimization
  W8(0x0B);
  W8(0xC0 | (reg1<<3) | reg2);
#ifdef DEBUG
  printf("OR %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void ORI(uint8 reg,uint32 data)
{
 W8(0x81);
 W8(0xc8|reg);
 W32(data);
#ifdef DEBUG
  printf("OR %s, 0x%x\n",regnames[reg],data);
#endif
}

void XOR(uint8 reg1,uint8 reg2)
{
  W8(0x33);
  W8(0xC0 | (reg1<<3) | reg2);
#ifdef DEBUG
  printf("OR %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void XORI(uint8 reg,uint32 data)
{
 W8(0x81);
 W8(0xf0|reg);
 W32(data);
#ifdef DEBUG
  printf("OR %s, 0x%x\n",regnames[reg],data);
#endif
}

void RET(void)
{
  W8(0xC3);

#ifdef DEBUG
  printf("RET\n");
#endif
}

void ADD(uint8 reg1,uint8 reg2)
{
  W8(0x01);
  W8(0xC0 | (reg1<<3) | reg2);

#ifdef DEBUG
  printf("ADD %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void ADDI(uint8 reg, uint32 data)
{
/*
  if(data<=0xff)
   {
     W8(0x83);
     W8(0xC0|reg);
     W8(data);     
   }
  else
*/   {
     W8(0x81);
     W8(0xC0|reg);
     W32(data);
   }

#ifdef DEBUG
  printf("ADD %s, 0x%x\n",regnames[reg],data);
#endif
}

void ADD_MemToEAX(void *addr)
{
  W8(0x03);
  W8(0x05);
  W32((uint32)addr);
#ifdef DEBUG
  printf("ADD EAX, [0x%x]\n",addr);
#endif
}

void ADD_ImmToMem(void *addr,uint32 data)
{
   W8(0x81);
   W8(0x05);
   W32((uint32)addr);
   W32(data);
#ifdef DEBUG
   printf("ADD [0x%x], $0x%x\n",addr,data);
#endif
}

void SUB(uint8 reg1,uint8 reg2)
{
  W8(0x29);
  W8(0xC0 | (reg1<<3) | reg2);

#ifdef DEBUG
  printf("SUB %s, %s\n",regnames[reg1],regnames[reg2]);
#endif
}

void SUBI(uint8 reg, uint8 data)
{
  W8(0x83);
  W8(0xe8 | reg);
  W8(data);

#ifdef DEBUG
  printf("SUB %s, 0x%x\n",regnames[reg],data);
#endif 
}

void BT32_RegWithImm(void *addr,uint8 data)
{

  W8(0x0f);
  W8(0xba);
  W8(0x25);
  W32((uint32)addr);
  W8(data);

#ifdef DEBUG
  printf("BT32 [0x%x],0x%x\n",addr,data);
#endif
}

