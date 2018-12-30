#include <general.h>
#include <module.h>
#include "rdpcommand.h"
#include "rcp_state.h"
#include "rcp_core.h"

#define MAX_STACK 32

rcp_call rcp_dcmd[0x100];
rcp_call rcp_ecmd[0x100];

void (*rcp_core)(OSTask*);

void basic_do_disassemble(rdp_command cmd, int depth) {

	int i;

	if (test_debug(D_RCP, D_DEBUG)) {
		printd(D_RCP, D_DEBUG, "+");
		for (i = 0; i < depth + 1; i++) {
			printd(D_RCP, D_DEBUG, "-");
		}
		printd(D_RCP, D_DEBUG, "Command: %.2x %.2x %.2x %.2x - %.2x %.2x %.2x %.2x\n", cmd.b8[3], cmd.b8[2], cmd.b8[1], cmd.b8[0], cmd.b8[7], cmd.b8[6], cmd.b8[5], cmd.b8[4]);
		printd(D_RCP, D_DEBUG, "|");
		for (i = 0; i < depth + 1; i++) {
			printd(D_RCP, D_DEBUG, " ");
		}
		if (rcp_dcmd[RDP_CMD(cmd)]) {
			(rcp_dcmd[RDP_CMD(cmd)])(cmd);
		} else {
			printd(D_RCP, D_DEBUG, "Unable to disassemble %#x\n", RDP_CMD(cmd));
		}
	}
}

void basic_rcp_core(OSTask* task) {

	rdp_command cmd;
	uint64* dlist;
	uint64* eip;
	uint64* stack[MAX_STACK];
	int stacklevel = 0;

	eip = (uint64*)(modules->cpu->translate_addr_f((uint32)task->data_ptr));

	while(1) {
		cmd.b64 = *eip;

		basic_do_disassemble(cmd, stacklevel);

		if (RDP_CMD(cmd) == 0x06) { // G_DL
			dlist = (uint64*)(modules->cpu->translate_addr_f(translate_dma_address(G_DL_S(cmd))));
			if (!G_DL_P(cmd)) {
				stack[stacklevel] = eip;
				stacklevel++;
				eip = dlist - 1;
			} else {
				eip = dlist - 1;
			}
		} else if (RDP_CMD(cmd) == 0xb8) { // G_ENDDL
			if (stacklevel) {
				stacklevel--;
				eip = stack[stacklevel];
			} else {
				return;
			}
		} else {
			if (rcp_ecmd[RDP_CMD(cmd)]) {
				rcp_ecmd[RDP_CMD(cmd)](cmd);
			} else {
				printd(D_RCP, D_DEBUG, "Unable to execute %#x\n", RDP_CMD(cmd));
				test_and_kill_rcp();
			}
		}

		eip++;
	}
}
