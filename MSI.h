#ifndef _MSI
#define _MSI

#include "HelperFunctions.h"

void MSI (int event, int index, int way, unsigned int address, int *Snoop_Response, int *Bus_Operation, int* Get_Snoop, int* status);

#endif
