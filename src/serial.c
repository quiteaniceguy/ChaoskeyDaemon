#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../include/externs.h"
#include "../include/cephes.h"  

double psi2(int m, int n, char bits[]);

double serial(char bits[], int n, int m, double *returnValues)
{
	double psim0, psim1, psim2, del1, del2;
	double p_values[2];
	
	
	
	

	psim0 = psi2(m, n, bits);
	psim1 = psi2(m-1, n, bits);
	psim2 = psi2(m-2, n, bits);
	del1 = psim0 - psim1;
	del2 = psim0 - 2.0*psim1 + psim2;

	p_values[0] = cephes_igamc(pow(2, m-1)/2, del1/2.0);
	p_values[1] = cephes_igamc(pow(2, m-2)/2, del2/2.0);

		
	
	returnValues[0]=p_values[0];
	returnValues[1]=p_values[1];	
	
	return 2;


}

double
psi2(int m, int n, char bits[])
{
	
		
	int				i, j, k,q,powLen;
	double			sum, numOfBlocks;
	unsigned int	*P;
	
	if ( (m == 0) || (m == -1) )
		return 0.0;
	numOfBlocks = n;
	powLen = (int)pow(2, m+1)-1;

	if ( (P = (unsigned int*)calloc(powLen,sizeof(unsigned int)))== NULL ) {	
		return 0.0;
	}
	
	for ( i=1; i<powLen-1; i++ )
		P[i] = 0;
		  
	for ( i=0; i<numOfBlocks; i++ ) {		   
		k = 1;
		for ( j=0; j<m; j++ ) {
			
			q=((i+j)%n);
			if ( ((bits[q/8]>>(7-(q%8)))&1) == 0 )
				k *= 2;
			else if ( ((bits[q/8]>>(7-(q%8)))&1) == 1 )
				k = 2*k+1;
			
			
		}
		P[k-1]++;
	}
	
	sum = 0.0;
	for ( i=(int)pow(2, m)-1; i<(int)pow(2, m+1)-1; i++ )
		sum += pow(P[i], 2);
	sum = (sum * pow(2, m)/(double)n) - (double)n;
	
	
	return sum;
	
	
	
}
