#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "func_def.h"
#define FALSE 0
#define TRUE 1
#define MINPVALUE .01f

void asciiBitFileReader(char *outputBits, FILE *inputFile, int bitCount);
int rawBitReader(char *outputbits, FILE *inputFile, int bitCount);
void printBytes(char *bits, int bitCount);
int runNistTests(char *bits, int bitCount);

int bitCount;
int raw = FALSE;
int chaoskeybits = FALSE;

char *bits;

FILE *file;

int main(int argc, char *argv[]){
	int argi = 1;
	while (argi < argc) {
		if ( strcmp(argv[argi], "--chaoskeybits") == 0 ) {
			chaoskeybits = TRUE;				
			argi++;
		}
		else if ( strcmp(argv[argi], "--raw") == 0 ) {
			raw = TRUE;
			argi++;
		}
		else if ( (strcmp(argv[argi], "--nbytes") == 0) && argi+1 < argc) {
			bitCount = atoi(argv[argi+1]); 
			bits = malloc(sizeof(int)*bitCount);
			
			bitCount *= 8;
			argi += 2;
		} else {
			break;
		}		
	}
		
	if (argi + 1 == argc) {
		file = fopen(argv[argi], "r");
		
	} else {
		printf("usage: TestSuite [--raw] [--chaoskeybits] [--nbytes <nbytes>] [<file>]\n");
		return 1;
	}
	
	if (raw) {
		if (rawBitReader(bits, file, bitCount)) {
			printf("failed reading file");
			return 1;
		}
		printf("reading raw bits\n");
	} else {
		asciiBitFileReader(bits, file, bitCount);
	}

	printBytes(bits, bitCount);
	int testsPassed = runNistTests(bits, bitCount);
			
	return testsPassed;	
}

void asciiBitFileReader(char *outputBits, FILE *inputFile, int bitCount){
	char c;
	int nBitsRead = 0;
	while (nBitsRead < bitCount && c != EOF) {
		c = getc(inputFile);
		if (c-'0' == 0 || c-'0' == 1) {
			outputBits[nBitsRead / 8] |= (c - '0') << (7 - (nBitsRead % 8));		
			nBitsRead++;
		}
	}
	
	if (nBitsRead<bitCount) {	
		printf("too few bits found, changed bit count to %i\n", bitCount);
	}
}

int rawBitReader(char *outputBits, FILE *inputFile, int bitCount){
	fread(outputBits, 1,  bitCount / 8, inputFile);
	if (file != 0) {
		return 0;
		
	} else {	
		return 1;
	}
}

void printBytes(char *bits, int bitCount){
	int j, k;
	for (j = 0; j < bitCount / 8; j++) {	
		for (k = 0; k < 8; k++) {	
			printf("%i", (bits[j]>>k) & 1 );	
		}	
		printf("\n%i\n", j);	
	}
	//printf("byte Count %i" , bitCount/8);
}

int runNistTests(char *bits, int bitCount){

	printf("frequency test: %f\n", frequency(bits, bitCount));
	printf("block test: %f\n", blockFrequency(bits, bitCount, 10)); 
	printf("runs test: %f\n", runs(bits, bitCount));
	printf("longest runs of ones test: %f\n", longestRunsOfOnes(bits, bitCount));
	printf("rank test: %f\n", rank(bits, bitCount));
	printf("discreteFourierTransform test: %f\n", discreteFourierTransform(bits, bitCount));
	printf("universal Test: %f\n", universal(bits, bitCount));
	printf("approxEntropy test: %f\n", approximateEntropy(bits, bitCount, 2));

	if (frequency(bits, bitCount) < MINPVALUE) return 1;
	if (blockFrequency(bits, bitCount, 10) < MINPVALUE) return 1;
	if (runs(bits, bitCount) < MINPVALUE) return 1;
	if (rank(bits, bitCount) < MINPVALUE) return 1;
	if (discreteFourierTransform(bits, bitCount) < MINPVALUE) return 1;
	if (universal(bits, bitCount) < MINPVALUE) return 1;
	if (approximateEntropy(bits, bitCount, 2) < MINPVALUE) return 1;

	int i;

	double *cusumValues = malloc(sizeof(double)*20);
	double cusumSize = cumulativeSums(bits, bitCount, cusumValues);

	for (i = 0;i < cusumSize; i++) {
		printf("cumsum return value %f\n", cusumValues[i]);
		if (cusumValues[i] < MINPVALUE) return 1;
	}
	
	double *reValues = malloc(sizeof(double)*20);
	double reSize = randomExcursions(bits, bitCount, reValues);

	for (i = 0; i < reSize; i++) {
		printf("random excursions test: %f\n", reValues[i]);
		if (reValues[i] < MINPVALUE) return 1;
	}

	double *revValues = malloc(sizeof(double)*20);
	double revSize = randomExcursionsVariant(bits, bitCount, revValues);

	for (i = 0; i < revSize; i++) {
		printf("random excursions variant test: %f\n", revValues[i]);
		if (revValues[i] < MINPVALUE) return 1;
	}
	
	double *serialValues = malloc(sizeof(double)*20);
	double serialSize = serial(bits, bitCount, 2, serialValues);

	for(i = 0; i < serialSize; i++){
		printf("serial  : %f\n", serialValues[i]);
		if (serialValues[i] < MINPVALUE) return 1;
	}

	printf("linear complexity test : %f\n", linearComplexity(bits, bitCount, 1000));	
	printf("nonOverlappingtemplate test whateasfj: %f\n", nonOverlappingTemplateMatchings(bits, bitCount, 9));
	printf("overlappingtemplate test: %f\n", overlappingTemplateMatchings(bits, bitCount, 9));
	
	if (linearComplexity(bits, bitCount, 1000) < MINPVALUE) return 1;
	if (nonOverlappingTemplateMatchings(bits, bitCount, 9) < MINPVALUE) return 1;
	if (overlappingTemplateMatchings(bits, bitCount, 9) < MINPVALUE) return 1;

	return 0;
}
