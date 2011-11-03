#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "RWLock.h"
#define CANT_THREADS 10

int compartida;
RWLock lock;

void *read(void *p_args){
  int* args = (int*) p_args;
  if(args[0] == -1)
    sleep(args[1]);

  lock.rlock();
  printf("Compartida: %d\n", compartida);
  lock.runlock();

  pthread_exit(NULL);
}

void *write(void *p_args){
  int* args = (int*) p_args;
  sleep(args[1]);

  lock.wlock();
  sleep(5);
  compartida = 42;
  lock.wunlock();

  pthread_exit(NULL);
}

int main(int argc, char **argv){
  pthread_t thread[CANT_THREADS];
  int tids[CANT_THREADS], tid;

  compartida = 0;
  lock = RWLock();

  /**** Test1 ****/

  printf("\nTest 1 - 3 reads\n");

  for (tid = 0; tid < 3; ++tid) {
    tids[tid] = tid;
    pthread_create(&thread[tid], NULL, read, &tids[tid]);
  }

  for (tid = 0; tid < 3; ++tid)
    pthread_join(thread[tid], NULL);

  /**** Test2 ****/

  printf("\nTest 1 - 2 reads, 1 write, 1 read\n");

  int args1[2] = {3, 5};
  int args2[2] = {-1, 6};
  int args3[2] = {0, 6};

  for (tid = 0; tid < 4; ++tid) {
    tids[tid] = tid;
    if(tid == 2){
      pthread_create(&thread[tid], NULL, write, (void*)args1);
    }else{
      if(tid == 3){
        pthread_create(&thread[tid], NULL, read, (void*)args2);
      }else{
        pthread_create(&thread[tid], NULL, read, (void*)args3);
      }
    }
  }

  for (tid = 0; tid < 4; ++tid)
    pthread_join(thread[tid], NULL);
  return 0;
}

