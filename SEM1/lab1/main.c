#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 30
#define COL 40
#define SEED 1738114

double r() {
	return rand() * 1.0 / RAND_MAX;
}

void fill(char g[][COL], double p);
void show(char g[][COL]);
int numOn(char g[][COL]);
int leftOn(char g[][COL]);
int nextOn(char g[][COL]);
int adjacent(char g[][COL]);
double run100();
void writeToFile(FILE *f, int o, double p, int fi, int s, int a, int r);
void sendToPrinter(char printer[]);


int main(int argc, char **argv) {
	srand(1738114);
	char grid[ROW][COL];


	if (argc == 1) {
		printf("Usage ./<filename> <printer_name>\n");
		printf("Error: Missing Printer Name\n");
		printf("Pass With --no-print flag to bypass\n");
		return 1;
	}
	FILE *f = fopen("file.txt", "w");
	if (f == NULL) {
		printf("Error Opening file\n");
		return 1;
	}

	fill(grid, 0.60);
	show(grid);

	int on = numOn(grid);
	double percentOn = (on / ((double)ROW * COL)) * 100;
	int firstOn = leftOn(grid);
	int secondOn = nextOn(grid);
	int adj = adjacent(grid);
	double run = run100();

	writeToFile(f, on, percentOn, firstOn, secondOn, adj, run * 100);
	if (strcmp(argv[1], "--no-print") != 0) {
		sendToPrinter(argv[1]);
	}

	return 0;

}

void fill(char g[][COL], double p ) {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if (r() < p) {
				g[i][j] = 'M';
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

int numOn(char g[][COL]) {
	int i, j;
	int count = 0;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if (g[i][j] == 'M') {
				count++;
			}
		}
	}
	return count;
}

int leftOn(char g[][COL]) {
	int count = 0;
	int i;
	for (i = 0; i < ROW; i++) {
		if (g[i][0] == 'M') {
			count++;
		}
	}
	return count;
}

int nextOn(char g[][COL]) {
	int count = 0;
	int i;
	for (i = 0; i < ROW; i++) {
		if (g[i][1] == 'M') {
			count++;
		}
	}
	return count;
}

int adjacent(char g[][COL]) {
	int count = 0;
	int i;
	for (i = 0; i < ROW; i++) {
		if (g[i][0] == 'M' && g[i][1] == 'M') {
			count++;
		}
	}
	return count;
}

double run100() {
	double avg = 0.0;
	int i;
	for (i = 0; i < 100; i++) {
		char temp[ROW][COL];
		fill(temp, 0.60);
		avg += (numOn(temp) / ((double)ROW * COL));
	}
	return avg / 100;
}

void writeToFile(FILE *f, int o, double p, int fi, int s, int a, int r) {
	fprintf(f, "Print your name: %s\n\n", "Anup Bagali");
	fprintf(f, "Today's date: %s\n\n", "09/03/2019");
	fprintf(f, "Class Period: %d\n\n", 3);
	fprintf(f, "-----------------------------------------------------\n\n");
	fprintf(f, "1. Use your student ID as the random number seed.\n\n");
	fprintf(f, "2. Initialize a grid 30 rows -by- 40 columns.\n\n");
	fprintf(f, "3. Each slot has a %s chance to be turned ON.\n\n", "60%");
	fprintf(f, "4. How many slots are actually turned ON?\n");
	fprintf(f, "\t%d are turned ON.\n\n", o);
	fprintf(f, "5. What percent of the 1200 total slots is this?\n");
	fprintf(f, "\t%f%% are turned ON.\n\n", p);
	fprintf(f, "6. How many of the 30 slots in the left column are on?\n");
	fprintf(f, "\t%d slots in column 0 are turned ON.\n\n", fi);
	fprintf(f, "7. How many of the 30 slots in the next column are on?\n");
	fprintf(f, "\t%d slots in column 1 are turned ON.\n\n", s);
	fprintf(f, "8. Of the on-slots in column #2 how many are adjacent to an on-slot from column #1?\n");
	fprintf(f, "\t%d slots have adjacent slots that are also turned ON.\n\n", a);
	fprintf(f, "9. Initialize 100 grids instead of just one.\n\n");
	fprintf(f, "10.What is the average percent from Question #5 over all of these grids?\n");
	fprintf(f, "\t%lf%%\n\n", r);
	fprintf(f, "-----------------------------------------------------\nEND");
	fclose(f);

}

void sendToPrinter(char printer[]) {
	char command[150];
	strcpy(command, "lpr -P ");
	strcat(command, printer);
	strcat(command, " file.txt");
	system(command);
	//printf("%s\n",command );
}