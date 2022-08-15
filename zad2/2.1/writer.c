#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc, char* argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: %s [num] [file]\n", argv[0]);
		return -1;
	}
	FILE* file = fopen(argv[2], "wb");
	if(!file) {
		fprintf(stderr, "%s: %s: %s\n", argv[0], argv[2], strerror(errno));
		return -1;
	}
	uint32_t num = atoi(argv[1]);
	fwrite(&num, sizeof(num), 1, file);
	fclose(file);
	return 0;
}
