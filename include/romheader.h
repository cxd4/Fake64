#ifndef ROMHEADER_H
#define ROMHEADER_H
struct rom
{
 uint8 PI_BSB_DOM1_PGS_REG2; // 0x80
 uint8 PI_BSB_DOM1_PWD_REG; // 0x37
 uint8 PI_BSB_DOM1_PGS_REG; // 0x12
 uint8 PI_BSB_DOM1_LAT_REG; // 0x40
 int clockrate;
 int PC;
 int release;
 int CRC1,CRC2;
 int unknown1,unknown2;
 char name[20];
 int unknown3;
 int manufacturer;
 short int country_code;
 short int cardridge_id;
 uint8 *image;
 uint8 *header;
 uint8 *bootcode;
 uint8 *progcode;
 int length;
};
#endif
