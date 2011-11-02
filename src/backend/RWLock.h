#ifndef RWLock_h
#define RWLock_h
#include <iostream>
#include <sempahore.h>

class RWLock {
  public:
    RWLock();
    ~RWLock();
    void rlock();
    void wlock();
    void runlock();
    void wunlock();

  private:
    bool is_writing;
    sem_t *is_writing_mutex;
    sem_t *write_complete;

    int readers;
    sem_t *readers_mutex;
    sem_t *no_readers;

    queue<sem_t*> writers;
};

#endif
