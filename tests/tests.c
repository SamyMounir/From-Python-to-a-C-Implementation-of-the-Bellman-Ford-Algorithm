#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../src/graph.c"
#include "../src/file.c"

graph_t* graphe_normal;
graph_t* graphe_tous_les_liens_partent_du_noeud_0;
graph_t* graphe_non_connexe;
graph_t* graphe_cycle_negatif;

void free_graphe(graph_t* graphe) {
    for (uint32_t j = 0; j < graphe->NB_LINKS; j++) {
        free((graphe->links)[j]);
    }
    free(graphe->links); free(graphe);
}

void test_bellman_ford(void) {
    bool verbose = false;

    // Test avec GRAPHE NORMAL
    uint32_t source = 2;
    result_bellman_ford_t* result_bf_graphe_normal_source_2 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(result_bf_graphe_normal_source_2);
    CU_ASSERT_EQUAL(bellman_ford(&source, &verbose, graphe_normal,result_bf_graphe_normal_source_2), 0);

    // Initialisation du résultat à obtenir
    result_bellman_ford_t* expected_result_bf_graphe_normal_source_2 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_normal_source_2);
    expected_result_bf_graphe_normal_source_2->dist = (int64_t*)malloc(graphe_normal->NB_NODES*sizeof(int64_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_normal_source_2->dist);
    expected_result_bf_graphe_normal_source_2->path = (uint32_t*)malloc(graphe_normal->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_normal_source_2->path);
    expected_result_bf_graphe_normal_source_2->dist[0] = 3;
    expected_result_bf_graphe_normal_source_2->dist[1] = 7;
    expected_result_bf_graphe_normal_source_2->dist[2] = 0;
    expected_result_bf_graphe_normal_source_2->dist[3] = 17;
    expected_result_bf_graphe_normal_source_2->dist[4] = 10;
    expected_result_bf_graphe_normal_source_2->path[0] = 2;
    expected_result_bf_graphe_normal_source_2->path[1] = 0;
    expected_result_bf_graphe_normal_source_2->path[2] = -1;
    expected_result_bf_graphe_normal_source_2->path[3] = 1;
    expected_result_bf_graphe_normal_source_2->path[4] = 0;
    
    // Vérification du résultat
    for (uint32_t i = 0; i < graphe_normal->NB_NODES; i++) {
        CU_ASSERT_EQUAL(result_bf_graphe_normal_source_2->dist[i], expected_result_bf_graphe_normal_source_2->dist[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_normal_source_2->path[i], expected_result_bf_graphe_normal_source_2->path[i]);
    }

    free(expected_result_bf_graphe_normal_source_2->dist);
    free(expected_result_bf_graphe_normal_source_2->path);
    free(expected_result_bf_graphe_normal_source_2);
    free(result_bf_graphe_normal_source_2->dist);
    free(result_bf_graphe_normal_source_2->path);
    free(result_bf_graphe_normal_source_2);

    // Test avec GRAPHE AVEC TOUS LES LIENS QUI PARTENT DU MÊME NOEUD
    source = 0;
    result_bellman_ford_t* result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0);
    CU_ASSERT_EQUAL(bellman_ford(&source, &verbose, graphe_tous_les_liens_partent_du_noeud_0,result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0), 0);
    
    source = 2;
    result_bellman_ford_t* result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2);
    CU_ASSERT_EQUAL(bellman_ford(&source, &verbose, graphe_tous_les_liens_partent_du_noeud_0,result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2), 0);

    // Initialisation des résultats à obtenir
    result_bellman_ford_t* expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0);
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist = (int64_t*)malloc(graphe_tous_les_liens_partent_du_noeud_0->NB_NODES*sizeof(int64_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist);
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path = (uint32_t*)malloc(graphe_tous_les_liens_partent_du_noeud_0->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path);
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[0] = 0;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[1] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[2] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[3] = 2;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[4] = 2;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[0] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[1] = 0;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[2] = 0;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[3] = 0;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[4] = 0;

    result_bellman_ford_t* expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2);
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist = (int64_t*)malloc(graphe_tous_les_liens_partent_du_noeud_0->NB_NODES*sizeof(int64_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist);
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path = (uint32_t*)malloc(graphe_tous_les_liens_partent_du_noeud_0->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path);
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[0] = INT64_MAX;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[1] = INT64_MAX;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[2] = 0;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[3] = INT64_MAX;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[4] = INT64_MAX;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[0] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[1] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[2] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[3] = -1;
    expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[4] = -1;

    // Vérification des résultats
    for (uint32_t i = 0; i < graphe_tous_les_liens_partent_du_noeud_0->NB_NODES; i++) {
        CU_ASSERT_EQUAL(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[i], expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[i], expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[i], expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[i], expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path[i]);
    }
    
    free(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist);
    free(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path);
    free(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0);
    free(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->dist);
    free(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0->path);
    free(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_0);
    
    free(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist);
    free(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path);
    free(expected_result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2);
    free(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->dist);
    free(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2->path);
    free(result_bf_graphe_tous_les_liens_partent_du_noeud_0_source_2);
    
    // Test avec GRAPHE NON CONNEXE
    source = 0;
    result_bellman_ford_t* result_bf_graphe_non_connexe_source_0 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(result_bf_graphe_non_connexe_source_0);
    CU_ASSERT_EQUAL(bellman_ford(&source, &verbose, graphe_non_connexe, result_bf_graphe_non_connexe_source_0), 0);
    
    source = 2;
    result_bellman_ford_t* result_bf_graphe_non_connexe_source_2 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(result_bf_graphe_non_connexe_source_2);
    CU_ASSERT_EQUAL(bellman_ford(&source, &verbose, graphe_non_connexe, result_bf_graphe_non_connexe_source_2), 0);

    // Initialisation des résultats à obtenir
    result_bellman_ford_t* expected_result_bf_graphe_non_connexe_source_0 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_non_connexe_source_0);
    expected_result_bf_graphe_non_connexe_source_0->dist = (int64_t*)malloc(graphe_non_connexe->NB_NODES*sizeof(int64_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_non_connexe_source_0->dist);
    expected_result_bf_graphe_non_connexe_source_0->path = (uint32_t*)malloc(graphe_non_connexe->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_non_connexe_source_0->path);
    expected_result_bf_graphe_non_connexe_source_0->dist[0] = 0;
    expected_result_bf_graphe_non_connexe_source_0->dist[1] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_0->dist[2] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_0->dist[3] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_0->dist[4] = -1;
    expected_result_bf_graphe_non_connexe_source_0->path[0] = -1;
    expected_result_bf_graphe_non_connexe_source_0->path[1] = -1;
    expected_result_bf_graphe_non_connexe_source_0->path[2] = -1;
    expected_result_bf_graphe_non_connexe_source_0->path[3] = -1;
    expected_result_bf_graphe_non_connexe_source_0->path[4] = 0;

    result_bellman_ford_t* expected_result_bf_graphe_non_connexe_source_2 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_non_connexe_source_2);
    expected_result_bf_graphe_non_connexe_source_2->dist = (int64_t*)malloc(graphe_non_connexe->NB_NODES*sizeof(int64_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_non_connexe_source_2->dist);
    expected_result_bf_graphe_non_connexe_source_2->path = (uint32_t*)malloc(graphe_non_connexe->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_non_connexe_source_2->path);
    expected_result_bf_graphe_non_connexe_source_2->dist[0] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_2->dist[1] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_2->dist[2] = 0;
    expected_result_bf_graphe_non_connexe_source_2->dist[3] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_2->dist[4] = INT64_MAX;
    expected_result_bf_graphe_non_connexe_source_2->path[0] = -1;
    expected_result_bf_graphe_non_connexe_source_2->path[1] = -1;
    expected_result_bf_graphe_non_connexe_source_2->path[2] = -1;
    expected_result_bf_graphe_non_connexe_source_2->path[3] = -1;
    expected_result_bf_graphe_non_connexe_source_2->path[4] = -1;
    
    // Vérification des résultats
    for (uint32_t i = 0; i < graphe_non_connexe->NB_NODES; i++) {
        CU_ASSERT_EQUAL(result_bf_graphe_non_connexe_source_0->dist[i], expected_result_bf_graphe_non_connexe_source_0->dist[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_non_connexe_source_0->path[i], expected_result_bf_graphe_non_connexe_source_0->path[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_non_connexe_source_2->dist[i], expected_result_bf_graphe_non_connexe_source_2->dist[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_non_connexe_source_2->path[i], expected_result_bf_graphe_non_connexe_source_2->path[i]);
    }
    
    free(expected_result_bf_graphe_non_connexe_source_0->dist);
    free(expected_result_bf_graphe_non_connexe_source_0->path);
    free(expected_result_bf_graphe_non_connexe_source_0);
    free(result_bf_graphe_non_connexe_source_0->dist);
    free(result_bf_graphe_non_connexe_source_0->path);
    free(result_bf_graphe_non_connexe_source_0);

    free(expected_result_bf_graphe_non_connexe_source_2->dist);
    free(expected_result_bf_graphe_non_connexe_source_2->path);
    free(expected_result_bf_graphe_non_connexe_source_2);
    free(result_bf_graphe_non_connexe_source_2->dist);
    free(result_bf_graphe_non_connexe_source_2->path);
    free(result_bf_graphe_non_connexe_source_2);

    // Test avec GRAPHE AVEC CYCLE NÉGATIF
    source = 0;
    result_bellman_ford_t* result_bf_graphe_cycle_negatif_source_0 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(result_bf_graphe_cycle_negatif_source_0);
    CU_ASSERT_EQUAL(bellman_ford(&source, &verbose, graphe_cycle_negatif,result_bf_graphe_cycle_negatif_source_0), 0);

    // Initialisation du résultat à obtenir
    result_bellman_ford_t* expected_result_bf_graphe_cycle_negatif_source_0 = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_cycle_negatif_source_0);
    expected_result_bf_graphe_cycle_negatif_source_0->dist = (int64_t*)malloc(graphe_cycle_negatif->NB_NODES*sizeof(int64_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_cycle_negatif_source_0->dist);
    expected_result_bf_graphe_cycle_negatif_source_0->path = (uint32_t*)malloc(graphe_cycle_negatif->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_bf_graphe_cycle_negatif_source_0->path);
    expected_result_bf_graphe_cycle_negatif_source_0->dist[0] = 0;
    expected_result_bf_graphe_cycle_negatif_source_0->dist[1] = INT64_MAX;
    expected_result_bf_graphe_cycle_negatif_source_0->dist[2] = INT64_MAX;
    expected_result_bf_graphe_cycle_negatif_source_0->dist[3] = INT64_MAX;
    expected_result_bf_graphe_cycle_negatif_source_0->dist[4] = INT64_MAX;
    expected_result_bf_graphe_cycle_negatif_source_0->path[0] = -1;
    expected_result_bf_graphe_cycle_negatif_source_0->path[1] = -1;
    expected_result_bf_graphe_cycle_negatif_source_0->path[2] = -1;
    expected_result_bf_graphe_cycle_negatif_source_0->path[3] = -1;
    expected_result_bf_graphe_cycle_negatif_source_0->path[4] = -1;
    
    // Vérification du résultat
    for (uint32_t i = 0; i < graphe_cycle_negatif->NB_NODES; i++) {
        CU_ASSERT_EQUAL(result_bf_graphe_cycle_negatif_source_0->dist[i], expected_result_bf_graphe_cycle_negatif_source_0->dist[i]);
        CU_ASSERT_EQUAL(result_bf_graphe_cycle_negatif_source_0->path[i], expected_result_bf_graphe_cycle_negatif_source_0->path[i]);
    }

    free(expected_result_bf_graphe_cycle_negatif_source_0->dist);
    free(expected_result_bf_graphe_cycle_negatif_source_0->path);
    free(expected_result_bf_graphe_cycle_negatif_source_0);
    free(result_bf_graphe_cycle_negatif_source_0->dist);
    free(result_bf_graphe_cycle_negatif_source_0->path);
    free(result_bf_graphe_cycle_negatif_source_0);

}

void test_get_path(void){
    // Test avec GRAPHE NORMAL
    uint32_t* path_normal_source_2 = (uint32_t*)malloc(graphe_normal->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(path_normal_source_2);
    path_normal_source_2[0] = 2;
    path_normal_source_2[1] = 0;
    path_normal_source_2[2] = -1;
    path_normal_source_2[3] = 1;
    path_normal_source_2[4] = 0;

    uint32_t source = 2; uint32_t dest = 4;
    result_get_path_t* result_getpath_graphe_normal_source_2 = (result_get_path_t*)malloc(sizeof(result_get_path_t));
    CU_ASSERT_PTR_NOT_NULL(result_getpath_graphe_normal_source_2);
    CU_ASSERT_EQUAL(get_path(&dest,path_normal_source_2,&source,result_getpath_graphe_normal_source_2), 0);
    
    result_get_path_t* expected_result_getpath_graphe_normal_source_2 = (result_get_path_t*)malloc(sizeof(result_get_path_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_getpath_graphe_normal_source_2);
    expected_result_getpath_graphe_normal_source_2->length_path = 3;
    expected_result_getpath_graphe_normal_source_2->r = (uint32_t*)malloc(expected_result_getpath_graphe_normal_source_2->length_path*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_getpath_graphe_normal_source_2->r);
    expected_result_getpath_graphe_normal_source_2->r[0]= 2;
    expected_result_getpath_graphe_normal_source_2->r[1]= 0;
    expected_result_getpath_graphe_normal_source_2->r[2]= 4;
    
    CU_ASSERT_EQUAL(expected_result_getpath_graphe_normal_source_2->length_path,result_getpath_graphe_normal_source_2->length_path);
    for (uint32_t i = 0; i < expected_result_getpath_graphe_normal_source_2->length_path ; i++) {
        CU_ASSERT_EQUAL(expected_result_getpath_graphe_normal_source_2->r[i], result_getpath_graphe_normal_source_2->r[i]);
    }

    free(expected_result_getpath_graphe_normal_source_2->r);
    free(expected_result_getpath_graphe_normal_source_2);
    free(result_getpath_graphe_normal_source_2->r);
    free(result_getpath_graphe_normal_source_2);
    
    // Test avec GRAPHE NON CONNEXE (la destination doit être accessible à partir du noeud source)
        // Source 0 et destination 4
    uint32_t* path_nonconnexe_s_0 = (uint32_t*)malloc(graphe_non_connexe->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(path_nonconnexe_s_0);
    path_nonconnexe_s_0[0] = -1;
    path_nonconnexe_s_0[1] = -1;
    path_nonconnexe_s_0[2] = -1;
    path_nonconnexe_s_0[3] = -1;
    path_nonconnexe_s_0[4] = 0;

    source = 0; dest = 4;
    result_get_path_t* result_getpath_nonconnexe_s_0_dest_4 = (result_get_path_t*)malloc(sizeof(result_get_path_t));
    CU_ASSERT_PTR_NOT_NULL(result_getpath_nonconnexe_s_0_dest_4);
    CU_ASSERT_EQUAL(get_path(&dest,path_nonconnexe_s_0,&source,result_getpath_nonconnexe_s_0_dest_4), 0);
    
    result_get_path_t* expected_result_getpath_nonconnexe_s_0_dest_4 = (result_get_path_t*)malloc(sizeof(result_get_path_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_getpath_nonconnexe_s_0_dest_4);
    expected_result_getpath_nonconnexe_s_0_dest_4->length_path = 2;
    expected_result_getpath_nonconnexe_s_0_dest_4->r = (uint32_t*)malloc(expected_result_getpath_nonconnexe_s_0_dest_4->length_path*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_getpath_nonconnexe_s_0_dest_4->r);
    expected_result_getpath_nonconnexe_s_0_dest_4->r[0]=0;
    expected_result_getpath_nonconnexe_s_0_dest_4->r[1]=4;
    
    CU_ASSERT_EQUAL(expected_result_getpath_nonconnexe_s_0_dest_4->length_path,result_getpath_nonconnexe_s_0_dest_4->length_path);
    for (uint32_t i = 0; i < expected_result_getpath_nonconnexe_s_0_dest_4->length_path ; i++) {
        CU_ASSERT_EQUAL(expected_result_getpath_nonconnexe_s_0_dest_4->r[i], result_getpath_nonconnexe_s_0_dest_4->r[i]);
    }
    
    free(expected_result_getpath_nonconnexe_s_0_dest_4->r);
    free(expected_result_getpath_nonconnexe_s_0_dest_4);
    free(result_getpath_nonconnexe_s_0_dest_4->r);
    free(result_getpath_nonconnexe_s_0_dest_4 );
    
        // Source 2 et destination 2
    uint32_t* path_nonconnexe_s_2 = (uint32_t*)malloc(graphe_non_connexe->NB_NODES*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(path_nonconnexe_s_2);
    path_nonconnexe_s_2[0] = -1;
    path_nonconnexe_s_2[1] = -1;
    path_nonconnexe_s_2[2] = -1;
    path_nonconnexe_s_2[3] = -1;
    path_nonconnexe_s_2[4] = -1;

    source = 2; dest = 2;
    result_get_path_t* result_getpath_nonconnexe_s_2_dest_2 = (result_get_path_t*)malloc(sizeof(result_get_path_t));
    CU_ASSERT_PTR_NOT_NULL(result_getpath_nonconnexe_s_2_dest_2);
    CU_ASSERT_EQUAL(get_path(&dest,path_nonconnexe_s_2,&source,result_getpath_nonconnexe_s_2_dest_2), 0);

    result_get_path_t* expected_result_getpath_nonconnexe_s_2_dest_2 = (result_get_path_t*)malloc(sizeof(result_get_path_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_getpath_nonconnexe_s_2_dest_2);
    expected_result_getpath_nonconnexe_s_2_dest_2->length_path = 1;
    expected_result_getpath_nonconnexe_s_2_dest_2->r = (uint32_t*)malloc(expected_result_getpath_nonconnexe_s_2_dest_2->length_path*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(expected_result_getpath_nonconnexe_s_2_dest_2->r);
    expected_result_getpath_nonconnexe_s_2_dest_2->r[0]=2;

    CU_ASSERT_EQUAL(expected_result_getpath_nonconnexe_s_2_dest_2->length_path,result_getpath_nonconnexe_s_2_dest_2->length_path);
    CU_ASSERT_EQUAL(expected_result_getpath_nonconnexe_s_2_dest_2->r[0], result_getpath_nonconnexe_s_2_dest_2->r[0]);

    free(expected_result_getpath_nonconnexe_s_2_dest_2->r);
    free(expected_result_getpath_nonconnexe_s_2_dest_2);
    free(result_getpath_nonconnexe_s_2_dest_2->r);
    free(result_getpath_nonconnexe_s_2_dest_2);

    // Les autres graphes n'apportent pas d'autres situations intéressantes à tester.
}

void test_get_max(void){
    // Test avec GRAPHE NORMAL
    int64_t list_gn_source_2[] = {3, 7, 0, 17, 10};
    uint32_t source = 2;
    result_get_max_t* result_gm_graphe_normal_source_2 = (result_get_max_t*)malloc(sizeof(result_get_max_t));
    get_max(list_gn_source_2, &source, graphe_normal, result_gm_graphe_normal_source_2);

    result_get_max_t* expected_result_gm_graphe_normal_source_2 = (result_get_max_t*)malloc(sizeof(result_get_max_t));;
    expected_result_gm_graphe_normal_source_2->max_cost = 17;
    expected_result_gm_graphe_normal_source_2->max_node = 3;
    
    CU_ASSERT_EQUAL(result_gm_graphe_normal_source_2->max_cost,expected_result_gm_graphe_normal_source_2->max_cost);
    CU_ASSERT_EQUAL(result_gm_graphe_normal_source_2->max_node,expected_result_gm_graphe_normal_source_2->max_node);
    
    free(result_gm_graphe_normal_source_2);
    free(expected_result_gm_graphe_normal_source_2);

    // Test avec GRAPHE AVEC TOUS LES LIENS QUI PARTENT DU MÊME NOEUD
        // source 0
    int64_t list_liens_partent_du_noeud_0_source_0[] = {0,-1,-1,2,2};
    source = 0;
    result_get_max_t* result_gm_graphe_meme_noeud_source_0 = (result_get_max_t*)malloc(sizeof(result_get_max_t));
    get_max(list_liens_partent_du_noeud_0_source_0, &source, graphe_tous_les_liens_partent_du_noeud_0,result_gm_graphe_meme_noeud_source_0);
    
    result_get_max_t* expected_result_gm_graphe_meme_noeud_source_0 = (result_get_max_t*)malloc(sizeof(result_get_max_t));
    expected_result_gm_graphe_meme_noeud_source_0->max_cost = 2;
    expected_result_gm_graphe_meme_noeud_source_0->max_node = 3;
    
    CU_ASSERT_EQUAL(result_gm_graphe_meme_noeud_source_0->max_cost,expected_result_gm_graphe_meme_noeud_source_0->max_cost);
    CU_ASSERT_EQUAL(result_gm_graphe_meme_noeud_source_0->max_node,expected_result_gm_graphe_meme_noeud_source_0->max_node);
    
    free(result_gm_graphe_meme_noeud_source_0);
    free(expected_result_gm_graphe_meme_noeud_source_0);
        
        // source 2
    int64_t list_liens_partent_du_noeud_0_source_2[] = {INT64_MAX,INT64_MAX,0,INT64_MAX,INT64_MAX};
    source = 2;
    result_get_max_t* result_gm_graphe_meme_noeud_source_2 = (result_get_max_t*)malloc(sizeof(result_get_max_t));
    get_max(list_liens_partent_du_noeud_0_source_2, &source, graphe_tous_les_liens_partent_du_noeud_0,result_gm_graphe_meme_noeud_source_2 );
   
    result_get_max_t* expected_result_gm_graphe_meme_noeud_source_2 = (result_get_max_t*)malloc(sizeof(result_get_max_t));
    expected_result_gm_graphe_meme_noeud_source_2->max_cost = 0;
    expected_result_gm_graphe_meme_noeud_source_2->max_node = 2;
    
    CU_ASSERT_EQUAL(result_gm_graphe_meme_noeud_source_2->max_cost,expected_result_gm_graphe_meme_noeud_source_2->max_cost);
    CU_ASSERT_EQUAL(result_gm_graphe_meme_noeud_source_2->max_node,expected_result_gm_graphe_meme_noeud_source_2->max_node);
    
    free(result_gm_graphe_meme_noeud_source_2);
    free(expected_result_gm_graphe_meme_noeud_source_2);
    // Les autres graphes n'apportent pas d'autres situations intéressantes à tester.
}

void test_read_graph (void) {
    // Test graphe normal
    FILE* file_graphe_normal = fopen("tests/graph/graphe_normal.bin", "r");
    CU_ASSERT_PTR_NOT_NULL(file_graphe_normal);
    if (NULL == file_graphe_normal) {
        fprintf(stderr, "Impossible d'ouvrir le fichier qui contient l'instance de graphe graphe_normal.bin: %s\n", strerror(errno));
        return;
    }

    graph_t* read_graphe_normal = (graph_t*)malloc(sizeof(graph_t));
    CU_ASSERT_PTR_NOT_NULL(read_graphe_normal);
    if (NULL == read_graphe_normal) {
        fprintf(stderr, "Pas assez de mémoire disponible");
        close_file_in(file_graphe_normal); return;
    }

    CU_ASSERT_EQUAL(read_graph(file_graphe_normal, read_graphe_normal), 0);
    CU_ASSERT_EQUAL(read_graphe_normal->NB_NODES, graphe_normal->NB_NODES);
    CU_ASSERT_EQUAL(read_graphe_normal->NB_LINKS, graphe_normal->NB_LINKS);
    for (uint32_t i = 0; i < graphe_normal->NB_LINKS; i++) {
        CU_ASSERT_EQUAL(read_graphe_normal->links[i]->from, graphe_normal->links[i]->from);
        CU_ASSERT_EQUAL(read_graphe_normal->links[i]->to, graphe_normal->links[i]->to);
        CU_ASSERT_EQUAL(read_graphe_normal->links[i]->cost, graphe_normal->links[i]->cost);
    }
    
    free(read_graphe_normal);

    // Les autres graphes n'apportent pas d'autres situations intéressantes à tester car la lecture du fichier ne dépend pas des spécificités du graphe.
}

void test_file_output (void) {
    // Test avec GRAPHE NORMAL
    FILE* file_output_graphe_normal = fopen("tests/graph_output/graphe_normal_output.bin", "r");
    if (NULL == file_output_graphe_normal) {
        fprintf(stderr, "Impossible d'ouvrir le fichier qui contient le résultat de l'appel de sp.c pour graphe_normal.bin, graphe_normal_output.bin: %s\n", strerror(errno));
        return;
    }
    
    uint32_t nb_nodes;
    fread(&nb_nodes, 4, 1, file_output_graphe_normal);
    CU_ASSERT_EQUAL((uint32_t) be32toh(nb_nodes), graphe_normal->NB_NODES);

    // Nous vérifions que le contenu du fichier est correct pour le noeud source 0 et la suite devrait être alors être également correcte.
    
    uint32_t i;
    for (i = 0; i < graphe_normal->NB_NODES; i++) {
        uint32_t source_idx;
        fread(&source_idx, 4, 1, file_output_graphe_normal);
        if ((uint32_t) be32toh(source_idx) == 0) {
            break;
        }
        // Si c'est une autre source que 0, il faut sauter les informations de ce "mauvais" noeud source
        uint32_t buffer1;
        fread(&buffer1, 4, 1, file_output_graphe_normal);
        int64_t buffer2;
        fread(&buffer2, 8, 1, file_output_graphe_normal);
        fread(&buffer1, 4, 1, file_output_graphe_normal);
        uint32_t* buffer3 = (uint32_t*)malloc(((uint32_t) be32toh(buffer1))*sizeof(uint32_t));
        CU_ASSERT_PTR_NOT_NULL(buffer3);
        fread(buffer3, 4, (uint32_t) be32toh(buffer1), file_output_graphe_normal);
        free(buffer3);
    }
    CU_ASSERT_NOT_EQUAL(i, graphe_normal->NB_NODES);

    uint32_t destination_idx;
    fread(&destination_idx, 4, 1, file_output_graphe_normal);
    CU_ASSERT_EQUAL((uint32_t) be32toh(destination_idx), 3);
    
    int64_t _cost;
    fread(&_cost, 8, 1, file_output_graphe_normal);
    CU_ASSERT_EQUAL((int64_t) be64toh(_cost), 14);

    uint32_t path_len;
    fread(&path_len, 4, 1, file_output_graphe_normal);
    CU_ASSERT_EQUAL((uint32_t) be32toh(path_len), 3)

    uint32_t* path = (uint32_t*)malloc(((uint32_t) be32toh(path_len))*sizeof(uint32_t));
    CU_ASSERT_PTR_NOT_NULL(path);
    fread(path, 4, 3, file_output_graphe_normal);
    CU_ASSERT_EQUAL((uint32_t) be32toh(path[0]), 0);
    CU_ASSERT_EQUAL((uint32_t) be32toh(path[1]), 1);
    CU_ASSERT_EQUAL((uint32_t) be32toh(path[2]), 3);
    free(path);

    fclose(file_output_graphe_normal);

    // Les autres graphes n'apportent pas d'autres situations intéressantes à tester. En effet, si chaque graphe passe tous les tests
    // des autres fonctions, le fichier de sortie sera correct car l'écriture du fichier de sortie ne dépend pas des spécificités du graphe.
}

int setup(void) {

    // Initialisation graphe normal
    graphe_normal = (graph_t*)malloc(sizeof(graph_t));
    graphe_normal->NB_NODES = 5;
    graphe_normal->NB_LINKS = 10;
    graphe_normal->links = (link_t**)malloc(graphe_normal->NB_LINKS*sizeof(link_t));
    for (uint32_t i = 0; i < graphe_normal->NB_LINKS; i++) {
        graphe_normal->links[i] = (link_t*)malloc(sizeof(link_t));
    }
    graphe_normal->links[0]->from = 0;
    graphe_normal->links[0]->to = 1;
    graphe_normal->links[0]->cost = 4;
    graphe_normal->links[1]->from = 0;
    graphe_normal->links[1]->to = 2;
    graphe_normal->links[1]->cost = 4;
    graphe_normal->links[2]->from = 0;
    graphe_normal->links[2]->to = 4;
    graphe_normal->links[2]->cost = 7;
    graphe_normal->links[3]->from = 1;
    graphe_normal->links[3]->to = 0;
    graphe_normal->links[3]->cost = 9;
    graphe_normal->links[4]->from = 1;
    graphe_normal->links[4]->to = 2;
    graphe_normal->links[4]->cost = 2;
    graphe_normal->links[5]->from = 1;
    graphe_normal->links[5]->to = 3;
    graphe_normal->links[5]->cost = 10;
    graphe_normal->links[6]->from = 1;
    graphe_normal->links[6]->to = 4;
    graphe_normal->links[6]->cost = 7;
    graphe_normal->links[7]->from = 2;
    graphe_normal->links[7]->to = 0;
    graphe_normal->links[7]->cost = 3;
    graphe_normal->links[8]->from = 3;
    graphe_normal->links[8]->to = 2;
    graphe_normal->links[8]->cost = 5;
    graphe_normal->links[9]->from = 4;
    graphe_normal->links[9]->to = 0;
    graphe_normal->links[9]->cost = 9;

    // Initialisation graphe avec tous les liens qui partent du même noeud
    graphe_tous_les_liens_partent_du_noeud_0 = (graph_t*)malloc(sizeof(graph_t));
    graphe_tous_les_liens_partent_du_noeud_0->NB_NODES = 5;
    graphe_tous_les_liens_partent_du_noeud_0->NB_LINKS = 5;
    graphe_tous_les_liens_partent_du_noeud_0->links = (link_t**)malloc(graphe_tous_les_liens_partent_du_noeud_0->NB_LINKS*sizeof(link_t));
    for (uint32_t i = 0; i < graphe_tous_les_liens_partent_du_noeud_0->NB_LINKS; i++) {
        graphe_tous_les_liens_partent_du_noeud_0->links[i] = (link_t*)malloc(sizeof(link_t));
    }
    graphe_tous_les_liens_partent_du_noeud_0->links[0]->from = 0;
    graphe_tous_les_liens_partent_du_noeud_0->links[0]->to = 1;
    graphe_tous_les_liens_partent_du_noeud_0->links[0]->cost = -1;
    graphe_tous_les_liens_partent_du_noeud_0->links[1]->from = 0;
    graphe_tous_les_liens_partent_du_noeud_0->links[1]->to = 2;
    graphe_tous_les_liens_partent_du_noeud_0->links[1]->cost = 1;
    graphe_tous_les_liens_partent_du_noeud_0->links[2]->from = 0;
    graphe_tous_les_liens_partent_du_noeud_0->links[2]->to = 2;
    graphe_tous_les_liens_partent_du_noeud_0->links[2]->cost = -1;
    graphe_tous_les_liens_partent_du_noeud_0->links[3]->from = 0;
    graphe_tous_les_liens_partent_du_noeud_0->links[3]->to = 3;
    graphe_tous_les_liens_partent_du_noeud_0->links[3]->cost = 2;
    graphe_tous_les_liens_partent_du_noeud_0->links[4]->from = 0;
    graphe_tous_les_liens_partent_du_noeud_0->links[4]->to = 4;
    graphe_tous_les_liens_partent_du_noeud_0->links[4]->cost = 2;

    // Initialisation graphe non connexe
    graphe_non_connexe = (graph_t*)malloc(sizeof(graph_t));
    graphe_non_connexe->NB_NODES = 5;
    graphe_non_connexe->NB_LINKS = 4;
    graphe_non_connexe->links = (link_t**)malloc(graphe_non_connexe->NB_LINKS*sizeof(link_t));
    for (uint32_t i = 0; i < graphe_non_connexe->NB_LINKS; i++) {
        graphe_non_connexe->links[i] = (link_t*)malloc(sizeof(link_t));
    }
    graphe_non_connexe->links[0]->from = 0;
    graphe_non_connexe->links[0]->to = 4;
    graphe_non_connexe->links[0]->cost = -1;
    graphe_non_connexe->links[1]->from = 4;
    graphe_non_connexe->links[1]->to = 0;
    graphe_non_connexe->links[1]->cost = 2;
    graphe_non_connexe->links[2]->from = 1;
    graphe_non_connexe->links[2]->to = 3;
    graphe_non_connexe->links[2]->cost = -1;
    graphe_non_connexe->links[3]->from = 3;
    graphe_non_connexe->links[3]->to = 1;
    graphe_non_connexe->links[3]->cost = 2;
    
    // Initialisation graphe avec cycle négatif
    graphe_cycle_negatif = (graph_t*)malloc(sizeof(graph_t));
    graphe_cycle_negatif->NB_NODES = 5;
    graphe_cycle_negatif->NB_LINKS = 8;
    graphe_cycle_negatif->links = (link_t**)malloc(graphe_cycle_negatif->NB_LINKS*sizeof(link_t));
    for (uint32_t i = 0; i < graphe_cycle_negatif->NB_LINKS; i++) {
        graphe_cycle_negatif->links[i] = (link_t*)malloc(sizeof(link_t));
    }
    graphe_cycle_negatif->links[0]->from = 0;
    graphe_cycle_negatif->links[0]->to = 1;
    graphe_cycle_negatif->links[0]->cost = 5;
    graphe_cycle_negatif->links[1]->from = 0;
    graphe_cycle_negatif->links[1]->to = 2;
    graphe_cycle_negatif->links[1]->cost = 2;
    graphe_cycle_negatif->links[2]->from = 1;
    graphe_cycle_negatif->links[2]->to = 3;
    graphe_cycle_negatif->links[2]->cost = 1;
    graphe_cycle_negatif->links[3]->from = 2;
    graphe_cycle_negatif->links[3]->to = 4;
    graphe_cycle_negatif->links[3]->cost = -3;
    graphe_cycle_negatif->links[4]->from = 3;
    graphe_cycle_negatif->links[4]->to = 2;
    graphe_cycle_negatif->links[4]->cost = -1;
    graphe_cycle_negatif->links[5]->from = 3;
    graphe_cycle_negatif->links[5]->to = 4;
    graphe_cycle_negatif->links[5]->cost = 2;
    graphe_cycle_negatif->links[6]->from = 4;
    graphe_cycle_negatif->links[6]->to = 0;
    graphe_cycle_negatif->links[6]->cost = -1;
    graphe_cycle_negatif->links[7]->from = 4;
    graphe_cycle_negatif->links[7]->to = 1;
    graphe_cycle_negatif->links[7]->cost = 1;

    return 0;
}

int teardown(void) {
    free_graphe(graphe_normal);
    free_graphe(graphe_tous_les_liens_partent_du_noeud_0);
    free_graphe(graphe_non_connexe);
    free_graphe(graphe_cycle_negatif);
    return 0;
}

int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry()) {return CU_get_error();}

    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Tests pour le projet", setup, teardown);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "Test bellman_ford", test_bellman_ford)) ||
        (NULL == CU_add_test(pSuite, "Test get_path", test_get_path)) ||
        (NULL == CU_add_test(pSuite, "Test get_max", test_get_max)) ||
        (NULL == CU_add_test(pSuite, "Test read_graph", test_read_graph)) ||
        (NULL == CU_add_test(pSuite, "Test file_output", test_file_output))) {
            CU_cleanup_registry();
            return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return CU_get_error();
}