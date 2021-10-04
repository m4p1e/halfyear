#include "basic_type.h"
#include "hashtable/simple.h"
#include "helper/helper.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h> 
#include<queue/min_queue.h>
#include<disjoint_set/basic_type.h>
#include<queue/basic_type.h>

#define SIMPLE_VERTEX(_v)  ((simple_vertex *)(_v->attr))
#define SIMPLE_VERTEX_DTIME(_v) (((simple_vertex *)(_v->attr))->discovered_time)
#define SIMPLE_VERTEX_FTIME(_v) (((simple_vertex *)(_v->attr))->finished_time)
#define SIMPLE_VERTEX_PI(_v) (((simple_vertex *)(_v->attr))->parent) 
#define SIMPLE_SHORTEST_PATH(_v) (((simple_vertex *)(_v->attr))->shortest_path) 
#define SIMPLE_TOPOLOGICAL_ORDER(_v) (((simple_vertex *)(_v->attr))->topological_order)
#define SIMPLE_STRONGLY_CONNECTED_COMPONENT(_v) (((simple_vertex *)(_v->attr))->strongly_connected_component)
#define SIMPLE_DISJOINT_SET(_v) (((simple_vertex *)(_v->attr))->disjoint_set_e)
#define SIMPLE_MINIMUM_EDGE_WEIGHT(_v) (((simple_vertex *)(_v->attr))->minimum_edge_weight)
#define SIMPLE_QUEUE_E(_v) (((simple_vertex *)(_v->attr))->queue_e)
#define SIMPLE_KEYWORD(_v) (((simple_vertex *)(_v->attr))->keyword)
#define SIMPLE_SHORTEST_PATH_UPPER_BOUND(_v) (((simple_vertex *)(_v->attr))->shortest_path_upper_bound)
#define SET_SIMPLE_VERTEX_PI(_v, _pi) ((simple_vertex *)(_v->attr))->parent = _pi
#define SET_SIMPLE_VERTEX_DTIME(_v, _t)  ((simple_vertex *)(_v->attr))->discovered_time = _t
#define SET_SIMPLE_VERTEX_FTIME(_v, _t)  ((simple_vertex *)(_v->attr))->finished_time = _t
#define SET_SIMPLE_SHORTEST_PATH(_v, _p)  ((simple_vertex *)(_v->attr))->shortest_path = _p
#define SET_SIMPLE_TOPOLOGICAL_ORDER(_v, _o)  ((simple_vertex *)(_v->attr))->topological_order = _o
#define SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(_v, _c)  ((simple_vertex *)(_v->attr))->strongly_connected_component = _c
#define SET_SIMPLE_DISJOINT_SET(_v,_s) ((simple_vertex *)(_v->attr))->disjoint_set_e = _s
#define SET_SIMPLE_MINIMUM_EDGE_WEIGHT(_v,_w) ((simple_vertex *)(_v->attr))->minimum_edge_weight = _w
#define SET_SIMPLE_QUEUE_E(_v,_e) ((simple_vertex *)(_v->attr))->queue_e = _e
#define SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(_v,_b) ((simple_vertex *)(_v->attr))->shortest_path_upper_bound = _b

int time=0;
int order=0;

typedef struct _simple_vertex{
    char *keyword;
    struct _simple_vertex *parent;
    int discovered_time;
    int finished_time;
    int shortest_path;
    int topological_order;
    int strongly_connected_component;
    disjoin_set_element *disjoint_set_e;
    Bnode *queue_e;
    int minimum_edge_weight;
    int shortest_path_upper_bound;
}simple_vertex;

simple_vertex *create_simple_vertex(char *keyword){
    simple_vertex *v;
    v = malloc(sizeof(simple_vertex));
    v->keyword = keyword;
    v->discovered_time = 0;
    v->finished_time = 0;
    v->shortest_path = 0;
    v->topological_order = 0;
    v->strongly_connected_component = 0;
    v->disjoint_set_e = NULL;
    v->queue_e = NULL;
    v->minimum_edge_weight = INT32_MAX;
    v->shortest_path_upper_bound = INT32_MAX;
    return v;
}

graph_via_linklist *unserialize(char *vertexs, char *edges, hashtable *ht, int is_directed){
    graph_via_linklist *G;
    graph_vertex_based_linklist *src, *dst;
    simple_vertex *vertex;
    int weight;
    
    char *p,*q, *keyword;
    
    q = vertexs;
    p = q;
    G = create_graph_via_linklist();

    for( ; *p != '\0' ; p++){
        if(*p == ','){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';
            vertex = create_simple_vertex(keyword);
            hashtable_insert(ht,keyword, p-q+1, add_vertex_to_graph_via_linklist(G, vertex));
            q = p+1;
        }
    }

    //the last
    keyword = malloc(p-q+1);
    memcpy(keyword,q,p-q);
    *(keyword + (p-q)) = '\0';
    vertex = create_simple_vertex(keyword);
    hashtable_insert(ht,keyword, p-q+1, add_vertex_to_graph_via_linklist(G, vertex));

    q = edges;
    p = q;
    for( ; *p != '\0' ; p++){
        if(*p == '>'){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';
            src = hashtable_find(ht, keyword, p-q+1);
            free(keyword);
            q = p+1;
        }else if(*p == ':'){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';  
            dst = hashtable_find(ht, keyword, p-q+1);
            free(keyword);
            q = p+1;
        }else if(*p == ','){
            weight = str2int(q, p-q);
            add_edge_to_graph_via_linklist(src, dst, weight);
            if(!is_directed){
              add_edge_to_graph_via_linklist(dst, src, weight);  
            }
            q = p+1;
        }
    }

    weight = str2int(q, p-q);
    add_edge_to_graph_via_linklist(src, dst, weight);
    if(!is_directed){
        add_edge_to_graph_via_linklist(dst, src, weight);  
    }

    return G;
}

void dfs_visit(graph_via_linklist *G, graph_vertex_based_linklist *vertex, int is_topological_order, stack *decreasingly_finished_time_stack, int strongly_connected_component_order){
    slinklist_node *adj_vertex_linked_node;
    graph_vertex_based_linklist *adj_vertex;
    
    if(strongly_connected_component_order > 0){
        SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(vertex, strongly_connected_component_order);
    }

    adj_vertex_linked_node = slinklist_first(vertex->adj_vertex);
    while(adj_vertex_linked_node){
        adj_vertex = ((adj_vertex_based_linklist *)(adj_vertex_linked_node->attr))->adj_node;
        if(!SIMPLE_VERTEX_DTIME(adj_vertex)){
            SET_SIMPLE_VERTEX_PI(adj_vertex, SIMPLE_VERTEX(vertex));
            SET_SIMPLE_VERTEX_DTIME(adj_vertex, ++time);
            if(strongly_connected_component_order > 0){
                SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(adj_vertex, strongly_connected_component_order);
            }
            dfs_visit(G, adj_vertex, is_topological_order, decreasingly_finished_time_stack, strongly_connected_component_order);
        }
        adj_vertex_linked_node = adj_vertex_linked_node->next;
    }

    if(is_topological_order){
        SET_SIMPLE_TOPOLOGICAL_ORDER(vertex, order);
        order--;
    }

    if(decreasingly_finished_time_stack){
        stack_push(decreasingly_finished_time_stack, vertex);
    }

    SET_SIMPLE_VERTEX_FTIME(vertex, ++time);
}

void dfs(graph_via_linklist *G, graph_vertex_based_linklist *source, int is_topological_order, stack *decreasingly_finished_time_stack){
    slinklist_node *vertex_linked_node;
    time = 0;
    if(is_topological_order){
        order = G->vertexs_num;
    }

    SET_SIMPLE_VERTEX_DTIME(source, ++time);
    dfs_visit(G, source, is_topological_order, decreasingly_finished_time_stack, 0);
    for(int i = 0; i < G->vertexs_num; i++){
        if(!SIMPLE_VERTEX_DTIME(G->map[i])){
            SET_SIMPLE_VERTEX_DTIME(G->map[i], ++time);
            dfs_visit(G, G->map[i], is_topological_order, decreasingly_finished_time_stack, 0);
        }
    }
}

/*non-cyclic graph*/
void toplogical_order(graph_via_linklist *G, stack *decreasingly_finished_time_stack){
    graph_vertex_based_linklist *source;
    if(G->vertexs_num  == 0){
        return;
    }

    source = G->map[0];
    dfs(G, source, 1, decreasingly_finished_time_stack);
}

void node_printer(simple_vertex *v){
    printf("%s(%d/%d; parent = %s; shortest_path = %d; topological_order = %d; strongly_connected_component= %d; shortest_path_upper_bound= %d)", v->keyword, v->discovered_time, v->finished_time, v->parent ? v->parent->keyword : "null", v->shortest_path, v->topological_order, v->strongly_connected_component, v->shortest_path_upper_bound);
}

void edge_printer(simple_vertex *src, simple_vertex *dst, int weight, int mark){
    printf("%s:%d; minimum_spanning_edge=%d", dst->keyword, weight, mark);
}

int kruskal_comapre_edge_weight(adj_vertex_based_linklist *v, adj_vertex_based_linklist *u){
    return v->weight - u->weight;
}

void kruskal_decrease_edge_weight(adj_vertex_based_linklist *v, int weight){
    v->weight = weight;
}

void kruskal_minimum_spanning_tree(graph_via_linklist *G){
    int i,sum;
    min_queue *Q;
    slinklist_node *adj_vertex_linked_node;
    graph_vertex_based_linklist *adj_vertex;
    adj_vertex_based_linklist *edge;
    sum = 0;

    Q = create_min_queue(kruskal_comapre_edge_weight, kruskal_decrease_edge_weight);   
    for(i = 0; i < G->vertexs_num; i++){
        SET_SIMPLE_DISJOINT_SET(G->map[i], disjoint_set_make_set(G->map[i]));
        adj_vertex_linked_node = slinklist_first(G->map[i]->adj_vertex);
        while(adj_vertex_linked_node){
            min_queue_insert(Q, adj_vertex_linked_node->attr);
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }
    }

    while(min_queue_size(Q) != 0){
        edge = min_queue_extract_min(Q);
        if(disjoin_set_find(SIMPLE_DISJOINT_SET(edge->head)) != disjoin_set_find(SIMPLE_DISJOINT_SET(edge->adj_node))){
            edge->mark = 1;
            sum = sum + edge->weight;
            disjoint_set_union(SIMPLE_DISJOINT_SET(edge->head), SIMPLE_DISJOINT_SET(edge->adj_node));
        }
    }

    //printf("weight_sum: %d\n", sum);
}

int prim_comapre_edge_weight(graph_vertex_based_linklist *v, graph_vertex_based_linklist *u){
    return SIMPLE_MINIMUM_EDGE_WEIGHT(v) - SIMPLE_MINIMUM_EDGE_WEIGHT(u);
}

void prim_decrease_edge_weight(graph_vertex_based_linklist *v, int weight){
    SET_SIMPLE_MINIMUM_EDGE_WEIGHT(v, weight);
}

void min_queue_node_printer(graph_vertex_based_linklist *v){
    printf("%s(%d)", SIMPLE_KEYWORD(v), SIMPLE_MINIMUM_EDGE_WEIGHT(v));
}

void prim_min_queue_reverse_binding(Bnode *node, graph_vertex_based_linklist *v){
    SET_SIMPLE_QUEUE_E(v, node);
}

void prim_minimum_spanning_tree(graph_via_linklist *G){
    min_queue *Q;
    int i, sum;
    graph_vertex_based_linklist *vertex;
    slinklist_node *adj_vertex_linked_node;
    graph_vertex_based_linklist *adj_vertex;
    adj_vertex_based_linklist *edge;
    
    sum = 0;
    Q = create_min_queue(prim_comapre_edge_weight, prim_decrease_edge_weight);
    min_queue_set_reverse_binding(Q, prim_min_queue_reverse_binding);
    for(i = 0; i < G->vertexs_num; i++)
    {   
        SET_SIMPLE_MINIMUM_EDGE_WEIGHT(G->map[i], INT32_MAX);
        min_queue_insert(Q, G->map[i]);
        SET_SIMPLE_VERTEX_PI(G->map[i], NULL);
    }

    //min_queue_printer(Q, min_queue_node_printer);

    //设置root， decrease之后树的结点可能改变
    min_queue_decrease_key(Q, SIMPLE_QUEUE_E(G->map[0]), 0);
    while(min_queue_size(Q) != 0){
        vertex = min_queue_extract_min(Q);
        
        sum = sum + SIMPLE_MINIMUM_EDGE_WEIGHT(vertex);
        adj_vertex_linked_node = slinklist_first(vertex->adj_vertex);
        while(adj_vertex_linked_node){
            edge = (adj_vertex_based_linklist*)(adj_vertex_linked_node->attr);
            adj_vertex = edge->adj_node;
            if(SIMPLE_QUEUE_E(adj_vertex) && edge->weight < SIMPLE_MINIMUM_EDGE_WEIGHT(adj_vertex)){
                min_queue_decrease_key(Q, SIMPLE_QUEUE_E(adj_vertex), edge->weight);
                SET_SIMPLE_VERTEX_PI(adj_vertex, SIMPLE_VERTEX(vertex));
            }
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }        
    }

    //printf("weight_sum: %d\n", sum);
}

int relax(adj_vertex_based_linklist *edge){

    //overflow check
    if(SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->head) == INT32_MAX && edge->weight > 0){
        return 0;
    }

    if(SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->head)+edge->weight < SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->adj_node)){
        SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->adj_node, SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->head)+edge->weight);
        SET_SIMPLE_VERTEX_PI(edge->adj_node, SIMPLE_VERTEX(edge->head));
        return 1;
    }

    return 0;
}

void bellman_ford_shortest_path(graph_via_linklist *G, graph_vertex_based_linklist *source){
    int i, j;
    slinklist_node *adj_vertex_linked_node;
    adj_vertex_based_linklist *edge;
    for (i = 0; i < G->vertexs_num; i++)
    {
        SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(G->map[i], INT32_MAX);
        SET_SIMPLE_VERTEX_PI(G->map[i], NULL);
    }

    SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(source, 0);

    for (i = 0; i < G->vertexs_num-1; i++)
    {
        for (j = 0; j < G->vertexs_num; j++)
        {
            adj_vertex_linked_node = slinklist_first(G->map[j]->adj_vertex);
            while (adj_vertex_linked_node)
            {
                relax(adj_vertex_linked_node->attr);
                adj_vertex_linked_node = adj_vertex_linked_node->next;
            }
        }
    }

    //负环判断    
    for (j = 0; j < G->vertexs_num; j++)
    {
        adj_vertex_linked_node = slinklist_first(G->map[j]->adj_vertex);
        while (adj_vertex_linked_node)
        {
            edge = adj_vertex_linked_node->attr;
            if(SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->head)+edge->weight < SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->adj_node)){
                printf("exists negative weight cycle!");
                exit(0);
            }
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }
    }
}

void dag_shortest_path(graph_via_linklist *G, graph_vertex_based_linklist *source){
    stack *decreasingly_finished_time_stack;
    graph_vertex_based_linklist *vertex;
    slinklist_node *adj_vertex_linked_node;
    adj_vertex_based_linklist *edge;
    int i;

    decreasingly_finished_time_stack = create_stack();
    toplogical_order(G, decreasingly_finished_time_stack);

    for (i = 0; i < G->vertexs_num; i++)
    {
        SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(G->map[i], INT32_MAX);
        SET_SIMPLE_VERTEX_PI(G->map[i], NULL);
    }

    SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(source, 0);

    while(!stack_is_empty(decreasingly_finished_time_stack)){
        vertex = stack_pop(decreasingly_finished_time_stack);
        adj_vertex_linked_node = slinklist_first(vertex->adj_vertex);
        while (adj_vertex_linked_node)
        {
            relax(adj_vertex_linked_node->attr);
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }
    }
}

int dijkstra_comapre_shortest_upper_bound(graph_vertex_based_linklist *v, graph_vertex_based_linklist *u){
    return SIMPLE_SHORTEST_PATH_UPPER_BOUND(v) - SIMPLE_SHORTEST_PATH_UPPER_BOUND(u);
}

void dijkstra_decrease_shortest_upper_bound(graph_vertex_based_linklist *v, int ub){
    SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(v, ub);
}

void dijkstra_min_queue_reverse_binding(Bnode *node, graph_vertex_based_linklist *v){
    SET_SIMPLE_QUEUE_E(v, node);
}

void dijkstra_shortest_path(graph_via_linklist *G, graph_vertex_based_linklist *source){
    min_queue *Q;
    graph_vertex_based_linklist *vertex;
    slinklist_node *adj_vertex_linked_node;
    adj_vertex_based_linklist *edge;
    int i;
    Q = create_min_queue(dijkstra_comapre_shortest_upper_bound, dijkstra_decrease_shortest_upper_bound);
    min_queue_set_reverse_binding(Q,dijkstra_min_queue_reverse_binding);
    for(i = 0; i < G->vertexs_num; i++)
    {   
        SET_SIMPLE_SHORTEST_PATH_UPPER_BOUND(G->map[i], INT32_MAX);
        min_queue_insert(Q, G->map[i]);
        SET_SIMPLE_VERTEX_PI(G->map[i], NULL);
    }

    min_queue_decrease_key(Q, SIMPLE_QUEUE_E(source), 0);

    while(min_queue_size(Q) != 0){
        vertex = min_queue_extract_min(Q);
        adj_vertex_linked_node = slinklist_first(vertex->adj_vertex);
        while(adj_vertex_linked_node){
            edge = adj_vertex_linked_node->attr;
            if(SIMPLE_QUEUE_E(edge->adj_node)){
                if(relax(edge)){
                    min_queue_decrease_key(Q, SIMPLE_QUEUE_E(edge->adj_node), SIMPLE_SHORTEST_PATH_UPPER_BOUND(edge->adj_node));
                }
            }
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }
    }
}

int main(){
    setbuf(stdout,NULL);
    graph_via_linklist *G;
    graph_vertex_based_linklist *source;
    hashtable *ht;
    ht = create_hashtable();
    char *undirected_vertexs = "a,b,c,d,e,f,g,h,i";
    char *undirected_edges = "a-b:4,a-h:8,b-c:8,b-h:11,c-d:7,c-f:4,c-i:2,d-e:9,d-f:14,e-f:10,f-g:2,g-i:6,g-h:1,h-i:7";
    char *directed_cyclic_vertexs = "s,t,x,y,z";
    char *directed_cyclic_edges = "s>t:6,s>y:7,t>x:5,t>y:8,t>z:-4,x>t:-2,z>x:7,z>s:2,y>z:9,y>x:-3";
    char *directed_noncyclic_vertexs = "r,s,t,x,y,z";
    char *directed_noncyclic_edges = "r>s:5,r>t:3,s>t:2,s>x:6,t>x:7,t>y:4,t>z:2,x>y:-1,x>z:1,y>z:-2";
    char *directed_positive_vertexs = "s,t,x,y,z";
    char *directed_positive_edges = "s>t:10,s>y:5,t>x:1,t>y:2,x>z:4,y>x:9,y>z:2,y>t:3,z>s:7,z>x:6";
    G = unserialize(directed_positive_vertexs, directed_positive_edges, ht, 1);
    source = hashtable_find(ht, "s", sizeof("s"));
    //kruskal_minimum_spanning_tree(G);
    //prim_minimum_spanning_tree(G);
    //bellman_ford_shortest_path(G, source);
    dijkstra_shortest_path(G, source);
    graph_via_linklist_printer(G,node_printer,edge_printer);
    return 0;
}