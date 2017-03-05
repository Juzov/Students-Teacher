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

    srand(time(NULL) + procid);

    for(int i = 0; i < numprocs; i++)   partners[i] = 0;

    if (procid == 0) {
        int randomchoice = (rand() % (numprocs - 1) + 1);
        printf("Teacher says: Student %d start!\n",randomchoice);

        MPI_Send(partners, numprocs, MPI_INT, randomchoice, 0,MPI_COMM_WORLD);
        MPI_Recv(partners, numprocs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        for(int i = 1; i < numprocs; i++)
            printf("partners[%d] %d\n",i, partners[i]);
    }
    else {
        MPI_Recv(partners, numprocs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        int rem = 0,myself = 0, choice, partner;
        int preference[numprocs - 1];

        for(int i = 1; i < numprocs; i++){
            if(procid == i && partners[i] == 0)
                myself = 1;
            else if(partners[i] == procid) 
                partners[procid] = i;
            else if(partners[i] == 0){
                preference[rem] = i;
                rem += 1;
            }
        }
        if(rem > 0){
            choice = (rand() % rem);
            if(partners[procid] != 0)
                MPI_Send(partners, numprocs, MPI_INT, preference[choice], 0,MPI_COMM_WORLD);
            partner = preference[choice];
            partners[procid] = partner;
            partners[partner] = procid;
            MPI_Send(partners, numprocs, MPI_INT, partner, 0,MPI_COMM_WORLD);
        }
        else if(rem == 0){
            if(myself == 1) partners[procid] = procid;
            MPI_Send(partners, numprocs, MPI_INT, 0, 0,MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}