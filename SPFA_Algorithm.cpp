/*
 * SPFA_Algorithm.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: liuda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue>

#define INF 1000000
#define MAXN 10

using namespace std;

struct ArcNode {
	int to;
	int weight;
	ArcNode *next;
};

queue<int> Q;
int n; //The number of vertexes.
ArcNode * List[MAXN];
int inq[MAXN]; //The tag whether the vertex is in the queue.
int path[MAXN], dist[MAXN];

void SPFA(int src) {
	int i, u;
	ArcNode * temp;
	for (i = 0; i < n; i++) {
		dist[i] = INF;
		path[i] = src;
		inq[i] = 0;
	}
	dist[src] = 0;
	path[src] = src;
	inq[src]++;
	Q.push(src);
	while (!Q.empty()) {
		u = Q.front();
		Q.pop();
		inq[u]--;
		temp = List[u];
		while (temp != NULL) {
			int v = temp->to;
			if (dist[v] > dist[u] + temp->weight) {
				dist[v] = dist[u] + temp->weight;
				path[v] = u;
				if (!inq[v]) {
					Q.push(v);
					inq[v]++;
				}
			}
			temp = temp->next;
		}
	}
}

int main() {
	int i, j;
	int u, v, w; //Start, end vertexes and weight.
	printf("Please input the number of vertexes:\n");
	scanf("%d", &n);
	memset(List, 0, sizeof(List));
	ArcNode *temp;

	while (1) {
		printf("Please input the start vertex, end vertex, and weight:\n");
		scanf("%d%d%d", &u, &v, &w);
		if (u == -1 && v == -1 && w == -1)
			break;
		temp = new ArcNode;
		temp->to = v;
		temp->weight = w;
		temp->next = NULL;
		if (List[u] == NULL)
			List[u] = temp;
		else {
			temp->next = List[u];
			List[u] = temp;
		}
	}

	SPFA(0);

	for (j = 0; j < n; j++) {
		temp = List[j];
		while (temp != NULL) {
			List[j] = temp->next;
			delete temp;
			temp = List[j];
		}
	}
	int shortest[MAXN];

	for (i = 1; i < n; i++) {
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
