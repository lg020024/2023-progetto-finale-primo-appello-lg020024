/***********************************************************************************
*
*
*Questo file contiene le dichiarazioni delle tre funzioni utilizzate dal gioco
*e le definizioni dei tipi utilizzati nella libreria
*
*
***************************************************************************************/
#define MAX_LENGHT_NAME      80   //macro che definisce la massima lunghezza del nome di un giocatore


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

enum Tipo_zona				{	CARAVAN=1 ,
								CUCINA	  ,
								SOGGIORNO ,
								CAMERA	  ,
								BAGNO	  ,
								GARAGE	  ,
								SEMINTERRATO,
								END_ZONA
							};

enum Tipo_prova 			{	PROVA_EMF=1			,
								PROVA_SPIRIT_BOX	,
								PROVA_VIDEOCAMERA	,
								NESSUNA_PROVA       ,
								END_PROVA
							};

struct Zona_mappa
{
   enum Tipo_zona 			zona;
   enum Tipo_prova 			prova;
   enum Tipo_oggetto_zona   oggetto_zona;
   int  num_elemento;
   struct Zona_mappa      	*next;
};
typedef struct Zona_mappa Zona_mappa;


typedef struct
{
	char 			nome_giocatore[MAX_LENGHT_NAME+1];
	unsigned char 	sanita_mentale;
	unsigned char 	zaino[4];
	Zona_mappa * 	posizione;

} Giocatore;

void imposta_gioco	 (int *numero_giocatori, Giocatore *Giocatori);
void gioca           (int numero_giocatori, Giocatore *Giocatori);
void termina_gioco   (void);
