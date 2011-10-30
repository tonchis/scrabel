#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

#define MAX_THREADS 32

void comprimir_archivo(char *nombre)
{
	int bytes;
	char nom_sal[256], buffer[1024];
	strcpy(nom_sal, nombre);
	strcat(nom_sal, ".gz");
	FILE *entrada = fopen(nombre, "rb");
	gzFile salida = gzopen(nom_sal, "wb");
	while ((bytes = fread(buffer, 1, sizeof(buffer), entrada)) > 0)
		gzwrite(salida, buffer, bytes);
	fclose(entrada);
	gzclose(salida);
}

typedef struct {
	int id;
	pthread_t thread;
	char nombre[256];
} trabajo_t;

void reportar(const char *mensaje, trabajo_t *trabajo)
{
	static const char fmt[] = "%s #%d (%s)\n";
	printf(fmt, mensaje, trabajo->id, trabajo->nombre);
}

void *trabajar(void *trabajo)
{
	trabajo_t *info = ((trabajo_t*) trabajo);
	reportar("empezando", info);
	comprimir_archivo(info->nombre);
	reportar("terminando", info);
	return NULL;
}

int main(int argc, char **argv) 
{
	trabajo_t *ts; int i; --argc;

	if(argc == 0 || argc > MAX_THREADS) {
		fprintf(stderr, "Uso: %s <archivos>\n", argv[0]); exit(1);
	} else ++argv;

	ts = (trabajo_t *) malloc(argc * sizeof(trabajo_t));

	for(i = 0; i < argc; ++i) {
		ts[i].id = i; strcpy(ts[i].nombre, argv[i]);
		pthread_create(&(ts[i].thread), NULL, trabajar, (void*)&ts[i]);
	}

	for(i = 0; i < argc; ++i)
		pthread_join(ts[i].thread, NULL);

	free(ts);
	return 0;
}

