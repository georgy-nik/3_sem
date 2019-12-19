#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

const int bytes = 4096*1024;

int main()
{
   char  *array;
   char mas[100];
   int   shmid;
   int   new = 1;
   char  pathname[] = "solve_task1_2.c";
   key_t key;

   if((key = ftok(pathname,0)) < 0){
     printf("Can\'t generate key\n");
     exit(-1);
   }

   if((shmid = shmget(key, 102*sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      if(errno != EEXIST){
         printf("Can\'t create shared memory\n");
         exit(-1);
      } else {
         if((shmid = shmget(key, 102*sizeof(char), 0)) < 0){
            printf("Can\'t find shared memory\n");
            exit(-1);
	 }
         new = 0;
      }
   }

   if (new == 1)
   {printf("EMPTY!\n"); exit(-1); }

   if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
      printf("Can't attach shared memory\n");
      exit(-1);
    }

    int j = 0;
    int count = 0;

    clock_t start = clock();
    while(1)
    {
        while(array[100] == 0)
        {}

        mas[j]=array[j];
        j++;
        count++;
        if((j==100)||(count == bytes))
        {
            array[100]=0;
            j=0;
            if(count == bytes)
                break;
        }

    }
    printf("Time: %.3f ms\n", 1000*((double)clock()-start)/((double)CLOCKS_PER_SEC));

   if(shmdt(array) < 0){
      printf("Can't detach shared memory\n");
      exit(-1);
   }

   return 0;
}
