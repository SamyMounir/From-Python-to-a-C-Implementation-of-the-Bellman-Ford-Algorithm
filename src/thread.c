#include "../include/portable_semaphore.h"
#include "../include/thread.h"

graph_t* graphe;
FILE* out;
bool* verb;
uint8_t* nb_thr_bf;

uint32_t source = 0;
pthread_mutex_t mutex_source;

sem_t* full;
sem_t* empty;
node_t* head = NULL;
node_t* tail = NULL;
pthread_mutex_t mutex_queue;

uint8_t err = 0;

uint32_t write_index = 0;
pthread_mutex_t mutex_write_index;
pthread_mutex_t mutex_write;

uint8_t enqueue(result_bellman_ford_t* bf) {
    node_t* new = (node_t*)malloc(sizeof(node_t));
    if (new == NULL) {
        fprintf(stderr, "Pas assez de mémoire disponible\n");
        free(bf->dist); free(bf->path); free(bf);
        return -1;
    }
    new->prev = NULL;
    new->result_bf = bf;

    if (sem_wait(empty) != 0) {
        fprintf(stderr, "Erreur lors de sem_wait\n");
        free(bf->dist); free(bf->path); free(bf); free(new);
        return -1;
    }
    if (pthread_mutex_lock(&mutex_queue) != 0) {
        fprintf(stderr, "Erreur lors du lock des mutex\n");
        free(bf->dist); free(bf->path); free(bf); free(new);
        return -1;
    }
    if (head==NULL) {new->next = NULL; tail = new;}
    else {new->next = head; head->prev = new;}
    head = new;
    if (pthread_mutex_unlock(&mutex_queue) != 0) {
        fprintf(stderr, "Erreur lors du unlock des mutex\n");
        return -1;
    }
    if (sem_post(full) != 0) {
        fprintf(stderr, "Erreur lors de sem_post\n");
        return -1;
    }
    return 0;
}

result_bellman_ford_t* dequeue(void) {
    if (sem_wait(full) != 0) {
        fprintf(stderr, "Erreur lors de sem_wait\n");
        return NULL;
    }
    if (pthread_mutex_lock(&mutex_queue)) {
        fprintf(stderr, "Erreur lors du lock des mutex\n");
        return NULL;
    }
    node_t* last = tail;
    tail = tail->prev;
    if (tail == NULL) {head = NULL;}
    else {tail->next = NULL;}
    if (pthread_mutex_unlock(&mutex_queue) != 0) {
        fprintf(stderr, "Erreur lors du unlock des mutex\n");
        free(last->result_bf);
        last->prev = NULL;
        free(last);
        return NULL;
    }
    if (sem_post(empty) != 0) {
        fprintf(stderr, "Erreur lors de sem_post\n");
        free(last->result_bf);
        last->prev = NULL;
        free(last);
        return NULL;
    }
    
    result_bellman_ford_t* bf = last->result_bf;
    last->prev = NULL;
    free(last);
    return bf;
}

int8_t clear(pthread_t* threads) {
    free(threads);
    uint8_t errclear = 0;
    if (sem_destroy(full) != 0) {fprintf(stderr, "Erreur lors de la destruction des sémaphores\n"); errclear = 1;}
    if (sem_destroy(empty) != 0) {fprintf(stderr, "Erreur lors de la destruction des sémaphores\n"); errclear = 1;}
    if (pthread_mutex_destroy(&mutex_source) != 0) {fprintf(stderr, "Erreur lors de la destruction des mutex\n"); errclear = 1;}
    if (pthread_mutex_destroy(&mutex_queue) != 0) {fprintf(stderr, "Erreur lors de la destruction des mutex\n"); errclear = 1;}
    if (pthread_mutex_destroy(&mutex_write_index) != 0) {fprintf(stderr, "Erreur lors de la destruction des mutex\n"); errclear = 1;}
    if (pthread_mutex_destroy(&mutex_write) != 0) {fprintf(stderr, "Erreur lors de la destruction des mutex\n"); errclear = 1;}

    if (errclear == 1) {return -1;}
    return 0;
}

void free_queue(void) {
    if (head == NULL) {return;}
    result_bellman_ford_t* result_bf = dequeue();
    free(result_bf->dist); free(result_bf->path); free(result_bf);
    free_queue();
}

int8_t launch_threads(graph_t* graph, bool* verbose, uint8_t* nb_threads_bf, FILE* output_stream) {
    graphe = graph;
    out = output_stream;
    verb = verbose;
    nb_thr_bf = nb_threads_bf;

    full = sem_init(0);
    if (full==NULL) {
        fprintf(stderr, "Erreur lors de l'initialisation des sémaphores\n");
        return -1;
    }
    empty = sem_init(1);
    if (empty==NULL) {
        sem_destroy(full);
        fprintf(stderr, "Erreur lors de l'initialisation des sémaphores\n");
        return -1;
    }
    
    if (pthread_mutex_init(&mutex_source, NULL) != 0) {fprintf(stderr, "Erreur les de la création des mutex\n"); sem_destroy(full); sem_destroy(empty); return -1;}
    if (pthread_mutex_init(&mutex_queue, NULL) != 0) {fprintf(stderr, "Erreur les de la création des mutex\n"); sem_destroy(full); sem_destroy(empty); pthread_mutex_destroy(&mutex_source); return -1;}
    if (pthread_mutex_init(&mutex_write_index, NULL) != 0) {fprintf(stderr, "Erreur les de la création des mutex\n"); sem_destroy(full); sem_destroy(empty); pthread_mutex_destroy(&mutex_source); pthread_mutex_destroy(&mutex_queue); return -1;}
    if (pthread_mutex_init(&mutex_write, NULL) != 0){fprintf(stderr, "Erreur les de la création des mutex\n"); sem_destroy(full); sem_destroy(empty); pthread_mutex_destroy(&mutex_source); pthread_mutex_destroy(&mutex_queue); pthread_mutex_destroy(&mutex_write); return -1;}

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*(*nb_threads_bf + 1));
    if (threads == NULL) {
        fprintf(stderr, "Pas assez de mémoire disponible\n");
        return -1;
    }

    for (uint8_t i = 0; i < *nb_threads_bf; i++) {
        if (pthread_create(&(threads[i]),NULL,&launch_bf,NULL) != 0) {
            fprintf(stderr, "Erreur lors de la création des threads\n");
            clear(threads);
            return -1;
        }
    }

    if (pthread_create(&(threads[*nb_threads_bf]),NULL,&launch_gm_gp,NULL) != 0) {
        fprintf(stderr, "Erreur lors de la création des threads\n");
        clear(threads);
        return -1;
    }

    for (uint8_t i = 0; i <= *nb_threads_bf; i++) {
        if (pthread_join(threads[i],NULL) != 0) {
            fprintf(stderr, "Erreur lors de l'attente de la fin d'exécution des threads\n");
            clear(threads);
            return -1;
        }
    }

    if (err != 0) {free_queue();}
    if (clear(threads) != 0) {return -1;}
    return 0;
}

void* launch_bf(void* inutile) {
    (void) inutile; // Pour éviter d'avoir un avertissement de variable inutilisée.

    while(true) {
        if (err != 0) {pthread_exit(NULL);}

        if (pthread_mutex_lock(&mutex_source) != 0) {fprintf(stderr, "Erreur lors du lock des mutex\n"); err = 1; pthread_exit(NULL);}
        if (source >= graphe->NB_NODES) {
            if (pthread_mutex_unlock(&mutex_source) != 0) {fprintf(stderr, "Erreur lors du unlock des mutex\n"); err = 1; pthread_exit(NULL);}
            return NULL;
        }
        uint32_t current_source = source;
        source++;
        if (pthread_mutex_unlock(&mutex_source) != 0) {fprintf(stderr, "Erreur lors du unlock des mutex\n"); err = 1; pthread_exit(NULL);}

        result_bellman_ford_t* result_bf = (result_bellman_ford_t*)malloc(sizeof(result_bellman_ford_t));
        if (result_bf == NULL) {
            fprintf(stderr, "Pas assez de mémoire disponible\n");
            pthread_exit(NULL);
        }

        if (bellman_ford(&current_source, verb, graphe, result_bf) != 0) {
            err = 1;            
            free(result_bf);
            pthread_exit(NULL);
        }

        if (enqueue(result_bf) != 0) {
            err = 1;            
            pthread_exit(NULL);
        }
    }
}

void error_gm_gp(void) {
    err = 1;
    for (uint8_t i = 0; i < *nb_thr_bf; i++) {sem_post(empty);}
}

void* launch_gm_gp(void* inutile) {
    (void) inutile; // Pour éviter d'avoir un avertissement de variable inutilisée.

    while(true) {
        if (err != 0) {
            for (uint8_t i = 0; i < *nb_thr_bf; i++) {sem_post(empty);}
            pthread_exit(NULL);
        }

        if (pthread_mutex_lock(&mutex_write_index) != 0) {fprintf(stderr, "Erreur lors du lock des mutex\n"); err = 1; pthread_exit(NULL);}
        if (write_index >= graphe->NB_NODES) {
            if (pthread_mutex_unlock(&mutex_write_index) != 0) {fprintf(stderr, "Erreur lors du unlock des mutex\n"); err = 1; pthread_exit(NULL);}
            return NULL;
        }
        write_index++;
        if (pthread_mutex_unlock(&mutex_write_index) != 0){fprintf(stderr, "Erreur lors du unlock des mutex\n"); err = 1; pthread_exit(NULL);}

        result_bellman_ford_t* result_bf = dequeue();
        if (result_bf == NULL) {err = 1; pthread_exit(NULL);}

        result_get_max_t* result_gm = (result_get_max_t*)malloc(sizeof(result_get_max_t));
        if (result_gm == NULL) {
            error_gm_gp();
            fprintf(stderr, "Pas assez de mémoire disponible\n");
            free(result_bf->dist); free(result_bf->path); free(result_bf);
            pthread_exit(NULL);
        }
        get_max(result_bf->dist, &(result_bf->source), graphe, result_gm);

        result_get_path_t* result_gp = (result_get_path_t*)malloc(sizeof(result_get_path_t));
        if (result_gp == NULL) {
            error_gm_gp();
            fprintf(stderr, "Pas assez de mémoire disponible\n");
            free(result_bf->dist); free(result_bf->path); free(result_bf); free(result_gm);
            pthread_exit(NULL);
        }
        if (get_path(&(result_gm->max_node), result_bf->path, &(result_bf->source), result_gp) != 0) {
            error_gm_gp();
            free(result_bf->dist); free(result_bf); free(result_gm); free(result_gp);
            pthread_exit(NULL);
        }

        if ((out==stdout)||(out==stderr)) {
            if (pthread_mutex_lock(&mutex_write) != 0) {fprintf(stderr, "Erreur lors du lock des mutex\n"); err = 1; free(result_bf->dist); free(result_bf); free(result_gm); free(result_gp->r); free(result_gp); pthread_exit(NULL);}
            show(out, &graphe->NB_NODES, result_bf, result_gm, result_gp);
            if (pthread_mutex_unlock(&mutex_write) != 0) {fprintf(stderr, "Erreur lors du unlock des mutex\n"); err = 1; pthread_exit(NULL);}
        }
        else {
            if (pthread_mutex_lock(&mutex_write) != 0) {fprintf(stderr, "Erreur lors du lock des mutex\n"); free(result_bf->dist); free(result_bf); free(result_gm); free(result_gp->r); free(result_gp); error_gm_gp(); pthread_exit(NULL);}
            if (write_file(out, result_bf, result_gm, result_gp) != 0) {
                error_gm_gp();
                pthread_mutex_unlock(&mutex_write);
                pthread_exit(NULL);
            }
            if (pthread_mutex_unlock(&mutex_write) != 0) {fprintf(stderr, "Erreur lors du unlock des mutex\n"); err = 1; pthread_exit(NULL);}
        }
    }
}