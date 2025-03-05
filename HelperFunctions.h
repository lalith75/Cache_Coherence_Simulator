#ifndef _HELPER_FUNCTIONS
#define _HELPER_FUNCTIONS

extern int IndexBits;
extern int ByteSel;

extern FILE *outfp;

char* Print_String (int num, int type);

void Print_States (int BusOp, int SnoopResponse, int GetSnoop, int event, int index, int way);

void Clear_Cache(void);

int Check_TagBits(int index, int tag, int *hit);

void Cache_Display(void);

int Index_Mask (unsigned int address);

int Get_Snoop_Result (unsigned int address);

void Update_LRU(int index, int way);

int Get_LRU(int index, int*BusOp);

void Message(int address, int status);

void Print_Stat(int read, int write, int hit, int miss);

#endif
