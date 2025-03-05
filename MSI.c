#include <stdio.h>
#include <stdlib.h>
#include "CacheFunctions.h"
#include "MSI.h"
struct cacheline SimCache[INDEX][WAY];

void MSI (int event, int index, int way, unsigned int address, int *Snoop_Response, int *Bus_Operation, int* Get_Snoop, int* status)
{
    switch(SimCache[index][way].PROT)
    {

        case S :    if((event == PROCREAD) || (event == PROCINSTRREAD))
                    {
                        SimCache[index][way].PROT = S;
                        *Bus_Operation = NOBUSOP;
			*status =0;
			*Get_Snoop = Get_Snoop_Result(address);
                    }

                     else if(event == PROCWRITE)
                     {
                        *Bus_Operation = INVALIDATE;
                        SimCache[index][way].PROT = M;
			*status =0;
			*Get_Snoop = Get_Snoop_Result(address);
                     }
                     else if(event == SNOOPREAD)
                     {
                        *Snoop_Response = HIT;
                        SimCache[index][way].PROT = S;
                        *Bus_Operation = NOBUSOP;
			*status =0;
                     }
                     else if((event == SNOOPINV) || (event == SNOOPRWIM))
                     {
                         SimCache[index][way].PROT = I;
			 *Snoop_Response = NOHIT;
			 *status =1;
			 *Bus_Operation = NOBUSOP;
                     }

                     else
                    {
			*status =0;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                
                    break;

        case I :    if((event == PROCREAD) || (event == PROCINSTRREAD))
                     {
                        *Bus_Operation = READ;
                        *Get_Snoop = Get_Snoop_Result(address);
						SimCache[index][way].PROT = S;
			*status =0;
                     }
                    else if(event == PROCWRITE)
                     {
                        *Bus_Operation = RWIM;
                        SimCache[index][way].PROT = M;
			*Get_Snoop = Get_Snoop_Result(address);
			*status =0;
                     }
                    else if((event == SNOOPREAD )|| (event == SNOOPINV )|| (event == SNOOPWRITE )|| (event == SNOOPRWIM))
                     {
                        *Snoop_Response = NOHIT;
                        SimCache[index][way].PROT = I;
                        *Bus_Operation = NOBUSOP;
			*status =0;
			*Bus_Operation = NOBUSOP;
                     }
                    else
                    {
			*status =0;
			*Bus_Operation = NOBUSOP;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                    
                    break;

 	case M :    if((event == PROCREAD) || (event == PROCWRITE) ||  (event == PROCINSTRREAD))
                    {
                        SimCache[index][way].PROT = M;
                        *Bus_Operation = NOBUSOP;
			*status =0;
			*Get_Snoop = Get_Snoop_Result(address);
                    }

                    else if((event == SNOOPREAD))
                    {
                        *Snoop_Response = HITM;
                        *Bus_Operation = WRITE;
                        SimCache[index][way].PROT = S;
			*status =0;
                    }
                    else if((event == SNOOPRWIM))
                    {
                        *Snoop_Response = HITM;
                        *Bus_Operation = WRITE;
                        SimCache[index][way].PROT = I;
			*status =1;
                    }
                    else
		    {
			*status =0;
			*Bus_Operation = NOBUSOP;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                        
                    break;

        default:    fprintf(outfp,"Error State \n");
		    *status =0;
		    *Bus_Operation = NOBUSOP;
                  
                    break;

    }

}
