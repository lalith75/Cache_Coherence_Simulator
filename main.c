#include <stdio.h>
#include <stdlib.h>
#include "HelperFunctions.h"
#include "MESI.h"
#include "MOESI.h"
#include "MSI.h"
#include "CacheFunctions.h"


int SnoopResponse;
int BusOp;//Stores the Bus operations of the current trace execution 
int GetSnoop;//Stores snoop result of other processors in the system
int rd = 0; //counts reads
int wr =0; //counts  writes
int miss =0; //counts misses
int hit =0; //counts hits
FILE *outfp; 
int status =0; //Status flag 
FILE *ifp;
struct cacheline SimCache[INDEX][WAY]; //structure representing the cache

char *input;
char *output;


int main(int argc, char * argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <input_trace> <output_file>\n", argv[0]);
		return 1;
	}

	input = argv[1];
	output = argv[2];

	ifp = fopen(input, "r"); //Open Input File
	if (ifp == NULL) {
		fprintf(stderr, "Error: cannot open input file '%s'\n", input);
		return 1;
	}
	outfp = fopen(output, "w");//Open output file in 'Write' mode
	if (outfp == NULL) {
		fprintf(stderr, "Error: cannot open output file '%s'\n", output);
		fclose(ifp);
		return 1;
	}
	
	int k;
	int i=0;
	int event,address;

	Clear_Cache();
	
	
	while ((fscanf(ifp, "%d %x\n", &event, &address)!= EOF))
	{
		//Detect and report invalid event
		if((event == 7) || (event > 9))
		{
			fprintf(outfp, "Invalid Event\n");
			continue;
		}
		//Detect reads
		else if(event == PROCREAD || event == PROCINSTRREAD)
			rd++;
		//Detect writes
		else if(event == PROCWRITE)
			wr++;
		//Detect reset event
		else if(event == RESET)
		{
			Clear_Cache();
			continue;
		}
		//Detect cache display event 
		else if(event == PRINTALL)
		{
			Cache_Display();
			continue;
		}

		//Calculate Cache Properties
		int index = +Index_Mask(address);
		int tagshift = ByteSel + IndexBits;
		int tag = address >> tagshift;
		int way = Check_TagBits(index, tag, &hit);

		//Check if Cache Access is a Miss
		if(way == (WAY +1))
		{
			fprintf(outfp, "\n********MISS********\n");
			miss++;
			way = Get_LRU(index, &BusOp);
			SimCache[index][way].PROT = I;
		}
		
		
		SimCache[index][way].TAG = tag;



		//Print Address
		fprintf(outfp, "ADDRESS = %x\n", address);
		//Print Event
		fprintf(outfp, "EVENT = %d \t %s\n", event, Print_String(event, 3));
		//Print Way and Index
		fprintf (outfp, "The WAY is %d and the INDEX is %d\n", way, index);
		//Print Current Iteration Number
		fprintf(outfp, "Iteration : %d \n",i);
		//printf("Iteration : %d \n",i);
		//Print Current State
		fprintf(outfp,"Current State: %s\n", Print_String(SimCache[index][way].PROT,0));

		#ifdef MSI_PROT
		MSI(event, index, way, address, &SnoopResponse, &BusOp, &GetSnoop, &status);
		#endif
		
		#ifdef MESI_PROT
		MESI(event, index, way, address, &SnoopResponse, &BusOp, &GetSnoop, &status);
		#endif
		
		#ifdef MOESI_PROT
		MOESI(event, index, way, address, &SnoopResponse, &BusOp, &GetSnoop, &status);
		#endif
		
		//Print result of each trace request
		Print_States(BusOp, SnoopResponse, GetSnoop, event, index, way );
		
		//Send message to higher level cache
		if(BusOp != NOBUSOP)
			Message(address, status);
		
		/***Print LRU Status Before and after exectution****/
		fprintf(outfp, "LRU of current Index before updating:\t");
		for(k=0;k<WAY;k++)
			fprintf(outfp, "%d \t", SimCache[index][k].LRU);
		Update_LRU(index,way);
		fprintf(outfp, "LRU of current Index after updating:\t");
		for(k=0;k<WAY;k++)
			fprintf(outfp, "%d \t", SimCache[index][k].LRU);
		fprintf(outfp,"\n");

		i++;//Update Iteration number
		BusOp = NOBUSOP;//Reset Bus operation to No Bus Operation

	}
	
	Print_Stat(rd,wr,hit,miss);
	
	fclose(ifp);//Close input file
	fclose(outfp);//Close output file

}
