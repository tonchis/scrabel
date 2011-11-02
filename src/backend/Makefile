CC = g++
CFLAGS = -g -Wall -Werror
LDFLAGS = -pthread
SEMFLAGS = -c -lpthread
BINARIES = backend

all: $(BINARIES)

backend: Backend_mono.cpp Backend_mono.h Enviar_recibir.o Enviar_recibir.h Encabezado.h Casillero.h RWLock.o
	$(CC) $(LDFLAGS) $(CFLAGS) Backend_mono.cpp Enviar_recibir.o -o backend

RWLock.o: RWLock.cpp RWLock.h
	$(CC) $(SEMFLAGS) $(CFLAGS) RWLock.cpp

Enviar_recibir.o: Enviar_recibir.cpp Enviar_recibir.h Encabezado.h
	$(CC) $(CFLAGS) -c Enviar_recibir.cpp

clean:
	rm -f *.o
	rm -f $(BINARIES)
