#if !defined(IO_H)
#define IO_H
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Legge dal FD e immagazzina sul buffer
 * @param fd        File Desc su cui leggere
 * @param buffer    Destinazione dei dati letti
 * @param size      Byte da leggere
 * @return numero di byte letti, error su errore
 */
int readn(long fd, void *buf, size_t size);

/**
 * Scrive dal FD il contenuto del buffer
 * @param fd        File Desc su cui scrivere
 * @param buffer    origine dei dati 
 * @param size      Byte da scrivere
 * @return 1, error su errore
 */
int writen(long fd, void *buf, size_t size);
#endif