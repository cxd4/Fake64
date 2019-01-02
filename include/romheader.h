#ifndef ROMHEADER_H
#define ROMHEADER_H

struct rom {
    uint8 PI_BSB_DOM1_LAT_REG;  /* 0x80 */
    uint8 PI_BSB_DOM1_PGS_REG;  /* 0x37 */
    uint8 PI_BSB_DOM1_PWD_REG;  /* 0x12 */
    uint8 PI_BSB_DOM1_PGS_REG2; /* 0x40 */

    int32 clockrate;
    uint32 PC;
    int32 release;

    uint32 CRC1,CRC2;
    int32 unknown1, unknown2;

    char name[20 + sizeof("")];
    int32 unknown3;
    char manufacturer;
    int16 /* wrong...uint8 */ country_code;
    uint16 cartridge_id;

    uint8 *image;
    uint8 *header;
    uint8 *bootcode;
    uint8 *progcode;
    int length;
};

#endif
