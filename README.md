# Progetto_LSO_18_19
Progetto del corso di Laboratorio di Sistemi Operativi dell'Universita di Pisa, anno accademico 2018/2019.

Tutto il codice è stato scritto da me, ad accezione della tabella hash utilizzata, che è di proprietà di [Tony Thompson](https://gist.github.com/tonious/1377667/104d94bb2f5b87a09dfd8cb181e302f19e46e1aa) e poi modificata per l'occorrenza.

### Overview
Il progetto consiste nella realizzazione di una struttura client-server.
Il server, definito anche _objstore_, riceve le richieste di memorizzazione e recupero di blocchi di dati dei client (definite nel dettaglio successivamente) e la comunicazione fra i due avviene in una socket del dominio locale.

### Eseguire il progetto
All'interno della directory sono presenti 3 script bash.
Il primo ```setUpProject.sh```, serve per generare i file oggetto e le librerie necessarie all'esecuzione.
Successivamente è possibile eseguire il server digitando nel teminale:
```./bin/server.o```
Per far partire la batteria di test si può usare lo script ```test1.sh``` o usare il comando ```make test```.
Una volta eseguita la batteria di test, si può controllare l'esito eseguendo lo script di analisi ```testsum.sh```.

### ObjectStore
L'_objectstore_ è un eseguibile il cui scopo è quello di ricevere dai client delle richieste di memorizzare, recuperare, cancellare blocchi di dati dotati di nome, detti _oggetti_.
L'objectstore gestisce uno spazio di memorizzazione separato per ogni client. I nomi degli oggetti sono garantiti essere univoci all'interno dello spazio di memorizzazione di un client, e i nomi dei client sono garantiti essere tutti distinti. Tutti i nomi rispettano il formato dei nomi di file POSIX.

Per collegarsi all'_objectstore_ il client manda un messaggio di richiesta, con un formato conosciuto; in risposta il server crea un thread per gestire le eventuali richieste del client. Il thread servente termina quando il client manda un messaggio esplicito di deregistrazione oppure se si verifica un errore.

Internamente, l'_objectstore_ memorizza gli oggetti che gli vengono affidati (e altri eventuali dati che si
rendessero necessari) nel file system, all'interno di file che hanno per nome il nome dell'oggetto. Questi file
sono poi contenuti in directory che hanno per nome il nome del client a cui l'oggetto appartiene. Tutte le
directory dei client sono contenute in una directory data all'interno della working directory del server.

### Protocollo di Comunicazione
La comunicazione fra client e server è definita dalle seguenti coppie di messaggio-risposta:

* Registrazione: Inviato dal cliente per registrarsi sull'objectstore; quest'ultimo risponde con l'esito
dell'operazione
  * Request: REGISTER name \n
  * Response: OK \n (registrazione riuscita) oppure KO message \n (registrazione fallita)
  
* Memorizzazione: inviato dal cliente per memorizzare un oggetto nel proprio spazio privato
sull'objectstore; quest'ultimo risponde con l'esito dell'operazione.
  * Request: STORE name len \n data (in cui data è un blocco binario di lunghezza len bytes)
  * Response: OK \n (memorizzazione riuscita) oppure KO message \n (memorizzazione fallita)

* Lettura: inviato dal cliente per recuperare i dati di un oggetto precedentemente memorizzato nel
proprio spazio privato sull'objectstore; quest'ultimo risponde con i dati richiesti, oppure una
segnalazione di errore.
  * Request: RETRIEVE name \n
  * Response: DATA len \n data (se l'operazione ha avuto successo, in cui data è un blocco binario
di lunghezza len bytes) oppure KO message \n (in caso di fallimento)

* Cancellazione: inviato dal client per cancellare un oggetto memorizzato sull'objectstore;
quest'ultimo risponde con una indicazione di successo.
  * Request: DELETE name \n
  * Response: OK \n (cancellazione riuscita) oppure KO message \n (cancellazione fallita)

* Disconnessione: inviato dal client per chiudere in maniera ordinata la connessione verso l'objectstore.Ha sempre successo.
  * Request: LEAVE \n
  * Response: OK \n
  
#### Per gli studenti
Per gli studenti frequentanti il corso, è consigliabile visionare il progetto solo per vedere una possibile implementazione. Il copia-incolla di codice sorgente, è altamente sconsigliato. Non mi assumo nessuna responsabilita' di come userete questa implementazione del progetto.
