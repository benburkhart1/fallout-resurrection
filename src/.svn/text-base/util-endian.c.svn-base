#include "util-endian.h"

static long _TestEndian=1;

int IsLittleEndian(void) {
	return *(char*)&_TestEndian;
}

void *swap_endian(void* addr, const int nb) {
	static char swapped[16];
	switch (nb) {
		case 2:	swapped[0]=*((char*)addr+1);
				swapped[1]=*((char*)addr  );
				break;
		case 3:	// As far as I know, 3 is used only with RGB images
				swapped[0]=*((char*)addr+2);
				swapped[1]=*((char*)addr+1);
				swapped[2]=*((char*)addr  );
				break;
		case 4:	swapped[0]=*((char*)addr+3);
				swapped[1]=*((char*)addr+2);
				swapped[2]=*((char*)addr+1);
				swapped[3]=*((char*)addr  );
				break;
		case 8:	swapped[0]=*((char*)addr+7);
				swapped[1]=*((char*)addr+6);
				swapped[2]=*((char*)addr+5);
				swapped[3]=*((char*)addr+4);
				swapped[4]=*((char*)addr+3);
				swapped[5]=*((char*)addr+2);
				swapped[6]=*((char*)addr+1);
				swapped[7]=*((char*)addr  );
				break;
		case 16:swapped[0]=*((char*)addr+15);
				swapped[1]=*((char*)addr+14);
				swapped[2]=*((char*)addr+13);
				swapped[3]=*((char*)addr+12);
				swapped[4]=*((char*)addr+11);
				swapped[5]=*((char*)addr+10);
				swapped[6]=*((char*)addr+9);
				swapped[7]=*((char*)addr+8);
				swapped[8]=*((char*)addr+7);
				swapped[9]=*((char*)addr+6);
				swapped[10]=*((char*)addr+5);
				swapped[11]=*((char*)addr+4);
				swapped[12]=*((char*)addr+3);
				swapped[13]=*((char*)addr+2);
				swapped[14]=*((char*)addr+1);
				swapped[15]=*((char*)addr  );
				break;
	}
	return (void*)swapped;
}
