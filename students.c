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
    int partners[numprocs];
    for(int i = 0; i < numprocs; i++){
        partners[i] = 0;
    }
    //How many time are we supposed to send
    int sendamount = (numprocs - 1 / 4);
    if(numprocs - 1 % 4 != 0)
        sendamount += 1;

    
    /*
    send amount = times children should send to teacher
    retrieve amount = times children retrieves a message from teacher

    numprocs = 5 -> c: 1234 -> 12; 34; sendback
    -> retrieveamount 1; sendamount 1; -> Sendamount = 4(numprocs) / 4 = 1

    numprocs = 6 -> c: 12345 -> 1234 sendback I'm alone send myself; retrieve; sendback
    ->  retrieveamount 2; sendamount  2 -> Sendamount = 5(numprocs) / 4 = 2

    numprocs = 8 -> c 1234567 -> 1234 send -> sendback -> 56 7 send -> sendback
    ->  retrieveamount 2; sendamount  2 -> Sendamount = 7(numprocs) / 4 = 0

    numprocs = 9 -> c 1234567 -> 1234 send -> sendback -> 56 78 send -> sendback

    sendamount == retrieveamount
    */

    
    if (procid == 0) {
        printf("Sendamount %d\n", sendamount);
        //obtained from Recieved
        int obtained[numprocs-1][2];
        int rem = 1;
        int sent = 0;
        while(rem != 0){

            rem = 0;
            for(int i = 1; i < numprocs; i++){
                if(partners[i] == 0)
                    rem += 1;
            }

            for(int j = 0; j < rem; j++){
                MPI_Recv(pairing, 2, MPI_INT, MPI_ANY_SOURCE, sent, MPI_COMM_WORLD, &status);
                obtained[j][0] = pairing[0];
                obtained[j][1] = pairing[1];
            }

            int oldrem = rem;

            partners[obtained[0][0]] = obtained[0][1];
            partners[obtained[0][1]] = obtained[0][0];
            if(rem > 1){
                if(
                obtained[0][0] != (obtained[1][0])
                && obtained[0][0] != (obtained[1][1]) 
                && obtained[0][1] != (obtained[1][0])
                && obtained[1][1] != (obtained[1][1])
                )
                {
                        partners[obtained[1][0]] = obtained[1][1];
                        partners[obtained[1][1]] = obtained[1][0];
                        rem -= 2;
                }
                else
                    rem -= 1;    
            }
            else{
                rem == 0;
            }

            for(int j = 0; j < oldrem; j++)
                MPI_Send(partners, numprocs, MPI_INT, obtained[j][0], sent + 1, MPI_COMM_WORLD);
            sent += 1;
        }

    }
    else {
        printf("\n");
        int preference[numprocs - 2];
        
        
        int p = 0;
        //0 1 2 3
        int rec = 0;
        while(partners[procid] == 0){      
            
            for(int m = 1; m < numprocs; m++){
                if(partners[m] == 0 && procid != m){
                    preference[p] == partners[m];
                    p++;
                }
            }
            
            int want = preference[rand() % p];

            pairing[0]=procid;
            pairing[1]=want;
            if(p == 0)
                pairing[1]=procid;

            printf("id %d, preference %d\n", procid, pairing[1]);

            MPI_Send(pairing, 2, MPI_INT, 0, rec, MPI_COMM_WORLD);
            printf("Got it!",procid);
            MPI_Recv(partners, numprocs, MPI_INT, 0, rec + 1, MPI_COMM_WORLD, &status);

            rec += 1;
            p = 0;
        }
    }

    MPI_Finalize();

    printf("id %d, partner %d\n", procid, partners[procid]);
}