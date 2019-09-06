/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria di supporto per il Server
 */

#define _POSIX_C_SOURCE 200112L
#include <IO/io.h>
#include <MyHash/hash.h>
#include <common/common.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <serverLib/serverLib.h>
#include <signal.h>
#include <socket/socket.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

static pthread_mutex_t mutexHash = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexNumClient = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexNumByte = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexNumFile = PTHREAD_MUTEX_INITIALIZER;

static int numClientConnected; // Numero di client connessi
static unsigned long long
    objStoreSizeByte; // indica la grandezza in byte dell'objstore
static unsigned long long
    objStoreNumFile; // indica il numero di file contenuti nell'objstore

// pthread_cond_t condNumThread = PTHREAD_COND_INITIALIZER;

static MyHash *hashtable;

void initInfo() {
  pthread_mutex_lock(&mutexNumClient);
  numClientConnected = 0;
  pthread_mutex_unlock(&mutexNumClient);

  pthread_mutex_lock(&mutexNumByte);
  objStoreSizeByte = 0;
  pthread_mutex_unlock(&mutexNumByte);

  pthread_mutex_lock(&mutexNumFile);
  objStoreNumFile = 0;
  pthread_mutex_unlock(&mutexNumFile);
}

void incrClient(int i) {
  pthread_mutex_lock(&mutexNumClient);
  numClientConnected += i;
  pthread_mutex_unlock(&mutexNumClient);
}

void incrByte(int byte) {
  pthread_mutex_lock(&mutexNumByte);
  objStoreSizeByte += byte;
  pthread_mutex_unlock(&mutexNumByte);
}

void incrFile(int file) {
  pthread_mutex_lock(&mutexNumByte);
  objStoreNumFile += file;
  pthread_mutex_unlock(&mutexNumByte);
}

int initServerHash() {
  pthread_mutex_lock(&mutexHash);
  hashtable = initHash(HASHSIZE);
  pthread_mutex_unlock(&mutexHash);
  if (hashtable == NULL) {
    return 0;
  }
  return 1;
}

int initData() {
  struct stat sb = {0};
  if (!(stat(STORE_DIR, &sb) == 0 && S_ISDIR(sb.st_mode))) {
    if (mkdir(STORE_DIR, 0777) == 0)
      return 1;
    return 0;
  }
  return 1;
}

int setUpServer() {
  int notUsed;
  if ((notUsed = initServerHash()) != 1) {
    SENDERROR("Errore creazione Hash\n", 0);
  }

  if ((notUsed = initData()) != 1) {
    SENDERROR("Errore creazione Directory iniziale\n", 0);
  }
  initInfo();

  int conFD = -1;
  struct sockaddr_un sockAddr = createSockAddr();
  SYSCALL(conFD, socket(AF_UNIX, SOCK_STREAM, 0), "Creazione socket Server");

  if (bind(conFD, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1) {
    perror("Bind");
    exit(0);
  }
  if (listen(conFD, SOMAXCONN) == -1) {
    perror("Listen");
    exit(0);
  }
  return conFD;
}

int ParseOperation(int conFD, char *message) {
  char *op = (char *)calloc(9, sizeof(char));
  char *name = (char *)calloc(255, sizeof(char));
  size_t len = 0;
  sscanf(message, "%s %s %zu \n", op, name, &len);

  int ok;
  if (strcmp(op, "REGISTER") == 0)
    ok = registerClient(conFD, name);
  else if (strcmp(op, "STORE") == 0)
    ok = storeClientData(conFD, name, len);
  else if (strcmp(op, "RETRIVE") == 0)
    ok = SendBlock(conFD, name);
  else if (strcmp(op, "DELETE") == 0)
    ok = DeleteBlock(conFD, name);
  else if (strcmp(op, "LEAVE") == 0)
    ok = LeaveClient(conFD);
  else {
    ok = -1;
    sendError(conFD, -1); // Richiesta non riconosciuta
  }

  // Libera la memoria occupata dalle stringhe
  free(op);
  free(name);
  // Restituisce il flag restituito dai controller
  return ok;
}

int registerClient(int conFD, char *name) {
  char *dirPath = initPath(name, NULL);
  if (createDir(dirPath) != 1) {
    sendError(conFD, -2); // Errore nella registrazione
    SENDERROR("Errore crezione cartella\n", 0);
  }
  free(dirPath);
  pthread_mutex_lock(&mutexHash);
  HashSetPair(hashtable, conFD, name);
  pthread_mutex_unlock(&mutexHash);

  incrClient(1);

  sendOK(conFD);
  return 1;
}

int storeClientData(int conFD, char *name, int len) {
  sendOK(conFD);
  // Sono pronto a recuperare il File;
  pthread_mutex_lock(&mutexHash);
  char *dirName = GetValue(hashtable, conFD);
  pthread_mutex_unlock(&mutexHash);
  char *path = initPath(dirName, name);
  int fileFD = open(path, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0700);
  if (fileFD == -1) {
    sendError(conFD, -3); // Errore nella store
    SENDERROR("ERRORE Nell'apertura del file\n", 0);
  }
  unsigned char *array = malloc(sizeof(unsigned char) * len);
  if (array == NULL) {
    sendError(conFD, -3); // Errore nella store
    SENDERROR("Array di supporto nullo\n", 0);
  }

  if (readn(conFD, array, len) < 0) { // Legge il blocco sul FD
    sendError(conFD, -7);             // Errore nella read
    return 0;
  }
  if (writen(fileFD, array, len) != 1) { // scrive il blocco sul FD
    sendError(conFD, -8);                // Errore nella write
    return 0;
  }
  incrByte(len);
  incrFile(1);
  sendOK(conFD);

  return 1;
}

int SendBlock(int conFD, char *objName) {
  pthread_mutex_lock(&mutexHash);
  char *dirName = GetValue(hashtable, conFD);
  pthread_mutex_unlock(&mutexHash);
  char *path = initPath(dirName, objName);

  size_t filesize = getSize(path);
  char *message = initMessage(MAXMESSAGE);
  sprintf(message, "DATA %zu \n", filesize);
  if (SendMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  free(message);
  message = initMessage(MAXMESSAGE);
  if (GetMessage(conFD, message, sizeof(char) * MAXMESSAGE) != 1)
    exit(0); // Stampo già dentro un eventuale errore
  if (strcmp(message, "OK \n") != 0) {
    printf("%s\n", message);
    return 0;
  }

  int fileFD = open(path, O_RDONLY, 0700);
  void *buffer = malloc(filesize);
  if (readn(fileFD, buffer, filesize) < 0) {
    sendError(conFD, -7); // Errore nella readn
    return 0;
  }
  close(fileFD);
  SendMessage(conFD, buffer, filesize);
  return 1;
}

int DeleteBlock(int conFD, char *objName) {
  pthread_mutex_lock(&mutexHash);
  char *dirName = GetValue(hashtable, conFD);
  pthread_mutex_unlock(&mutexHash);
  char *path = initPath(dirName, objName);
  int size = getSize(path);
  remove(path);
  incrByte(-size);
  incrFile(-1);
  sendOK(conFD);
  return 1;
}

int LeaveClient(int conFD) {
  sendOK(conFD);
  incrClient(-1);
  return 2;
}

void stampaRecord() {
  pthread_mutex_lock(&mutexNumClient);
  fprintf(stdout, "Dati:\nNumero Client attualmente attivi: %d\n",
          numClientConnected);
  pthread_mutex_unlock(&mutexNumClient);

  pthread_mutex_lock(&mutexNumFile);
  fprintf(stdout, "Numero Files attualmente in store: %llu\n", objStoreNumFile);
  pthread_mutex_unlock(&mutexNumFile);

  pthread_mutex_lock(&mutexNumByte);
  fprintf(stdout, "Numero Bytes: %llu bytes attualmente in store\n",
          objStoreSizeByte);
  pthread_mutex_unlock(&mutexNumByte);
}

int CheckNumClient() {
  pthread_mutex_lock(&mutexNumClient);
  int num = numClientConnected;
  pthread_mutex_unlock(&mutexNumClient);
  return num;
}