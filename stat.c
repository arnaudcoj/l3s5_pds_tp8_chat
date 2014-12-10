/*
  Les fonctions utilisées pour afficher la structure stat
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct stats {
  int nb_clients;
  int l_recues;
  int l_envoyees;
  int max_clients;
  int l_max_recues;
  int l_max_envoyees;
};

static pthread_mutex_t mutex_stats = PTHREAD_MUTEX_INITIALIZER; 

void init_stats(struct stats *stats) {
  pthread_mutex_lock(&mutex_stats);
  stats->nb_clients     = 0;
  stats->l_recues       = 0;
  stats->l_envoyees     = 0;
  stats->max_clients    = 0;
  stats->l_max_recues   = 0;
  stats->l_max_envoyees = 0;
  pthread_mutex_unlock(&mutex_stats);
  return;
}

void print_stats(struct stats *stats) {
  pthread_mutex_lock(&mutex_stats);
  printf("Statistiques du serveur\n");
  printf("===================================\n");
  printf("Nombre de clients connecté : %d\n", stats->nb_clients);
  printf("Nombre de lignes reçues :    %d\n", stats->l_recues);
  printf("Nombre de lignes envoyées :  %d\n", stats->l_envoyees);
  printf("Maximum de clients :         %d\n", stats->max_clients);
  printf("Maximum de lignes reçues :   %d\n", stats->l_max_recues);
  printf("Maximum de lignes envoyées : %d\n", stats->l_max_envoyees);
  printf("===================================\n");
  pthread_mutex_unlock(&mutex_stats);
  return;
}

void add_nb_clients(struct stats* stats, int to_add) {
  pthread_mutex_lock(&mutex_stats);
  stats->nb_clients += to_add;
  if(stats->nb_clients > stats->max_clients)
    stats->max_clients = stats->nb_clients;
  pthread_mutex_unlock(&mutex_stats);
  return;
}

void add_l_recues(struct stats* stats, int to_add) {
  pthread_mutex_lock(&mutex_stats);
  stats->l_recues += to_add;
  pthread_mutex_unlock(&mutex_stats);
  return;
}

void add_l_envoyees(struct stats* stats, int to_add) {
  pthread_mutex_lock(&mutex_stats);
  stats->l_envoyees += to_add;
  pthread_mutex_unlock(&mutex_stats);
  return;
}

/* deprecated */
void add_max_clients(struct stats* stats, int to_add) {
  pthread_mutex_lock(&mutex_stats);
  stats->max_clients += to_add;
  pthread_mutex_unlock(&mutex_stats);
  return;
}

void add_l_max_recues(struct stats* stats, int to_add) {
  pthread_mutex_lock(&mutex_stats);
  if(to_add > stats->l_max_recues)
    stats->l_max_recues = to_add;
  pthread_mutex_unlock(&mutex_stats);
  return;
}

void add_l_max_envoyees(struct stats* stats, int to_add) {
  pthread_mutex_lock(&mutex_stats);
  if(to_add > stats->l_max_envoyees)
    stats->l_max_envoyees = to_add;
  pthread_mutex_unlock(&mutex_stats);
  return;
}
