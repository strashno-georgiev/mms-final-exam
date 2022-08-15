#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

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

int main(int argc, char *argv[]) {
	FILE* file;
	pid_t pid = getpid();
	int pipefd[2];
	if(pipe(pipefd)) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
	}
	int this_file, child_status;
	uint32_t total_count = 0;
	for(int i=1; i < argc; i++) {
		pid = fork();
		if(pid == 0) {
			this_file = i;
			break;
		}	
	}
	if(pid == 0) {
		uint32_t mask;
		uint32_t count;
		file = fopen(argv[this_file], "rb");
		if(!file) {
			fprintf(stderr, "%s: %s: %s\n", argv[0], argv[this_file], strerror(errno));
			return -1;
		}
		fread(&mask, sizeof(mask), 1, file);
		count = count_ones(mask);
		fclose(file);
		printf("%s: %d\n", argv[this_file], count);
		write(pipefd[PIPE_WRITE], &count, sizeof(count));
		return 0;
	}
	
	for(int i=1; i < argc; i++) {
		uint32_t current_count = 0;
		wait(&child_status);
		if(child_status == 0) {
			
			if(read(pipefd[PIPE_READ], &current_count, sizeof(uint32_t)) < 0) {
				perror("read failed");
				return -1;
			}
			total_count += current_count;
		}
	}
	printf("Total count - %u\n", total_count);
	close(pipefd[0]);
	close(pipefd[1]);
}
