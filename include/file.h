#ifndef LEPL1503__FILE__
#define LEPL1503__FILE__

#include "./include_and_struct.h"
#include "./portable_endian.h"

/**
 * @author Charles
 * 
 * Libère l'espace alloué aux liens du graphe donné en argument
 * 
 * @param (graph_t*)  : un pointeur vers un graphe dont l'espace mémoire nécessaire pour le tableau de liens a déjà été alloué.
 * @param (uint32_t*) : un pointeur vers un entier représentant le nombre de liens pour lesquelles de la mémoire a été allouée.
 * 
 * @return /
*/
void free_links(graph_t*, const uint32_t*);

/**
 * @author Charles
 * 
 * Ferme le fichier donné en argument
 * 
 * @param (FILE*) : pointeur vers un fichier binairé
 * 
 * @return (int8_t) : 0 si tout c'est bien passé, -1 sinon
*/
int8_t close_file_in(FILE* file_in);

/**
 * @authors Brieuc, Charles et Matya
 * 
 * Récupère les informations sur le graphe contenues dans le fichier binaire pointé par file_in.
 * 
 * @result le graphe pointé en argument a été complété avec les données du fichier :
 *             - NB_NODES (uint32_t) : le nombre de noeuds du graphe ;
 *             - NB_LINKS (uint32_t) : le nombre d'arêtes du graphe ;
 *             - links (link_t**) : une liste de pointeurs vers des structures link_t dont les attributs sont :
 *                 - from (uint32_t) : le noeud de départ de l'arête ;
 *                 - to (uint32_t) : le noeud d'arrivée de l'arête ;
 *                 - cost (int32_t) : le coût de l'arête (peut être négatif)
 * 
 * @param (FILE*)    : pointeur vers un fichier binaire contenant les données d'un graphe
 * @param (graph_t*) : pointeur vers un graphe dont la mémoire a été allouée dynamiquement
 * 
 * @return (int8_t) : 0 si tout s'est bien passé, -1 sinon
 */
int8_t read_graph(FILE*, graph_t*);

/**
 * @authors Floriane et Charles
 * 
 * Affiche à l'utilisateur les résultats de l'algorithme de Bellman-Ford pour un graphe et un noeud source
 * et libère l'espace qui est encore alloué aux structures result_bellman_ford_t, result_get_path_t et result_get_max_t
 * 
 * @param (FILE*)                  : stdout ou stderr, le canal dans lequel les résultats doivent être affichés.
 * @param (uint32_t)               : un pointeur vers le nombre de noeuds du graphe
 * @param (result_bellman_ford_t*) : un pointeur vers une structure bellman_ford_t contenant les résultats de l'algorithme de Bellman-Ford pour un graphe et un noeud source (donné dans l'attribut source de la structure result_bellman_ford_t)
 * @param (result_get_max_t*)      : un pointeur vers une structure result_get_max_t contenant le noeud et le coût du plus long plus court chemin à partir d'un noeud source pour un graphe
 * @param (result_get_path_t*)     : un pointeur vers une structure result_get_path_t contenant les noeuds par lesquels il faut passer pour aller du noeud source au noeud destination (qui est le noeud ayant le plus grand coût minimal à partir de la source)
 * 
 * @return /
*/
void show(FILE*, const uint32_t*, result_bellman_ford_t*, result_get_max_t*, result_get_path_t*);

/**
 * @authors Floriane et Charles
 * 
 * Enregistre dans un fichier les résultats de l'algorithme de Bellman-Ford pour un graphe et un noeud source
 * et libère l'espace qui est encore alloué aux structures result_bellman_ford_t, result_get_path_t et result_get_max_t
 * 
 * @param (FILE*)                  : un pointeur vers le fichier binaire dans lequel les résultats doivent être écrits.
 * @param (result_bellman_ford_t*) : un pointeur vers une structure bellman_ford_t contenant les résultats de l'algorithme de Bellman-Ford pour un graphe et un noeud source (donné dans l'attribut source de la structure result_bellman_ford_t)
 * @param (result_get_max_t*)      : un pointeur vers une structure result_get_max_t contenant le noeud et le coût du plus long plus court chemin à partir d'un noeud source pour un graphe
 * @param (result_get_path_t*)     : un pointeur vers une structure result_get_path_t contenant les noeuds par lesquels il faut passer pour aller du noeud source au noeud destination (qui est le noeud ayant le plus grand coût minimal à partir de la source)
 * 
 * @return (int8_t) : 0 si tout s'est bien passé, -1 sinon
*/
int8_t write_file(FILE*, result_bellman_ford_t*, result_get_max_t*, result_get_path_t*);

#endif // LEPL1503__FILE__