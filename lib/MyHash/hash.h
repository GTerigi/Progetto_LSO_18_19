#if !defined(HASH_H)
#define HASH_H

/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria per le operazioni con la Hash
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashElem_ {
  int key;
  char* value;
  struct hashElem_ *next;
} HashElem;

typedef struct Hash_ {
  size_t size;
  HashElem **table;
} MyHash;

/**
 * Inizialiazza una MyHash di 2*@size Elementi
 * @size --> La dimensione della MyHash
 */
MyHash *initHash(size_t size);

size_t GetHash(int key, size_t size);

/**
 * Istanzia una nuova coppia chiave-valore Int-String
 * @key -> Chiave della coppia Int
 * @value -> Valore della coppia String
 */
HashElem *growAPair(int key, char* value);
/**
 * Se è già presente una chiave relativa a key viene sovrascritta, altrimenti
 * viene istanziato una nuova coppia.
 * \param key -> Chiave della coppia int
 * \param value -> Valore della coppia String
 */
void HashSetPair(MyHash *myHash, int key, char *value);

 /**
 * Restituisce una rappresentazione umanamente leggibile della Hash.
 *
 * \param myHash -> HashTable su cui effettuare la ricerca
 *
 * \retval s Stringa che descrive il contenuto della hash.
 */
char *HashToStringStd(MyHash *myHash);

/**
 * Restituisce una rappresentazione umanamente leggibile della Hash.
 * Include il fattore di carico, la dimensione, il numero di celle occupate e
 * tutte le celle, pure quelle vuote
 *
 * \param myHash -> HashTable su cui effettuare la ricerca
 *
 * \retval s Stringa che descrive il contenuto della hash.
 */
char *HashToStringFull(MyHash *myHash);


/**
 * Restituisce la value della coppia, se presente
 *
 * @param myHash --> La tabella
 * @param key --> chiave della coppia
 *
 * @retun char* --> La chiave se presente, altrimenti NULL
 */
char *GetValue(MyHash *myHash, int key);

/**
 * Rimuove dalla Hash un valore di chiave key
 *
 * @param myHash --> Hash su cui effettuare l'operazione
 * @key --> chiave della coppia
 */
void HashRemovePair(MyHash *myHash, int key);

void deleteList(HashElem *head_ref);

void FreeHash(MyHash *hashtable);

#endif