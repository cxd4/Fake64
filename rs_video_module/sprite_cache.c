#include <stdlib.h>
#include <general.h>
#include "config_options.h"
#include "rcp_state.h"
#include "SDL.h"

uint32* spriteind[0x10000];

SDL_PixelFormat glfb;

uint32* get_new_sprite(struct sprite_cfg*);

uint32* scale_sprite(uint32* data, struct sprite_cfg *sprite, SDL_PixelFormat *fmt, float scalex, float scaley) {

	int size;
	uint32* scaled;
	int x, y;
	int maxx, maxy, mapx, mapy;
	float recipx, recipy;
	int current, scurrent;

	maxx = (int)((float)sprite->sprite_w * (float)scalex);
	maxy = (int)((float)sprite->sprite_h * (float)scaley);

	recipx = 1/scalex;
	recipy = 1/scaley;

	size = (fmt->BytesPerPixel * maxx * maxy);

	scaled = (uint32*)malloc(size);

	if (fmt->BytesPerPixel != 4) {
		printf("OH MY GOD NO! THIS ISN'T A 32 bit SPRITE!\n");
	} else {
		for (y = 0; y < maxy; y++) {
			for (x = 0; x < maxx; x++) {
				mapx = (int)((float)x * (float)recipx);
				mapy = (int)((float)(maxy - y) * (float)recipy);
				current = (mapy * sprite->sprite_w) + mapx;
				scurrent = (y * maxx) + x;
	//			printf("%i,%i -> %i,%i", mapx, mapy, x, y);
	//			printf(" = %x(%i)\n", data[current], current);
				scaled[scurrent] = data[current];
			}
		}
	}

	return scaled;
}

void init_sprite_cache() {

	int red, green, blue, alpha, size;

	memset(spriteind, 0x00, (4 * 0x10000));

	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &size);
	SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &red);
	SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &green);
	SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &blue);
	SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &alpha);

	printf("%i %i %i %i %i\n", size, red, green, blue, alpha);

	// Make that do stuff one day
	// For now we're going for 8 8 8 8

	glfb.palette = NULL;
	glfb.BitsPerPixel = 32;
	glfb.BytesPerPixel = 4;
	glfb.Rmask = 0x000000ff;
	glfb.Gmask = 0x0000ff00;
	glfb.Bmask = 0x00ff0000;
	glfb.Amask = 0xff000000;
	glfb.Rshift = 0;
	glfb.Gshift = 8;
	glfb.Bshift = 16;
	glfb.Ashift = 24;
	glfb.Rloss = 0;
	glfb.Gloss = 0;
	glfb.Bloss = 0;
	glfb.Aloss = 0;
	glfb.colorkey = 0;
	glfb.alpha = 255;

}

uint16 calc_crc(uint32 addr, uint16* src, int length) {

	uint16 crc = 0;
	int i;

	crc += ((addr & 0xffff0000) >> 16);
	crc += (addr & 0x0000ffff);

	for (i = 0; i < length; i++) {
		crc += src[i];
	}

	return crc;
}

uint32* get_sprite(struct sprite_cfg *sprite) {

	uint16 crc;

	crc = calc_crc(sprite->src, sprite->r_src, spritecrclen);

	if(spriteind[crc]) {
		return spriteind[crc];
	} else {
		spriteind[crc] = get_new_sprite(sprite);
		return spriteind[crc];
	}
}

uint32* get_new_sprite(struct sprite_cfg *sprite) {

	int size, chsize, dchsize;
	SDL_PixelFormat sprfmt;
	Uint8 r, g, b, a;
	uint32 *dchunk, *scaled;
	int i;
	uint32 pixel = 0;

	switch(sprite->src_bits) {
		case 2:
			size = 2;
			sprfmt.palette = NULL;
			sprfmt.Rmask = 0xf800;
			sprfmt.Gmask = 0x07c0;
			sprfmt.Bmask = 0x003e;
			sprfmt.Amask = 0x0001;
			sprfmt.Rshift = 11;
			sprfmt.Gshift = 6;
			sprfmt.Bshift = 1;
			sprfmt.Ashift = 0;
			sprfmt.Rloss = 3;
			sprfmt.Gloss = 3;
			sprfmt.Bloss = 3;
			sprfmt.Aloss = 7;
			sprfmt.BitsPerPixel = 16;
			sprfmt.BytesPerPixel = 2;
			sprfmt.colorkey = 0;
			sprfmt.alpha = 255;
			break;
		default:
			printf("Evil sprite size which is evil\n");
			exit(0);
			break;
	}

	chsize = sprfmt.BytesPerPixel * sprite->sprite_w * sprite->sprite_h;
	dchsize = glfb.BytesPerPixel * sprite->sprite_w * sprite->sprite_h;

	printf("%i bytes allocated for sprite\n", dchsize);

	dchunk = (uint32*)malloc(dchsize);

	for (i = 0; i < (sprite->sprite_w * sprite->sprite_h); i++) {
		switch(sprfmt.BytesPerPixel) {
			case 2:
				pixel = ((uint16*)sprite->r_src)[i^0x1];
				break;
			default:
				printf("Add support for this ...\n");
				break;
		}

		SDL_GetRGBA(pixel, &sprfmt, &r, &g, &b, &a);
		if (a) {
			a = (uint8)(((int32)a * (int32)2) - 1);
		}
		pixel = SDL_MapRGBA(&glfb, r, g, b, a);
//		printf("%x %x %x %x : %x\n", r, g, b, a, pixel);

		switch(glfb.BytesPerPixel) {
			case 4:
				((uint32*)dchunk)[i] = pixel;
				break;
			default:
				printf("You really should code this ...\n");
				printf(
					"Relevant?  size = %i, chsize = %i\n",
					size, chsize
				);
				break;
		}
	}

	scaled = scale_sprite(dchunk, sprite, &glfb, win_scalex, win_scaley);

	free(dchunk);

	return scaled;
}
