#include <stdio.h>
#include "RWLock.h"

RWLock :: RWLock() {
  sem_init(&mutex, 0, 1);
  readers = 0;
}

RWLock :: ~RWLock() {
  while(!writers_queue.empty()){
    Batch* current_batch = writers_queue.front();
    writers_queue.pop();
    delete current_batch;
  }
  sem_destroy(&mutex);
}

void RWLock :: rlock() {
  sem_wait(&mutex);
  readers++;
  if(writers_queue.empty()){
    printf("rlock: no hay nadie en la cola\n");
    sem_post(&mutex);
  }else{
    printf("rlock: hay alguien en la cola!!\n");
    Batch* last_writer = writers_queue.back();
    last_writer->readers++;

    sem_post(&mutex);
    sem_wait(&(last_writer->readers_sem));
    sem_post(&(last_writer->readers_sem)); // cascada para que sigamos leyendo todos
  }
}

void RWLock :: wlock() {
  Batch* new_batch = new Batch();

  sem_wait(&mutex);
  writers_queue.push(new_batch);
  if(writers_queue.size() == 1 && readers == 0)
    sem_post(&(new_batch->write_permission));
  else
    if(readers > 0) printf("wlock: hay alguien leyendo!!\n");

  sem_post(&mutex);
  sem_wait(&(new_batch->write_permission));
  printf("wlock: a escribir!!\n");
}

void RWLock :: runlock() {
  sem_wait(&mutex);
  readers--;
  if(!writers_queue.empty()){
    Batch* current_batch = writers_queue.front();
    if(current_batch->readers > 0)
      current_batch->readers--;

    if(current_batch->readers > 0){
      sem_post(&mutex);
    }else{
      if(current_batch->written){
        writers_queue.pop();
        delete current_batch;
      }

      if(!writers_queue.empty())
        sem_post(&(writers_queue.front()->write_permission));
      sem_post(&mutex);
    }
  }else
    sem_post(&mutex);
}

void RWLock :: wunlock() {
  sem_wait(&mutex);
  Batch* current_batch = writers_queue.front();
  current_batch->written = true;
  if(current_batch->readers > 0){
    sem_post(&(current_batch->readers_sem));
    sem_post(&mutex);
  }else{
    Batch* current_batch = writers_queue.front();
    writers_queue.pop();
    delete current_batch;

    if(!writers_queue.empty())
      sem_post(&(writers_queue.front()->write_permission));
    sem_post(&mutex);
  }
}

