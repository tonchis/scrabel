#include<pthread.h>
#include<stdio.h>

void *aloha_honua(void *vargp)
{
	printf("Aloha honua!\n");
	return NULL;
}

int main() {
	pthread_t tid;
	
	pthread_create(&tid, NULL, aloha_honua, NULL);
	pthread_join(tid, NULL);
	
	return 0;
}
