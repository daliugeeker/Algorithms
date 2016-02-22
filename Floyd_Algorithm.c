/*
 * Floyd_Algorithm.c
 *
 *  Created on: Feb 17, 2016
 *      Author: liuda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 100000
#define MAXN 8

int  n; //The number of vertexes.
int Edge[MAXN][MAXN]; //The adjacency_List.
int A[MAXN][MAXN], path[MAXN][MAXN];

void Floyd() {
	int i, j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			A[i][j] = Edge[i][j];
			if (i != j && A[i][j] < INF)
				path[i][j] = i;
			else
				path[i][j] = -1;
		}
	}
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (k == i || k == j)
					continue;
				if (A[i][k] + A[k][j] < A[i][j]) {
					A[i][j] = A[i][k] + A[k][j];
					path[i][j] = path[k][j];
				}
			}
		}
	}
}

int main() {
	int i, j;
	int u, v, w; //Start, end vertexes and weight.
	printf("Please input the number of vertexes:\n");
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			Edge[i][j] = INF;
	}
	for (i = 0; i < n; i++) {
		Edge[i][i] = 0;
	}
	while (1) {
		printf("Please input the start vertex, end vertex, and weight:\n");
		scanf("%d%d%d", &u, &v, &w);
		if (u == -1 && v == -1 && w == -1)
			break;
		Edge[u][v] = w;
	}

	Floyd();
	int shortest[MAXN];
	int t;
	for (i = 0; i < n; i++) {
		for (j = 1; j <= n; j++) {
			if (i == j)
				continue; //Step ahead.
			printf("%d=>%d\t%d\t", i, j, A[i][j]);
			memset(shortest, 0, sizeof(shortest));
			int k = 0;
			shortest[k] = j;
			while (path[i][shortest[k]] != i) {
				k++;
				shortest[k] = path[i][shortest[k - 1]];
			}
			k++;
			shortest[k] = i;
			for (t = k; t > 0; t--)
				printf("%d-->", shortest[t]);
			printf("%d\n", shortest[0]);
		}
	}
	return 0;
}
