#include <IO/io.h>
#include <common/common.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

struct sockaddr_un createSockAddr() {
  // Alloca la struttura dati
  struct sockaddr_un sockAddr;
  memset(&sockAddr, '0', sizeof(sockAddr)); // Setta tutti i byte a 0
  sockAddr.sun_family = AF_UNIX;
  strncpy(sockAddr.sun_path, SOCKETNAME, strlen(SOCKETNAME) + 1);
  return sockAddr;
}

int SendMessage(int FileDesc, char *buffer, size_t size) {
  if (!(FileDesc > 0 && buffer != NULL && size > 0)) {
    SENDERROR("Invalid Argument nella writen\n", 0);
  }
  if (writen(FileDesc, buffer, size) != 1) {
    SENDERROR("Errore mentre si scriveva su FD\n", 0);
  }
  return 1;
}
int GetMessage(int FileDesc, char *buffer, size_t size) {
  if (!(FileDesc > 0 && buffer != NULL && size > 0)) {
    SENDERROR("Invalid Argument nella writen\n", 0);
  }
  if (readn(FileDesc, buffer, size) < 0) {
    SENDERROR("Errore nella Readn\n", 0);
  }
  return 1;
}

void sendOK(int conFD) {
  if (SendMessage(conFD, "OK \n", sizeof(char) * MAXMESSAGE) != 1) {
    SENDERROR("Errore nell'inviare il messagio\n", 0);
  }
}

void sendError(int conFD, int error) {
  char *err = initMessage(MAXMESSAGE);
  sprintf(err, "KO %d\n", error);
  if (SendMessage(conFD, err, sizeof(char) * MAXMESSAGE) != 1) {
    SENDERROR("Errore nell'inviare il messagio\n", 0);
  }
}