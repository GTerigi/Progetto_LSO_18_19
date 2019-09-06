/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria contenente i dati condivisi fra Client e Server, e alcune funzioni
 * miscellanee
 */

#include <common/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

char *concatChar(char *dest, char c) {
  size_t len = strlen(dest);
  char *str2 = malloc(len + 1 + 1); /* Il char in più e \0 */
  strcpy(str2, dest);
  str2[len] = c;
  str2[len + 1] = '\0';
  return str2;
}

char *initMessage(size_t size) {
  char *b = malloc(sizeof(char) * size);
  memset(b, '\0', MAXMESSAGE);
  return b;
}

char *initPath(char *directory, char *filename) {
  size_t length = strlen(STORE_DIR) + strlen(directory) + 2;
  if (filename)
    length += strlen(filename) + 1;
  char *path = (char *)malloc(sizeof(char) * length);

  if (filename)
    sprintf(path, "%s/%s/%s", STORE_DIR, directory, filename);
  else
    sprintf(path, "%s/%s", STORE_DIR, directory);
  return path;
}

int createDir(char *path) {
  struct stat sb = {0};
  if (!(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))) {
    if (mkdir(path, 0777) == 0)
      return 1;
    return 0;
  }
  return 1;
}

size_t getSize(char *file_name) {
  struct stat sb = {0};
  if (stat(file_name, &sb) == -1) {
    errno = EINVAL;
    return -1;
  }
  return sb.st_size;
}

char *concatString(char *s1, char *s2) {
  char *result =
      malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
  // in real code you would check for errors in malloc here
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}
