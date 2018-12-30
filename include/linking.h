#ifndef LINKING_H
#define LINKING_H
// this is to define whether we use lazy linking or not


#ifdef NORMAL_LINKING

	#define vi_display			modules->video->vi_display_f
	#define vi_intr_reg_write	modules->video->vi_intr_reg_write_f
	#define vi_width_reg_write	modules->video->vi_width_reg_write_f
	#define vi_origin_reg_write	modules->video->vi_origin_reg_write_f
	#define vi_status_reg_write	modules->video->vi_status_reg_write_f
	#define vi_deinit			modules->video->vi_deinit_f

	#define dump_rcp_command		modules->video->dump_rcp_command_f

	#define ai_dma_request		modules->audio->ai_dma_request_f
	#define	ai_getlength		modules->audio->ai_getlength_f


	#define pifram_interrupt		modules->input->pifram_interrupt_f
	#define init_pifram				modules->input->init_pifram_f

#endif
#endif
