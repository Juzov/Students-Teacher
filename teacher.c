#include<unistd.h>
#include<time.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "mpi.h" 


int main (int argc, char *argv[]){

/*    if (argc < 2) { 
        fprintf(stderr, "Num of children required.\n"); 
        exit(EXIT_FAILURE); 
    }*/

    ierr = MPI_Init(&argc,&argv);
    int ierr, procid, numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if (numprocs != 1) { 
        fprintf(stderr, "Server too big"); 
        exit(EXIT_FAILURE); 
    }
    

    int pairings[], int senta, int sentb, int children;

    MPI_Recieve(&children, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    pairings = new int[children];

    

    MPI_Recieve(&senta, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recieve(&sentb, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int first = rand() % numprocs + 1;
    if(procid == 0 || procid == 1){
        MPI_Send(&first, 1, MPI_INT, dest, 0, comm );
    }


    if(numprocs % 2 != 0 && partner == -1){
        partner == procid + 1;
    }

    //first second choice

    int second = rand() % numprocs + 1;
    while(first == second){
        second = rand() % numprocs + 1;
    }


    /*
    int=favouri

    */

    printf("Hello world! I am pid %d out of %d\n",procid,numprocs);

    ierr = MPI_Finalize();
}