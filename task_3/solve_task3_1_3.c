#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

const int bytes = 4096*1024;

int main()
{
   char *  array;
   int     i, shmid;
   char    pathname[] = "solve_task1_1.c";
   key_t   key;

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
      }
   }

   if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
      printf("Can't attach shared memory\n");
      exit(-1);
   }

   for(i=0; i<102;i++)
        array[i] = 0;

    int j = 0;
    int count = 0;
    while(1)
    {
        while(array[100] == 1)
        {}

        array[j]='s';
        j++;
        count++;
        if((j==100)||(count == bytes))
        {
            array[101]=count % 100;
            if(count==bytes)
                array[101]=-1;
            array[100]=1;
            j=0;
            if(count == bytes)
                break;
        }

    }

   printf("Program 1 exit");

   if(shmdt(array) < 0){
      printf("Can't detach shared memory\n");
      exit(-1);
   }

   return 0;
}
