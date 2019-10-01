#include <stdlib.h>
#include <stdio.h>


void change(int *p){
	p[0] = 100;
}


int main(){
	int val[2] = {50,40};
	int *ptr = val;

	printf("Starting: %d\n",val[0]);
	change(ptr);
	printf("Ending: %d\n", val[0]);
}
