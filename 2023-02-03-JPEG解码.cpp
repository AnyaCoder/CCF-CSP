#include <bits/stdc++.h>
// http://118.190.20.162/view.page?gpid=T158
using namespace std;

int Q[8][8], M[8][8];
double Mp[8][8];
int n;
int T;
int A[101];

void input();
void fill_M();
void print_M();
void print_Mp();
void mul_M_with_Q();
void DCT();
void Norm();
const double PI = acos(-1);
const double inv_sq2 = 1.0 / sqrtl(2);

#define alpha(x) (x == 0 ? inv_sq2 : 1)

int main() {

	input();
	fill_M();
	if (T == 0) {
		print_M();
		return 0;
	}
	mul_M_with_Q();
	//print_M();
	if (T == 1) {
		print_M();
		return 0;
	}
	DCT();
	//print_Mp();
	Norm();
	if (T == 2) {
		print_M();
	}
	
	return 0;
}
void Norm() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			M[i][j] = floor(Mp[i][j] + 128 + 0.5);
			M[i][j] = M[i][j] > 255 ? 255 : M[i][j];
			M[i][j] = M[i][j] < 0 ? 0 : M[i][j];
		}
	}
} 
void DCT() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			double res = 0;
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					res += alpha(u) * alpha(v) * M[u][v] * cos(PI / 8 * (i + 1.0/2) * u) * cos(PI / 8 * (j + 1.0/2) * v);
				}
			}
			Mp[i][j] = res / 4;
		}
	}
}

void mul_M_with_Q() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			M[i][j] *= Q[i][j];
		}
	}
}
void fill_M() {
	int x = 0, y = 0, cnt = 0;
	do {
		if (y < 7) {
			M[x][y] = A[cnt++];
			y++;
		}
		while (x < 7 && y > 0) {
			if ((x & 1) && y == 7) break;
			M[x][y] = A[cnt++];
			x++;
			y--;
		}
		
		if (x < 7) {
			M[x][y] = A[cnt++];
			x++;
		}

		while (x > 0 && y < 7) {
			if (x == 7 && y % 2 == 0) break;
			M[x][y] = A[cnt++];
			x--;
			y++;
		}

	}
	while (!(x == 7 && y == 7));
	M[7][7] = A[cnt++];
	// print_M();
	// printf("(x: %d, y: %d)\n", x, y);
}
void print_Mp() {
	for (int i = 0; i < 8; i++) {
		printf("%.2f", Mp[i][0]);
		for (int j = 1; j < 8; j++) {
			printf(" %.2f", Mp[i][j]);
		}
		puts("");
	}
}
void print_M() {
	for (int i = 0; i < 8; i++) {
		printf("%d", M[i][0]);
		for (int j = 1; j < 8; j++) {
			printf(" %d", M[i][j]);
		}
		puts("");
	}
}
void input() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			scanf("%d", &Q[i][j]);
		}
	}
	scanf("%d", &n);
	scanf("%d", &T);
	for (int i = 0; i < n; i++) {
		scanf("%d", &A[i]);
	}
}

/*
16 11 10 16 24 40 51 61
12 12 14 19 26 58 60 55
14 13 16 24 40 57 69 56
14 17 22 29 51 87 80 62
18 22 37 56 68 109 103 77
24 35 55 64 81 104 113 92
49 64 78 87 103 121 120 101
72 92 95 98 112 100 103 99
26
2
-26 -3 0 -3 -2 -6 2 -4 1 -3 1 1 5 1 2 -1 1 -1 2 0 0 0 0 0 -1 -1


/*
0 1 2 3 4 5 6 7
8 9 10 11 12 13 14 15
16 17 18 19 20 21 22 23
24 25 26 27 28 29 30 31
32 33 34 35 36 37 38 39 
40 41 42 43 44 45 46 47
48 49 50 51 52 53 54 55
56 57 58 59 60 61 62 63
64
0
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 
40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63


*/
