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
        MPI_Recv(partners, numprocs, MPI_DOUBLE, MPI_ANY_SOURCE, 666, MPI_COMM_WORLD, &status);
    }
    else {
        MPI_Recv(partners, numprocs, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        
        printf("Got it!I am %d\n",procid);

        //check if I have been taken
       
        /* Check if
        *  I have been taken
            *  yes? set partners[procid] = partner
            *  no? am I alone?
            *   yes? That means you're the last send message
        *  Partner has been taken
            *  yes? redo
            *  no? assign partner
        */
        int notalone = 0;
        int k = 0;
        for(int i = 1; i < numprocs; i++){
            //if i already have a partner
            //assign the partner to me
            if(partners[i] == procid){
                partners[procid] = i;
                notalone = 1;
                break;
            }
            //How many people are left without partners?
            else if(partners[i] == 0){
                k++;
            }
        }
        //Am I the only one left
        if(k == 1){
            //I should assign myself, if I don't have a pairing
            if(notalone == 0)
                partners[procid]=procid;
            MPI_Send(partners, numprocs, MPI_DOUBLE, 0, 666,MPI_COMM_WORLD);
        }

        //Not the only one left
        else{
            int left[k];            
            int a=0;
            int sendtopos= rand() % k, partnerpos=rand() % k, sendto=0;
            //assign an array for the people without partners
            for(int i = 1; i < numprocs; i++){
                if(partners[i] == 0){
                    left[a] = i;
                    a += 1;
                }
            }
            if(partners[procid] == 0){
                //assign random partner make sure partner isn't myself
                while(left[partnerpos] == procid)
                    partnerpos=rand() % k;
                partners[procid] = left[partnerpos];
            }
            //Who am I supposed to send the next message to, can't be me
            //can't be the guy who was just assigned
            while(left[sendtopos] == procid || sendtopos == partnerpos)
                sendtopos  = rand() % k;

            sendto=left[sendtopos];
            printf("sendto %d\n",sendto);
            printf("I am %d, my partner is %d\n",procid,partners[procid]);
            
            MPI_Send(partners, numprocs, MPI_DOUBLE, sendto, 0,MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
}