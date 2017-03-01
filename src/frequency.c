#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../include/externs.h"
#include <unistd.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                          F R E Q U E N C Y  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double frequency(char bits[], int nBits){
	int i;
	double	f, s_obs, p_value, sum, sqrt2 = 1.41421356237309504880; 

	sum = 0.0;

	for ( i=0; i<nBits; i++ ){
		
		sum += 2*((bits[i/8]>>(7-(i%8)))&1)-1;	
		//printf("bit value: %i and sum is %f\n", 2*((bits[i/8]>>(7-(i%8)))&1)-1, sum);
	}		
			
	
	s_obs = fabs(sum)/sqrt(nBits);
	f = s_obs/sqrt2;
	p_value = erfc(f);
	//printf("frequency test returns: %f with sum of %f\n", p_value, sum);
	return p_value;
}
