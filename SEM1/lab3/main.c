#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Tuple.h" 


#define ROW 150
#define COL 150
#define T 100
#define dP 0.05
#define SEED 1738114
typedef struct Tuple tuple;

void fill(char g[][COL], double p);
int startFire(char g[][COL]);
bool isEmpty(tuple x[ROW * COL]);
void enqueue(tuple q[ROW * COL], int qX, int qY, int time);
tuple dequeue(tuple q[ROW * COL]);



int qSize;
int timeCounter;
double P = 0.0;


double r() {
	return rand() * 1.0 / RAND_MAX;
}

int main() {
	srand(SEED);
	char grid[ROW][COL];

	for(double i=0;i<1.000000000001;i+=dP)
	{
		P = i;
		double step = 0;
		for(int j=0;j<T;j++){
			fill(grid, P);
			step = step + (double)(startFire(grid)) / ROW;
		}
		printf("%lf\n", step/T);

	}
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

int startFire(char g[][COL]) {
	int i, j;
	int burning = 0;


	for (i = 0; i < ROW; i++) {
		if (g[i][0] == 'T') {
			g[i][0] = '*';
		}
	}
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
			if (g[t.x - 1][t.y] == 'T') {
				visited[t.x - 1][t.y] = true;
				enqueue(queue, t.x - 1, t.y, t.z+1);
				g[t.x - 1][t.y] = '*';
			}
		}
		if (t.x < ROW - 1) {
			if (g[t.x + 1][t.y] == 'T') {
				visited[t.x + 1][t.y] = true;
				enqueue(queue, t.x + 1, t.y, t.z+1);
				g[t.x + 1][t.y] = '*';
			}
		}
		if (t.y > 0) {
			if (g[t.x][t.y - 1] == 'T') {
				visited[t.x][t.y - 1] = true;
				enqueue(queue, t.x, t.y - 1, t.z+1);
				g[t.x][t.y - 1] = '*';
			}
		}
		if (t.y < COL - 1 ) {
			if (g[t.x][t.y + 1] == 'T') {
				visited[t.x][t.y + 1] = true;
				enqueue(queue, t.x, t.y + 1, t.z+1);
				g[t.x][t.y + 1] = '*';
			}
		}
		if (t.z > prev.z) {
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
