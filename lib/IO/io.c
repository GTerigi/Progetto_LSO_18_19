#include <errno.h>
#include <stdio.h>
#include <unistd.h>
int readn(long fd, void *buf, size_t size) {
  size_t left = size;
  int r;
  char *bufptr = (char *)buf;

  while (left > 0) {
    if ((r = read((int)fd, bufptr, left)) == -1) {
      if (errno == EINTR)
        continue; // in caso di segnale non mascherato che interrompe la read
      return -1;
    }

    if (r == 0)
      return 0; // raggiunto EOF

    left -= r;
    bufptr += r;
  }

  return size;
}

int writen(long fd, void *buf, size_t size) {
  size_t left = size;
  int r;
  char *bufptr = (char *)buf;

  while (left > 0) {
    if ((r = write((int)fd, bufptr, left)) == -1) {
      if (errno == EINTR)
        continue;
      return -1;
    }

    if (r == 0)
      return 0;
    left -= r;
    bufptr += r;
  }

  return 1;
}