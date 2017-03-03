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
    int partner;
    
    srand((unsigned)time(NULL)+procid*numprocs);
    int pairing[2], partners[numprocs];

    for(int i = 0; i < numprocs; i++)   partners[i] = 0;
    
    if (procid == 0) {
        int obtained[numprocs-1][2];
        int rem = 1, sent = 0;
        while(rem != 0){
            
            rem = 0;
            for(int i = 1; i < numprocs; i++)
                if(partners[i] == 0)    rem += 1;

            for(int j = 0; j < rem; j++){
                MPI_Recv(pairing, 2, MPI_INT, MPI_ANY_SOURCE, sent, MPI_COMM_WORLD, &status);
                obtained[j][0] = pairing[0];
                obtained[j][1] = pairing[1];
            }

            int oldrem = rem;

            partners[obtained[0][0]] = obtained[0][1];
            partners[obtained[0][1]] = obtained[0][0];
            if(rem > 1){
                if(obtained[1][0] == obtained[1][1])
                    partners[obtained[1][0]]=obtained[1][0]; 
                else{
                    int check = 0;
                    for(int i = 0; i < 2; i++){
                        if(obtained[1][i] == obtained[0][0]) check = 1;
                        if(obtained[1][1-i] == obtained[0][1]) check = 1;
                    }
                    if(check == 0){
                        partners[obtained[1][0]] = obtained[1][1];
                        partners[obtained[1][1]] = obtained[1][0];      
                    }
                }  
            }

            rem = 0;
            for(int i = 1; i < numprocs; i++)
                if(partners[i] == 0)    rem += 1;
            sent += 1;

            for(int j = 0; j < oldrem; j++)
                MPI_Send(partners, numprocs, MPI_INT, obtained[j][0], sent, MPI_COMM_WORLD);

        }
    }
    else {
        int preference[numprocs - 2];
        int p = 1,rec = 0, choice;
        pairing[0]=procid;

        while(partners[procid] == 0){      
            p = 0;
            for(int m = 1; m < numprocs; m++){
                if(partners[m] == 0 && procid != m){
                    preference[p] = m;
                    p += 1;
                }
            }
            
            if(p != 0)  pairing[1]=preference[rand() % p];
            else        pairing[1]=procid;

            MPI_Send(pairing, 2, MPI_INT, 0, rec, MPI_COMM_WORLD);
            rec += 1;
            MPI_Recv(partners, numprocs, MPI_INT, 0, rec, MPI_COMM_WORLD, &status);
        }
    }

    MPI_Finalize();
    if(procid != 0) printf("id %d, partner %d\n", procid, partners[procid]);
}