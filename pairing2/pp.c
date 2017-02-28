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
    int partners[numprocs];
    int partner = -1;
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
        int preference[numprocs-2];
        int i = 0;
        printf("I got it %d\n",procid);

        while(i < numprocs - 2){
            int pref = rand() % (numprocs - 1) + 1;
            while(pref == procid)
                pref = rand() % (numprocs - 1) + 1;
            int j = i;
            while(j >= 0){
                if(pref == preference[j]){
                    pref = rand() % (numprocs - 1) + 1;
                    while(pref == procid)
                        pref = rand() % (numprocs - 1) + 1;
                    j=i;
                }
                else
                    j-=1;
            }
            preference[i] = pref;
            printf("%d,",pref);
            i+=1;
        }

        printf("\n");
        printf("HELLO");
            
        
        int count=0;
        int k = 0;
        
        for(int a = 1; a < numprocs; a++){
            if(partners[i] == procid){
                partners[procid]=i;
                k=1;
            }
            count+=1;
        }


        int order = 0;
        i=0;
        if(k == 0){
            while(i < numprocs){
                if(partners[i] == preference[order]){
                    i = 0;
                    order+=1;
                }
                else{
                    i+=1;
                }
            }
            partners[procid] = preference[order];
        }

        if( count == 1)
            MPI_Send(partners, numprocs, MPI_INT, 0, 0,MPI_COMM_WORLD);
        else{
            order+=1;
            i = 0;
            while(i < numprocs){
                if(partners[i] == preference[order]){
                    i = 0;
                    order+=1;
                }
                else
                    i+=1;
            }
            int sendto = preference[order];

            MPI_Send(partners, numprocs, MPI_INT,  sendto, 0,MPI_COMM_WORLD);
        }      
    }

    MPI_Finalize();
}