#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Tuple.h"


#define ROW 40
#define COL 40
#define T 100
#define dP 0.05
#define SEED 1738114
typedef struct Tuple tuple;

void fill(char g[][COL], double p);
void show(char g[][COL]);
int startFire(char g[][COL]);
int spread(char g[][COL]);
void writeToFile(FILE *f);
bool isEmpty(tuple x[ROW * COL]);
void enqueue(tuple q[ROW * COL], int qX, int qY, int time);
tuple dequeue(tuple q[ROW * COL]);
tuple peek(tuple q[ROW * COL]);
bool hasVisited(bool v[ROW][COL], int x, int y);
void printArr(tuple a[ROW * COL]);



int qSize;
int timeCounter;
double P = 0.0;


double r() {
	return rand() * 1.0 / RAND_MAX;
}

void delay(int s) {
	int ms = 100 * s;
	clock_t start = clock();
	while (clock() < start + ms);
}


int main() {
	srand(SEED);
	char grid[ROW][COL];
	FILE *f = fopen("output.txt", "w");
	FILE *df = fopen("data.txt","w");
	if (f == NULL || df == NULL) {
		printf("Error Opening file\n");
		return 1;
	}

	printf("Start fire\n");
	for(double i=0;i<1.000000000001;i+=dP)
	{
		P = i;
		fill(grid, P);
		show(grid);
		printf("I%lf\n",i);
		double step = (double)(startFire(grid)) / ROW;
		fprintf(df, "%lf\n", step);
	}
	//printf("Steps: %d/%d\n",step, COL );
	writeToFile(f);

}

void fill(char g[][COL], double p ) {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if (r() < p) {
				g[i][j] = 'T';
			} else {
				g[i][j] = ' ';
			}
		}
	}
}

void show(char g[][COL]) {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			printf("%c", g[i][j]);
		}
		printf("\n");
	}
}

int startFire(char g[][COL]) {
	int i, j;
	int burning = 0;


	for (i = 0; i < ROW; i++) {
		if (g[i][0] == 'T') {
			g[i][0] = '*';
		}
	}
	return spread(g);
}


void writeToFile(FILE *f) {
	fprintf(f, "Print your name: Anup Bagali\n\n\nToday's date: 09/05/19\n\n\nClass period: 3\n\n\n-----------------------------------------------------\n1. Initialize a grid M rows -by- N columns.\n2. Each slot has a P%% chance to be turned ON.\n3. At time zero IGNITE the on-slots in the left column.\n4. Then count the number of steps it takes to BURNOUT.\n5. At each timestep spread to the four nearest neighbors.\n6. Do not include diagonal neighbors.\n7. Normalize the final count by dividing by the width.\n8. Average the normalized burnout time over T trials.\n9. Plot the average step counts for inputs 0 < P < 100.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n10. Report the random number seed, M, N, T, and delta P.\nM: %d, N: %d, T: %d, delta P: %lf\n\n\n\n\n-----------------------------------------------------\nEND\n", ROW, COL, T, dP );
}

int spread(char g[][COL]) {
	tuple queue[ROW * COL];

	bool visited[ROW][COL];
	int step = 0;

	for (int i = 0; i < ROW * COL; i++) {
		queue[i].x = -1;
		queue[i].y = -1;
		queue[i].z = -1;
	}
	for (int i = 0; i < ROW; i++) {
		if (g[i][0] == '*') {
			visited[i][0] = true;
			timeCounter = 0;
			enqueue(queue, i, 0, timeCounter);

		}
	}

	tuple prev;
	while (!isEmpty(queue)) {
		tuple t = dequeue(queue);
		g[t.x][t.y] = ' ';

		if (t.x > 0) {
			if (g[t.x - 1][t.y] == 'T' && !hasVisited(visited, t.x - 1, t.y)) {
				visited[t.x - 1][t.y] = true;
				enqueue(queue, t.x - 1, t.y, t.z+1);
				g[t.x - 1][t.y] = '*';
			}
		}
		if (t.x < ROW - 1) {
			if (g[t.x + 1][t.y] == 'T' && !hasVisited(visited, t.x + 1, t.y)) {
				visited[t.x + 1][t.y] = true;
				enqueue(queue, t.x + 1, t.y, t.z+1);
				g[t.x + 1][t.y] = '*';
			}
		}
		if (t.y > 0) {
			if (g[t.x][t.y - 1] == 'T' && !hasVisited(visited, t.x, t.y - 1)) {
				visited[t.x][t.y - 1] = true;
				enqueue(queue, t.x, t.y - 1, t.z+1);
				g[t.x][t.y - 1] = '*';
			}
		}
		if (t.y < COL - 1 ) {
			if (g[t.x][t.y + 1] == 'T' && !hasVisited(visited, t.x, t.y + 1)) {
				visited[t.x][t.y + 1] = true;
				enqueue(queue, t.x, t.y + 1, t.z+1);
				g[t.x][t.y + 1] = '*';
			}
		}
		if (t.z > prev.z) {
//			system("clear");
//			show(g);
//			printf("%d/%d\n", step, COL);
//			printf("-----------------\n");
//			delay(2000);
			step++;
		}
		prev = t;
	}
	return step;
}


bool isEmpty(tuple x[ROW * COL]) {
	return qSize == 0;
}

void enqueue(tuple q[ROW * COL], int qX, int qY, int t) {
	//printf("ENQUEUE: (%d,%d)\n", qX, qY );
	q[qSize].x = qX;
	q[qSize].y = qY;
	q[qSize].z = t;
	qSize++;
}

tuple dequeue(tuple q[ROW * COL]) {
	tuple ret = q[0];
	qSize--;
	for (int i = 0; i < ROW * COL; i++) {
		q[i] = q[i + 1];
	}
	return ret;
}

tuple peek(tuple q[ROW * COL]) {
	return q[0];
}

bool hasVisited(bool v[ROW][COL], int x, int y) {
	return false; 
}

void printArr(tuple a[ROW * COL]) {
	for (int i = 0; i < ROW * COL; i++) {
		if (a[i].x != -1 && a[i].y != -1) {
			printf("(%d,%d),", a[i].x, a[i].y );
		}
	}
	printf("\n");
}
