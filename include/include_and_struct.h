#ifndef LEPL1503__INCLUDE__
#define LEPL1503__INCLUDE__

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link {
    uint32_t from;
    uint32_t to;
    int32_t cost;
} link_t;

typedef struct graph {
    uint32_t NB_NODES;
    uint32_t NB_LINKS;
    link_t** links; // tableau de référence vers des liens
} graph_t;

typedef struct result_bellman_ford {
    uint32_t source;
    int64_t* dist;
    uint32_t* path;
} result_bellman_ford_t;

typedef struct result_get_path {
    uint32_t length_path;
    uint32_t* r;
} result_get_path_t;

typedef struct result_get_max {
    int64_t max_cost;
    uint32_t max_node;
} result_get_max_t;

#endif // LEPL1503__INCLUDE__