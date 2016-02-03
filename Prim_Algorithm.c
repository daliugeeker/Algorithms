/*
 * Prim_Algorithm.c
 *
 *  Created on: Jan 11, 2016
 *      Author: liuda
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INF 1000000
#define MAXN 21 //The max number of vertexes
int n, m; //The number of vertexes and arcs.
int Edge[MAXN][MAXN]; //Adjacency matrix.
int lowcost[MAXN];
int nearvex[MAXN];

void prim(int u0) {
	int i, j; //Loop variables.
	int sumweight = 0;
	for (i = 1; i <= n; i++) {
		lowcost[i] = Edge[u0][i];
		nearvex[i] = u0;
	}
	nearvex[u0] = -1;
	for (i = 1; i <= n; i++) {
		int min = INF;
		int v = -1;
		for (j=1; j<=n; j++) {
			if (nearvex[j] != -1 && lowcost[j] < min){
				v = j;
				min = lowcost[j];
			}
		}
		if(v!=-1){
			printf("%2d%2d%2d\n",nearvex[v], v, lowcost[v]);
			nearvex[v] = -1;
			sumweight += lowcost[v];
			for(j=1; j<=n; j++){
				if(nearvex[j] != -1 && Edge[v][j]<lowcost[j]){
					nearvex[j] = v;
				}
			}
		}
	}
}

int main(){
	int i, j;
	int u, v, w;
	printf("Please input the number of vertexes and arcs:\n");
	scanf("%d%d",&n,&m);
	memset(Edge, 0, sizeof(Edge));
	for(i=0; i<m; i++){
			printf("Please input the start vertex, end vertex, and weight:\n");
			scanf("%d%d%d",&u, &v, &w);
			Edge[u][v] = Edge[v][u] = w;
	}
	for(i=0;i<m;i++){
		for(j=1; j<=n; j++){
			if(i == j)
				Edge[i][j] = 0;
			else if(Edge[i][j] == 0)
				Edge[i][j] = INF;
		}
	}
	prim(1);
	return 0;
}
