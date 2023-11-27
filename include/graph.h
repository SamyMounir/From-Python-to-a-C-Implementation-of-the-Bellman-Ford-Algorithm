#ifndef LEPL1503__GRAPH__
#define LEPL1503__GRAPH__

#include "include_and_struct.h"

/**
 * @authors Floriane et Charles
 * 
 * Cette fonction est appelée si un cycle négatif a été détecté. Elle complète la structure result_bellman_ford_t en conséquence.
 * 
 * @result la structure result_bellman_ford_t pointée en argument a été complétée :
 *             - source (uint32_t) : (non modifié) ;
 *             - dist (int64_t*)   : INT64_MAX (de la librairie stdint.h ; représente une distance infinie) pour tous les noeuds sauf le noeud source qui est à une distance nulle de lui-même ;
 *             - path (uint32_t*)  : -1 pour tous les noeuds
 * 
 * @param (uint32_t*)              : pointeur vers le noeud source.
 * @param (bool*)                  : pointeur vers le booléen qui indique si on veut afficher à l'écran un message pour signaler qu'un cycle négatif est détecté.
 * @param (uint32_t*)              : pointeur vers le nombre de noeuds du graphe pour lequel l'algorithme de Bellman-Ford a été appliqué.
 * @param (result_bellman_ford_t*) : pointeur vers une structure result_bellman_ford_t pour laquelle on doit compléter les tableaux de distance et de précédence suite à un cycle négatif.
 * 
 * @return /    
 */
void cycle_negatif(const uint32_t*, bool*, const uint32_t*, result_bellman_ford_t*);

/**
 * @authors Floriane et Charles
 * 
 * Applique l'algorithme de Bellman-Ford à un graphe et pour un noeud source donnés.
 * 
 * @result la structure result_bellman_ford_t pointée en argument a été complétée :
 *             - source (uint32_t) : indice du noeud source pour lequel a été appliqué l'algorithme ;
 *             - dist (int64_t*)   : un tableau de distances pour atteindre chaque noeud à partir du noeud source ;
 *             - path (uint32_t*)  : un tableau de précédence qui indique à l'indice i le dernier noeud parcouru pour arriver au noeud i.
 *                                   À l'indice du noeud source, le tableau de précédence contient un -1.
 *                                   Si le noeud j est isolé, la distance pour ce noeud est infinie (INT64_MAX de la librairie stdint.h) et le tableau de précédence contient -1 à l'indice j.
 *                                   Si un cycle négatif est détecté, la distance est infinie (INT64_MAX de la librairie stdint.h) pour tous les noeuds à l'exception du noeud source pour lequel la distance est nulle.
 *                                   Dans ce cas, le tableau de précédence contient des -1 à tous les indices.
 * 
 * @param (uint32_t*)              : pointeur vers le noeud source.
 * @param (bool*)                  : pointeur vers le booléen qui indique si on veut afficher à l'écran un message si un cycle négatif est détecté.
 * @param (graph_t*)               : pointeur vers le graphe pour lequel on veut appliquer l'algorithme de Bellman-Ford.
 * @param (result_bellman_ford_t*) : pointeur vers une structure result_bellman_ford_t dont la mémoire a été allouée dynamiquement.
 * 
 * @return (int8_t) : 0 si tout s'est bien passé, -1 sinon      
 */
int8_t bellman_ford(const uint32_t*, bool*, graph_t*, result_bellman_ford_t*);

/**
 * @authors Louise et Brieuc
 *
 * Calcule le trajet allant d'un noeud source à un noeud destination dans un graphe
 *
 * @result la structure result_get_path_t pointée en argument a été complétée :
 *             - length_path (uint32_t) : le nombre de noeuds qui doivent être parcourus pour aller du noeud source au noeud destination inclus ;
 *             - r (uint32_t*)          : tableau indiquant les noeuds par lesquels passe le chemin
 * 
 * @param (uint32_t*)          : indice du noeud de destination. Le noeud de destination doit toujours être accessible à partir du noeud source (path[destination] != -1 or destination == source).
 * @param (uint32_t*)          : tableau de précédence pour le noeud source `source`.
 * @param (uint32_t*)          : indice du noeud source.
 * @param (result_get_path_t*) : pointeur vers une structure result_get_path_t dont la mémoire a été allouée dynamiquement.
 *
 * @return (int8_t) : 0 si tout s'est bien passé, -1 sinon
 */
int8_t get_path(const uint32_t*, uint32_t*, const uint32_t*, result_get_path_t*);

/**
 * @authors Matya et Samy 
 * 
 * Cherche le noeud ayant le plus grand plus petit coût à partir d'un noeud source pour un graphe donné.
 * 
 * @result la structure result_get_max_t pointée en argument a été complétée :
 *             - (int64_t)  : le coût du plus long plus court chemin à partir du noeud source dans le graphe
 *             - (uint32_t) : l'indice du noeud ayant un chemin de ce coût. Si plusieurs noeuds ont ce même coût maximal, c'est le noeud ayant le plus petit indice qui est choisi.
 * 
 * @param (int64_t*)          : tableau d'entiers donnant le coût du chemin entre le noeud source et chaque noeud.
 * @param (uint32_t*)         : pointeur vers l'indice du noeud source
 * @param (graph_t*)          : pointeur vers le graphe sur lequel on recherche le plus grand plus court chemin à partir du noeud source
 * @param (result_get_max_t*) : pointeur vers une structure result_get_max_t dont la mémoire a été allouée dynanmiquement
 * 
 * @return /
 */
void get_max(const int64_t*, const uint32_t*, graph_t*, result_get_max_t*);

#endif // LEPL1503__GRAPH__