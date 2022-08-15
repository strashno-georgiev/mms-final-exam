#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>


char checkbit(uint32_t num, int bit) {
	uint32_t one = 1;
	return !!(num & (one << bit));
}

int count_ones(uint32_t mask) {
	int count = 0;
	for(int i=0; i < sizeof(mask) * CHAR_BIT; i++) {
		count += checkbit(mask, i);
	}
	return count;
}


void* procedure(void *data) {
	FILE* file;
	char* this_file = *(char**)data;
	uint32_t mask;
	uint32_t *count = malloc(sizeof(uint32_t));
	file = fopen(this_file, "rb");
	if(!file) {
		fprintf(stderr, "%s: %s\n", this_file, strerror(errno));
		pthread_exit(NULL);
	}
	fread(&mask, sizeof(mask), 1, file);
	*count = count_ones(mask);
	fclose(file);
	printf("%s: %d\n", this_file, *count);
	
	pthread_exit(count); 
}

int main(int argc, char *argv[]) {
	uint32_t total_count = 0;
	pthread_t threads[argc-1];
	for(int i=1; i < argc; i++) {
		pthread_create(threads+i-1, NULL, procedure, argv+i);
	}
	
	for(int i=0; i < argc-1; i++) {
		uint32_t *current_count;;
		pthread_join(threads[i], (void**)&current_count);
		if(current_count != NULL) {
			total_count += *current_count;
		}
		free(current_count);
	}
	
	printf("Total count - %u\n", total_count);
}
