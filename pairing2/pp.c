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


    if (procid == 0) {
        
        for(int i = 0; i < numprocs; i++)
            partners[i] = 0;

        int randomchoice = (rand() % (numprocs - 1) + 1);
        printf("Teacher says: Student %d start!\n",randomchoice);

        MPI_Send(partners, numprocs, MPI_INT, randomchoice, 0,MPI_COMM_WORLD);
        MPI_Recv(partners, numprocs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("YO\n");
        for(int i = 0; i < numprocs; i++){
            printf("partners[%d] %d\n",i, partners[i]);
        }
        printf("\n");
        
    }
    else {
        MPI_Recv(partners, numprocs, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        printf("I got it %d\n",procid);
        int remaining = 0;
        int myself = 0;

        //Check how many remains
        //with yourself
        //if you are already set, set yourself to partner
        //if remaining == 1 -> set yourself to yourself
        
        //2 1 0 5 4 0
        printf("partners: ");
        for(int i = 1; i < numprocs; i++){
            if(procid == i){
                printf("[%d]:%d, ",i,partners[i]);
                if(partners[i] == 0)
                    myself += 1;
                continue;
            }
            else if(partners[i] == procid){
                partners[procid] = i;
            }
            else if(partners[i] == 0)
                remaining += 1;
            printf("[%d]:%d, ",i,partners[i]);
        }
        printf("\n");

        if(remaining == 0){
            if(myself == 1)
                partners[procid] = procid;
            printf("I AM THE LAST \n");
            MPI_Send(partners, numprocs, MPI_INT, 0, 0,MPI_COMM_WORLD);
        }
        else{
            int preference[remaining], ncount = 0;

            for(int i = 1; i < numprocs; i++){
                if(i == procid){
                    continue;
                }
                else if(partners[i] == 0){
                    preference[ncount] = i;
                    ncount += 1;
                }
            }

            for(int i = 0 ; i < remaining; i++){
                printf("preference[%d]:%d, ",i,preference[i]);
            }
            int sendto;
            printf("\nRemaining %d %d\n",remaining,ncount);
            

                if(partners[procid] == 0){
                    int partner = rand() % remaining;
                    partners[procid] = preference[partner];
                    sendto = preference[partner];
                }
                else{
                    sendto = rand() % remaining;
                    sendto = preference[sendto];
                }

                if(remaining == 1)
                sendto = preference[0];
                printf("partner %d, sendto %d\n",partners[procid],sendto);

            
            
            MPI_Send(partners, numprocs, MPI_INT, sendto, 0,MPI_COMM_WORLD);
        }     
    }

    

    MPI_Finalize();
}