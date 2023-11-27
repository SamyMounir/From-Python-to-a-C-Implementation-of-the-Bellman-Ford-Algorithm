#include "../include/file.h"

void free_links(graph_t* graphe, const uint32_t* links_allocated) {
    for (uint32_t j = 0; j < *links_allocated; j++) {
        free((graphe->links)[j]);
    }
    free(graphe->links);
}

int8_t close_file_in(FILE* file_in) {
    if(fclose(file_in) == EOF){
        fprintf(stderr, "Erreur lors de la fermeture du fichier d'entrée");
        return -1;
    }
    return 0;
}

int8_t read_graph(FILE* file_in, graph_t* graphe){
    uint32_t* nb_nodes_and_links = (uint32_t*)malloc(2*sizeof(uint32_t));
    if(nb_nodes_and_links == NULL){
        fprintf(stderr, "Pas assez de mémoire disponible");
        close_file_in(file_in);
        return -1;
    }

    if (2 != fread(nb_nodes_and_links, 4, 2, file_in)){
        fprintf(stderr, "Erreur lors de la lecture du fichier d'entrée");
        free(nb_nodes_and_links);
        close_file_in(file_in);
        return -1;
    }

    // Nombre de noeuds et d'arêtes
    graphe->NB_NODES = (uint32_t) be32toh(nb_nodes_and_links[0]); 
    graphe->NB_LINKS = (uint32_t) be32toh(nb_nodes_and_links[1]);
    free(nb_nodes_and_links);

    // Tableau de liens
    graphe->links = (link_t**)malloc((graphe->NB_LINKS)*sizeof(link_t*));
    if(graphe->links == NULL){
        fprintf(stderr, "Pas assez de mémoire disponible");
        close_file_in(file_in);
        return -1;
    }
    for (uint32_t i = 0; i < graphe->NB_LINKS; i++) {
        (graphe->links)[i] = (link_t*)malloc(sizeof(link_t));
        if ((graphe->links)[i] == NULL) {
            fprintf(stderr, "Pas assez de mémoire disponible");
            free_links(graphe, &i);
            close_file_in(file_in);
            return -1;
        }
    }

    // Remplissage
    uint32_t from;
    uint32_t to;
    int32_t cost;

    for(uint32_t i = 0; i<graphe->NB_LINKS; i++){
        if(1 != fread(&from, 4, 1, file_in)){
            fprintf(stderr, "Erreur lors de la lecture du fichier d'entrée");
            free_links(graphe, &(graphe->NB_NODES));
            close_file_in(file_in);
            return -1;
        }
        ((graphe->links)[i])->from = (uint32_t) be32toh(from);
        if(1 != fread(&to, 4, 1, file_in)){
            fprintf(stderr, "Erreur lors de la lecture du fichier d'entrée");
            free_links(graphe, &(graphe->NB_NODES));
            close_file_in(file_in);
            return -1;
        }
        ((graphe->links)[i])->to = (uint32_t) be32toh(to);
        if(1 != fread(&cost, 4, 1, file_in)){
            fprintf(stderr, "Erreur lors de la lecture du fichier d'entrée");
            free_links(graphe, &(graphe->NB_NODES));
            close_file_in(file_in);
            return -1;
        }
        ((graphe->links)[i])->cost = (int32_t) be32toh(cost);
    }

    if(close_file_in(file_in) != 0){
        free_links(graphe, &(graphe->NB_NODES));
        return -1;
    }
    return 0;
}

void show(FILE* stream_out, const uint32_t* nb_nodes, result_bellman_ford_t* result_bf, result_get_max_t* result_gm, result_get_path_t* result_gp) {
    fprintf(stream_out, "source : %" PRIu32 "\n", result_bf->source);
    fprintf(stream_out, "Distances : [");
    for (uint32_t i = 0; i < *nb_nodes; i++) {
        if (result_bf->dist[i] == INT64_MAX) {
            printf(" infini");
        }
        else{
            printf(" %" PRIi64 "", (result_bf->dist)[i]);
        }
    }
    free(result_bf->dist); free(result_bf); // On n'utilisera plus ces variables plus tard donc on peut déjà libérer la mémoire.
    printf(" ]\n");

    fprintf(stream_out, "\tdestination : %" PRIu32 "\n", result_gm->max_node);
    fprintf(stream_out, "\tcout : %" PRIi64 "\n", result_gm->max_cost);
    free(result_gm); // On n'utilisera plus cette variable plus tard donc on peut déjà libérer la mémoire.
    fprintf(stream_out, "\tnombre de noeuds : %" PRIu32 "\n", result_gp->length_path);
    printf("\tchemin :");
    for (uint32_t i = 0; i < result_gp->length_path; i++) {
        fprintf(stream_out, " %" PRIu32 "", result_gp->r[i]);
    }
    free(result_gp->r); free(result_gp); // On n'utilisera plus ces variables plus tard donc on peut déjà libérer la mémoire.          
    printf("\n");
}

int8_t write_file(FILE* file_out, result_bellman_ford_t* result_bf, result_get_max_t* result_gm, result_get_path_t* result_gp) {
    free(result_bf->dist); // On n'utilisera plus cette variable plus tard donc on peut déjà libérer la mémoire.
    uint32_t num_source = htobe32(result_bf->source);
    free(result_bf); // On n'utilisera plus cette variable plus tard donc on peut déjà libérer la mémoire.
    if(1 != fwrite(&num_source, 4, 1, file_out)){
        fprintf(stderr, "Erreur lors de l'écriture du fichier de sortie");
        free(result_gm); free(result_gp->r); free(result_gp);
        return -1;
    }

    uint32_t max_node = htobe32(result_gm->max_node);
    if(1 != fwrite(&max_node, 4, 1, file_out)){
        fprintf(stderr, "Erreur lors de l'écriture du fichier de sortie");
        free(result_gm); free(result_gp->r); free(result_gp);
        return -1;
    }
    uint64_t max_cost = htobe64(result_gm->max_cost);
    free(result_gm); // On n'utilisera plus cette variable plus tard donc on peut déjà libérer la mémoire.
    if(1 != fwrite(&max_cost, 8, 1, file_out)){
        fprintf(stderr, "Erreur lors de l'écriture du fichier de sortie");
        free(result_gp->r); free(result_gp);
        return -1;
    }

    uint32_t length_path = htobe32(result_gp->length_path);
    if(1 != fwrite(&length_path, 4, 1, file_out)){
        fprintf(stderr, "Erreur lors de l'écriture du fichier de sortie");
        free(result_gp->r); free(result_gp);
        return -1;
    }
    uint32_t* r = (uint32_t*)malloc((result_gp->length_path)*sizeof(uint32_t));
    if (r == NULL) {
        fprintf(stderr, "Pas assez de mémoire disponible");
        free(result_gp->r); free(result_gp);
        return -1;
    }
    for (uint32_t i = 0; i < result_gp->length_path; i++) {
        r[i] = htobe32((result_gp->r)[i]);
    }
    free(result_gp->r); // On n'utilisera plus ces variables plus tard donc on peut déjà libérer la mémoire.
    
    if(result_gp->length_path != fwrite(r, 4, result_gp->length_path, file_out)){
        fprintf(stderr, "Erreur lors de l'écriture du fichier de sortie");
        free(r); free(result_gp);
        return -1;
    }
    free(result_gp); // On n'utilisera plus ces variables plus tard donc on peut déjà libérer la mémoire.
    free(r);
    return 0;
}