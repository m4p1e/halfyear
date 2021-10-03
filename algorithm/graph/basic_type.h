#include "linklist/singlylinklist.h"

typedef slinklist ** node_map;
typedef struct _graph_vertex_based_linklist
{
    void *attr;
    slinklist *adj_vertex;
}graph_vertex_based_linklist;

typedef struct _graph_via_linklist{
    slinklist *vertexs;
    graph_vertex_based_linklist **map;
    int vertexs_num;
}graph_via_linklist;


typedef struct _adj_vertex_based_linklist{
    graph_vertex_based_linklist *adj_node;
    graph_vertex_based_linklist *head;
    int mark;
    int weight;
}adj_vertex_based_linklist;

graph_via_linklist* create_graph_via_linklist();
graph_vertex_based_linklist *add_vertex_to_graph_via_linklist(graph_via_linklist* graph, void *vertex_attr);
graph_vertex_based_linklist *search_vertex_by_attr_of_graph_via_linklist(graph_via_linklist* graph, void *vertex_attr);
void add_edge_to_graph_via_linklist(graph_vertex_based_linklist *src, graph_vertex_based_linklist *dst, int weight);
void graph_via_linklist_printer(graph_via_linklist *graph, void (*node_printer)(void *attr), void (*edge_printer)(void *src, void *dst, int weight, int mark));
graph_via_linklist* transpose_graph_via_linklist(graph_via_linklist *graph);


typedef int* graph_via_matrix;