CC = g++
CFLAGS = -g -Wall -Werror
LDFLAGS = -pthread
SEMFLAGS = -c -lpthread
BINARIES = backend lock_test

all: $(BINARIES)

backend: Backend.cpp Backend.h Enviar_recibir.o Enviar_recibir.h Encabezado.h Casillero.h RWLock.o
	$(CC) $(LDFLAGS) $(CFLAGS) Backend.cpp Enviar_recibir.o RWLock.o -o backend

lock_test: RWLock_test.cpp RWLock.o
	$(CC) $(CFLAGS) -lpthread RWLock.o RWLock_test.cpp -o lock_test

RWLock.o: RWLock.cpp RWLock.h
	$(CC) $(SEMFLAGS) $(CFLAGS) RWLock.cpp

Enviar_recibir.o: Enviar_recibir.cpp Enviar_recibir.h Encabezado.h
	$(CC) $(CFLAGS) -c Enviar_recibir.cpp

clean:
	rm -f *.o
	rm -f $(BINARIES)
