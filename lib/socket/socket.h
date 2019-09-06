#if !defined(SOCKET_H)
#define SOCKET_H

struct sockaddr_un createSockAddr();

int SendMessage(int FileDesc, char *buffer, size_t size);

int GetMessage(int FileDesc, char *buffer, size_t size);

void sendOK(int conFD);
void sendError(int conFD, int error);
#endif