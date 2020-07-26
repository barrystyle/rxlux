#include "rxlux.h"

int main()
{
	rxlux hashinst;

        char blkheader[144]={48};
	char seedhash[32]={24};

	hashinst.rxinitialize(seedhash);

        for (int z=0; z<255; z++) {
           blkheader[143] = z;
           hashinst.setheader(blkheader);
           hashinst.rx_slow_hash();
           char *result = hashinst.getresult();

           for (int i=0; i<32; i++)
              printf("%02hhx", result[i]);
           printf("\n");
        }

	return(0);
}


