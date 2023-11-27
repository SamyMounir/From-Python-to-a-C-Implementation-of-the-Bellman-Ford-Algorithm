#include "include_and_struct.h"
#include <getopt.h>
#include "../include/file.h" // Fonction read_graph
#include "../include/thread.h" //Fonctions multi_bf, put et get

typedef struct {
    FILE *input_file;
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

void usage(char *prog_name) {
    fprintf(stderr, "UTILISATION:\n");
    fprintf(stderr, "    %s [OPTIONS] input_file\n", prog_name);
    fprintf(stderr,
            "    input_file: chemin vers le fichier d'instance representant le "
            "graphe a traiter.\n");
    fprintf(
        stderr,
        "    -f output_file: chemin vers le fichier qui contiendra le resultat "
        "de programme, au format specifie dans l'enonce. Defaut : stdout.\n");
    fprintf(stderr,
            "    -n n_threads: nombre de threads de calcul executant "
            "l'algorithme de plus court chemin. Defaut : 4.\n");
    fprintf(stderr,
            "    -v: autorise les messages de debug. Si ce n'est pas active, "
            "aucun message de ce type ne peut etre affiche, excepte les "
            "messages d'erreur en cas d'echec. Defaut : false.\n");
}


//Décompose les arguments fournis et associe ses attributs à args
int parse_args(args_t *args, int argc, char *argv[]) {
    memset(args, 0, sizeof(args_t));

    // Valeurs par défaut pour les arguments.
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    args->input_file = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "hn:vf:")) != -1) {
        switch (opt) {
            case 'n':
                args->nb_threads = atoi(optarg);
                if (args->nb_threads == 0) {
                    fprintf(stderr,
                            "Le nombre de threads de calcul doit etre superieur a 0 ! Recu : %s\n",
                            optarg);
                    return -1;
                }
                break;
            case 'v':
                args->verbose = true;
                break;
            case 'f':
                args->output_stream = fopen(optarg, "w+");
                if (args->output_stream == NULL) {
                    fprintf(stderr,
                            "Impossible to open the output file %s: %s\n",
                            optarg, strerror(errno));
                    return -1;
                }
                break;
            case '?':
            case 'h':
                usage(argv[0]);
                return 1;
            default:
                usage(argv[0]);
                return -1;
        }
    }
    if (optind == argc) {
        fprintf(stderr,
                "Vous devez fournir un fichier d'instance en entree !\n");
        return -1;
    }

    if (NULL == (args->input_file = fopen(argv[optind], "r"))) {
        fprintf(stderr,
                "Impossible d'ouvrir le fichier qui contient l'instance de graphe %s: %s\n",
                argv[optind], strerror(errno));
        return -1;
    }
    return 0;
}

/**
 * @author Floriane
 * 
 * Si un fichier de sortie a été spécifié en entrée (différent des canaux stdout et stderr), celui-ci est fermé.
 * 
 * @param (FILE*) : fichier de sortie binaire
 * 
 * @return (int8_t) : 0 si tout s'est bien passé, -1 sinon
*/
int8_t close_file_out(FILE *output_stream) {
    if (output_stream != stdout && output_stream != stderr) {
        if(fclose(output_stream) == EOF){
            fprintf(stderr, "Erreur lors de la fermeture du fichier de sortie");
            return -1;
        }
    }
    return 0;
}

/**
 * @author Charles
 * 
 * Libère la mémoire allouée à un graphe
 * 
 * @param (graph_t*) : un pointeur vers le graphe pour lequel il faut libérer la mémoire.
 * 
 * @return /
*/
void free_graphe(graph_t* graphe) {
    for (uint32_t j = 0; j < graphe->NB_LINKS; j++) {
        free((graphe->links)[j]);
    }
    free(graphe->links); free(graphe);
}

/**
 * @authors Floriane et Brieuc
 * 
 * Les étapes suivantes sont effectuées :
 * - Attribution des arguments à args ;
 * - Création du graphe par lecture du fichier d'entrée ;
 * - Si un fichier de sortie est donné, écriture du nombre de noeuds du graphe dans ce fichier.
 * - Application de l'algorithme de Bellman-Ford et recherche du chemin de coût maximal en prenant chaque noeud comme noeud source.
 * - Si un fichier de sortie est donné, écriture des résultats dans ce fichier. Sinon, impression des résultats à l'écran.
 * 
 * Si l'argument verbose est activé, le nomre de noeuds et de liens du graphe, les liens.
 * 
 * @param argc (int) nombre d'éléments passés en argument
 * @param argv[] (char*) tableau de caractères contenant les éléments passés en arguments (attributs de args)
*/
int main(int argc, char *argv[]) {
    args_t args;
    int err = parse_args(&args, argc, argv);
    if (err == -1) {
        close_file_in(args.input_file);
        close_file_out(args.output_stream);
        exit(EXIT_FAILURE);
    } else if (err == 1) { // Dans le cas où l'utilisateur a fait appel à l'argument -h pour avoir les indications sur les arguments.
        exit(EXIT_SUCCESS);
    }

    // Création du graphe
    graph_t* graphe = (graph_t*)malloc(sizeof(graph_t));
    if (graphe == NULL) {
        close_file_in(args.input_file);
        close_file_out(args.output_stream);
        exit(EXIT_FAILURE);
    }

    // Vérification qu'il n'y a pas eu d'erreur dans l'exécution de read_graph
    if (read_graph(args.input_file, graphe) != 0) {
        close_file_out(args.output_stream);
        free(graphe);
        exit(EXIT_FAILURE);
    }

    if (args.verbose) {
        fprintf(stderr, "Nombre de noeuds: %" PRIu32 "\n", graphe->NB_NODES);
        fprintf(stderr, "Nombre de liens: %" PRIu32 "\n", graphe->NB_LINKS);
        for (uint32_t i = 0; i < graphe->NB_LINKS; i++) {
            fprintf(stderr,
                    "Lien: %" PRIu32 " -> %" PRIu32 " (cout de %" PRIi32 ")\n",
                    (uint32_t)((graphe->links)[i])->from, (uint32_t)((graphe->links)[i])->to,
                    (int32_t)((graphe->links)[i])->cost);
        }
    }

    // Si un fichier de sortie est fourni, le nombre de noeuds du graphes y est écrit
    if ((args.output_stream != stdout) && (args.output_stream != stderr)) {
        uint32_t nb_nodes = htobe32(graphe->NB_NODES);
        if(1 != fwrite(&nb_nodes, 4, 1, args.output_stream)){
            close_file_out(args.output_stream);
            free_graphe(graphe);
            exit(EXIT_FAILURE);
        }
    }

    if (launch_threads(graphe, &args.verbose, &args.nb_threads, args.output_stream) != 0) {
        close_file_out(args.output_stream);
        free_graphe(graphe);
        exit(EXIT_FAILURE);
    }

    if (close_file_out(args.output_stream) != 0) {
        free_graphe(graphe);
        exit(EXIT_FAILURE);
    }

    free_graphe(graphe);
    exit(EXIT_SUCCESS);
}