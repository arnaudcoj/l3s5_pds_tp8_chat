#ifndef STAT_H
#define STAT_H

/*
  Les fonctions utilisées pour afficher la structure stat
 */
struct stats {
  int nb_clients;
  int l_recues;
  int l_envoyees;
  int max_clients;
  int l_max_recues;
  int l_max_envoyees;
};
void init_stats(struct stats* stats);
void print_stats(struct stats* stats);
void add_nb_clients(struct stats* stats, int to_add);
void add_l_recues(struct stats* stats, int to_add);
void add_l_envoyees(struct stats* stats, int to_add);
void add_max_clients(struct stats* stats, int to_add);
void add_l_max_recues(struct stats* stats, int to_add);
void add_l_max_envoyees(struct stats* stats, int to_add);

#endif
