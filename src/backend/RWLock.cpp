#include "RWLock.h"

RWLock :: RWLock() {
  sem_init(is_writing_mutex, 0, 1);
  sem_init(write_complete, 0, 0);
  is_writing = false;

  sem_init(readers_mutex, 0, 1);
  sem_init(no_readers, 0, 0);
  readers = 0;
}

RWLock :: ~RWLock() {
  sem_destroy(is_writing_mutex);
  sem_destroy(write_complete);

  sem_destroy(readers_mutex);
  sem_destroy(no_readers);
}

void RWLock :: rlock() {
  sem_wait(is_writing_mutex);
  if(!is_writing){
    sem_post(is_writing_mutex);

    sem_wait(readers_mutex);
    readers++;
    sem_post(readers_mutex);
  }else{
    sem_post(is_writing_mutex);
    sem_wait(write_complete);
    sem_post(write_complete); // cascada para avisarles a los pibes que se puede leer
  }
}

void RWLock :: wlock() {
  sem_wait(readers_mutex);
  if(readers > 0){
    sem_post(readers_mutex);
    sem_wait(no_readers);
  }

  sem_wait(is_writing_mutex);
  if(is_writing)
    sem_wait(write_complete);
  is_writing = true;
  sem_post(is_writing_mutex);
}

void RWLock :: runlock() {
  sem_wait(readers_mutex);
  readers--;
  if(readers == 0)
    sem_post(no_readers);
  sem_post(readers_mutex);
}

void RWLock :: wunlock() {
  sem_wait(is_writing_mutex);
  is_writing = false;
  sem_post(is_writing_mutex);
  sem_post(write_complete);
}

