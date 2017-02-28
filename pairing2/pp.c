#include<unistd.h>
#include<time.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "mpi.h"

int main(int argc, char **argv) {
    int procid, numprocs;
    
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    time_t t;
    int partners[numprocs], partner = -1, variables = numprocs - 1;

    srand((unsigned)time(NULL)+procid*numprocs);

    if (procid == 0) {
        
        for(int i = 0; i<numprocs; i++)
            partners[i]=0;

        int randomchoice = (rand() % (numprocs - 1) + 1);
        printf("Teacher says: Student %d start!\n",randomchoice);

        MPI_Send(partners, numprocs, MPI_INT, randomchoice, 0,MPI_COMM_WORLD);
        MPI_Recv(partners, numprocs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  
    }
    else {
        MPI_Recv(partners, numprocs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        printf("I got it %d\n",procid);
        int remaining = 0;
        for(int i = 1; i < numprocs; i++){
            if(partners[i] == 0)
                remaining += 1;
            else if(partners[i] == procid)
                partners[procid] = partners[a];
        }

        if(remaining == 0){
            MPI_Send(partners, numprocs, MPI_INT, 0, 0,MPI_COMM_WORLD);
        }
        else if(remaining == 1){
            partners[procid] = procid;
            MPI_Send(partners, numprocs, MPI_INT, 0, 0,MPI_COMM_WORLD);
        }
        else{
            int preference[remaining], ncount;
            for(int i = 1; i < numprocs; i++){
                if(partners[i] == 0){
                    preference[ncount++] = partners[i];
                }
            }
            int partner = rand() % remaining, sendto = partner;
            partners[procid] = newpreference[partner];
            while(sendto == partner){
                sendto = rand() % remaining;
            }
            sendto = newpreference[sendto];
            MPI_Send(partners, numprocs, MPI_INT, sendto, 0,MPI_COMM_WORLD);
        }     
    }

    MPI_Finalize();
}