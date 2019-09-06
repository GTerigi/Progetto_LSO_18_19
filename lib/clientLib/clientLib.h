#if !defined (CLIENTLIB_H)
#define CLIENTLIB_H

/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria di supporto per le operazioni del Client
 */

/** 
 * Connette il Client al Server
 * @param name  Nome del Client da connettere
 * @return  1 se la connessione è avvenuta con successo, altrimenti un errore
 */
int os_connect(char *name);

/**
 * Immagazzina nel server 20 elementi di grandezza variabile
 * @param name  Nome del Client
 * @return 1 Se è avvenuta correttamente la store, errore altrimenti
 */
int os_store( char *name);

/**
 * Immagazzina l'array di byte sul server
 * @param name  Nome dell'oggetto
 * @param array Oggetto da immagazzinare
 * @param size  Size dell'oggetto
 * @param conFD File Desc della connessione
 * @return 1 se è avvenuta con successo la store dell'oggetto, errore altrimenti
 */
int storeData(char *name, unsigned char *array, int size);

/**
 * Recupera dal server i files del Client
 * @param name  Nome del Client
 * @return 1 Files recuperati correttamene, error altrimenti
 */
int os_retrive( char *name);

/**
 * Recupera il file di nome ObjName
 * @param ObjName   Nome del File
 * @return unsigned char array  Buffer contenente il file 
 */
unsigned char *retriveBlock( char *ObjName);


/**
 * Elimina i file del Client
 * @param name  Nome del Client
 * @return 1 delete avvenuta con successo, error altrimenti
 */
int os_delete( char *name);

/**
 * Elimina il file di nome ObjName
 * @param ObjName   Nome del File
 * @return 1 on success, error altrimenti
 */
int deleteBlock( char *ObjName);

/**
 * Disconnette il client dal server
 * @return 1 on success, error altrimenti
 */
int os_disconnect();

/**
 * Fa il parse del messaggio e restituisce l'errore
 * @param message   Messaggio del server
 * @return error
 */
int checkError(char *message);
#endif