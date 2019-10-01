#include <stdlib.h>
#include <stdio.h>

int main(){
	int val = 50;
	int *ptr = &val;

	printf("Starting: %d\n",val);
	*ptr = *ptr + 1;
	printf("Ending: %d\n", val);
}
