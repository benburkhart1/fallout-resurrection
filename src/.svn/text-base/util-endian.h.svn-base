#ifndef _SWAP_ENDIAN
#define _SWAP_ENDIAN

// Macs and SGIs are Big-Endian; PCs are little endian
// returns TRUE if current machine is little endian
extern int IsLittleEndian(void);

/******************************************************************************
  FUNCTION: swap_endian
  PURPOSE: Swap the byte order of a structure
  EXAMPLE: float F=123.456;; SWAP_FLOAT(F);
******************************************************************************/

#define SWAP_SHORT(Var)  Var = *(short*)         swap_endian((void*)&Var, sizeof(short))
#define SWAP_USHORT(Var) Var = *(unsigned short*)swap_endian((void*)&Var, sizeof(short))
#define SWAP_LONG(Var)   Var = *(long*)          swap_endian((void*)&Var, sizeof(long))
#define SWAP_ULONG(Var)  Var = *(unsigned long*) swap_endian((void*)&Var, sizeof(long))
#define SWAP_RGB(Var)    Var = *(int*)           swap_endian((void*)&Var, 3)
#define SWAP_UINT32(Var) Var = *(unsigned int*)  swap_endian((void*)&Var, 4)
#define SWAP_UINT16(Var) Var = *(unsigned short*)  swap_endian((void*)&Var, 2)

#define SWAP_FLOAT(Var)  Var = *(float*)         swap_endian((void*)&Var, sizeof(float))
#define SWAP_DOUBLE(Var) Var = *(double*)        swap_endian((void*)&Var, sizeof(double))

extern void *swap_endian(void* addr, const int nb);

#endif
