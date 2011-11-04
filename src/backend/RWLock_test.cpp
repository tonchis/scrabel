#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "RWLock.h"
#define CANT_THREADS 10

int compartida = 0;
RWLock lock;

void *read(void *p_args){
  int* args = (int*) p_args;
  if(args[0] == -1)
    sleep(args[1]);

  lock.rlock();
  sleep(2);
  printf("Compartida: %d\n", compartida);
  lock.runlock();

  pthread_exit(NULL);
}

void *write(void *p_args){
  int* args = (int*) p_args;
  if(args[0] == -1)
    sleep(args[1]);

  lock.wlock();
  sleep(5);
  compartida = args[2];
  lock.wunlock();

  pthread_exit(NULL);
}

int main(int argc, char **argv){
  pthread_t thread[CANT_THREADS];
  int tids[CANT_THREADS], tid;

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
  printf("\nTest 2 - 2 reads, 1 write, 1 read\n");

  int args21[3] = {-1, 1, 21};
  int args22[2] = {-1, 6};
  int args23[2] = {0, 6};

  for (tid = 0; tid < 4; ++tid) {
    tids[tid] = tid;
    if(tid == 2){
      pthread_create(&thread[tid], NULL, write, (void*)args21);
    }else{
      if(tid == 3){
        pthread_create(&thread[tid], NULL, read, (void*)args22);
      }else{
        pthread_create(&thread[tid], NULL, read, (void*)args23);
      }
    }
  }

  for (tid = 0; tid < 4; ++tid)
    pthread_join(thread[tid], NULL);

  /**** Test3 ****/
  printf("\nTest 3 - 1 write, 2 reads\n");

  int args31[3] = {0, 5, 31};
  int args32[2] = {-1, 1};

  for (tid = 0; tid < 3; ++tid) {
    tids[tid] = tid;
    if(tid == 0){
      pthread_create(&thread[tid], NULL, write, (void*)args31);
    }else{
        pthread_create(&thread[tid], NULL, read, (void*)args32);
    }
  }

  for (tid = 0; tid < 3; ++tid)
    pthread_join(thread[tid], NULL);

  /**** Test4 ****/
  printf("\nTest 4 - 2 reads, 2 write, 1 read\n");

  int args41[3] = {-1, 1, 41};
  int args42[3] = {-1, 2, 42};
  int args43[2] = {-1, 3};
  int args44[2] = {0, 2};

  for (tid = 0; tid < 5; ++tid) {
    tids[tid] = tid;
    if(tid == 2){
      pthread_create(&thread[tid], NULL, write, (void*)args41);
    }else{
      if(tid == 3){
        pthread_create(&thread[tid], NULL, write, (void*)args42);
      }else{
        if(tid == 4){
          pthread_create(&thread[tid], NULL, read, (void*)args43);
        }else{
          pthread_create(&thread[tid], NULL, read, (void*)args44);
        }
      }
    }
  }

  for (tid = 0; tid < 5; ++tid)
    pthread_join(thread[tid], NULL);

  return 0;
}

