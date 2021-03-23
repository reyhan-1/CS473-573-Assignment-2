#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "minheap.h"

int main(int argc,char* argv[])
{
	int i, j, vertex_num, entry_count;
	int buffer_size = 2048;
	FILE* file;
	if (argc != 2)
	{
		printf("Command line argument requirements did not match");
		exit(1);
	}

	if ((file = fopen(argv[1], "r")) == NULL) {
		printf("File does not exist");
		// Program exits if file pointer returns NULL.
		exit(1);
    }

	char buff[buffer_size];

	if(fgets(buff, buffer_size, file) != NULL)
	{
		vertex_num = atoi(strtok(buff," "));
		strtok(NULL, " ");
		entry_count = atoi(strtok(NULL," "));
	}

	float graph[vertex_num][vertex_num];

	for(i = 0; i < vertex_num; i++)
	{
		for(j = 0; j < vertex_num; j++)
		{
			graph[i][j] = -1.0;	
		}
	}

	while (fgets(buff, buffer_size, file) != NULL)
	{
		int source = atoi(strtok(buff, " "));
		int destination = atoi(strtok(NULL, " "));
		graph[source - 1][destination - 1] = strtof(strtok(NULL, " "), NULL);
	}

	//print the graph
	/*
	for(i = 0; i < vertex_num; i++)
	{
		for(j = 0; j < vertex_num; j++)
		{
			printf("%.1f ", graph[i][j]);
		}
		printf("\n");
	}
	return 0;
	*/

	float dist[vertex_num];
	bool sptSet[vertex_num];

	for(i = 0; i < vertex_num; i++)
	{
		dist[i] = -1;
		sptSet[i] = false;
	}
	
	dist[0] = 0;
	sptSet[0] =  true;

	int current_vertex = 0;
	minheap h = minheap_create();
	bool finished;
	do
	{
		finished = true;
		for(i = 0; i < vertex_num; i++)
		{
			if(!sptSet[i] && graph[current_vertex][i] >= 0.0)
			{
				key_type item;
				item.id = i;
				item.distance = dist[current_vertex] + graph[current_vertex][i];
				//printf("Adding vertex: %d\n", i);
				minheap_insert(h,item);
			}
		}

		if(!minheap_is_empty(h))
		{
			key_type shortest = minheap_findmin(h);
			if(!sptSet[shortest.id])
			{
				dist[shortest.id] = shortest.distance;
				current_vertex = shortest.id;
				finished = false;
				sptSet[shortest.id] = true;
			}
			minheap_deletemin(h);
		}
	}while(!finished || !minheap_is_empty(h));

	file = fopen("a.txt", "w");
	for(i = 0 ; i < vertex_num; i++)
	{
		if(dist[i] >= 0.0)
			fprintf(file, "%.8f\n", dist[i]);
		else
			fprintf(file, "-1\n");
	}
	fclose(file);
	minheap_destroy(h);
	return 0;
}