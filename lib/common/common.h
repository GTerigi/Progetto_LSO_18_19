

#if !defined(COMMON_H)
#define COMMON_H
/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria contenente i dati condivisi fra Client e Server, e alcune funzioni
 * miscellanee
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(r, c, print, e)                                                  \
  if ((r = c) != 1) {                                                          \
    print;                                                                     \
    exit(e);                                                                   \
  }

#define SENDERROR(p, e)                                                        \
  fprintf(stderr, "%s", p);                                                    \
  exit(e);

#define SYSCALL(r, c, e)                                                       \
  if ((r = c) == -1) {                                                         \
    perror(e);                                                                 \
    exit(errno);                                                               \
  }

#define SOCKETNAME "./objstore.sock"

#define HASHSIZE 100

// I nomi dei file in Linux sono di massimo 255 caratteri
#define MAXMESSAGE 267

#define STORE_DIR "./data"

/**
 * Concatena in coda una stringa e un carattere
 * @param dest  stringa di base
 * @param c     Carattere da aggiungere
 * @return char* stringa concatenata
 */
char *concatChar(char *dest, char c);

/**
 * Inizializza una stringa vuota di soli '\0' di lunghezza size
 * @param size  Lunghezza della stringa
 * @return char* stringa formattata
 */
char *initMessage(size_t size);

/**
 * Restituisce il percorso formattato
 * @param directory   Nome della dir
 * @param filename    Nome del File (op)
 * @return path
 */
char *initPath(char *directory, char *filename);

/**
 * Crea la directory al percorso dato
 * @param path  percorso della dir
 * @return 1 se è presente la cartella o è stata creata, 0 altrimeni
 */
int createDir(char *path);

/**
 * Restituisce la size del file
 * @param filename  Percorso del File
 * @return size del file, -1 su errore
 */
size_t getSize(char *file_name);

char *concatString(char *dest, char *c);

#endif