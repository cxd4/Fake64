#include <stdlib.h>
#include <errno.h>
#include "../romloader/general.h"

char * tobin(uint8 *byte, int nbytes, char *str, int ssize) 
{ 
        /* mask values: 10000000, 01000000, ..., 00000001 */  
        static int mval [] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 }; 
        int i, j, b=0;

        /* cycle through bytes */ 
        for (i=0; i < nbytes; i++) 
        { 
                /* bounds check */ 
                if ((b+8) > (ssize-1)) { 
                        errno = EFAULT;  
                        return NULL; 
                } 

                /* cycle through bits */  
                for (j=0; j < 8; j++) 
                { 
                        if (byte[i] & mval[j]) 
                                str[b++] = '1'; 
                        else 
                                str[b++] = '0'; 
                } 
        } 

        str[b] = 0; 
        return str; 
}
