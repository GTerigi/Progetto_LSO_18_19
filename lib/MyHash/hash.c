/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria per le operazioni con la Hash
 */
#include <MyHash/hash.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Inizialiazza una MyHash di 2*@size Elementi
 * @size --> La dimensione della MyHash
 */
MyHash *initHash(size_t size) {
  MyHash *hashTable = NULL;
  if (size < 1)
    return NULL;

  // Inizializzazione della MyHash
  if ((hashTable = malloc(sizeof(MyHash))) == NULL)
    return NULL;

  if ((hashTable->table = malloc(sizeof(HashElem) * (2 * size))) == NULL)
    return NULL;

  for (int i = 0; i < (2 * size); i++) {
    hashTable->table[i] = NULL;
  }

  hashTable->size = (2 * size);

  return hashTable;
}

size_t GetHash(int key, size_t size) {
  size_t hash = 5381;
  return (key * hash) % (size);
}

/**
 * Istanzia una nuova coppia chiave-valore Int-String
 * @key -> Chiave della coppia Int
 * @value -> Valore della coppia String
 */
HashElem *growAPair(int key, char *value) {
  HashElem *newPair = NULL;

  if ((newPair = malloc(sizeof(HashElem))) == NULL)
    return NULL;

  if ((newPair->value = strdup(value)) == NULL)
    return NULL;
  newPair->key = key;
  newPair->next = NULL;

  return newPair;
}

void HashSetPair(MyHash *myHash, int key, char *value) {
  size_t cell = GetHash(key, myHash->size);
  HashElem *newpair = NULL;
  HashElem *next = NULL;
  HashElem *last = NULL;
  next = myHash->table[cell];

  // Scorro la lista finchè non trovo la chiave o ho finito la lista
  while (next != NULL && next->key > key) {
    last = next;
    next = next->next;
  }
  // Se ho trovato la chiave che cerco
  if (next != NULL && next->key == key) {
    next->value = strdup(value); // Cambio la chiave
  } else {
    // Non ho trovato nulla --> Grow A Pair!
    newpair = growAPair(key, value);
    if (next == myHash->table[cell]) {
      newpair->next = next;
      myHash->table[cell] = newpair;

    } else if (next == NULL) {
      last->next = newpair;
    } else {
      newpair->next = next;
      last->next = newpair;
    }
  }
}

/**
 * Rimuove dalla Hash un valore di chiave key
 *
 * @param myHash --> Hash su cui effettuare l'operazione
 * @key --> chiave della coppia
 */
void HashRemovePair(MyHash *myHash, int key) {
  size_t cell = GetHash(key, myHash->size);
  HashElem *next = NULL;
  HashElem *last = NULL;
  next = myHash->table[cell];

  // Scorro la lista finchè non trovo la chiave o ho finito la lista
  while (next != NULL && next->key > key) {
    last = next;
    next = next->next;
  }
  // Se ho trovato la chiave che cerco
  if (next != NULL && next->key == key) {
    if (next->next == NULL)
      free(next);
    else {
      last->next = next->next;
      free(next);
    }
  }
  if (last == NULL) {
    myHash->table[cell] = NULL;
  }
}

/**
 * Restituisce una rappresentazione umanamente leggibile della Hash.
 *
 * \param myHash -> HashTable su cui effettuare la ricerca
 *
 * \retval s Stringa che descrive il contenuto della hash.
 */
char *HashToStringStd(MyHash *myHash) {
  char *toString = malloc(sizeof(char) * 16384); // Sticazzi
  char *string = malloc(sizeof(char) * 256);
  int count = 0;
  for (int i = 0; i < myHash->size; i++) {
    HashElem *tmp = myHash->table[i];
    while (tmp != NULL) {
      count++;
      sprintf(string, "\t[%d] -> [%s]\n", tmp->key, tmp->value);
      strcat(toString, string);
      tmp = tmp->next;
    }
  }
  strcat(toString, "\0");
  return toString;
}

/**
 * Restituisce una rappresentazione umanamente leggibile della Hash.
 * Include il fattore di carico, la dimensione, il numero di celle occupate e
 * tutte le celle, pure quelle vuote
 *
 * \param myHash -> HashTable su cui effettuare la ricerca
 *
 * \retval s Stringa che descrive il contenuto della hash.
 */
char *HashToStringFull(MyHash *myHash) {
  char *toString = malloc(sizeof(char) * 16384); // Sticazzi
  char *string = malloc(sizeof(char) * 256);
  int count = 0;
  sprintf(string, "HASH\nSize -> %ld \n", myHash->size);
  strcat(toString, string);
  for (int i = 0; i < myHash->size; i++) {
    HashElem *tmp = myHash->table[i];
    sprintf(string, "Cella N.%d\n", i);
    strcat(toString, string);
    while (tmp != NULL) {
      count++;
      sprintf(string, "\t[%d] -> [%s]\n", tmp->key, tmp->value);
      strcat(toString, string);
      tmp = tmp->next;
    }
  }
  float fattoreDiCarico = (float)count / myHash->size;
  sprintf(string, "Actual Size -> %d\nFattore di Carico -> %1.3f", count,
          fattoreDiCarico);
  strcat(toString, string);
  strcat(toString, "\0");
  return toString;
}

/**
 * Restituisce la value della coppia, se presente
 *
 * @param myHash --> La tabella
 * @param key --> chiave della coppia
 *
 * @retun char* --> La chiave se presente, altrimenti NULL
 */
char *GetValue(MyHash *myHash, int key) {
  size_t cell = GetHash(key, myHash->size);
  HashElem *pair;

  /* Step through the bin, looking for our value. */
  pair = myHash->table[cell];
  while (pair != NULL && pair->key > key) {
    pair = pair->next;
  }

  /* Did we actually find anything? */
  if (pair == NULL || pair->key != key) {
    return NULL;

  } else {
    return pair->value;
  }
}

void deleteList(HashElem *head_ref) {
  HashElem *current = head_ref;
  HashElem *next;
  while (current != NULL) {

    next = current->next;
    free(current);
    current = next;
  }
  head_ref = NULL;
}

void FreeHash(MyHash *hashtable) {
  for (int i = 0; i < hashtable->size; i++) {
    deleteList(hashtable->table[i]);
    hashtable->table[i] = NULL;
  }
}