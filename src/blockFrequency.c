#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../include/externs.h"
#include "../include/cephes.h"
#include <unistd.h>


/* * * * * * * * In "classic" C language (C89/90) when you call an undeclared function, C assumes that it returns an int and also attempts to derive the types of its parameters from the types of the actual arguments (no, it doesn't assume that it has no parameters, as someone suggested before).* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                    B L O C K  F R E Q U E N C Y  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double blockFrequency(char bits[], int n, int M){
	int		i, j, N, blockSum;
	double	p_value, sum, pi, v, chi_squared;
	
	N = n/M; 		/* # OF SUBSTRING BLOCKS      */
	sum = 0.0;

	
	

	for ( i=0; i<N; i++ ) {
		blockSum = 0;
		for ( j=0; j<M; j++ )
			blockSum += bits[(j+i*M)/8]>>(7-((j+i*M)%8))&1;
		pi = (double)blockSum/(double)M;
		v = pi - 0.5;
		sum += v*v;
	}
	chi_squared = 4.0 * M * sum;
	p_value = cephes_igamc(N/2.0, chi_squared/2.0);
	//printf("p_value from source is: %f and sum is %f\n", p_value, sum);

	return p_value;

	
}
