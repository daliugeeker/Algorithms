/*
 * Bellman-Ford_Algorithm.c
 *
 *  Created on: Jan 28, 2016
 *      Author: liuda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1000000
#define MAXN 8

int n; // The number of vertexes.
int Edge[MAXN][MAXN]; //Adjacency Matrix.
int dist[MAXN]; //"dist[i]" means the shortest distance from vertex 0 to vertex i.
int path[MAXN]; //"path[i]" means the predecessor vertex of vertex i along the shortest path from vertex 0 to vertex i.

void Bellman(int v0) { //Seek the shortest path from vertex v0 to other vertexes.
	int i, j, k, u; //Loop variables.
	for (i = 0; i < n; i++) { //Initialize the dist[i].
		dist[i] = Edge[v0][i];
		if (i != v0 && dist[i] < INF)
			path[i] = v0;
		else
			path[i] = -1;
	}
	for (k = 2; k < n; k++) {
		for (u = 0; u < n; u++) {
			if (u != v0) {
				for (j = 0; j < n; j++) {
					if (Edge[j][u] < INF && dist[j] + Edge[j][u] < dist[u]) {
						dist[u] = dist[j] + Edge[j][u];
						path[u] = j;
					}
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
	while (1) {
		printf("Please input the start vertex, end vertex, and weight:\n");
		scanf("%d%d%d", &u, &v, &w);
		if (u == -1 && v == -1 && w == -1)
			break;
		Edge[u][v] = w;
	}
	for (i = 0; i < n; i++) {
		for (j = 1; j <= n; j++) {
			if (i == j)
				Edge[i][j] = 0;
			else if (Edge[i][j] == 0)
				Edge[i][j] = INF;
		}
	}

	Bellman(0);

	int shortest[MAXN];
	for (i = 0; i < n; i++) {
		printf("%d\t", dist[i]);
		memset(shortest, 0, sizeof(shortest));
		int k = 0;
		shortest[k] = i;
		while (path[shortest[k]] != 0) {
			k++;
			shortest[k] = path[shortest[k - 1]];
		}
		k++;
		shortest[k] = 0;
		for (j = k; j > 0; j--)
			printf("%d->", shortest[j]);
		printf("%d\n", shortest[0]);
	}
	return 0;
}
