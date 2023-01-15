//menù principale del gioco con solo la dichiarazione delle funzioni principali

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"gamelib.h"

/*Funziona di utilità per creare un delay di n secondi*/
void delay(int n)
{

	int milsec = 1000 * n;

	clock_t startTime = clock();

	while(clock() < (startTime + milsec));

}

//Punto di ingresso del programma
int main(int argc, char const *argv[])
{

	system("clear");

	//Variabile utilizzata per uscire dalla schermata di menù
	int loop = 1;

	//Variabile che indica se l'impostazione gioco è conclusa e si può procedere al gioco
	int f_impostato_gioco = 0;

	//Variabile utilizzata per leggere la scelta di menu fatta dall'utente
	char s_scelta[10+1];

	//Variabile con la trasformazione in numero della scelta di menu dell'utente
	int  scelta = 0;

	//Array di struttura per memorizzare informazioni dei giocatori
	Giocatore Giocatori[4];

	//variabile che indica il numero effettivo di giocatori scelto in fase di impostazione gioco
	int numero_giocatori = 0;


	printf(" ------------------------------------------------------------------------------------------------------------------------------------------------------------- \n");
	printf("| * * * * * *   *         *   * * * * * *   *             * * * * * *   * * * * * *   * * * * * *   *         *   * * * * * *   * * * * * *   *   * * * * * * |\n");
	printf("| *         *   *         *   *         *   *             *             *         *   *         *   *         *   *         *   *         *   *   *         * |\n");
	printf("| *         *   *         *   *         *   *             *             *         *   *         *   *         *   *         *   *         *   *   *         * |\n");
	printf("| * * * * * *   * * * * * *   * * * * * *   *             * * * * * *   *         *   * * * * * *   * * * * * *   *         *   * * * * *     *   * * * * * * |\n");
	printf("| *             *         *   *         *   *                       *   *         *   *             *         *   *         *   *         *   *   *         * |\n");
	printf("| *             *         *   *         *   *                       *   *         *   *             *         *   *         *   *         *   *   *         * |\n");
	printf("| *             *         *   *         *   * * * * * *   * * * * * *   * * * * * *   *             *         *   * * * * * *   * * * * * *   *   *         * |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------------------------------------- \n");


	printf("\n Un team di investigatori del paranormale viene contrattualizzato per raccogliere prove");
	printf("\n sulle presenze che si aggirano in luoghi infestati. \n");
	printf("\n Riusciranno a capire di che fantasma si tratta prima di impazzire? \n");

	printf("\n Sei pronto a giocare?       \n");

	do{
		printf(" ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- \n");
		printf("| *         *   * * * * * *   **        *   *         * '      * * * * *     * * * * * *   *                  * * * * * *   *   * * * * * *   * * * * * *   * * * * * * |\n");
		printf("| * *     * *   *             * *       *   *         *        *         *   *             *                  *             *   *         *   *             *         * |\n");
		printf("| *   * *   *   *             *   *     *   *         *        *         *   *             *                  *             *   *         *   *             *         * |\n");
		printf("| *    *    *   * * * * * *   *     *   *   *         *        *         *   * * * * * *   *                  *   * * * *   *   *         *   *             *         * |\n");
		printf("| *         *   *             *       * *   *         *        *         *   *             *                  *         *   *   *         *   *             *         * |\n");
		printf("| *         *   *             *        **   *         *        *         *   *             *                  *         *   *   *         *   *             *         * |\n");
		printf("| *         *   * * * * * *   *         *   * * * * * *        * * * * *     * * * * * *   * * * * * *        * * * * * *   *   * * * * * *   * * * * * *   * * * * * * |\n");
		printf(" ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- \n");

		printf("\n1) Imposta il gioco\n");
		printf("\n2) Gioca\n");
		printf("\n3) Termina gioco\n");
		printf("\n\n Scegliere un comando (1,2,3): \n");

		if (fgets(s_scelta, sizeof(s_scelta), stdin)==NULL)
		{
			printf("ERRORE LETTURA TIPO DIFFICOLTA");
		}
		else
		{
			s_scelta[strlen(s_scelta)-1]= '\0';
		}
		scelta = atoi(s_scelta);


		switch(scelta){

			case 1:
				printf("\n  ----------------------- \n");
				printf("\n | IMPOSTAZIONI DI GIOCO |\n");
				printf("\n  ----------------------- \n\n");
				imposta_gioco(&numero_giocatori, Giocatori);
				f_impostato_gioco = 1;
				system("clear");
			break;

			case 2:
				if (f_impostato_gioco == 1)
				{
					printf("\n ---------------------- \n");
					printf("\n|  INIZIA A GIOCARE !! |\n");
					printf("\n ---------------------- \n");
					gioca(numero_giocatori, Giocatori);
				}
				else{
					printf("\n  --------------------------------------------------------------- \n");
					printf("\n | Prima di iniziare devi impostare il gioco (Opzione 1 del menù)|\n ");
					printf("\n  --------------------------------------------------------------- \n");
				}
			break;

			case 3:
				printf("\n   --------------------------------------- \n");
				printf("\n  |   GRAZIE PER AVER GIOCATO CON NOI!!   |\n");
				printf("\n  |       TORNA PRESTO A GIOCARE!         |\n");
				printf("\n   --------------------------------------- \n");
				termina_gioco();
				delay(3);
				loop = 0;
			break;

			default:
				printf("\n La scelta %d non e' tra quelle possibili (1,2,3)\n",scelta);
			break;
		}


	} while (loop);

	return 0;

}
