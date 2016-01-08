/*
 * Kruskal_Algorithm.c
 *
 *  Created on: Jan 7, 2016
 *      Author: liuda
 */
#include <stdio.h>
#include <string.h>

#define MAXN 11 //The max number of vertexes.
#define MAXM 20 //The max number of arcs.

typedef struct edge {
	int u, v, w; //Start vertex, end vertex, and weight.
}Edge, edges[MAXM];

int parent[MAXN];
int n, m; //The number of vertexes and arcs.
int i, j; //Loop variables.

void UFset() {
	for (i = 0; i <= n; i++)
		parent[i] = -1;
}

int Find(int x){ //Find and return the root vertex of vertex x.
	int s;
	for(s=x;parent[s]>=0;s=parent[s]);
	while(s!=x){
		int temp = parent[x];
		parent[x] = s;
		x = temp;
	}
	return s;
}

void Union(int R1, int R2){
	int r1 = Find(R1), r2 = Find(R2);
	int tmp = parent[r1]+parent[r2]; //parent[r1] and parent[r2] are both negative numbers.

	if(parent[r1] > parent[r2]){ //If the number of vertexes in the Tree of R2, is larger than that of R1.
		parent[r1] = r2;
		parent[r2] = tmp;
	}
	else{
		parent[r2] = r1;
		parent[r1] = tmp;
	}
}

int cmp(const void *a, const void *b){ //Ranking
	Edge aa = *(const Edge *)a;
	Edge bb = *(const Edge *)b;
	return aa.w - bb.w;
}

void Kruskal(){
	int sumweight = 0;
	int num = 0;
	int u, v;
	UFset();
	for(i=0;i<m;i++){
		u = edges[i].u;
		v = edges[i].v;
		if(Find(u)!=Find(v)){
			printf("%2d%2d%2d\n", u, v, edges[i].w);
		}
	}
}

