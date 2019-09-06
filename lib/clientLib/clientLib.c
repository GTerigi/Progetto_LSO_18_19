/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria di supporto per le operazioni del Client
 */

#include <IO/io.h>
#include <clientLib/clientLib.h>
#include <common/common.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <socket/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

static int conFD = -1;

int os_connect(char *name) {
  if (name == NULL || strlen(name) < 0) {
    SENDERROR("Invalid Argument", 0);
  }
  struct sockaddr_un sockAddr = createSockAddr();

  SYSCALL(conFD, socket(AF_UNIX, SOCK_STREAM, 0), "Creazione Socket Client");
  // Eseguo la connessione del Client alla socket
  int myErr;
  SYSCALL(myErr, connect(conFD, (struct sockaddr *)&sockAddr, sizeof(sockAddr)),
          "Connect ClientFD");

  // Sono connesso

  char *message = initMessage(MAXMESSAGE);
  sprintf(message, "REGISTER %s \n", name);
  if (SendMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  free(message);
  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore

  if (strcmp(message, "OK \n") != 0) {
    int error = checkError(message);
    return error;
  } else {
    printf("[%s] CONNESSIONE completata correttamente\n", name);
  }
  return 1;
}

int os_store(char *name) {
  char alfabeto[2] = "A"; // Lettera e \0
  // 100000 Byte --> 100KByte
  unsigned char *array = (unsigned char *)malloc(100000);
  for (int i = 0; i < 100000; i++)
    array[i] = (unsigned char)i;

  int size = 1000;
  int step = (100000 - 1000) / 20;
  int check;
  for (int i = 0; i < 20; i++) {
    if (i == 19)
      size = 100000;
    char *tmp2 = concatChar(name, alfabeto[0]);
    char *tmp = concatString(tmp2, ".txt");
    if ((check = storeData(tmp, array, size)) != 1) {
      free(array);
      SENDERROR("Errore nella store\n", 0);
    }
    size += step;
    alfabeto[0]++;
  }
  return 1;
}

int storeData(char *name, unsigned char *array, int size) {
  if (!(name != NULL && array != NULL && size > 0 && conFD > 0)) {
    SENDERROR("Invalid Argument per la store\n", 0);
  }

  char *message = initMessage(MAXMESSAGE);
  sprintf(message, "STORE %s %ld \n", name, size);
  if (SendMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  free(message);
  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  if (strcmp(message, "OK \n") != 0) {
    printf("%s\n", message);
    int error = checkError(message);
    return error;
  }

  if (SendMessage(conFD, array, sizeof(char) * size) != 1)
    exit(0); // Stampo già dentro un eventuale errore

  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore

  if (strcmp(message, "OK \n") != 0) {
    printf("%s\n", message);
    int error = checkError(message);
    return error;
  }
  return 1;
}

int os_retrive(char *name) {
  char alfabeto[2] = "A"; // Lettera e \0
  unsigned char *array = (unsigned char *)malloc(100000);
  for (int i = 0; i < 100000; i++)
    array[i] = (unsigned char)i;

  int step = (100000 - 1000) / 20;
  int size = 1000;

  for (int i = 0; i < 20; i++) {
    if (i == 19) // Ultimo blocco
      size = 100000;
    char *tmp2 = concatChar(name, alfabeto[0]);
    char *tmp = concatString(tmp2, ".txt");
    unsigned char *retrivedBlock = retriveBlock(tmp); // Recupera il dato
    if (retrivedBlock == NULL) {                      // Controllo non sia NULL
      free(retrivedBlock);
      SENDERROR("Blocco recuperato NULL\n", 0);
    }

    int ok = 1;
    for (int i = 0; i < size; i++) {
      if (array[i] != retrivedBlock[i]) {
        ok = 0;
        break;
      }
    }
    free(tmp);
    free(retrivedBlock);
    if (ok != 1) {
      SENDERROR("DATA NOT CORRESPONDING\n", 0);
    }

    size += step;
    alfabeto[0]++;
  }
  return 1;
}

unsigned char *retriveBlock(char *ObjName) {
  char *message = initMessage(MAXMESSAGE);
  sprintf(message, "RETRIVE %s \n", ObjName);
  if (SendMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  free(message);
  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  size_t len = 0;
  sscanf(message, "DATA %zu \n", &len);
  free(message);
  sendOK(conFD);

  unsigned char *array = malloc(sizeof(unsigned char) * len);
  readn(conFD, array, len);
  return array;
}

int os_delete(char *name) {
  char alfabeto[2] = "A"; // Lettera e \0

  for (int i = 0; i < 20; i++) {
    char *tmp = concatChar(name, alfabeto[0]);
    if (deleteBlock(tmp) != 1) {
      SENDERROR("Errore nell'eliminazione di un file\n", 0);
    }
    alfabeto[0]++;
  }
  return 1;
}

int deleteBlock(char *ObjName) {
  char *message = initMessage(MAXMESSAGE);
  sprintf(message, "DELETE %s \n", ObjName);
  if (SendMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore

  if (strcmp(message, "OK \n") != 0) {
    printf("%s\n", message);
    int error = checkError(message);
    return error;
  }
  return 1;
}

int os_disconnect() {
  char *message = initMessage(MAXMESSAGE);
  sprintf(message, "LEAVE \n");
  if (SendMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore

  if (strcmp(message, "OK \n") != 0) {
    printf("%s\n", message);
    int error = checkError(message);
    return error;
  }
  return 1;
}

int checkError(char *message) {
  int error;
  sscanf(message, "KO %d\n", &error);
  return error;
}