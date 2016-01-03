/*
 * Adjacency_List_Operations.c
 *
 *  Created on: Jan 2, 2016
 *      Author: DA LIU
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXN 100

typedef struct ArcNode { //Edge Node
	int adjvex; //The order number of the other adjacent vertex on the same directed edge.
	struct ArcNode * nextarc;
//The pointer pointing to the next edge node.
};

typedef struct VNode { //Vertex
	int data; //Vertex data
	struct ArcNode * headout;
	struct ArcNode * headin;
//List head pointer of in edge
};

typedef struct LGragh { //The adjacency list structure of the graph.
	struct VNode vertexs[MAXN]; //
	int vexnum, arcnum; //The number of vertex and edges.
};

struct LGragh lg; //Declaring a graph

void CreateLG() { //Employing the adjacency list, creating directed graph.
	int i; //Loop variable
	struct ArcNode * pi; //Edge node pointer for building the edges list
	int v1, v2; //The vertexes of directed edges.

	for (i = 0; i < lg.vexnum; i++) //Initialize the list head pointers to be NULL;
		lg.vertexs[i].headin = lg.vertexs[i].headout = NULL;
	for (i = 0; i < lg.arcnum; i++) {
		scanf("%d%d", &v1, &v2); //Input the start and end vertexes of an edge
		v1--;
		v2--;
		pi = (struct ArcNode*) malloc(sizeof(struct ArcNode));
		pi->adjvex = v2;
		pi->nextarc = lg.vertexs[v1].headout; //Insert the edge into the list
		lg.vertexs[v1].headout = pi;

		pi = (struct ArcNode*) malloc(sizeof(struct ArcNode));
		pi->adjvex = v1;
		pi->nextarc = lg.vertexs[v2].headin; //Insert the edge into the list
		lg.vertexs[v2].headin = pi;
	} //Loop ends
} //CreateLG ends

void DeleteLG() {
	int i; //Loop variable
	struct ArcNode * pi = (struct ArcNode*) malloc(sizeof(struct ArcNode));; //Pointing to the edge nodes in the edges list

	for (i = 0; i < lg.vexnum; i++) {
		pi = (struct ArcNode*) malloc(sizeof(struct ArcNode));
		pi = lg.vertexs[i].headout; //Release the edge nodes of out edges list for vertex i
		while (pi != NULL) {
			lg.vertexs[i].headout = pi->nextarc;
			free(pi);
			pi = lg.vertexs[i].headout;
		}

		pi = lg.vertexs[i].headin; //Release the edge nodes of in edges list for vertex i
		while(pi != NULL){
			lg.vertexs[i].headin = pi->nextarc;
			free(pi);
			pi = lg.vertexs[i].headin;
		}
	}
}

int main() {
	int i; //Loop variable
	int id, od; //The in and out degree of vertexes
	struct ArcNode * pi = (struct ArcNode*) malloc(sizeof(struct ArcNode)); //The pointer for traversing edge nodes in the edge list

	while (1) {
		lg.vexnum = lg.arcnum = 0;
		scanf("%d%d", &lg.vexnum, &lg.arcnum);
		if (lg.vexnum == 0)
			break;
		CreateLG();
		for (i = 0; i < lg.vexnum; i++) { //Statistic out degrees of each vertexes and print out
			od = 0;
			pi = lg.vertexs[i].headout;
			while (pi != NULL) {
				od++;
				pi = pi->nextarc;
			}
			if (i == 0)
				printf("%d", od);
			else
				printf("%d", od);
		}
		printf("\n");

		for (i = 0; i < lg.vexnum; i++) { //Statistic in degrees of each vertexes and print out
			id = 0;
			pi = lg.vertexs[i].headin;
			while (pi != NULL) {
				id++;
				pi = pi->nextarc;
			}
			if (i == 0)
				printf("%d", id);
			else
				printf("%d", id);
		}
		printf("\n");
		DeleteLG();
	}
	return 0;
}
