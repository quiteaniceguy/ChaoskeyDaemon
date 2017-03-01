#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "func_def.h"
#define FALSE 0
#define TRUE 1

void asciiBitFileReader(char *outputBits, FILE *inputFile, int bitCount);
void rawBitReader(char *outputbits, FILE *inputFile, int bitCount);
void printBytes(char *bits, int bitCount);

int bitCount;
int raw=FALSE;
int chaoskeybits=FALSE;
///same as an array; a[0]=int bits, and every a[x] after is just a pointer to x past int bits
char *bits;

FILE *file;

int main(int argc, char *argv[]){
	int argi=1;
	while(argi<argc){
		if( strcmp(argv[argi], "--chaoskeybits") == 0 ){
			chaoskeybits=TRUE;				
			argi++;
		}
		else if( strcmp(argv[argi], "--raw") == 0 ){
			raw=TRUE;
			argi++;
		}
		else if( (strcmp(argv[argi], "--nbytes") == 0) && argi+1 < argc){
			bitCount = atoi(argv[argi+1]);
			///doesn't return exactly 125000.... 
			bits=malloc(sizeof(int)*bitCount);
			
			bitCount*=8;
			argi+=2;
		}else{
			break;
		}
		
	}

		
	if(argi+1==argc){
		file=fopen(argv[argi], "r");
		
	}else{
		printf("usage: TestSuite [--raw] [--chaoskeybits] [--nbytes <nbytes>] [<file>]\n");
		return 1;
	}
	
	if(raw){
		rawBitReader(bits, file, bitCount);
		printf("reading raw bits\n");
	}else{
		asciiBitFileReader(bits, file, bitCount);
	}

	printBytes(bits, bitCount);
	printf("frequency test: %f\n", frequency(bits, bitCount));
	printf("block test: %f\n", blockFrequency(bits, bitCount, 10)); 
	printf("runs test: %f\n", runs(bits, bitCount));
	printf("longest runs of ones test: %f\n", longestRunsOfOnes(bits, bitCount));
	printf("rank test: %f\n", rank(bits, bitCount));
	printf("discreteFourierTransform test: %f\n", discreteFourierTransform(bits, bitCount));
	printf("universal Test: %f\n", universal(bits, bitCount));
	printf("approxEntropy test: %f\n", approximateEntropy(bits, bitCount, 2));
		
	double *cusumValues=malloc(sizeof(double)*20);
	double cusumSize=cumulativeSums(bits, bitCount, cusumValues);

	int i;
	for(i=0;i<cusumSize;i++){
		printf("cumsum return value %f\n", cusumValues[i]);
	}
	



	double *reValues=malloc(sizeof(double)*20);
	double reSize=randomExcursions(bits, bitCount, reValues);

	for(i=0; i<reSize; i++){
		printf("random excursions test: %f\n", reValues[i]);
	}



	double *revValues=malloc(sizeof(double)*20);
	double revSize=randomExcursionsVariant(bits, bitCount, revValues);

	for(i=0; i<3; i++){
		printf("random excursions variant test: %f\n", revValues[i]);
	}
	



	double *serialValues=malloc(sizeof(double)*20);
	double serialSize=serial(bits, bitCount, 2, serialValues);

	for(i=0; i<serialSize; i++){
		printf("serial  : %f\n", serialValues[i]);
	}

	printf("linear complexity test : %f\n", linearComplexity(bits, bitCount, 1000));
	
	printf("nonOverlappingtemplate test whateasfj: %f\n", nonOverlappingTemplateMatchings(bits, bitCount, 9));

	printf("overlappingtemplate test: %f\n", overlappingTemplateMatchings(bits, bitCount, 9));
	
		
	
	return 0;		
}
void asciiBitFileReader(char *outputBits, FILE *inputFile, int bitCount){
	char c;
	int nBitsRead=0;
	while(nBitsRead<bitCount && c != EOF){
		c=getc(inputFile);
		if( c-'0'==0 || c-'0'==1){
			outputBits[nBitsRead/8] |= (c-'0') << (7-(nBitsRead%8));		
			//printf("setting bit %i to %i\n", i/8, c-'0');
			nBitsRead++;
		}
	}
	
	if(nBitsRead<bitCount){	
		printf("too few bits found, changed bit count to %i\n", bitCount);
	}
}

void rawBitReader(char *outputBits, FILE *inputFile, int bitCount){
	//(ptr to array, size of each byte, number of bytes to read, inputfile)
	fread(outputBits, 1,  bitCount/8, inputFile);
	

}
void printBytes(char *bits, int bitCount){
	int j, k;
	for(j=0; j<bitCount/8; j++){	
		for(k=0; k<8; k++){	
			printf("%i", (bits[j]>>k) & 1 );	
		}	
		printf("\n%i\n", j);	
	}
	//printf("byte Count %i" , bitCount/8);
}

