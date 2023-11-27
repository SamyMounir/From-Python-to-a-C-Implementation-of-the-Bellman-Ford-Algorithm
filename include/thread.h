#ifndef LEPL1503__THREAD__
#define LEPL1503__THREAD__

#include "include_and_struct.h"
#include <pthread.h>
#include "graph.h"
#include "file.h"

typedef struct node {
    struct node *prev;
    struct node *next;
    result_bellman_ford_t* result_bf;
} node_t;

/**
 * @author Charles
 * 
 * Ajoute un élément au début de la queue.
 * 
 * @param (result_bellman_ford_t*) : un pointeur vers la structure result_bellman_ford_t qui doit être stockée dans le noeud de la queue
 * 
 * @return (uint8_t) : 0 si tout s'est bien passé, -1 sinon
*/
uint8_t enqueue(result_bellman_ford_t*);

/**
 * @author Charles
 * 
 * Retire un élément à la fin de la queue.
 * 
 * @param /
 * 
 * @return (result_bellman_ford_t*) : un pointeur vers la structure result_bellman_ford_t qui était la valeur du dernier noeud de la queue.
*/
result_bellman_ford_t* dequeue(void);

/**
 * @author Charles
 * 
 * Libère l'espace alloué aux threads et détruit le sémaphore et les mutex
 * 
 * @param (pthread_t*) : une liste de threads dont la mémoire doit être libérée
 * 
 * @return /
*/
int8_t clear(pthread_t*);

/**
 * @author Charles
 * 
 * Libère l'espace mémoire qui est occupé par les différentes valeurs des noeuds de la queue
 * 
 * @param /
 * 
 * @return /
*/
void free_queue(void);

/**
 * @authors Matya et Samy
 * 
 * Lance les threads pour appliquer l'algorithme de Bellman-Ford à un graphe.
 * Les résultats de l'algorithme sont soit affichés à l'écran, soit enregistrés dans un fichier binaire.
 * 
 * @param (graph_t*) : un pointeur vers une structure graph_t qui est le graphe sur lequel l'algorithme de Bellman-Ford doit être appliqué
 * @param (bool*)    : un pointeur vers un booléen qui indique si des messages doivent être affichés à l'écran pour avertir l'utilisateur de problèmes
 * @param (uint8_t*) : un pointeur vers un entier non signé qui contient le nombre de threads qui doivent être utilisés pour appliquer l'algorithme de Bellman-Ford
 * @param (FILE*)    : stdout, stderr si on veut afficher les résultats de l'algorithme de Bellman-Ford sur ces canaux ; un pointeur vers un fichier binaire si on préfère enregistrer les résultats dans ce fichier.
 * 
 * @return (int8_t) : 0 si tout s'est bien passé, -1 sinon
*/
int8_t launch_threads(graph_t*, bool*, uint8_t*, FILE*);

/**
 * @authors Matya et Samy
 * 
 * Cette fonction doit être appelée par un thread.
 * Elle permet de lancer l'algorithme de Bellman-Ford autant de fois que nécessaire, en coordination avec les autres threads pour appliquer l'algorithme une fois pour chaque noeud source.
 * 
 * @result Ajoute des noeuds à la queue (producteur) avec comme valeurs les résultats de l'algorithme de Bellman-Ford pour des noeuds sources différents.
 * 
 * @param (void*) : NULL (c'est argument est inutile pour éviter avertissements lors de la compilation)
 * 
 * @return (void*) : NULL
*/
void* launch_bf(void*);


/**
 * @author Charles
 * 
 * Assigne la valeur 1 à la variable d'erreur et libère de l'espace dans la queue pour pouvoir stocker les résultats
 * des threads de Bellman-Ford qui sont encore occupés, avant de découvrir qu'une erreur s'est produite.
 * 
 * @param /
 * 
 * @return /
*/
void error_gm_gp(void);

/**
 * @authors Matya et Samy
 * 
 * Cette fonction doit être appelée par un thread.
 * Elle permet de trouver le plus long plus court chemin à partir d'un noeud source, pour tous les noeuds sources.
 * La mémoire qui ne sera plus utilisée après est libérée.
 * 
 * @result Le résultat de l'algorithme de Bellman-Ford et le plus long plus court chemin sont affichés à l'écran ou enregistrés dans un fichier binaire pour tous les noeuds sources.
 * 
 * @param (void*) : NULL (c'est argument est inutile pour éviter avertissements lors de la compilation)
 * 
 * @return (void*) : NULL
*/
void* launch_gm_gp(void*);

#endif // LEPL1503__THREAD__