//questo file contiene tutte le definizioni di funzioni necessarie per il gioco.
//tutte le funzioni che non sono imposta_gioco(), gioca(), termina_gioco(), devono
//essere definite con lo specificatore static, dato che non devono essere visibili
//all'esterno di questo file.

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"gamelib.h"

//Varibile che determina la fine del gioco
int fine_gioco = 1;

//Puntatore alla testa della mappa
Zona_mappa* pTestaListaZona;

//Puntatore alla coda della mappa
Zona_mappa* pCodaListaZona;

//Variabile array utilizzata nella tracciatura delle prove superate dai giocatori
int ProveTrovate[END_PROVA];


enum Tipo_difficolta 		{	DILETTANTE=0,
								INTERMEDIO	,
								INCUBO
							};

enum Contenuto_Zaino		{
								OGGETTO_INIZIALE=0,
								OGGETTO_ZONA,
								PROVA,
								DUMMY
							};

enum Tipo_difficolta Difficolta;

//Variabile utilizzata nella scelta del turno di gioco per evitare
//che un giocatore giochi più volte nella stessa rondata di gioco
int  escludiGiocatore[4];

//Variabile che indica che tutte le prove sono state trovate
int  FlagTutteProveTrovate;

//Variabile che indica la presenza del fantasma in zona dove
//un giocatore ha raccolto una prova
int  FantasmaPresente;

//Varibile che indica la quantitè (in base al livello di gioco)
//di decremento della sanita mentale
int  quantita;


/******************************************************************************************
*
*
*      FUNZIONI DI UTILITA'
*
*
*******************************************************************************************/

/*Struttura dati utilizzata per stampare a video le informazioni di Nome Oggetto Iniziale*/
static const char * const Nome_Oggetto_Iniziale[] =
{
	[EMF] 			  = "EMF",
	[SPIRIT_BOX] 	  = "SPIRIT_BOX",
	[VIDEOCAMERA]     = "VIDEOCAMERA",
	[END_TO_INI]      = "NESSUN OGGETTO",
};

/*Struttura dati utilizzata per stampare a video le informazioni di Nome Livello DIfficolta*/
static const char * const Nome_Livello_Difficolta[] =
{
	[DILETTANTE] 	= "DILETTANTE",
	[INTERMEDIO] 	= "INTERMEDIO",
	[INCUBO] 		= "INCUBO"
};

/*Struttura dati utilizzata per stampare a video le informazioni di Nome Tipo Zona*/
static const char * const Nome_Tipo_zona[]=
{

  [CARAVAN] 	 	 = "CARAVAN",
	[CUCINA] 		   = "CUCINA",
	[SOGGIORNO]  	 = "SOGGIORNO",
	[CAMERA] 		   = "CAMERA",
	[BAGNO] 		   = "BAGNO",
	[GARAGE] 		   = "GARAGE",
	[SEMINTERRATO] = "SEMINTERRATO"
};

static const char * const Nome_Tipo_oggetto_zona[]=
{

  [ADRENALINA] 	  = "ADRENALINA",
	[CENTO_DOLLARI]	= "CENTO_DOLLARI",
	[COLTELLO] 		  = "COLTELLO",
	[CALMANTI] 	    = "CALMANTI",
	[SALE] 			    = "SALE",
	[NESSUN_OGGETTO]= "NESSUN_OGGETTO"

};

static const char * const Nome_Tipo_prova[]=
{

  [PROVA_EMF] 		    = "PROVA_EMF",
	[PROVA_SPIRIT_BOX]	= "PROVA_SPIRIT_BOX",
	[PROVA_VIDEOCAMERA]	= "PROVA_VIDEOCAMERA",
	[NESSUNA_PROVA] 	  = "NESSUNA_PROVA"
};

void LeggiInput(char Sinput[], int dim)
{

	//utilizzo la sizeof per evitare la fflush e per svuotare tutti i caratteri speciali

	if (fgets(Sinput, sizeof(char)*dim, stdin)==NULL)
	{
		printf("ERRORE LETTURA TIPO DIFFICOLTA");
	}
	else{
		Sinput[strlen(Sinput)-1]= '\0';
	}
}

int Cercavalore (int valore, int arr[])
{
	int i=0;
	int ret=0;

	for (i=0; i<4; i++)
	{
		if(valore==arr[i])
		{
			ret=1;
			break;
		}
	}
	return (ret);
}

int Generazione_casuale (int minimum_number, int max_number, int escludi[4])
{

	int valore=0;

	if (escludi[0]!=-1)
	{
		valore= rand() % (max_number + 1 - minimum_number) + minimum_number;

		while(Cercavalore(valore,escludi))
		{
			valore=rand() % (max_number + 1 - minimum_number) + minimum_number;
		}
	}
	else
	{
		valore= rand() % (max_number + 1 - minimum_number) + minimum_number;
	}
	return(valore);
}

//fuzione utilita per trovare il tetto massimo di arr
int findCeil(int arr[], int r, int l, int h)
{
	int mid;
	while(l<h)
    {
		mid = l + ((h - l) >> 1);
		(r > arr[mid]) ? (l = mid + 1) : (h = mid);
	}
	return (arr[l] >= r) ? l : -1;
}

//funzione che genera numero casuale sulla base di una frequenza di probabilita
int myRand(int arr[], int freq[], int n)
{

	//creare e riempire gli array prefissi
	int prefix[n], i;
	prefix[0] = freq[0];

	for (i = 1; i < n; i++)
	{
		prefix[i] = prefix[i - 1] + freq[i];
	}
	//prefix[i-1] è la somma di tutte le frequenze. Generare un numero random con
	//valore tra 1 e questa somma
	int r = (rand()% prefix[n - 1]) + 1;

	//trova l'indice del massimale di r nell'array di prefissi
	int indexc= findCeil (prefix, r, 0, n-1);

	return arr[indexc];
}

//inserimento di un elemento in coda
void inserisci_zona(void)
{

	int arr[]  = {CUCINA, SOGGIORNO, CAMERA, BAGNO,GARAGE,SEMINTERRATO};
	int freq[] = {100, 100, 100, 100, 100, 100};
	int arr2[]  = {ADRENALINA, CENTO_DOLLARI, COLTELLO, CALMANTI, SALE, NESSUN_OGGETTO};
	int freq2[] = {60, 60, 60, 60, 60, 40};
	int n = sizeof(arr) / sizeof(arr[0]);
	int n2 = sizeof(arr2) / sizeof(arr2[0]);

   //crea una zona
   Zona_mappa *z = (Zona_mappa*) calloc(1, sizeof(Zona_mappa));

	 //inserisce elementi zona
	 z->zona         = myRand(arr, freq, n);
	 z->oggetto_zona = myRand(arr2, freq2, n2);
	 z->prova        = NESSUNA_PROVA;


	 if (pCodaListaZona == NULL)
	 {

		 z->num_elemento=1;
		 z->next = z;
		 pCodaListaZona = z;
		 pTestaListaZona=pCodaListaZona;

		 printf("\n (zona: %-15s oggetto: %-15s prova: %-15s) \n",Nome_Tipo_zona[z->zona],
																  Nome_Tipo_oggetto_zona[z->oggetto_zona],
																  Nome_Tipo_prova[z->prova]);
	 }//fine if
	 else
	 {
		 z->num_elemento= 1+ pCodaListaZona->num_elemento;
		 z->next = pCodaListaZona->next;
		 pCodaListaZona->next = z;
		 pCodaListaZona = z;

		 printf("\n (zona: %-15s oggetto: %-15s prova: %-15s) \n",Nome_Tipo_zona[z->zona],
																  Nome_Tipo_oggetto_zona[z->oggetto_zona],
																  Nome_Tipo_prova[z->prova]);
	 }//fine else
}

//cancella l'ultima zona in CODA
void cancella_zona(void)
{

	//salva il riferimento all'attuale coda della lista zona
	Zona_mappa *ElementoDaEliminare	= NULL;
	Zona_mappa *ElementoPrecedente	= NULL;
	Zona_mappa* pTesta;
	Zona_mappa* pCoda;
	int start=0;
	int stop=0;

	pTesta=pTestaListaZona;
	pCoda=pCodaListaZona;


	if (pTesta==NULL)
	{
		printf("---------------------\n");
		printf("LA LISTA MAPPA VUOTA \n");
		printf("---------------------\n");
		printf("PER INIZIARE IL GIOCO INSERIRE ALMENO UN ELEMENTO NELLA MAPPA!!\n");
    }
    else
	{

		stop = pCoda->num_elemento;

		for(start = 0; start < stop - 1; start++)
		{

			ElementoPrecedente=pTesta;
			pTesta = pTesta->next;
		}//fine for

		ElementoDaEliminare=pTesta;
		if (stop==1)
		{

			free(ElementoDaEliminare);
			pTestaListaZona=NULL;
			pCodaListaZona=NULL;
		}
		else
		{
			ElementoPrecedente->next = ElementoDaEliminare->next;
			pCodaListaZona = ElementoPrecedente;
			free(ElementoDaEliminare);
		}
    }//fine else
}

//vengono stampati tutti i campi di tutte le zone create
void stampa_mappa(void)
{

	Zona_mappa* pTesta;
	Zona_mappa* pCoda;
	int start=0;
	int stop=0;

	pTesta = pTestaListaZona;
	pCoda = pCodaListaZona;

	if (pTesta==NULL)
	{

		printf("LA LISTA MAPPA E' VUOTA\n");

	}
	else
	{

		stop = pCoda->num_elemento;

		for(start=0; start < stop; start++)
		{
			printf(" (zona: %-15s oggetto: %-15s prova: %15s) \n",	Nome_Tipo_zona[pTesta->zona],
																	Nome_Tipo_oggetto_zona[pTesta->oggetto_zona],
																	Nome_Tipo_prova[pTesta->prova]	);
			pTesta = pTesta->next;
		}
	}

	printf("\n Il numero di zone inserite è: %d\n",stop);
}

/*menu che consente di creare una mappa, quindi: inserire una zona, cancellare l'ultima zona inserita,
stampare tutte le zone create*/
void creazione_mappa(void)
{

	int b;
	int loop=1;
	char s_b[2+1];

	pTestaListaZona = NULL;
	pCodaListaZona = pTestaListaZona;

	do
	{
		printf("\n   --------- \n");
		printf("\n  |  MENU': |\n");
		printf("\n   --------- \n");
		printf("\n\n1) INSERIMENTO DI UNA ZONA AL TERMINE DELLA LISTA\n");
		printf("\n	2) CANCELLA L'ULTIMA ZONA INSERITA NEL PERCORSO\n");
		printf("\n	3) STAMPA I CAMPI DI TUTTE LE ZONE CREATE\n");
		printf("\n	4) FINE DELLA CREAZIONE DELLA MAPPA\n");
		printf("\n	SCEGLIERE IL COMANDO (1,2,3,4): \n");
		LeggiInput(s_b, sizeof(s_b));
		b = atoi(s_b);


		switch (b)
		{
			case 1:
				inserisci_zona();
				printf("\n--------------------------------------\n");
				printf("\n HAI INSERITO CORRETTAMENTE LA ZONA!!\n");
				printf("\n--------------------------------------\n");
			break;

			case 2:
				printf("\n----------------------------------------\n");
				printf("\n CANCELLAZIONE ULTIMA ZONA INSERITA!!\n");
				printf("\n----------------------------------------\n");
				cancella_zona();
				stampa_mappa();
			break;

			case 3:
				printf("\n----------------------------\n");
				printf("\n RIEPILOGO MAPPA INSERITA \n");
				printf("\n----------------------------\n");
				stampa_mappa();
			break;

			case 4:
				if (pTestaListaZona==NULL)
				{
					printf("\n-------------------------------------------------------------------\n");
					printf("\n               NON  RISULTA INSERITA NESSUNA ZONA! \n");
					printf("\n QUESTA IMPOSTAZIONE DI GIOCO E' NECESSARIA PER INIZARE A GIOCARE! \n");
					printf("\n-------------------------------------------------------------------\n");
				}
				else
				{
					loop=0;
				}
			break;

			default:
				printf("\n------------------------------------------\n");
				printf("   INSERISCI UN COMANDO VALIDO!!  (1,2,3,4) \n");
				printf("\n------------------------------------------\n");
			break;
		}
	}
	while(loop);
}

/*funzione che serve per riportare le prove al caravan, grazie a questo
se si riportano tutte le prove si vince il gioco*/
void torna_caravan(int numero_giocatore, int numero_giocatori, Giocatore *Giocatori, enum Tipo_zona ZonaDelFantasma)
{
    int escludi[4] = {-1,-1,-1,-1};
    int i=0;
    int countp=0;

    for (i=0; i<numero_giocatori; i++)
    {
       if (Giocatori[i].zaino[OGGETTO_INIZIALE]==EMF)
       {
           escludi[0]=0;
       }
       else if (Giocatori[i].zaino[OGGETTO_INIZIALE]==SPIRIT_BOX)
       {
           escludi[1]=0;
       }
       else if (Giocatori[i].zaino[OGGETTO_INIZIALE]==VIDEOCAMERA)
       {
           escludi[2]=0;
       }
    }

    for (i=0; i<4; i++)
    {
        if (escludi[i]==0) countp++;
    }

    if (countp==3)
    {
        escludiGiocatore[0] = -1;
        escludiGiocatore[1] = -1;
        escludiGiocatore[2] = -1;
        escludiGiocatore[3] = -1;
    }

	if (Giocatori[numero_giocatore].zaino[PROVA] == NESSUNA_PROVA) {

		printf("\n Non possiedi prove da scaricare.\n");
		printf("\n Hai perso il turno!!\n");
		printf("\n----------------------------------\n");
	}
	else {

		if (Giocatori[numero_giocatore].posizione->zona == ZonaDelFantasma)
        {
            ; //non faccio nulla
        }
        else
        {
            ProveTrovate[Giocatori[numero_giocatore].zaino[PROVA]]++;
            printf("\n Giocatore %s hai scaricato nel caravan la prova %s",Giocatori[numero_giocatore].nome_giocatore, Nome_Tipo_prova[Giocatori[numero_giocatore].zaino[PROVA]]);
            Giocatori[numero_giocatore].zaino[OGGETTO_INIZIALE] = Generazione_casuale(EMF, END_TO_INI-1,escludi);
            Giocatori[numero_giocatore].zaino[OGGETTO_ZONA] = NESSUN_OGGETTO;
            Giocatori[numero_giocatore].zaino[PROVA] = NESSUNA_PROVA;
            Giocatori[numero_giocatore].zaino[DUMMY] = 0;
            Giocatori[numero_giocatore].posizione=pTestaListaZona;

            if  ((ProveTrovate[PROVA_EMF] >= 1) &&
            (ProveTrovate[PROVA_SPIRIT_BOX] >= 1) &&
            (ProveTrovate[PROVA_VIDEOCAMERA] >= 1))
            {

                    FlagTutteProveTrovate =  1;
                    fine_gioco = 0;
                    printf("\n ------------------------------------------- \n");
                    printf("\n TUTTE LE PROVE SONO STATE SUPERATE!! \n");
                    printf("\n ------------------------------------------- \n");
            }
            else
            {
                printf("\n ---CI SONO ANCORA PROVE DA SUPERARE--- \n");
            }
        }
	}


}

//elencati tutti i dati dei giocatori
void stampa_giocatore(int numero_giocatore, Giocatore *Giocatori)
{

    printf("\n NOME DEL GIOCATORE %s \n",             Giocatori[numero_giocatore].nome_giocatore);
	printf("\n SANITA MENTALE %d \n",                 Giocatori[numero_giocatore].sanita_mentale);
	printf("\n OGGETTO INIZIALE NELLO ZAINO %s \n",   Nome_Oggetto_Iniziale[Giocatori[numero_giocatore].zaino[OGGETTO_INIZIALE]]);
	printf("\n OGGETTO NELLO ZAINO %s \n",            Nome_Tipo_oggetto_zona[Giocatori[numero_giocatore].zaino[OGGETTO_ZONA]]);
	printf("\n PROVA NELLO ZAINO %s \n",              Nome_Tipo_prova[Giocatori[numero_giocatore].zaino[PROVA]]);
	printf("\n --------------------------------\n");
}

/*elencati i campi dove si trova il giocatore, che tipo di prova è presente
in quella determinata zona e che tipo di oggetto si trova nella zona dove è il giocatore*/
void stampa_zona(int numero_giocatore, Giocatore *Giocatori)
{
	printf("\n La zona dove si trova il GIOCATORE è: %s \n",                       Nome_Tipo_zona[Giocatori[numero_giocatore].posizione->zona]);
	printf("\n ---------------------------------------------------------------- \n");
	printf("\n PROVA presente nella zona dove si trova il GIOCATORE è: %s \n",  Nome_Tipo_prova[Giocatori[numero_giocatore].posizione->prova]);
	printf("\n ---------------------------------------------------------------- \n");
	printf("\n OGGETTO nella zona dove si trova il GIOCATORE è: %s \n",         Nome_Tipo_oggetto_zona[Giocatori[numero_giocatore].posizione->oggetto_zona]);
	printf("\n ---------------------------------------------------------------- \n");
}

//funzione che permette al giocatore di avanzara su un'altra zona casuale
void avanza(int numero_giocatore, Giocatore *Giocatori)
{

	int arr[]  = {PROVA_EMF, PROVA_SPIRIT_BOX, PROVA_VIDEOCAMERA, NESSUNA_PROVA};
	int freq[] = {20, 20, 20, 20, 40};
	int n = sizeof(arr) / sizeof(arr[0]);

  //Il giocatore avanza sulla zona successiva della mappa
	Giocatori[numero_giocatore].posizione=Giocatori[numero_giocatore].posizione->next;
	printf("\n Il GIOCATORE %s e' avanzato alla zona --> %s \n",Giocatori[numero_giocatore].nome_giocatore, Nome_Tipo_zona[Giocatori[numero_giocatore].posizione->zona]);

	//Quando un giocatore avanza e arriva in una certa zona, il campo prova di questa zona viene aggiornato in modo casuale
	Giocatori[numero_giocatore].posizione->prova = myRand(arr, freq, n);
	printf("\n La nuova PROVA nella ZONA %s e': %s \n", Nome_Tipo_zona[Giocatori[numero_giocatore].posizione->zona], Nome_Tipo_prova[Giocatori[numero_giocatore].posizione->prova]);
}

/*funzione che serve per impostare la probabilita che il fantasma compare
in base al livello di difficolta scelto*/
int PresenzaFantasma(int *quantita)
{

  int arr[]  = {0, 1};
	int freq[] = {50, 50};
	int n = sizeof(arr) / sizeof(arr[0]);
	int ret=0;

	if (Difficolta==DILETTANTE)
    {
       freq[0]=80;
       freq[1]=20;
       *quantita = 10;
    }
    else if (Difficolta==INTERMEDIO)
    {
       freq[0]=50;
       freq[1]=50;
       *quantita=20;
    }
    else
    {
       freq[0]=20;
       freq[1]=80;
       *quantita=30;
    }
    ret=myRand(arr, freq, n);

    return(ret);
}

//funzione che permette di raccogliere l'oggetto nella stanza se è possibile
void raccogli_oggetto(int numero_giocatore, Giocatore *Giocatori)
{

	if (Giocatori[numero_giocatore].posizione->oggetto_zona != NESSUN_OGGETTO)
    {

		if (Giocatori[numero_giocatore].zaino[OGGETTO_ZONA] == NESSUN_OGGETTO)
        {

	        Giocatori[numero_giocatore].zaino[OGGETTO_ZONA] = Giocatori[numero_giocatore].posizione->oggetto_zona;
					printf("\n Hai raccolto : %s\n", Nome_Tipo_oggetto_zona[Giocatori[numero_giocatore].posizione->oggetto_zona]);
				}
				else{
					printf("\n---------------------------------------------\n");
					printf("\n  Non puoi raccogliere, lo ZAINO e' pieno!! \n");
					printf("\n---------------------------------------------\n");

				}

	}
	else{

		printf("\n-----------------------------------------------\n");
		printf("\n Non puoi raccogliere perchè non e' presente!! \n");
		printf("\n-----------------------------------------------\n");
	}
}

//funzione che permette di utilizzare l'oggetto presente nello zaino del giocatore
void usa_oggetto(int numero_giocatore, int numero_giocatori, Giocatore *Giocatori)
{

	switch(Giocatori[numero_giocatore].zaino[1])
	{
	    case SALE:
			if (FantasmaPresente){

				Giocatori[numero_giocatore].sanita_mentale = Giocatori[numero_giocatore].sanita_mentale + quantita;
				printf("\nComplimenti, hai utilizzato l'oggetto SALE\n");
				printf("\n-------------------------------------------\n");
			}
	    break;

	    case CALMANTI:
			Giocatori[numero_giocatore].sanita_mentale = Giocatori[numero_giocatore].sanita_mentale + 40;
			printf("\nComplimenti, hai utilizzato l'oggetto CALMANTI\n");
			printf("\n-----------------------------------------------\n");
	    break;

	    case CENTO_DOLLARI:{
				int arr[]  = {CALMANTI, SALE};
        int freq[] = {50, 50};
        int n = sizeof(arr) / sizeof(arr[0]);
        Giocatori[numero_giocatore].zaino[1]=myRand(arr, freq, n);
				printf("\nComplimenti, hai utilizzato l'oggetto CENTO_DOLLARI\n");
				printf("\n----------------------------------------------------\n");
			}
			break;

	    case COLTELLO:
			if(Giocatori[numero_giocatore].sanita_mentale <= 30){

                int i = 0;
                for (i = 0; i < numero_giocatori; i++){

                    if (i != numero_giocatore){

                      Giocatori[numero_giocatore].sanita_mentale = 0;
                    }
                }

            }
						printf("Complimenti, hai utilizzato l'oggetto CONTELLO \n");
						printf("\n---------------------------------------------\n");

	    break;

	    case ADRENALINA:
			printf("\nComplimenti, hai utilizzato l'oggetto ADRENALINA\n");
			printf("\n------------------------------------------------\n");
			avanza(numero_giocatore,Giocatori);
	    break;
	}
}

//funzione che permette ai giocatori di passare il proprio turno
int passa(int numero_giocatore, Giocatore *Giocatori)
{

	printf("\nIl GIOCATORE %s ha passato il turno. \n", Giocatori[numero_giocatore].nome_giocatore);
	printf("\n-----------------------------------------------");
	return 0;


}

/*funzione che permette a chi gioca di: raccogliere la prova, sapere dove si trova il fantasma
e quanta sanita mentale rimane per giocare*/
void raccogli_prova(int numero_giocatore, int numero_giocatori, Giocatore *Giocatori)
{

    char * ptr=NULL;
    int i=0 , j=0;

    FantasmaPresente = 0;
    quantita = 0;
    printf("\n -GIOCATORE %s \n",Giocatori[numero_giocatore].nome_giocatore);
    printf("\n -L'oggetto nel tuo zaino: %s \n", Nome_Oggetto_Iniziale[Giocatori[numero_giocatore].zaino[OGGETTO_INIZIALE]]);
    printf("\n -La prova nella zona dove ti trovi: %s\n", Nome_Tipo_prova[Giocatori[numero_giocatore].posizione->prova]);

    FantasmaPresente=PresenzaFantasma(&quantita);

    // il giocatore � in possesso dell�oggetto INIZIALE?? corrispondente alla prova
    ptr = strstr(Nome_Tipo_prova[Giocatori[numero_giocatore].posizione->prova],Nome_Oggetto_Iniziale[Giocatori[numero_giocatore].zaino[OGGETTO_INIZIALE]] );
		if (ptr!=NULL){

        Giocatori[numero_giocatore].zaino[OGGETTO_INIZIALE]=Giocatori[numero_giocatore].posizione->prova;
        Giocatori[numero_giocatore].zaino[PROVA]=Giocatori[numero_giocatore].posizione->prova;
        printf("\n -Giocatore %s hai raccolto la prova %s\n", Giocatori[numero_giocatore].nome_giocatore, Nome_Tipo_prova[Giocatori[numero_giocatore].zaino[PROVA]]);

        if (FantasmaPresente){

            printf("\n -NO! ATTENZIONE %s , Il FANTASMA si e' spostato nella zona dove ti trovi!! \n",Giocatori[numero_giocatore].nome_giocatore);
            printf("\n -La sanita' mentale tua e degli altri giocatori che si trovano in questa zona sara'\n");
            printf("\n  decrementata di %d unita' \n",quantita);
            Giocatori[numero_giocatore].sanita_mentale = Giocatori[numero_giocatore].sanita_mentale - quantita;
            j=0;
            if ((Giocatori[numero_giocatore].sanita_mentale) <= 0){

              printf("\n -Giocatore %s hai perso la vita!..sei fuori dal gioco! \n",Giocatori[numero_giocatore].nome_giocatore);
              escludiGiocatore[j]=numero_giocatore;
            }
            j++;
            for (i=0; i<numero_giocatori; i++, j++){

                if ((Giocatori[numero_giocatore].posizione == Giocatori[i].posizione) && (i!= numero_giocatore)){

                   printf ("\n -Giocatore %s, anche tu se nella zona del fantasma!",Giocatori[i].nome_giocatore);
                   Giocatori[i].sanita_mentale= Giocatori[i].sanita_mentale - quantita;

                   if ((Giocatori[i].sanita_mentale) <= 0){

                      printf("\n -Giocatore %s hai perso la vita!..sei fuori dal gioco! \n",Giocatori[i].nome_giocatore);
                      escludiGiocatore[j]=i;
                   }
                }
            }
        }
    }
    else{

			printf("\n -NON PUOI RACCOGLIERE LA PROVA!\n");
			printf("\n -Per raccogliere la prova devi avere l'oggetto corrispondente!!\n");
			if (FantasmaPresente){

				printf("\n -NOOOO! ATTENZIONE|| %s , Il FANTASMA si e' spostato nella zone dove ti trovi!! \n",Giocatori[numero_giocatore].nome_giocatore);
        printf("\n -La sanita' mentale tua e degli altri giocatori che si trovano in questa zona sara' decrementata di %d unita' \n",quantita);

				if (Giocatori[numero_giocatore].sanita_mentale > quantita){

					Giocatori[numero_giocatore].sanita_mentale= Giocatori[numero_giocatore].sanita_mentale - quantita;
				}
				else {

					Giocatori[numero_giocatore].sanita_mentale=0;
				}

				j = 0;

				if ((Giocatori[numero_giocatore].sanita_mentale) == 0){

					printf("\n -Giocatore %s hai perso la vita!..sei fuori dal gioco! \n",Giocatori[numero_giocatore].nome_giocatore);
					escludiGiocatore[j]=numero_giocatore;
				}

				j++;

				for (i = 0; i < numero_giocatori; i++, j++){

					if ((Giocatori[numero_giocatore].posizione == Giocatori[i].posizione) && (i!= numero_giocatore)){

						printf ("\n -GIOCATORE %s, anche tu sei nella zona del FANTASMA!",Giocatori[i].nome_giocatore);

						if (Giocatori[i].sanita_mentale > quantita){

							Giocatori[i].sanita_mentale= Giocatori[i].sanita_mentale - quantita;
						}
						else {

							Giocatori[i].sanita_mentale=0;
						}

						if ((Giocatori[i].sanita_mentale) == 0){

							printf("\n -GIOCATORE %s HAI PERSO LA VITA!..SEI FUORI DAL GIOCO! \n",Giocatori[i].nome_giocatore);
							escludiGiocatore[j]=i;
						}
					}
				}

				{
					int tutti_morti = 0;

					for (i = 0; i < numero_giocatori; i++) {

						if ((Giocatori[i].sanita_mentale) == 0) {
							tutti_morti++;
						}
					}

					if (tutti_morti == numero_giocatori) {

						fine_gioco = 0;
					}
				}
			}
		}
	}

//funzione che libera la memoria
void dealloca(void)
{

	Zona_mappa* pTesta;
	Zona_mappa* pCoda;
	int start = 0;
	int stop = 0;

	pTesta = pTestaListaZona;
	pCoda = pCodaListaZona;

	if (pTesta != NULL){

		stop = pCoda->num_elemento;

		for(start = 0; start < stop; start++) {

			cancella_zona();
		}
	}

  pTestaListaZona = NULL;
	pCodaListaZona  = NULL;
}


/* -------------------
  | FUNZIONI DI GIOCO |
   -------------------  */


void imposta_gioco (int *numero_giocatori, Giocatore *Giocatori)
{

	int i = 0;
	int j = 0;
	int OggettoIniziale[4] = {0,0,0,0};
	int GiocatoreScelto = 0;
	int loop_num_giocatori = 1;
	int loop_scelta_oggetto_iniziale = 1;

	char s_num_giocatori[10+1];
	char nome_giocatore[MAX_LENGHT_NAME+1];
	char s_Difficolta[10+1];
	char SceltaOggettoIniziale[10+1];

	/*Inizializzazione*/
	memset(ProveTrovate,0,sizeof(ProveTrovate));
	Difficolta = DILETTANTE;
	for (i = 0; i < 4; i++){


		strcpy(Giocatori[i].nome_giocatore,"");
		Giocatori[i].sanita_mentale = 0;
		Giocatori[i].zaino[OGGETTO_INIZIALE] = 0;
		Giocatori[i].zaino[OGGETTO_ZONA] = NESSUN_OGGETTO;
		Giocatori[i].zaino[PROVA] = NESSUNA_PROVA;
		Giocatori[i].zaino[DUMMY] = 0;
		Giocatori[i].posizione = NULL;
	}

	do{

		printf ("Inserisci il numero di giocatori (MAX 4 giocatori): ");
		LeggiInput(s_num_giocatori,sizeof(s_num_giocatori));
		*numero_giocatori = atoi(s_num_giocatori);
		if ((*numero_giocatori >= 1) && (*numero_giocatori <= 4)){

			loop_num_giocatori = 0;
		}
		else {

			printf ("\nVALORE NON AMMESSO!\n\n");
		}
	} while(loop_num_giocatori);


	//INIZIALIZZAZIONE GIOCATORI CON NOME E SANITA_MENTAE
	for (i = 0; i < *numero_giocatori; i++){


		printf ("\nInserisci il nome del giocatore %d: ",i+1);
		LeggiInput(nome_giocatore,sizeof(nome_giocatore));

		// si tronca nome giocatore a MAX_LENGHT_NAME caratteri
		nome_giocatore[MAX_LENGHT_NAME]='\0';

		if (nome_giocatore[0] != '\0'){

			strcpy(Giocatori[i].nome_giocatore,nome_giocatore);
		}
		else {

			sprintf(Giocatori[i].nome_giocatore,"GIOCATORE %d", i+1);
		}

		Giocatori[i].sanita_mentale = 100;
	}

	// INPUT LIVELLO DI DIFFICOLTA
	printf ("\nInserisci il livello di difficolta del gioco: \n");
	printf ("\nDILETTANTE = 0; INTERMEDIO = 1; INCUBO = 2; \n");
	LeggiInput(s_Difficolta,sizeof(s_Difficolta));
	Difficolta = atoi(s_Difficolta);

	//CASO DI SCELTA FUORI RANGE
	if (Difficolta > 2) Difficolta = 2;
	if (Difficolta < 0) Difficolta = 0;

	//GENERAZIONE CASUALE OGGETTO INIZIALE DA FAR SCEGLIERE AI GIOCATORI
	{
		int escludi[4] = {-1,-1,-1,-1};

		printf("\nOggetti iniziali disponibili:");
		for (i = 0; i < *numero_giocatori; i++)
        {

			OggettoIniziale[i] = Generazione_casuale(EMF, END_TO_INI-1,escludi);
			printf("\n %d - %s \n", OggettoIniziale[i], Nome_Oggetto_Iniziale[OggettoIniziale[i]]);
		}

		for (i = 0; i < *numero_giocatori; i++)
        {
            loop_scelta_oggetto_iniziale = 1;
			GiocatoreScelto = Generazione_casuale(0, *numero_giocatori-1,escludi);
			printf("\n\n Giocatore %s scegli il tuo oggetto iniziale: ", Giocatori[GiocatoreScelto].nome_giocatore);

			do{

				LeggiInput(SceltaOggettoIniziale,sizeof(SceltaOggettoIniziale));

				if ((Cercavalore(atoi(SceltaOggettoIniziale),OggettoIniziale)) &&
				   (atoi(SceltaOggettoIniziale) > 0))
                {

					loop_scelta_oggetto_iniziale = 0;
				}
				else
                {

					printf ("\nScegli l'oggetto tra quelli disponibili!\n\n");

					for (j = 0; j< *numero_giocatori; j++) {

						printf("\n %d - %s", OggettoIniziale[j], Nome_Oggetto_Iniziale[OggettoIniziale[j]]);
					}

					printf("\n Giocatore %s scegli il tuo oggetto iniziale -> ", Giocatori[GiocatoreScelto].nome_giocatore);
				}
			}

			while(loop_scelta_oggetto_iniziale);

			printf("\n Oggetto iniziale %s assegnato al giocatore %s", Nome_Oggetto_Iniziale[atoi(SceltaOggettoIniziale)],Giocatori[GiocatoreScelto].nome_giocatore);
			Giocatori[GiocatoreScelto].zaino[OGGETTO_INIZIALE] = atoi(SceltaOggettoIniziale);
			escludi[i] = GiocatoreScelto;
		}
	}


	printf("\n --------------------------------- \n");
	printf("\n| RIEPILOGO IMPOSTAZIONI DI GIOCO |\n");
	printf("\n --------------------------------- \n");{

		int i = 0;
		printf("\n NUMERO GIOCATORI: ............... %-20d\n\n ",*numero_giocatori);
		for (i = 0; i < *numero_giocatori; i++){

			printf("\n NOME GIOCATORE %d: ...........%-20s ",i+1,Giocatori[i].nome_giocatore);
			printf("\n SANITA' MENTALE: ..............%-20d",Giocatori[i].sanita_mentale);
			printf("\n OGGETTO INIZIALE:.............%-20s",Nome_Oggetto_Iniziale[Giocatori[i].zaino[OGGETTO_INIZIALE]]);
			printf("\n \n");
		}
		printf("\n LIVELLO DIFFICOLTA SCELTO:....... %-20s",Nome_Livello_Difficolta[Difficolta]);

	}

	printf("\n\n------------------------------------------\n");
	printf("\n CREAZIONE MAPPA DI GIOCO\n");
	printf("\n--------------------------------------------\n\n");
	creazione_mappa();

}



void gioca (int numero_giocatori, Giocatore *Giocatori)
{


	int  i = 0;
	int  escludizona[4] = {-1,-1,-1,-1};
  char s_scelta[10+1];
  int  scelta = 0;
  int  GiocatoreScelto = 0;
  enum Tipo_zona  ZonaDelFantasma = 0;
  int  loop = 1;
	char s_termina[2+1];

	//Tutti i giocatori partono dall'inizioa della mappa
	for (i = 0; i < numero_giocatori; i++) {

		Giocatori[i].posizione = pTestaListaZona;
	}

	do {

		escludiGiocatore[0] = -1;
        escludiGiocatore[1] = -1;
        escludiGiocatore[2] = -1;
        escludiGiocatore[3] = -1;

		for (i = 0; i < numero_giocatori; i++){

			loop = 1;
		  GiocatoreScelto = Generazione_casuale(0, numero_giocatori-1,escludiGiocatore);
		  ZonaDelFantasma = Generazione_casuale(CUCINA, END_ZONA-1,escludizona);
		  printf("\n Attenzione!!!!.. in questo momento il fantasma si trova nella zona --> %s \n\n",Nome_Tipo_zona[ZonaDelFantasma]);
		  Giocatori[i].posizione	= pTestaListaZona;
		  printf("\n E' il turno del giocatore %s ... scegli quale azione vuoi eseguire: \n\n", Giocatori[GiocatoreScelto].nome_giocatore);

			do{

				printf("\n 1) Torna al caravan \n");
				printf("\n 2) Stampa Giocatore\n");
				printf("\n 3) Stampa zona\n");
				printf("\n 4) Avanza\n");
				printf("\n 5) Raccogli Prova\n");
				printf("\n 6) Raccogli Oggetto\n");
				printf("\n 7) Usa Oggetto\n");
				printf("\n 8) Passa\n");
				printf("\n 9) Termina il gioco\n");
				printf("\n SCEGLI IL COMANDO: ");

				LeggiInput(s_scelta,sizeof(s_scelta));
				scelta = atoi(s_scelta);

				switch(scelta){

					case 1:
					printf("\n  ------------------ \n");
					printf("\n | TORNA AL CARAVAN |\n");
					printf("\n  ------------------ \n\n");
					torna_caravan(GiocatoreScelto, numero_giocatori,Giocatori, ZonaDelFantasma);
					loop = 0;
					break;

					case 2:
					printf("\n  ------------------ \n");
					printf("\n | STAMPA GIOCATORE |\n");
					printf("\n  ------------------ \n\n");
				  stampa_giocatore(GiocatoreScelto, Giocatori);
				  break;

					case 3:
					printf("\n  ------------- \n");
					printf("\n | STAMPA ZONA |\n");
					printf("\n  ------------- \n\n");
				  stampa_zona(GiocatoreScelto, Giocatori);
					break;

					case 4:
					printf("\n  ------------ \n");
					printf("\n |   AVANZA   |\n");
					printf("\n  ------------ \n\n");
					avanza(GiocatoreScelto, Giocatori);
					loop = 0;
					break;

					case 5:
					printf("\n  ---------------- \n");
					printf("\n | RACCOGLI PROVA |\n");
					printf("\n  ---------------- \n\n");
				  raccogli_prova(GiocatoreScelto, numero_giocatori,Giocatori);
					loop = 0;
					break;

					case 6:
					printf("\n  -------------------- \n");
					printf("\n | RACCOGLI L'OGGETTO |\n");
					printf("\n  -------------------- \n\n");
				  raccogli_oggetto(GiocatoreScelto, Giocatori);
					loop = 0;
					break;

					case 7:
					printf("\n  ------------- \n");
					printf("\n | USA OGGETTO |\n");
					printf("\n  ------------- \n\n");
					usa_oggetto(GiocatoreScelto, numero_giocatori,Giocatori);
					loop = 0;
					break;

					case 8:
					printf("\n  ----------- \n");
					printf("\n |   PASSA   |\n");
					printf("\n  ----------- \n\n");
					loop = passa(GiocatoreScelto, Giocatori);
				  break;

					case 9:
					printf("\n Attenzione! Se termini gioco tutta la mappa gioco sara' concellata!!");
					printf("\n Procedo a terminare il gioco S/N?");
					LeggiInput(s_termina, sizeof(s_termina));
					if ((strcmp(s_termina,"S")==0) || (strcmp(s_termina,"s")==0)){

						loop = 0;
						fine_gioco = 0;
						i = numero_giocatori;
						termina_gioco();
					}
					break;

					default:
					printf("\n La scelta %d non e' tra quelle possibili. Scegliere tra 1, 2, 3, 4, 5, 6, 7, 8, 9.\n",scelta);
					break;
				}
			}while(loop);

			escludiGiocatore[i]=GiocatoreScelto;
		}
	} while(fine_gioco);
}



void termina_gioco(void)
{

		dealloca();
}
