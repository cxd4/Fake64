#ifdef OSTASK_H
#warning ostask got included twice
#endif

#define OSTASK_H

typedef struct {
        uint32     type;
        uint32     flags;
        
        uint32     *ucode_boot;
        uint32     ucode_boot_size;
        
        uint64     *ucode;
        uint32     ucode_size;
        
        uint64     *ucode_data;
        uint32     ucode_data_size;
        
        uint64     *dram_stack;
        uint32     dram_stack_size;

        uint64     *output_buff;
        uint64     *output_buff_size;

        uint64     *data_ptr;
        uint32     data_size;

        uint64     *yield_data_ptr;
        uint32     yield_data_size;

} OSTask;
