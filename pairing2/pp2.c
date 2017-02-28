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
    srand((unsigned) time(&t));

    if (procid == 0) {
        
        for(int i = 0; i<numprocs; i++)
            partners[i]=0;

        int randomchoice = (rand() % (numprocs - 1) + 1);
        printf("Teacher says: Student %d start!\n",randomchoice);
        MPI_Send(partners, numprocs, MPI_DOUBLE, randomchoice, 0,MPI_COMM_WORLD);
        MPI_Recv(partners, numprocs, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    }
    else {
        int sendpreference[numprocs-1];
        MPI_Recv(partners, numprocs, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        
        int i = 0;
        while(i < numprocs - 2){
            int pref = rand() % (numprocs - 1) + 1;
            while(while())
            int j = i;
            while(j >= 0){
                if(pref == preference[j]){
                    pref = rand() % (numprocs - 1) + 1;
                    j=0;
                }
                else
                    j-=1;
            }
            preference[i] = pref;
            printf("%d,",pref);
            i+=1;
        }
        printf("\n");
        printf("\n");
        
        
        
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
            order+=1;
            i=0;
        }

        while(i < numprocs){
                if(partners[i] == preference[order]){
                    i = 0;
                    order+=1;
                }
                else
                    i+=1;
        }
        int sendto = preference[order];

        if( count == 1)
            MPI_Send(partners, numprocs, MPI_DOUBLE, 0, 0,MPI_COMM_WORLD);
        else
            MPI_Send(partners, numprocs, MPI_DOUBLE, sendto, 0,MPI_COMM_WORLD);    
    }

    MPI_Finalize();
}