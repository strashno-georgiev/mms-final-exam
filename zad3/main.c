#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define THREAD_COUNT 5

double **ROWS;
double *RESULTS;
unsigned int N, M;
int NEXT_ROW;
pthread_mutex_t MUTEX;

void* procedure(void* data) {
	double result;
	int current_row;
	while(1) {
		result = 0;
		pthread_mutex_lock(&MUTEX);
		current_row = NEXT_ROW;
		NEXT_ROW++;
		pthread_mutex_unlock(&MUTEX);
		if(current_row >= N) {
			//printf("Thread %ld exitting\n", pthread_self());
			//fflush(stdout);
			pthread_exit(NULL);
		}
		//printf("Thread %ld starting work on row %d\n", pthread_self(), current_row);
		//fflush(stdout);
		for(int i=0; i < M; i++) {
			result += ROWS[current_row][i] * ROWS[current_row][i];
		}
		result = sqrt(result);
		RESULTS[current_row] = result;
		//Ако няма usleep е по-бързо за 1 нишка да обработи до 7 реда сама преди отблокирането на друга нишка (на моя компютър). Не го махам, защото без него не се изпълнява условието всяка нишка да обработи около N/THREAD_COUNT на брой реда (на моя компютър);
		usleep(1000 * 100);
	}
}

int main(int argc, char *argv[]) {
	FILE *file;
	char filename[128];
	pthread_t threads[THREAD_COUNT];
	scanf("%u %u", &N, &M);
	ROWS = calloc(N, sizeof(double*));
	RESULTS = calloc(N, sizeof(double));
	pthread_mutex_init(&MUTEX, NULL);
	for(int i=0; i < N; i++) {
		ROWS[i] = calloc(M, sizeof(double));
	}
	
	for(int i=0; i < N; i++) {
		for(int j=0; j < M; j++) {
			scanf("%lf", &ROWS[i][j]);
		}
	}
	
	scanf("%s", filename);
	if((file = fopen(filename, "r"))) {
		fprintf(stderr, "%s: file %s already exists\n", argv[0], filename);
		fclose(file);
		return -1;
	}
	file = fopen(filename, "w");
	if(!file) {
		fprintf(stderr, "%s: %s: %s\n", argv[0], filename, strerror(errno));
	}
	
	NEXT_ROW = 0;
	for(int i=0; i < THREAD_COUNT; i++) {
		pthread_create(threads+i, NULL, procedure, NULL);
	}
	
	for(int i=0; i < THREAD_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}
	
	for(int i=0; i < N; i++) {
		fprintf(file, "%lf\n", RESULTS[i]);
	}
	
	fclose(file);
	for(int i=0; i < N; i++) {
		free(ROWS[i]);
	}
	free(ROWS);
	free(RESULTS);
	pthread_mutex_destroy(&MUTEX);	
}
