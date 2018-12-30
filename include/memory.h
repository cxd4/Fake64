#ifndef MEMORY_H
#define MEMORY_H

#define RAM_SIZE	0x00800000

#define DRAM_BASE_REG	0x03F00000
#define SP_DMEM		0x04000000
#define SP_DMEM_END	0x04000FFF
#define SP_IMEM		0x04001000
#define SP_IMEM_END	0x04001FFF
#define SP_BASE_REG	0x04040000
#define DPC_BASE_REG	0x04100000
#define DPS_BASE_REG	0x04200000
#define MI_BASE_REG	0x04300000
#define VI_BASE_REG	0x04400000
#define AI_BASE_REG	0x04500000
#define PI_BASE_REG	0x04600000
#define RI_BASE_REG	0x04700000
#define SI_BASE_REG	0x04800000
#define ROM_BASE	0x10000000
#define PIF_MEM_BASE	0x1FC00000

extern uint8 *RDREGS;		// RDRAM registers
extern uint8 *SPDIMEM;
extern uint8 *SPREGS;		// SP registers
extern uint8 *SP_REG;
extern uint8 *DPREGS;		// DP registers
extern uint8 *DPSREGS;		// DP span registers
extern uint8 *MIREGS;		// MIPS Interface registers
extern uint8 *VIREGS;		// Video Interface registers
extern uint8 *AIREGS;		// Audio Interface registers
extern uint8 *PIREGS;		// Peripheral Interface registers
extern uint8 *RIREGS;		// RDRAM Interface registers
extern uint8 *SIREGS;		// Serial Interface registers
extern uint8 *PIFMEM;		// PIF area
extern uint8 *RDRAM;		// rdram
extern uint8 *ROM;		// rom(header&everythin)

extern int RAM_OFFSET_MAP[0x2000];

int alloc_memory (struct rom *rom);
void init_memory (int romlength);

#define SI_BASE_REG             0x04800000
#define SI_DRAM_ADDR_REG        (SI_BASE_REG+0x00)
#define SI_PIF_ADDR_RD64B_REG   (SI_BASE_REG+0x04)
#define SI_PIF_ADDR_WR64B_REG   (SI_BASE_REG+0x10)
#define SI_STATUS_REG           (SI_BASE_REG+0x18)

#define SI_STATUS_DMA_BUSY      0x0001
#define SI_STATUS_RD_BUSY       0x0002
#define SI_STATUS_DMA_ERROR     0x0008
#define SI_STATUS_INTERRUPT     0x1000

#define PI_BASE_REG             0x04600000
#define PI_DRAM_ADDR_REG        (PI_BASE_REG+0x00)
#define PI_CART_ADDR_REG        (PI_BASE_REG+0x04)
#define PI_RD_LEN_REG           (PI_BASE_REG+0x08)
#define PI_WR_LEN_REG           (PI_BASE_REG+0x0C)
#define PI_STATUS_REG           (PI_BASE_REG+0x10)
#define PI_BSD_DOM1_LAT_REG     (PI_BASE_REG+0x14)
#define PI_BSD_DOM1_PWD_REG     (PI_BASE_REG+0x18)
#define PI_BSD_DOM1_PGS_REG     (PI_BASE_REG+0x1C)
#define PI_BSD_DOM1_RLS_REG     (PI_BASE_REG+0x20)
#define PI_BSD_DOM2_LAT_REG     (PI_BASE_REG+0x24)
#define PI_BSD_DOM2_PWD_REG     (PI_BASE_REG+0x28)
#define PI_BSD_DOM2_PGS_REG     (PI_BASE_REG+0x2C)
#define PI_BSD_DOM2_RLS_REG     (PI_BASE_REG+0x30)

#define PI_DOMAIN1_REG          PI_BSD_DOM1_LAT_REG
#define PI_DOMAIN2_REG          PI_BSD_DOM2_LAT_REG

#define PI_DOM_LAT_OFS          0x00
#define PI_DOM_PWD_OFS          0x04
#define PI_DOM_PGS_OFS          0x08
#define PI_DOM_RLS_OFS          0x0C

#define PI_STATUS_ERROR         0x04
#define PI_STATUS_IO_BUSY       0x02
#define PI_STATUS_DMA_BUSY      0x01

#define PI_STATUS_RESET         0x01
#define PI_SET_RESET            PI_STATUS_RESET

#define PI_STATUS_CLR_INTR      0x02
#define PI_CLR_INTR             PI_STATUS_CLR_INTR

#define PI_DMA_BUFFER_SIZE      128

#define PI_DOM1_ADDR1           0x06000000
#define PI_DOM1_ADDR2           0x10000000
#define PI_DOM1_ADDR3           0x1FD00000
#define PI_DOM2_ADDR1           0x05000000
#define PI_DOM2_ADDR2           0x08000000

#define MI_BASE_REG             0x04300000
#define MI_INIT_MODE_REG        (MI_BASE_REG+0x00)
#define MI_MODE_REG             MI_INIT_MODE_REG

#define MI_CLR_INIT             0x0080
#define MI_SET_INIT             0x0100
#define MI_CLR_EBUS             0x0200
#define MI_SET_EBUS             0x0400
#define MI_CLR_DP_INTR          0x0800
#define MI_CLR_RDRAM            0x1000
#define MI_SET_RDRAM            0x2000

#define MI_MODE_INIT            0x0080
#define MI_MODE_EBUS            0x0100
#define MI_MODE_RDRAM           0x0200

#define MI_VERSION_REG          (MI_BASE_REG+0x04)
#define MI_NOOP_REG             MI_VERSION_REG
#define MI_INTR_REG             (MI_BASE_REG+0x08)
#define MI_INTR_REG_R		*((uint32 *)(MIREGS+0x08))
#define MI_INTR_MASK_REG        (MI_BASE_REG+0x0C)
#define MI_INTR_MASK_REG_R	*((uint32 *)(MIREGS+0x0C))

#define MI_INTR_SP              0x01
#define MI_INTR_SI              0x02
#define MI_INTR_AI              0x04
#define MI_INTR_VI              0x08
#define MI_INTR_PI              0x10
#define MI_INTR_DP              0x20

#define MI_INTR_MASK_CLR_SP     0x0001
#define MI_INTR_MASK_SET_SP     0x0002
#define MI_INTR_MASK_CLR_SI     0x0004
#define MI_INTR_MASK_SET_SI     0x0008
#define MI_INTR_MASK_CLR_AI     0x0010
#define MI_INTR_MASK_SET_AI     0x0020
#define MI_INTR_MASK_CLR_VI     0x0040
#define MI_INTR_MASK_SET_VI     0x0080
#define MI_INTR_MASK_CLR_PI     0x0100
#define MI_INTR_MASK_SET_PI     0x0200
#define MI_INTR_MASK_CLR_DP     0x0400
#define MI_INTR_MASK_SET_DP     0x0800

#define MI_INTR_MASK_SP         0x01
#define MI_INTR_MASK_SI         0x02
#define MI_INTR_MASK_AI         0x04
#define MI_INTR_MASK_VI         0x08
#define MI_INTR_MASK_PI         0x10
#define MI_INTR_MASK_DP         0x20

#define RI_BASE_REG             0x04700000
#define RI_MODE_REG             (RI_BASE_REG+0x00)
#define RI_CONFIG_REG           (RI_BASE_REG+0x04)
#define RI_CURRENT_LOAD_REG     (RI_BASE_REG+0x08)
#define RI_SELECT_REG           (RI_BASE_REG+0x0C)
#define RI_REFRESH_REG          (RI_BASE_REG+0x10)
#define RI_COUNT_REG            RI_REFRESH_REG
#define RI_LATENCY_REG          (RI_BASE_REG+0x14)
#define RI_RERROR_REG           (RI_BASE_REG+0x18)
#define RI_WERROR_REG           (RI_BASE_REG+0x1C)

#define SP_BASE_REG             0x04040000
#define SP_MEM_ADDR_REG         (SP_BASE_REG+0x00)
#define SP_DRAM_ADDR_REG        (SP_BASE_REG+0x04)
#define SP_RD_LEN_REG           (SP_BASE_REG+0x08)
#define SP_WR_LEN_REG           (SP_BASE_REG+0x0C)
#define SP_STATUS_REG           (SP_BASE_REG+0x10)
#define SP_DMA_FULL_REG         (SP_BASE_REG+0x14)
#define SP_DMA_BUSY_REG         (SP_BASE_REG+0x18)
#define SP_SEMAPHORE_REG        (SP_BASE_REG+0x1C)
#define SP_SEMAPHORE_REG        (SP_BASE_REG+0x1C)
#define SP_PC_REG               0x04080000
#define SP_DMA_DMEM             0x0000
#define SP_DMA_IMEM             0x1000 

#define SP_CLR_HALT             0x00001     /* Bit  0: clear halt */
#define SP_SET_HALT             0x00002     /* Bit  1: set halt */
#define SP_CLR_BROKE            0x00004     /* Bit  2: clear broke */
#define SP_CLR_INTR             0x00008     /* Bit  3: clear intr */
#define SP_SET_INTR             0x00010     /* Bit  4: set intr */
#define SP_CLR_SSTEP            0x00020     /* Bit  5: clear sstep */
#define SP_SET_SSTEP            0x00040     /* Bit  6: set sstep */
#define SP_CLR_INTR_BREAK       0x00080     /* Bit  7: clear intr on break */
#define SP_SET_INTR_BREAK       0x00100     /* Bit  8: set intr on break */
#define SP_CLR_SIG0             0x00200     /* Bit  9: clear signal 0 */
#define SP_SET_SIG0             0x00400     /* Bit 10: set signal 0 */
#define SP_CLR_SIG1             0x00800     /* Bit 11: clear signal 1 */
#define SP_SET_SIG1             0x01000     /* Bit 12: set signal 1 */
#define SP_CLR_SIG2             0x02000     /* Bit 13: clear signal 2 */
#define SP_SET_SIG2             0x04000     /* Bit 14: set signal 2 */
#define SP_CLR_SIG3             0x08000     /* Bit 15: clear signal 3 */
#define SP_SET_SIG3             0x10000     /* Bit 16: set signal 3 */
#define SP_CLR_SIG4             0x20000     /* Bit 17: clear signal 4 */
#define SP_SET_SIG4             0x40000     /* Bit 18: set signal 4 */
#define SP_CLR_SIG5             0x80000     /* Bit 19: clear signal 5 */
#define SP_SET_SIG5            0x100000     /* Bit 20: set signal 5 */
#define SP_CLR_SIG6            0x200000     /* Bit 21: clear signal 6 */
#define SP_SET_SIG6            0x400000     /* Bit 22: set signal 6 */
#define SP_CLR_SIG7            0x800000     /* Bit 23: clear signal 7 */
#define SP_SET_SIG7           0x1000000     /* Bit 24: set signal 7 */

#define SP_STATUS_HALT          0x001
#define SP_STATUS_BROKE         0x002
#define SP_STATUS_DMA_BUSY      0x004
#define SP_STATUS_DMA_FULL      0x008
#define SP_STATUS_IO_FULL       0x010         
#define SP_STATUS_SSTEP         0x020        
#define SP_STATUS_INTR_BREAK    0x040          
#define SP_STATUS_SIG0          0x080           
#define SP_STATUS_SIG1          0x100         
#define SP_STATUS_SIG2          0x200         
#define SP_STATUS_SIG3          0x400         
#define SP_STATUS_SIG4          0x800      
#define SP_STATUS_SIG5         0x1000           
#define SP_STATUS_SIG6         0x2000          
#define SP_STATUS_SIG7         0x4000        

#define AI_DRAM_ADDR_REG        (AI_BASE_REG+0x00)
#define AI_LEN_REG              (AI_BASE_REG+0x04)
#define AI_CONTROL_REG          (AI_BASE_REG+0x08)
#define AI_STATUS_REG           (AI_BASE_REG+0x0C)
#define AI_DACRATE_REG          (AI_BASE_REG+0x10)
#define AI_BITRATE_REG          (AI_BASE_REG+0x14)
#define AI_CONTROL_DMA_ON       0x01
#define AI_CONTROL_DMA_OFF      0x00
#define AI_STATUS_FIFO_FULL     0x80000000
#define AI_STATUS_DMA_BUSY      0x40000000
#define AI_MAX_DAC_RATE         16384
#define AI_MIN_DAC_RATE         132
#define AI_MAX_BIT_RATE         16 
#define AI_MIN_BIT_RATE         2
#define AI_NTSC_MAX_FREQ        368000
#define AI_NTSC_MIN_FREQ        3000
#define AI_PAL_MAX_FREQ         376000
#define AI_PAL_MIN_FREQ         3050
#define AI_MPAL_MAX_FREQ        368000
#define AI_MPAL_MIN_FREQ        3000

#define VI_STATUS_REG		(VI_BASE_REG)
#define VI_CONTROL_REG		VI_STATUS_REG
#define VI_ORIGIN_REG		(VI_BASE_REG+0x04)
#define VI_DRAM_ADDR_REG	VI_ORIGIN_REG
#define VI_WIDTH_REG		(VI_BASE_REG+0x08)
#define VI_H_WIDTH_REG		VI_WIDTH_REG
#define VI_INTR_REG		(VI_BASE_REG+0x0C)
#define VI_V_INTR_REG		VI_INTR_REG
#define VI_CURRENT_REG		(VI_BASE_REG+0x10)
#define VI_V_CURRENT_LINE_REG	VI_CURRENT_REG
#define VI_BURST_REG		(VI_BASE_REG+0x14)
#define VI_TIMING_REG		VI_BURST_REG
#define VI_V_SYNC_REG		(VI_BASE_REG+0x18)
#define VI_H_SYNC_REG		(VI_BASE_REG+0x1C)
#define VI_LEAP_REG		(VI_BASE_REG+0x20)
#define VI_H_SYNC_LEAP_REG	VI_LEAP_REG
#define VI_H_START_REG		(VI_BASE_REG+0x24)
#define VI_H_VIDEO_REG		VI_H_START_REG
#define VI_V_START_REG		(VI_BASE_REG+0x28)
#define VI_V_VIDEO_REG		VI_V_START_REG
#define VI_V_BURST_REG		(VI_BASE_REG+0x2C)
#define VI_X_SCALE_REG		(VI_BASE_REG+0x30)
#define VI_Y_SCALE_REG		(VI_BASE_REG+0x34)
#endif
