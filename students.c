#include<unistd.h>
#include<time.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "mpi.h" 


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
    
    srand((unsigned)time(NULL)*procid*numprocs);
    int pairing[2];
    int partners[numprocs - 1];
    //How many time are we supposed to send
    int sendamount = (numprocs - 1) / 2;
    
    //
    //1 to n-1
    //num procs = 5 -> children = 4 
    //1 2 3 4
    //num procs = 6 -> children = 5 
    //1 2 3 4 5 -> sendamount = numprocs - 1 / 2 , 5 / 2 + 1

    if((numprocs - 1) % 2 == 0)
        sendamount += 1;

    if (procid == 0) {
        printf("Sendamount %d\n", sendamount);
        //obtained from Recieved
        int obtained[numprocs-1];
       
        for(int i = 0; i < sendamount - 1; i++){
            //Recieve priority from all children
            //Tags: 0->1->2...
            for(int j = 0; j < numprocs - 1; j++){
                MPI_Recv(&partner, 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
                obtained[j]=partner;
            }
            printf("Recieved Done %d\n",i);

            int pairing[2];
            pairing[0]=obtained[0];
            
            if((i == sendamount - 2) && (i % 2 != 0)){
                pairing[1]=0;
                printf("Hello %d,%d\n",i,sendamount);
            }
            else{
                int k = 1;
                while((k <= numprocs-1)){
                    if(pairing[0] != obtained[k]){
                        pairing[1]=obtained[k];
                        break;
                    }
                    k+=1;
                }
            }
    
            printf("Pair %d: %d, %d\n",i,pairing[0],pairing[1]);

            for(int j = 1; j < numprocs; j++){
                MPI_Send(pairing, 2, MPI_INT, j, i + 1, MPI_COMM_WORLD);
            }
        }

    }
    else {
        printf("\n");
        int preference[numprocs - 2];
        int i = 0;
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
        
        int p = 0;
        //0 1 2 3
        for(int c = 0; c < sendamount - 1; c++){      
            //check if the preferred has been taken before?
            while(preference[p] == 0)
                p+=1;
            
            printf("id %d, preference %d\n", procid, preference[p]);

            //send the preferred
            //send 1 recieve

            MPI_Send(&preference[p], 1, MPI_INT, 0, c, MPI_COMM_WORLD);
            MPI_Recv(pairing, 2, MPI_INT, 0, c + 1, MPI_COMM_WORLD, &status);

            for(int m = 0; m < 2; m++){
                for(int n = 0; n < numprocs - 2; n++){
                    if(pairing[m] == preference[n])
                        preference[n] = 0;
                }
            }
        }
    

        
        
    }

    MPI_Finalize();
}