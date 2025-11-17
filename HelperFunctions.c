#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include "CacheFunctions.h"
#include "HelperFunctions.h"
int IndexBits = 15;
int ByteSel = 6;

extern struct cacheline SimCache[INDEX][WAY];

char* Print_String (int num, int type)
{
switch (type)
{
	case 0: switch(num)
	{
		case M: 	return (char*) "Modified";

		case E: 	return (char*) "Exclusive";

		case S: 	return (char*) "Shared";

		case I: 	return (char*) "Invalid";
		
		case O: 	return (char*) "Owned";

		default:	return (char*) "Error";
	}

	break;

	case 1: switch(num)
	{
		case READ:      return (char*) "Read";

		case WRITE:     return (char*) "Write";

		case INVALIDATE: return (char*) "Invalidate";

		case RWIM:      return (char*) "RWIM";

		case NOBUSOP:   return (char*) "No Bus OP";

		default:	return (char*) "Error";
	}

	break;

	case 2: switch(num)
	{
		case NOHIT: 	return (char*) "No Hit";

		case HIT:   	return (char*) "Hit";

		case HITM:  	return (char*) "Hit to a Modified Line";

		default:    	return (char*) "Error";
	}

	break;

	case 3: switch(num)
	{
		case PROCREAD: 	return (char*) "Processor Data Read";

		case PROCWRITE: return (char*) "Processor Write";

		case PROCINSTRREAD: return (char*) "Processor Instruction Read";

		case SNOOPINV: 	return (char*) "Snooped Invalidate";

		case SNOOPREAD: return (char*) "Snooped Read";

		case SNOOPWRITE: return (char*) "Snooped Write";

		case SNOOPRWIM: return (char*) "Snooped RWIM";

		case RESET: 	return (char*) "Reset";

		case PRINTALL: 	return (char*) "Print All";

		default:		return (char*) "Error";
	}
	break;

	default: 		return "Error";

	}

	return 0;
}




void Print_States (int index, int way, int event, int GetSnoop, int BusOp, int SnoopResponse)
{
    
    if(event == PROCINSTRREAD || event == PROCREAD || event == PROCWRITE)
    {
        if(BusOp != NOBUSOP)
        {
            fprintf(outfp,"BUS OP: %s\n", Print_String(BusOp,1));
            
        }
	fprintf(outfp,"Received Snoop Response: %s\n", Print_String(GetSnoop,2));
        fprintf(outfp, "Modified State: %s\n", Print_String(SimCache[index][way].PROT,0));
    }
    else if(event == SNOOPINV || event == SNOOPREAD || event == SNOOPWRITE || event == SNOOPRWIM)
    {
        fprintf(outfp,"Put Snoop Result: %s\n", Print_String(SnoopResponse, 2));
        if(BusOp != NOBUSOP)
            fprintf(outfp,"BUS OP: %s\n", Print_String(BusOp, 1));
        fprintf(outfp,"Modified State: %s\n", Print_String(SimCache[index][way].PROT,0));
    }
  
}




void Clear_Cache()
{
  
    int index;
    int way;
    fprintf(outfp, "\n**********Clearing Cache**********\n");
    for (index = 0; index < INDEX; index++)
    {
        for (way = 0; way < WAY; way++)
        {
             SimCache[index][way].PROT = I;
             SimCache[index][way].LRU = WAY -1;
        }
    }
}



int Check_TagBits(int index, int tag, int *hit)
{

  int way;
  for (way = 0; way < WAY; way++)
  {
    if ((SimCache[index][way].TAG == tag) && (SimCache[index][way].PROT != I))
    {
        fprintf(outfp, "\n*********HIT*********\n");
        (*hit)++;
        return way;
    }
  }
  return (WAY+1);//WAY +1 indicates an error
}



void Cache_Display()
{
   fprintf(outfp, "\n********Display Cache Lines******\n");
   int index;
   int way;
   int flag =0;
   for (index = 0; index < INDEX; index++)
   {
       for (way = 0; way < WAY; way++)
      {
        if ((SimCache[index][way].PROT != I) && flag == 0)
        {
            fprintf(outfp,"INDEX: %d\n",index);
            flag = 1;
        }
        if(SimCache[index][way].PROT != I)
        {
           fprintf(outfp,"WAY: %d \t LRU: %d \t PROT: %s \t TAG: %x\n", \
                way, SimCache[index][way].LRU, Print_String(SimCache[index][way].PROT,0), SimCache[index][way].TAG);
            
        }
      }
        flag = 0;
    }
}



int Index_Mask (unsigned int address)
{
    int i;
    int mask = 1;
    int index;
    address = address >> ByteSel;
    for(i=0;i<IndexBits;i++)
    {
        mask = mask *2;
    }
    index = address & (mask-1);
    return index;
}



int Get_Snoop_Result (unsigned int address)
{
    int snoopbits = (address & SNOOPMSK);
    int snoopresponse;
    switch (snoopbits)
    {
        case CASE0 :    snoopresponse = NOHIT;
                        break;
        case CASE1 :    snoopresponse = HIT;
                        break;
        case CASE2 :    snoopresponse = HITM;
                        break;
        default:        snoopresponse = NOHIT;
                        break;
    }
    return snoopresponse;
}



void Update_LRU(int index, int way)
{
     
      if (SimCache[index][way].LRU == 0)
          return;
      else
      {
          int ourbits = SimCache[index][way].LRU;
          int testway;

          for(testway = 0; testway <= WAY - 1; testway++)
          {
          	if(SimCache[index][testway].LRU < ourbits )
          		SimCache[index][testway].LRU++;
          }
        }

          SimCache[index][way].LRU = 0;
}



int Get_LRU(int index, int*BusOp)
{
    int way;
	for(way = 0; way <= WAY - 1 ; way++)
	{
		if(SimCache[index][way].PROT == I)
			return(way);
	}
	for(way = 0; way <= WAY - 1 ; way++)
	{
		if(SimCache[index][way].LRU == WAY - 1)
		{
		    if(SimCache[index][way].PROT == M)
		        *BusOp = WRITE;
		    return(way);
		}

	}
}



void Message(int address, int status)
{
    
	if(status)
		fprintf(outfp, "Message to Higher Level Cache: Invalidate Address = %x \n", address);
    
}



void Print_Stat(int read, int write, int hit, int miss)
{
    float hit_ratio = ((float)(hit))/((float)(hit+miss));
    fprintf(outfp, "\n********Printing Cache Statistics********");
    fprintf(outfp, "\nNo. of Cache Reads: %d \nNo. of Cache Writes: %d \nNo. of hits: %d \nNo. of Misses: %d \nHit Ratio: %f \n"\
            ,read,write,hit,miss,hit_ratio);
    printf("\nNo. of Cache Reads: %d \nNo. of Cache Writes: %d \nNo. of hits: %d \nNo. of Misses: %d \nHit Ratio: %f\n"\
            ,read,write,hit,miss,hit_ratio);
    
}
