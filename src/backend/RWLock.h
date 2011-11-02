#ifndef RWLock_h
#define RWLock_h
#include <queue>
#include <semaphore.h>

using namespace std;

struct Batch {
  sem_t* write_permission;
  sem_t* readers_sem;
  int readers;

  Batch(){
    sem_init(write_permission, 0, 0);
    sem_init(readers_sem, 0, 0);
    readers = 0;
  }

  ~Batch(){
    sem_destroy(write_permission);
    sem_destroy(readers_sem);
  }
};

class RWLock {
  public:
    RWLock();
    ~RWLock();
    void rlock();
    void wlock();
    void runlock();
    void wunlock();

  private:
    queue<Batch*> writers_queue;
    sem_t* mutex;
};

#endif
