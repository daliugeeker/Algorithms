/*
 * Dijkstra_Algorithm.c
 *
 *  Created on: Jan 11, 2016
 *      Author: liuda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1000000
#define MAXN 20

int n; //The number of vertex
int Edge[MAXN][MAXN];
int S[MAXN]; //When "S[i]=0" means the vertex i has not been stored, else it has been stored.
int dist[MAXN]; //"dist[i]" means the shortest distance from vertex 0 to vertex i.
int path[MAXN]; //"path[i]" means the predecessor vertex of vertex i along the shortest path from vertex 0 to vertex i.

void Dijkstra(int v0) {
	int i, j, k; //loop variables.
	for (i = 0; i < n; i++) {
		dist[i] = Edge[v0][i];
		S[i] = 0;
		if (i != v0 && dist[i] < INF)
			path[i] = v0;
		else
			path[i] = -1;
	}
	S[v0] = 1;
	dist[v0] = 0; //Store vertex v0 into set S.
	for (i = 0; i < n - 1; i++) {
		int min = INF;
		int u = v0;
		for (j = 0; j < n; j++) {
			if (!S[j] && dist[j] < min) {
				u = j;
				min = dist[j];
			}
		}
		S[u] = 1; //Store vertex u into the S set, meaning its shortest path has been found.
		for (k = 0; k < n; k++) {
			if (!S[k] && Edge[u][k] < INF && dist[u] + Edge[u][k] < dist[k]) {
				dist[k] = dist[u] + Edge[u][k];
				path[k] = u;
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

	Dijkstra(0);

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
		for(j = k;j>0;j--)
			printf("%d->",shortest[j]);
		printf("%d\n",shortest[0]);
	}
	return 0;
}
