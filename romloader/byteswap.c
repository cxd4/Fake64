#include <general.h>

void byteswap(int Size, uint8* Image) {
    int  k;
    uint8 swap[4]; 

    if (Image[0] == 0x80 && Image[1] == 0x37)
        for (k=Size; k >= 0; k-=4) {
            //Converts 40123780 to 0x80371240
            // 0x80 0x37 0x12 0x40 to 0x40 0x12 0x37 0x80
	   swap[0]=Image[k];
	   swap[1]=Image[k+1];
	   swap[2]=Image[k+2];
           // Doesn't like being watched
	   swap[3]=Image[k+3];
	   Image[k]=swap[3];
	   Image[k+1]=swap[2];
	   Image[k+2]=swap[1];
	   Image[k+3]=swap[0];
        }
    else if (Image[0] == 0x37 && Image[1] == 0x80)
       for(k=Size;k>=0;k-=4) {
         //Converts 0x12408037 to 0x80371240
         // 0x37 0x80 0x40 0x12 to 0x40 0x12 0x37 0x80
         // <opcode> <src> <dest>
           swap[0]=Image[k];
           swap[1]=Image[k+1];
           swap[2]=Image[k+2];
           swap[3]=Image[k+3];	 

	   Image[k]=swap[2]; // godddd this needs checking
	   Image[k+1]=swap[3];
	   Image[k+2]=swap[0]; 
	   Image[k+3]=swap[1];
        }
}

/*
void byteswap_code(int Size, uint8* Image)
{
    int  k;
    uint8 swap[4];

    Image+=0x1000;
        for (k=Size-0x1000; k >= 0; k-=4) {
            //Converts 40123780 to 0x80371240
            // 0x80 0x37 0x12 0x40 to 0x40 0x12 0x37 0x80
           swap[0]=Image[k];
           swap[1]=Image[k+1];
           swap[2]=Image[k+2];
           swap[3]=Image[k+3];
           Image[k]=swap[3];
           Image[k+1]=swap[2];
           Image[k+2]=swap[1];
           Image[k+3]=swap[0];
        }
}
*/
