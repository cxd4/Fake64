#define ROMLOADER

#include <general.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <module.h>

#define CHUNK_SIZE 1024
#define MAX_PATH_LEN 200

#define GCONF_PATH "/etc/fake64rc"

#ifdef NOT_INSTALLED
	#undef GCONF_PATH
	#define GCONF_PATH "./fake64rc"
#endif

char cpu_path[MAX_PATH_LEN];
char input_path[MAX_PATH_LEN];
char video_path[MAX_PATH_LEN];
char audio_path[MAX_PATH_LEN];


extern struct module_info  modules;

extern void *load_input_module(char*);
extern void *load_video_module(char*);
extern void *load_audio_module(char*);
extern void *load_cpu_core_module(char*);


/*void config_module(char* opt) {

	printf("Option: %s\n", opt);

}*/

void pre_parse_section(int, char*, char*, char);

void print_configurable()
{
	printf("Runtime configuration supported by: ");

	if (modules.input->config_module_f) {
		printf("input ");
	}
	if (modules.video->config_module_f) {
		printf("video ");
	}
	if (modules.audio->config_module_f) {
		printf("audio ");
	}
	if (modules.cpu->config_module_f) {
		printf("cpu ");
	}

	printf("\n");
}

void do_module_loading()
{
	modules.input = (input_module*)load_input_module(input_path);
	modules.audio = (audio_module*)load_audio_module(audio_path);
	modules.video = (video_module*)load_video_module(video_path);
	modules.cpu = (cpu_module*)load_cpu_core_module(cpu_path);
}

char* tonextline(char *ptr)
{
	ptr = strchr(ptr, '\n') + 1;
	return ptr;
}

char* seek_section(char *data, char *section) {

	char *current;
	char *found;
	int seclen;

	current = data;
	found = 0;
	seclen = strlen(section);

	while(found == 0 && current[0] != 0) {
		if (!strncasecmp(current, "Section ", 8)) {
			if (!strncasecmp(current + 8, section, seclen)) {
				found = current;
			} else {
				current = tonextline(current);
			}
		} else {
			current = tonextline(current);
		}
	}

	if (current[0] == 0) {
		return 0;
	} else {
		return current;
	}
}

void parse_debug(char *set) {

	char *next;
	debug_type type;
	debug_level level;

	next = strchr(set, ' ') + 1;

	if (!strncasecmp(set, "user ", 5)) { type = D_USER; }
	else if (!strncasecmp(set, "disass ", 7)) { type = D_DISASS; }
	else if (!strncasecmp(set, "rsp ", 4)) { type = D_RSP; }
	else if (!strncasecmp(set, "rdp ", 4)) { type = D_RDP; }
	else if (!strncasecmp(set, "rcp ", 4)) { type = D_RCP; }
	else if (!strncasecmp(set, "mi ", 3)) { type = D_MI; }
	else if (!strncasecmp(set, "vi ", 3)) { type = D_VI; }
	else if (!strncasecmp(set, "ai ", 3)) { type = D_AI; }
	else if (!strncasecmp(set, "pi ", 3)) { type = D_PI; }
	else if (!strncasecmp(set, "ri ", 3)) { type = D_RI; }
	else if (!strncasecmp(set, "si ", 3)) { type = D_SI; }
	else if (!strncasecmp(set, "video ", 6)) { type = D_VIDEO; }
	else if (!strncasecmp(set, "audio ", 6)) { type = D_AUDIO; }
	else if (!strncasecmp(set, "input ", 6)) { type = D_INPUT; }
	else if (!strncasecmp(set, "cpu ", 4)) { type = D_CPU; }

	if (!strncasecmp(next, "debug", 5)) { level = D_DEBUG; }
	else if (!strncasecmp(next, "info", 4)) { level = D_INFO; }
	else if (!strncasecmp(next, "notice", 6)) { level = D_NOTICE; }
	else if (!strncasecmp(next, "warn", 4)) { level = D_WARN; }
	else if (!strncasecmp(next, "error", 5)) { level = D_ERROR; }
	else if (!strncasecmp(next, "crit", 4)) { level = D_CRIT; }
	else if (!strncasecmp(next, "emerg", 5)) { level = D_EMERG; }
	else if (!strncasecmp(next, "fatal", 5)) { level = D_FATAL; }

	set_debug(type, level);

}

void pass_option(char *opt) {

	int optlen;
	char *optstor;

	optlen = (int)(strchr(opt, '\n') - opt);

	optstor = malloc(optlen + 1);
	memset(optstor, 0x00, optlen+1);

	if (!strncasecmp(opt, "input ", 6)) {
		strncpy(optstor, opt + 6, optlen - 6);
		if (modules.input->config_module_f) { modules.input->config_module_f(optstor); }
	} else if (!strncasecmp(opt, "video ", 6)) {
		strncpy(optstor, opt + 6, optlen - 6);
		if (modules.video->config_module_f) { modules.video->config_module_f(optstor); }
	} else if (!strncasecmp(opt, "audio ", 6)) {
		strncpy(optstor, opt + 6, optlen - 6);
		if (modules.audio->config_module_f) { modules.audio->config_module_f(optstor); }
	} else if (!strncasecmp(opt, "cpu ", 4)) {
		strncpy(optstor, opt + 4, optlen - 4);
		if (modules.cpu->config_module_f) { modules.cpu->config_module_f(optstor); }
	} else if (!strncasecmp(opt, "all ", 4)) {
		strncpy(optstor, opt + 4, optlen - 4);
		if (modules.cpu->config_module_f) { modules.cpu->config_module_f(optstor); }
		if (modules.audio->config_module_f) { modules.audio->config_module_f(optstor); }
		if (modules.video->config_module_f) { modules.video->config_module_f(optstor); }
		if (modules.input->config_module_f) { modules.input->config_module_f(optstor); }
	}

	free(optstor);
}

void store_module_conf(char* info) {

	int infolen;

	infolen = (int)(strchr(info, '\n') - info);

	if (infolen > MAX_PATH_LEN) {
		printf("PATH TOO LONG!\n");
	} else {

	if (!strncasecmp(info, "cpu ", 4)) {
		memset(cpu_path, 0x00, MAX_PATH_LEN);
		strncpy(cpu_path, info + 4, infolen - 4);
	} else if (!strncasecmp(info, "input ", 6)) {
		memset(input_path, 0x00, MAX_PATH_LEN);
		strncpy(input_path, info + 6, infolen - 6);
	} else if (!strncasecmp(info, "video ", 6)) {
		memset(video_path, 0x00, MAX_PATH_LEN);	
		strncpy(video_path, info + 6, infolen - 6);
	} else if (!strncasecmp(info, "audio ", 6)) {
		memset(audio_path, 0x00, MAX_PATH_LEN);
		strncpy(audio_path, info + 6, infolen - 6);
	}

	}

}

void parse_section(int phase, char *data, char *section) {

	char *secptr;
	int stillhere = 1;

	secptr = seek_section(data, section);

	if (!secptr) {
		printf(".fake64rc: Referenced section %s not found!\n", section);
		exit(0);
	}

	secptr = strchr(secptr, '{');

	while(strchr(secptr, '}') > (secptr = tonextline(secptr))) {
		while(secptr[0] == '\t' || secptr[0] == ' ') { secptr++; }
		if (!strncasecmp(secptr, "LoadModule ", 11)) {
			if (!phase) { store_module_conf(secptr+11); }
		} else if (!strncasecmp(secptr, "Include ", 8)) {
			pre_parse_section(phase, data, secptr+8, '\n');
		} else if (!strncasecmp(secptr, "Option ", 7)) {
			if (phase) { pass_option(secptr+7); }
		} else if (!strncasecmp(secptr, "Debug ", 6)) {
			if (phase) { parse_debug(secptr + 6); }
		}
	}
}

void pre_parse_section(int phase, char *data, char *section, char expect) {

	int len;
	char *secname;

	len = (int)(strchr(section, expect) - section);

	secname = malloc(len+1);
	memset(secname, 0, len+1);
	strncpy(secname, section, len);

	parse_section(phase, data, secname);

	free(secname);
}


char *load_file(char *data, int *dsize, FILE *conf) {

	int total, maxread, isayso, amtread;

	if (*dsize == 0) {
		data = (char *)malloc(CHUNK_SIZE);
		memset(data, 0x00, CHUNK_SIZE);
		total = 0;
	} else {
		total = strlen(data);
	}

	maxread = *dsize - total;
	isayso = 1;

	while(isayso) {
		amtread = fread((char*)(data + total), sizeof(char), maxread, conf);
		total += amtread;
		if (total == *dsize) {
			*dsize += CHUNK_SIZE;
			data = (char *)realloc(data, *dsize);
			maxread = *dsize - total;
		} else { isayso = 0; }	
	}

	return data;
}

char* seek_section_ptr(char *search, char *data) {

	char *current;
	int slen;

	current = data;

	slen = strlen(search);

	while(strncasecmp(current, search, slen) && current[0] != 0) {
		current = tonextline(current);
	}

	if (current[0] == 0) {
		return 0;
	}

	current += slen;

	return current;
}

void parse_default_section(int phase, char *data) {

	char *defptr;

	defptr = seek_section_ptr("SetDefault ", data);

	if (!defptr) {
		printf("Your .fake64rc has no default section pointer, perhaps you are using an old format .fake64rc?\n");
	}

	pre_parse_section(phase, data, defptr, '\n');

}

void parse_rom_section(int phase, char *data, char *romname) {

	char *romptr, *tempstr;
	int len;

	len = strlen(romname) + 7;
	tempstr = (char*)malloc(len+2);
	strcpy(tempstr, "SetRom ");
	strcat(tempstr, romname);
	strcat(tempstr, " ");

	romptr = seek_section_ptr(tempstr, data);
	
	if (romptr) {
		pre_parse_section(phase, data, romptr, '\n');
	}

}

void read_config(int phase, struct rom *header, char *romfile) {

	char path[200];
	FILE *conf;
	FILE *gconf;

	char *data, *romname;
	int dsize = 0;
	int files = 0;

	strcpy(path, (char*)getenv("HOME"));
	strcat(path, "/.fake64rc");

	conf = fopen(path, "r");
	gconf = fopen(GCONF_PATH, "r");

	if (!conf) {
		perror("Opening ~/.fake64rc:");
	} else {
		data = load_file(data, &dsize, conf);
		files++;
	}

	if (!gconf) {
		perror("Opening /etc/fake64rc:");
	} else {
		data = load_file(data, &dsize, gconf);
		files++;
	}

	if (files) {

		if (phase) { print_configurable(); }

		parse_default_section(phase, data);

		romname = (strrchr(romfile, '/')+1);

		if (romname ==(char*) 1) { romname = romfile; }

		parse_rom_section(phase, data, romname);

		if (!phase) { do_module_loading(); }

		free(data);
	}
}
