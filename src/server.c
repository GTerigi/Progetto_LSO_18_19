#include <common/common.h>
#include <pthread.h>
#include <serverLib/serverLib.h>
#include <signal.h>
#include <socket/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

/**
 * @author Giulio Terigi N.M. 550319
 *
 * Il Server è stato implementato come richiesto dalle linee guida del Progetto.
 * (http://didawiki.di.unipi.it/lib/exe/fetch.php/informatica/sol/laboratorio19/testoprogetto2019.pdf)
 *
 * Il server accetta le connessioni attraverso una pselect con timeout. Tutte le
 * nuove connessioni vengono gestite tramite un nuovo thread, che esegue tutte
 * le richieste del client fino a che non si disconnette. A quel punto il thread
 * chiude.
 *
 * Il server viene terminato non appena riceve un segnale
 */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500

static int terminated = 0;

void *signal_handler(void *ptr) {
  // Set di segnali da aspettare
  sigset_t set = *((sigset_t *)ptr);
  // Stampa un messaggio di log
  printf(
      "\n\n\t\t[SERVER] SigHandler ON\nInviare un segnale SIGINT, SIGTERM per "
      "chiudere il server\n\n");
  // Entra nel loop di attesa dei segnali
  int signal;
  int notUsed;
  while (!terminated) {
    // Attende un segnale
    SYSCALL(notUsed, sigwait(&set, &signal), "sigWait");
    // Riconosce il tipo di segnale
    if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT ||
        signal == SIGALRM) {
      terminated = 1;
    } else if (signal == SIGUSR1) {
      stampaRecord();
    }
  }
  printf("\n[SERVER] SigHandler OFF\n\n\t\tServer in fase di terminazione.\n");
  return NULL;
}

void cleanup() { unlink(SOCKETNAME); }
void *newConnectionHandler(void *arg);

int main(int argc, char **argv) {
  cleanup();
  atexit(cleanup);
  int notUsed;

  sigset_t set, oldSet;
  SYSCALL(notUsed, sigemptyset(&set), "sigemptyset");
  SYSCALL(notUsed, sigemptyset(&oldSet), "sigemptyset");
  SYSCALL(notUsed, sigfillset(&oldSet), "sigfillset");
  SYSCALL(notUsed, sigaddset(&set, SIGPIPE), "SIGPIPE");
  SYSCALL(notUsed, sigaddset(&set, SIGINT), "SIGINT");
  SYSCALL(notUsed, sigaddset(&set, SIGTERM), "SIGTERM");
  SYSCALL(notUsed, sigaddset(&set, SIGQUIT), "SIGQUIT");
  SYSCALL(notUsed, sigaddset(&set, SIGUSR1), "SIGUSR1");
  SYSCALL(notUsed, sigaddset(&set, SIGALRM), "SIGALARM");

  if (pthread_sigmask(2, &set, NULL) != 0) {
    SENDERROR("Errore nella pthread_sigmask", 1);
  }
  pthread_t sig_handler_id;
  pthread_create(&sig_handler_id, NULL, signal_handler, (void *)&set);

  int listenFD =
      setUpServer(); // Crea la hash, la directory e si mette in ascolto;
  if (listenFD < 0) {
    SENDERROR("FD del Server non valido\n", EXIT_FAILURE);
  }

  // node *ListThread = NULL;
  struct timespec t;
  t.tv_sec = 1;
  t.tv_nsec = 0;
  while (1) {
    sleep(0.5);
    fd_set FDset;
    FD_ZERO(&FDset);
    FD_SET(listenFD, &FDset);

    int err = pselect(listenFD + 1, &FDset, NULL, NULL, &t, NULL);
    if (err < 0 && errno != EINTR) {
      perror("pselect");
      break;
    }

    if (terminated) {
      break;
    }

    if (FD_ISSET(listenFD, &FDset)) {
      int clientFD;
      SYSCALL(clientFD, accept(listenFD, (struct sockaddr *)NULL, NULL),
              "Accept Server");
      pthread_t newConn;
      pthread_attr_t atth;
      if (clientFD > 0 && terminated == 0) {
        if (pthread_attr_init(&atth) != 0) {
          fprintf(stderr, "Error pthread_attr_init\n");
          break;
        }
        if (pthread_attr_setdetachstate(&atth, PTHREAD_CREATE_DETACHED) != 0) {
          fprintf(stderr, "errore pthread_attr_setdetachstate\n");
          pthread_attr_destroy(&atth);
          break;
        }

        if (pthread_create(&newConn, &atth, &newConnectionHandler,
                           (void *)clientFD) != 0) {
          fprintf(stderr, "errore pthread_create\n");
          pthread_attr_destroy(&atth);
          break;
        }
      }
    }
  }
  while (CheckNumClient()) {
    printf("Attendiamo che tutti i client si siano disconnessi\n");
    sleep(1);
  }
  stampaRecord();
  return 0;
}
void *newConnectionHandler(void *arg) {
  int clientFD = (int)arg;
  int kill = 0;
  if (!(clientFD > 0))
    pthread_exit(0);
  while (!kill) {
    char *buffer = initMessage(MAXMESSAGE);
    if (GetMessage(clientFD, buffer, sizeof(char) * MAXMESSAGE) != 1) {
      SENDERROR("Errore nel recuperare il buffer\n", 0);
    }
    int ok = ParseOperation(clientFD, buffer);
    if (ok != 1) {
      if (ok == 2)
        kill = 1;
      else {
        SENDERROR("Errore nella chiusura del thread", 0);
      }
    }
  }
  pthread_exit(0);
}
