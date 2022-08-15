#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>
#define LAPTOPS_COUNT 5

struct Laptop {
	uint64_t serial;
	char brand[100];
	double weight;
	double price;
};

//From min to max-1
int64_t randint(int64_t min, int64_t max) {
	return (rand() % (max-min)) + min;
}

double randreal(int min, int max) {
	return ((double)rand() / (double)RAND_MAX) * (max - min) + min;
}



struct Laptop randomLaptop() {
	struct Laptop new;
	int brandLen = randint(8, 13);
	new.serial = randint(0, UINT_MAX);
	new.brand[0] = randint('A', 'Z'+1);
	new.brand[1] = ' ';
	for(int i=2; i < brandLen*2; i+=2) {
		new.brand[i] = randint('a', 'z'+1);
		new.brand[i+1] = ' ';
	}
	new.brand[brandLen*2] = 0;
	new.weight = randreal(0.025, 10.00);
	new.price = randreal(10.00, 5000.00);
	return new;
}

double totalLaptopWeight(struct Laptop *laptops) {
	if(LAPTOPS_COUNT <= 0) {
		return -1;
	}
	double total=0;
	for(int i=0; i < LAPTOPS_COUNT; i++) {
		total += laptops[i].weight;
	}
	return total;
}

struct Laptop* minPriceItem(struct Laptop *laptops) {
	if(LAPTOPS_COUNT <=0 ) {
		return NULL;
	}
	int minInd = 0;
	for(int i=1; i < LAPTOPS_COUNT ; i++) {
		if(laptops[i].price - laptops[minInd].price < 0) {
			minInd = i;
		}
	}
	return (laptops+minInd);
}

void printLaptop(struct Laptop l) {
	printf("\t%s\n\t#%ld\n\t%.2lf kg\n\t$%.2lf\n\n", l.brand, l.serial, l.weight, l.price);
}



int main() {
	srand(time(NULL));
	struct Laptop *laptops = calloc(LAPTOPS_COUNT, sizeof(struct Laptop));
	for(int i=0; i < LAPTOPS_COUNT; i++) {
		laptops[i] = randomLaptop();
		printf("\tLaptop %d:\n", i+1);
		printLaptop(laptops[i]);
	}
	
	printf("Total weight: %lf kg\n", totalLaptopWeight(laptops));
	printf("Cheapest laptop:\n");
	if(minPriceItem(laptops) != NULL)
	printLaptop(*(minPriceItem(laptops)));
	
	free(laptops);
	
	return 0;
}
