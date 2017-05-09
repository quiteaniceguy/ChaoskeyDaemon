/* Copyright © 2017 Tommy Eblen */
/* Run NIST 800-22 randomness test suite on a ChaosKey */

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
void runNistTests(char *bits, int bitCount);

int bitCount;
int raw=FALSE;
int chaoskeybits=FALSE;
///same as an array; a[0]=int bits, and every a[x] after is just a pointer to x past int bits
char *bits;

FILE *file;

int main(int argc, char *argv[]) {
	int argi=1;
	while( argi < argc ) {
		if( strcmp(argv[argi], "--chaoskeybits") == 0 ) {
			chaoskeybits=TRUE;				
			argi++;
		}
		else if( strcmp(argv[argi], "--raw") == 0 ) {
			raw=TRUE;
			argi++;
		}
		else if( (strcmp(argv[argi], "--nbytes") == 0) && argi+1 < argc ) {
			bitCount = atoi(argv[argi+1]);
			///doesn't return exactly 125000.... 
			bits=malloc(sizeof(int)*bitCount);
			
			bitCount*=8;
			argi+=2;
		}
                else {
			break;
		}
	}

	if( argi + 1 == argc ) {
		file=fopen(argv[argi], "r");
                if( !file ) {
                    perror("fopen");
                    return 1;
                }
	} else {
                fprintf(stderr, "usage: TestSuite [--raw] [--chaoskeybits] [--nbytes <nbytes>] [<file>]\n");
		return 1;
	}
	
	if( raw ) {
		printf("reading raw bits\n");
		if( rawBitReader(bits, file, bitCount) ) {
                        fprintf(stderr, "failed reading file\n");
			return 1;
		}
	} else {
		asciiBitFileReader(bits, file, bitCount);
	}

	printBytes(bits, bitCount);
	runNistTests(bits, bitCount);
        return 0;
}

void asciiBitFileReader(char *outputBits, FILE *inputFile, int bitCount) {
	char c;
	int nBitsRead=0;
	while( nBitsRead<bitCount && c != EOF ) {
		c=getc(inputFile);
		if( c-'0'==0 || c-'0'==1 ){
			outputBits[nBitsRead/8] |= (c-'0') << (7-(nBitsRead%8));		
			//printf("setting bit %i to %i\n", i/8, c-'0');
			nBitsRead++;
		}
	}
	
	if( nBitsRead<bitCount ) {	
                fprintf(stderr, "too few bits found (%i)\n", bitCount);
                exit(1);
	}
}

int rawBitReader(char *outputBits, FILE *inputFile, int bitCount) {
	//(ptr to array, size of each byte, number of bytes to read, inputfile)
	
	size_t result = fread(outputBits, 1,  bitCount/8, inputFile);
	return result!=bitCount/8;
}

void printBytes(char *bits, int bitCount) {
	int j, k;
	for( j=0; j<bitCount/8; j++ ){	
		for( k=0; k<8; k++ ){	
			printf("%i", (bits[j]>>k) & 1 );	
		}	
		printf("\n%i\n", j);	
	}
}

void testPValue(char *name, double val) {
    printf("%s test: %f\n", name, val);
    if( val < MINPVALUE ) {
        fprintf(stderr, "%s test failed: %f < %f\n", name, val, MINPVALUE);
        exit(1);
    }
}

void runNistTests(char *bits, int bitCount) {

	testPValue("frequency", frequency(bits, bitCount));
	testPValue("block", blockFrequency(bits, bitCount, 10)); 
	testPValue("runs", runs(bits, bitCount));
	testPValue("longest runs of ones", longestRunsOfOnes(bits, bitCount));
	testPValue("rank", rank(bits, bitCount));
	testPValue("discreteFourierTransform", discreteFourierTransform(bits, bitCount));
	testPValue("universal", universal(bits, bitCount));
	testPValue("approxEntropy", approximateEntropy(bits, bitCount, 2));

	int i;
	double *testValues=malloc(sizeof(double)*20);

	double cusumSize=cumulativeSums(bits, bitCount, testValues);
	for( i=0; i<cusumSize; i++ ) {
		testPValue("cumsum", testValues[i]);
	}

	double reSize=randomExcursions(bits, bitCount, testValues);
	for( i=0; i<reSize; i++ ) {
		testPValue("random excursions", testValues[i]);
	}

	double revSize=randomExcursionsVariant(bits, bitCount, testValues);
	for( i=0; i<revSize; i++ ) {
		testPValue("random excursions variant", testValues[i]);
	}

	double serialSize=serial(bits, bitCount, 2, testValues);
	for( i=0; i<serialSize; i++ ) {
		testPValue("serial", testValues[i]);
	}

	testPValue("linear complexity", linearComplexity(bits, bitCount, 1000));
	testPValue("nonoverlapping templates", nonOverlappingTemplateMatchings(bits, bitCount, 9));
	testPValue("overlapping templates", overlappingTemplateMatchings(bits, bitCount, 9));
}
