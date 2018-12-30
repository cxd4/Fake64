#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <general.h>
#include <module.h>
#include "config_options.h"

int fps_counter_on = 0;
int frames_rendered = 0;
int started_at;

int limiter_active = 0;
int limiter_udelay = 0;

void exit_print_fps();

void frame_done() {
	int tdiff;
	float ffps;
	float upf, iupf; // usecs/frame ideal usecs/frame
	float uchange;

	frames_rendered++;

	if (limiter_active) {
		usleep(limiter_udelay);
	} 

	if (fps_limiter) {
		tdiff = time(NULL)-started_at;
		ffps = ((float)frames_rendered/(float)(tdiff));
		upf = (float)1000000 / ffps; // usecs a frame
		iupf = (float)1000000 / (float)fps_limit; // ideal
		uchange = (iupf - upf);
		limiter_udelay += (int)uchange;
		if (limiter_udelay > 0) {
			limiter_active = 1;
		} else {
			limiter_active = 0;
			limiter_udelay = 0;
		}
		printd(D_VIDEO, D_DEBUG, "fps: %f\n", ffps);
		printd(D_VIDEO, D_DEBUG, "delay set to %i\n", limiter_udelay);
	} 
}

void init_fps_counter() {
	fps_counter_on = 1;
	frames_rendered = 0;
	started_at = time(NULL);
}


void exit_print_fps() {
	int now;

	now = time(NULL);
	now -= started_at;

	printf("%i frames in %i seconds: %f fps\n", frames_rendered, now, (float)((float)frames_rendered / (float)now));
}


