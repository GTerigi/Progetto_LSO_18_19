#include <clientLib/clientLib.h>
#include <common/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/**
 * @author Giulio Terigi N.M. 550319
 *
 * Il Client è stato implementato come richiesto dalle linee guida del Progetto.
 * (http://didawiki.di.unipi.it/lib/exe/fetch.php/informatica/sol/laboratorio19/testoprogetto2019.pdf)
 *
 * Per eseguire le 3 operazioni viene usato un piccolo switch case, così
 * descritto: case 1: os_store; case 2: os_retrieve; case 3: os_delete;
 *
 * Il parametro di scelta viene passato come argomento al processo
 * Se non ci sono errori esegue la os_disconnect e termina il processo
 */

typedef struct client_argoments {
  char *name;
  int test;
} clientArgs;

int main(int argc, char **argv) {

  if (argc != 3) {
    fprintf(stderr, "\tError\nUse: %s <Process Name> <TEST>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  clientArgs *client = (clientArgs *)malloc(sizeof(clientArgs));
  client->name = strdup(argv[1]);
  client->test = strtol(argv[2], NULL, 10);
  if (client->test < 1 || client->test > 3) {
    fprintf(stderr, "Error, Test must be between 1 and 3");
    free(client);
    exit(EXIT_FAILURE);
  }
  if (!os_connect(client->name)) {
    exit(EXIT_FAILURE);
  }
  switch (client->test) {
  case 1: {
    if (os_store(client->name) == 1) {
      printf("[%s] OS_STORE completata correttamente\n", client->name);
    }
    break;
  }
  case 2: {
    if (os_retrive(client->name) == 1) {
      printf("[%s] OS_RETRIVE completata correttamente\n", client->name);
    }
    break;
  }
  case 3: {
    if (os_delete(client->name) == 1) {
      printf("[%s] OS_DELETE completata correttamente\n", client->name);
    }
    break;
  }
  default: {
    fprintf(stderr, "ERROR TEST CASE NOT SUPPORTED\nKILLING PROCESS\n");
    free(client);
    exit(EXIT_FAILURE);
    break; // non ci arriva mai..
  }
  }

  if (os_disconnect() == 1) {
    printf("[%s] DISCONNESSIONE completata correttamente\n", client->name);
  }
  return 0;
}
