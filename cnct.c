/*
   mtcs - a multithreaded chat serveur
   Philippe Marquet, Apr 2005

   Gestion de la connexion d'un client par un thread
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>

#include "stat.h"
#include "config.h"
#include "tools.h"
#include "cnct.h"

struct stats stats = { 0, 0, 0, 0, 0, 0 };

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

/* Gestion des sockets */
static int sockets[MAX_CONNECTION]; /* tableau initialisé a zero */
void assertError(int i);

static void
add_socket(int fd)
{
  int i;
  pthread_mutex_lock(&mutex);

  pgrs_in();
  for (i=0; i<MAX_CONNECTION; i++) {
    if (sockets[i] == 0) {
      sockets[i] = fd;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
  assert(i!=MAX_CONNECTION);

    add_nb_clients(&stats, 1);
  pgrs_out();
  
}

static void
del_socket(int fd)
{
  int i;
  pthread_mutex_lock(&mutex);
  
  pgrs_in();
  for (i=0; i<MAX_CONNECTION; i++) {
    if (sockets[i] == fd) {
      sockets[i] = 0;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
  assert(i!=MAX_CONNECTION);
    add_nb_clients(&stats, -1);
  pgrs_out();
}

/* Un client  */
static void
repeater(int sckt)
{
    char buf[MAX_BUFFER];
    int nbc, i;
    const char WELCOME[] = "mtcs : bienvenu\n";

    pgrs_in();
    write(sckt, WELCOME, strlen(WELCOME));

    pgrs("enregistrement d'une socket");
    add_socket(sckt);

    while (1) {
        pgrs("attente read");
        nbc = read(sckt, buf, MAX_BUFFER);
	add_l_recues(&stats, 1);
        if (nbc <= 0) {
            pgrs("fin lecture client");

            pgrs("desenregistrement d'une socket");
            del_socket(sckt);
            close(sckt);
            pgrs_out();
            return;
        }
        pgrs("boucle ecriture");
        for(i=0; i<MAX_CONNECTION; i++)
	  if (sockets[i]) {
	    write(sockets[i], buf, nbc);
	    add_l_envoyees(&stats, 1);
	  }
        pgrs("fin boucle ecriture");
    }
}

void* p_repeater(void * arg)
{
  repeater((int) arg);
  return arg;
}

/* Création d'un client */
int
manage_cnct(int fd)
{
  pthread_t thread;
  int status;
  pgrs_in();  

  status = pthread_create(&thread, NULL, p_repeater, (void*) fd);
  assertError(status);

  status = pthread_detach(thread);
  pgrs_out();
  return 0;
}

void assertError(int i){
  switch(i){
  case EAGAIN : printf("Ressources insuffisantes pour créer un nouveau  thread\n");
    exit(EXIT_FAILURE);
  case EINVAL : printf("Paramètres invalides dans attr.\n"); 
    exit(EXIT_FAILURE);
  case EPERM : printf("Permissions  insuffisantes pour définir la politique d'ordonnancement et les paramètres spécifiés dans attr.\n");
    exit(EXIT_FAILURE);
  default :;
  }
  return ;
}
