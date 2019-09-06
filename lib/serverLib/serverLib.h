/**
 * @author Giulio Terigi N.M. 550319
 *
 * Libreria di supporto per il Server
 */
#if !defined (SERVERLIB_H)
#define SERVERLIB_H

/**
 * Inizializza la tabella Hash
 * @return 1    on success, 0 altrimenti
 */
int initServerHash();

/**
 * Inizializza la cartella di supporto per immagazzinare i files
 * @return 1    on success, 0 altrimenti
 */
int initData();

/**
 * Setta il server per l'ascolto sulla socket
 * e inizializza tutto il necessario
 * @return 1    on success, 0 altrimenti
 */
int setUpServer();

/**
 * Recupera dal messaggio l'operazione da eseguire e
 * esegue la funzione adeguata
 * @param conFD     FD del client
 * @param message   Messaggio del Client
 * @return 1    on success, 0 altrimenti
 */
int ParseOperation(int conFD, char *message);

/**
 * Registra il client 
 * @param conFD FD del Client
 * @param name  Nome del Client
 * @return 1    on success, 0 altrimenti
 */
int registerClient(int conFD, char *name);

/**
 * Immagazzina l'oggetto inviato dal Client
 * @param conFD FD del Client
 * @param name  Nome dell'oggetto
 * @param len   Lunghezza in byte dell'oggetto
 * @return 1    on success, 0 altrimenti
 */
int storeClientData(int conFD, char *name, int len);

/**
 * Invia al Client l'oggetto richiesto
 * @param conFD FD del Client
 * @param objName  Nome dell'oggetto
 * @return 1    on success, 0 altrimenti
 */
int SendBlock(int conFD, char *objName);

/**
 * Elimina l'oggetto richiesto
 * @param conFD FD del Client
 * @param objName  nome dell'oggetto
 * @return 1    on success, 0 altrimenti
 */
int DeleteBlock(int conFD, char *objName);

/**
 * Stampa un record della situazione  attuale del Server
 */
void stampaRecord();

/**
 * Inizializza le informazioni generali del server
 */
void initInfo();

/**
 * Incrementa il numero di client connessi di i
 * @param i numero di client da aggiungere/togliere
 */
void incrClient(int i);

/**
 * Incrementa il numero di byte attualmente immagazzinati sul server
 * @param byte numero di byte da aggiungere/togliere
 */
void incrByte(int byte);

/**
 * Incrementa il numero di file attualmente immagazzinati sul server
 * @param file numero di byte da aggiungere/togliere
 */
void incrFile(int file);

/**
 * Esegue la disconnessione del client dal server
 * @param conFD FD del Client
 * @return 2 on success, 0 altrimenti
 */
int LeaveClient(int conFD);

/**
 * Controlla il numero di client connssi attualmente
 * @return num Client connessi
 */
int CheckNumClient();
#endif

