# PhalsoPhobia: Un gioco in C

Progetto finale per l'esame di Programmazione Procedurale con Laboratorio a.a. 2022/2023


Nome      -> Leonardo
Cognome   -> Giusti
Matricola -> 344522

Il programma è realizzato con 3 files con le relative dichiarazioni di strutture dati e funzioni.

- main.c    :
	-aggiunta funzione void delay(int n) che implementa un delay di n secondi per permettere al giocatore
	 di vedere in fase di terminazione del gioco il messaggio di saluto.

- gamelib.h :

   - Rispetto a quanto richiesto nelle specifiche originali del programma, sono state modificati i seguenti oggetti:

		enum Tipo_oggetto_iniziale 	{	EMF=1		,
										SPIRIT_BOX	,
										VIDEOCAMERA	,
										END_TO_INI
									};

		enum Tipo_oggetto_zona		{	ADRENALINA=1	,
										CENTO_DOLLARI	,
										COLTELLO		,
										CALMANTI		,
										SALE			,
										NESSUN_OGGETTO  ,
										END_TO_ZONA
									};

		Tale modifica permette di avere una gestione semplificata nella raccolta delle prove
		Infatti,come da regole del gioco,una certa prova, è possibile
		raccoglierla solo se il giocatore è in possesso dell’oggetto corrispondente.
		Ne consegue che con questa modifica il Tipo_oggetto_iniziale è allineato all'oggetto Tipo_prova

		enum Tipo_prova 			{	PROVA_EMF=1			,
										PROVA_SPIRIT_BOX	,
										PROVA_VIDEOCAMERA	,
										NESSUNA_PROVA       ,
										END_PROVA
									};


-gamelib.c:

- Inserita  la  voce di menu "termina_gioco()"  menù della funzione "gioca()".
  Tale voce di menù permette ai giocatori di abbandonare la partita anche se non conclusa.

- Per non rendere l'esito del gioco (Vittoria o Sconfitta) un "mistero", durante il funzionamento del gioco
  verrà dato sempre un riscontro dello stato di gioco via terminale.

- Come da regola del gioco, il giocatore che muove in un dato turno è scelto a caso, anche se prima di poter rigiocare un turno
  tutti i rimanenti giocatori devono aver giocato il loro.
  Tuttavia, può capitare che un giocatore abbia 2 turni consecutivi.
  Infatti al turno successivo, l'ordine è nuovamente ricalcolato e l'ultimo giocatore del turno precedente ha la stessa probabilità
  degli altri giocatori di essere il primo del turno successivo.
  Inoltre si è scelto di non considerare come turno i comandi di stampa_giocatore() e stampa_zona().

- Sono presenti delle  funzioni di utilità per la gestione del programma:

	"void LeggiInput ()": Legge l'input dell'utente tenendo  conto degli errori che possono nascere dalla lettura del buffer
	                      di input come il carattere di nuova riga.


	"int Cercavalore()": Cerca una chiave all'interno di un array di 4 posizioni.

	"int Generazione_casuale()" : Genera un numero casuale, con uguale probabilità, tra due valori estremi con la possibilità NEL CASO DI RANGE
	                              DI 4 VALORI di escluderne alcuni.
	"int findCeil()"
	"int myRand":    Funzioni di utilità che generano un numero casuale sulla base di una frequenza di probabilita

- Come scelta progettuale la mappa e conseguentemente la gestione delle liste è stata fatta utilizzando un campo di utilità (num_elemento) all'interno 
  della struttura Zona_mappa. Questo per semplificare la gestione delle liste senza utilizzare l'aritmetica dei puntatori.
