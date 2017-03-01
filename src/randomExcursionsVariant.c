#include <stdio.h> 
#include <math.h> 
#include <string.h>
#include <stdlib.h>
#include "../include/externs.h"
#include "../include/cephes.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
            R A N D O M  E X C U R S I O N S  V A R I A N T  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double randomExcursionsVariant(char bits[], int n, double *returnValue)
{
	int		i, p, J, x, count, *S_k;
	int		stateX[18] = { -9, -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9 };


	if ( (S_k = (int *)calloc(n, sizeof(int))) == NULL ) {
		return 0;
	}

	J = 0;
	S_k[0] = 2*(bits[0]>>7) - 1;
	for ( i=1; i<n; i++ ) {
		S_k[i] = S_k[i-1] + 2*((bits[i/8])>>(7-(i%8))&1) - 1;
		if ( S_k[i] == 0 )
			J++;
	}

	if ( S_k[n-1] != 0 )
		J++;
	
	
	
	for ( p=0; p<=17; p++ ) {
		x = stateX[p];
		count = 0;
		for ( i=0; i<n; i++ )
			if ( S_k[i] == x )
				count++;
		returnValue[p] = erfc(fabs(count-J)/(sqrt(2.0*J*(4.0*fabs(x)-2))));
		

	}
	
	free(S_k);

	return 17; 

	

}
