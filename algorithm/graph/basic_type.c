/*
the operation on graph based linklist and matrix
*/

#include "basic_type.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


graph_via_linklist* create_graph_via_linklist(){
    slinklist_node *vertex;
    graph_via_linklist *graph = malloc(sizeof(graph_via_linklist));

    graph->vertexs = create_slinklist();
    graph->map = NULL;
    graph->vertexs_num = 0;

    return graph;
}

graph_vertex_based_linklist *add_vertex_to_graph_via_linklist(graph_via_linklist* graph, void *vertex_attr){
    graph_vertex_based_linklist *vertex;

    if(search_vertex_by_attr_of_graph_via_linklist(graph,vertex_attr)){
        printf("can't insert same vectex twice");
        exit(1);     
    }

    vertex = malloc (sizeof(graph_vertex_based_linklist));
    vertex->adj_vertex = create_slinklist();
    vertex->attr = vertex_attr;
    slinklist_add(graph->vertexs, vertex);

    //这里不用hashtable， 退而求其次用线性表. 
    if(graph->map == NULL){ 
        graph->map = malloc(8*sizeof(graph_vertex_based_linklist *));
        memset(graph->map, '\0', 8*sizeof(graph_vertex_based_linklist *));
    }else{
        if(graph->vertexs_num > 7 &&  (graph->vertexs_num & (graph->vertexs_num-1)) == 0){ //*2扩容. 
            graph->map = realloc(graph->map, graph->vertexs_num*2*sizeof(graph_vertex_based_linklist *));
        }
    }

    graph->map[graph->vertexs_num] = vertex; 
    graph->vertexs_num++;

    return vertex;
}

graph_vertex_based_linklist *search_vertex_by_attr_of_graph_via_linklist(graph_via_linklist* graph, void *vertex_attr){
    for(int i = 0; i < graph->vertexs_num; i++){
        if(graph->map[i]->attr == vertex_attr){
            return graph->map[i];
        }
    }

    return NULL;
}

int get_map_position_of_graph_via_linklist(graph_via_linklist* graph, graph_vertex_based_linklist *vertex){
    for(int i = 0; i < graph->vertexs_num; i++){
        if(graph->map[i] == vertex){
            return i;
        }
    }

    return -1;
}

graph_vertex_based_linklist *search_vertex_by_custom_func_of_graph_via_linklist(graph_via_linklist* graph, int (*search)(void *attr)){
    for(int i = 0; i < graph->vertexs_num; i++){
        if(search(graph->map[i]->attr)){
            return graph->map[i];
        }
    }

    return NULL;
}


void add_edge_to_graph_via_linklist(graph_vertex_based_linklist *src, graph_vertex_based_linklist *dst, int weight){
    adj_vertex_based_linklist *adj_vertex;
    adj_vertex = malloc(sizeof(adj_vertex_based_linklist));
    adj_vertex->adj_node = dst;
    adj_vertex->weight = weight;
    adj_vertex->head = src;
    adj_vertex->mark = 0;
    slinklist_add(src->adj_vertex, adj_vertex);
}


void graph_via_linklist_printer(graph_via_linklist *graph, void (*node_printer)(void *attr), void (*edge_printer)(void *src, void *dst, int weight, int mark)){
    slinklist_node *vertex = slinklist_first(graph->vertexs);
    slinklist_node *adj_vertex;
    while(vertex){
        node_printer(((graph_vertex_based_linklist *)(vertex->attr))->attr);
        printf(" : ");
        adj_vertex = slinklist_first(((graph_vertex_based_linklist *)(vertex->attr))->adj_vertex);
        while(adj_vertex){
            edge_printer(((graph_vertex_based_linklist *)(vertex->attr))->attr,((adj_vertex_based_linklist *)(adj_vertex->attr))->adj_node->attr, ((adj_vertex_based_linklist *)(adj_vertex->attr))->weight, ((adj_vertex_based_linklist *)(adj_vertex->attr))->mark);
            printf(", ");
            adj_vertex = slinklist_next(adj_vertex); 
        }
        printf("\n");
        vertex = slinklist_next(vertex);
    }
}

graph_via_linklist *transpose_graph_via_linklist(graph_via_linklist *graph){
    graph_via_linklist *transposed_graph;
    slinklist_node *adj_linked_node;
    int i;
    transposed_graph = create_graph_via_linklist();

    for(i = 0; i < graph->vertexs_num; i++){
        add_vertex_to_graph_via_linklist(transposed_graph, graph->map[i]->attr);
    }


    for(i = 0; i < graph->vertexs_num; i++){
        adj_linked_node = slinklist_first(graph->map[i]->adj_vertex);
        while (adj_linked_node)
        {
            add_edge_to_graph_via_linklist(transposed_graph->map[get_map_position_of_graph_via_linklist(graph, ((adj_vertex_based_linklist *)(adj_linked_node->attr))->adj_node)], transposed_graph->map[i], ((adj_vertex_based_linklist *)(adj_linked_node->attr))->weight);    
            adj_linked_node = adj_linked_node->next;
        }
    }

    return transposed_graph;
}