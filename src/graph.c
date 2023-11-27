#include "../include/graph.h"

void cycle_negatif(const uint32_t* s, bool* verbose, const uint32_t* nb_nodes, result_bellman_ford_t* result_bf) {
    if (*verbose) {
        fprintf(stderr, "Cycle négatif détecté\n");
    }
    for (uint32_t i = 0; i < *nb_nodes; i++) {
        (result_bf->dist)[i] = INT64_MAX;
    }
    (result_bf->dist)[*s] = 0;
    memset(result_bf->path, -1, (*nb_nodes)*sizeof(uint32_t));
}

int8_t bellman_ford(const uint32_t* s, bool* verbose, graph_t* graphe, result_bellman_ford_t* result_bf) {
    // Initialisation
    result_bf->source=*s;
    result_bf->dist = (int64_t*)malloc((graphe->NB_NODES)*sizeof(int64_t));
    if (result_bf->dist == NULL) {
        fprintf(stderr, "Pas assez de mémoire disponible");
        return -1;
    }
    result_bf->path = (uint32_t*)malloc((graphe->NB_NODES)*sizeof(uint32_t));
    if (result_bf->path == NULL) {
        fprintf(stderr, "Pas assez de mémoire disponible");
        free(result_bf->dist);
        return -1;
    }

    // Initialisation des coûts à l'infini pour tous les noeuds
    for (uint32_t i = 0; i < graphe->NB_NODES; i++) {
        (result_bf->dist)[i] = INT64_MAX; // INT64_MAX, de la librairie stdint.h, représente le plus grand entier non signé sur 64 bits (similaire à l'infini).
    }
    (result_bf->dist)[*s] = 0; // Le noeud source est à distance 0 de lui-même

    // Initialisation du tableau de précédence
    memset(result_bf->path, -1, (graphe->NB_NODES)*sizeof(uint32_t));

    uint32_t node_from, node_to;
    int32_t cost;

    // Itération de Bellman-Ford
    for (uint32_t _ = 0; _ < ((graphe->NB_NODES)-1); _++) {
        bool modif = false;
        for (uint32_t j = 0; j < graphe->NB_LINKS; j++) {
            node_from = ((graphe->links)[j])->from;
            node_to = ((graphe->links)[j])->to;
            cost = ((graphe->links)[j])->cost;
            if (((result_bf->dist)[node_from] != INT64_MAX) && ((result_bf->dist)[node_to] > ((result_bf->dist)[node_from] + cost))) {
                (result_bf->dist)[node_to] = (result_bf->dist)[node_from] + cost;
                (result_bf->path)[node_to] = node_from;
                modif = true;
            }
        }
        if (result_bf->dist[*s] < 0) {cycle_negatif (s, verbose, &graphe->NB_NODES, result_bf); return 0;} // Si la distance au noeud source est négative, c'est qu'il y a un cycle négatif.
        if (!modif) {return 0;} // Si aucune modification n'a été apportée lors de cette itération, il n'y aura plus aucune modification aprés et on peut arrêter les itérations.
    }

    // Détection de cycles négatifs
    for (uint32_t j = 0; j < graphe->NB_LINKS; j++) {
        node_from = ((graphe->links)[j])->from;
        node_to = ((graphe->links)[j])->to;
        cost = ((graphe->links)[j])->cost;
        if (((result_bf->dist)[node_from] != INT64_MAX) && ((result_bf->dist)[node_to] > ((result_bf->dist)[node_from] + cost))) {
            cycle_negatif (s, verbose, &graphe->NB_NODES, result_bf);
            return 0; // Un cycle négatif ayant été détecté, il ne faut plus continuer la recherche.
        }
    }
    return 0;
}

int8_t get_path(const uint32_t* dest, uint32_t* path, const uint32_t* source, result_get_path_t* result_gp) {
    // Première boucle pour calculer la longueur du chemin
    result_gp->length_path = 0;
    uint32_t i = *dest;
    while(i != *source){
        i = path[i];
        result_gp->length_path++;
    }
    result_gp->length_path++; // On prend en compte le noeud source dans le chemin.

    result_gp->r = (uint32_t*)malloc((result_gp->length_path)*sizeof(uint32_t));
    if (result_gp->r == NULL) {
        free(path);
        fprintf(stderr, "Pas assez de mémoire disponible");
        return -1;
    }
    i = *dest;
    uint32_t j = result_gp->length_path - 1;

    // Seconde boucle pour remplir le tableau r
    while (i != *source) {
        (result_gp->r)[j] = i; //on rajoute de droite à gauche dans r
        j--;
        i = path[i];
    }
    free(path); // On n'aura plus besoin de cette variable après.
    (result_gp->r)[0] = *source;
    return 0;
}


void get_max(const int64_t* dist, const uint32_t* s, graph_t* graphe, result_get_max_t* result_gm) {
    result_gm->max_cost = -INT64_MAX;
    result_gm->max_node = *s;
    // Recherche du chemin de coût maximal:
    for (uint32_t node_idx = 0; node_idx<graphe->NB_NODES; node_idx++){
        if (node_idx != *s && dist[node_idx] != INT64_MAX && dist[node_idx] >= result_gm->max_cost){
            if (dist[node_idx] > result_gm->max_cost || result_gm->max_node == *s || node_idx < result_gm->max_node) {
                result_gm->max_cost = dist[node_idx];
                result_gm->max_node = node_idx;
            }
        }
    }

    if (result_gm->max_cost == -INT64_MAX){
        if (dist[*s] != INT64_MAX && dist[*s] >= result_gm->max_cost) {
            result_gm->max_cost = dist[*s];
        }
    }
}
