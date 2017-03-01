#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../include/externs.h"
#include "../include/cephes.h"
#include <unistd.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                              R U N S  T E S T 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double runs(char bitsList[], int n)
{
	int		S, k;
	double	pi, V, erfc_arg, p_value;	
	
	S = 0;
	for ( k=0; k<n; k++ )
		if ( bitsList[k/8]>>(7-(k%8))&1)
			S++;
	pi = (double)S / (double)n;

	if ( fabs(pi - 0.5) > (2.0 / sqrt(n)) ) {
		p_value = 0.0;
	}
	else {

		V = 1;
		for ( k=1; k<n; k++ )
			if ( (bitsList[k/8]>>(7-(k%8))&1) != (bitsList[(k-1)/8]>>(7-( (k-1) %8))&1) )
				V++;
	
		erfc_arg = fabs(V - 2.0 * n * pi * (1-pi)) / (2.0 * pi * (1-pi) * sqrt(2*n));
		p_value = erfc(erfc_arg);	
		
	}
	return p_value;
}
