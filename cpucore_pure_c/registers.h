struct cpu_reg {
	long long gpr[32];  //r4300i registers
	long long gpr0[32];	//COP0 registers
	double fgr_1[32];	//COP1 floating pointers
	int fcr[32];		//COP1 Control registers (only 0 and 31 exist)
				//But i'd rather trade memory in for speed than
				//vice versa
	long long pc;		//Program counter
	long long HI;		//High 64 bits of multiply/divide result
	long long LO;		//Low 64 bits of multiply/divide result
	char LLbit;		//Load/Link bit, as a char :O)
};

#define MAXTLB 32

struct tlb_struct
{
    uint32  valid;
    uint32  EntryHi;
    uint32  EntryLo1;
    uint32  EntryLo0;
    uint64  PageMask;
    uint32  LoCompare;
    uint32  MyHiMask;
};

extern struct tlb_struct        TLB[MAXTLB];
