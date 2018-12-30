struct cpu_reg {
	long long gpr[32];  //r4300i registers
	long long gpr0[32]; //COP0 registers
	
  long long gpr1[32]; //COP1 registers

	unsigned int cr0[32];	     //COP0 Control registers (do they exist?)
	unsigned int cr1[32];	     //COP1 Control registers (only 0 and 31 exist)
				//But i'd rather trade memory in for speed than
				//vice versa
	long long pc;		//Program counter
	long long HI;		//High 64 bits of multiply/divide result
	long long LO;		//Low 64 bits of multiply/divide result
	char LLbit;		//Load/Link bit, as a char :O)
	int64 CPUdelayPC;
	uint32 CPUdelay;

  long long VInextInt; // when we fire a new VI interrupt
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

#define COP0_COUNT	reg.gpr0[9]
#define COP0_STATUS	reg.gpr0[12]
#define COP0_CAUSE	reg.gpr0[13]
#define COP0_EPC	reg.gpr0[14]
#define COP0_ERROR_EPC	reg.gpr0[30]

// COP0_STATUS bitmasks etc
#define COP0_STATUS_IE		( COP0_STATUS & 1 )
#define COP0_STATUS_EXL		( COP0_STATUS & 2 )
#define COP0_STATUS_ERL		( COP0_STATUS  & 4 )
#define COP0_STATUS_KSU		(( COP0_STATUS & 0x18)>>3)
#define COP0_STATUS_UX		( COP0_STATUS & 0x20 )
#define COP0_STATUS_SX		( COP0_STATUS & 0x40 )
#define COP0_STATUS_KX		( COP0_STATUS & 0x80 )
#define COP0_STATUS_IMF		(( COP0_STATUS & 0xFF00 ) >> 8) // add subfileds if needed
#define COP0_STATUS_DS		(( COP0_STATUS & 0x1FF0000 ) >> 16 ) // ditto
#define COP0_STATUS_RE		( COP0_STATUS & 0x2000000 )
#define COP0_STATUS_FR		( COP0_STATUS & 0x4000000 )
#define COP0_STATUS_RP		( COP0_STATUS & 0x8000000 )
#define COP0_STATUS_CU		(( COP0_STATUS & 0x8000000 )>> 28 )

// bitmasks useful for setting stuff
#define COP0_STATUS_IE_BM	( 1 )
#define COP0_STATUS_EXL_BM	( 2 )
#define COP0_STATUS_ERL_BM	( 4 )
#define COP0_STATUS_KSU_BM	( 0x18 )
#define COP0_STATUS_UX_BM	( 0x20 )
#define COP0_STATUS_SX_BM	( 0x40 )
#define COP0_STATUS_KX_BM	( 0x80 )
#define COP0_STATUS_IMF_BM	( 0xFF00 )
#define COP0_STATUS_DS_BM	( 0x1FF0000 )
#define COP0_STATUS_RE_BM	( 0x2000000 )
#define COP0_STATUS_FR_BM	( 0x4000000 )
#define COP0_STATUS_RP_BM	( 0x8000000 )
#define COP0_STATUS_CU_BM	( 0x8000000  )

#define COP0_CAUSE_BD_BM	( 0x80000000 )
#define COP0_CAUSE_EXCCODE_BM	( 0x7c )

// Other tasks:

#define OTHER_PIF               0x01
#define OTHER_DMA_RD            0x02
#define OTHER_DMA_WR            0x04
#define OTHER_SI_DMA_RD         0x08
#define OTHER_SI_DMA_WR         0x10
#define OTHER_AI                0x20
