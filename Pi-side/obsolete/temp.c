//laten compilen met deze command: sudo gcc -o temp $(mysql_config --cflags) temp.c $(mysql_config --libs) -std=gnu99 -lphidget21
//vervolgens temp.sh shell script laten runnen! sudo ./temp.sh

#include <phidget21.h> // voor phidgetfucties
#include <unistd.h>  // voor usleep()
#include <stdio.h>   // voor printf()
#include <string.h>	// voor strcat, strcpy etc
#include <stdlib.h> 	//voor sprintf en overige c bibliotheek functies
#include <mysql/mysql.h>

int main (int argc, char* argv[])
{
	int	sensorValue;  	  // sensorwaarde opslag
	int	temperatuurValue; // temperatuurwaarde opslag
	char	temperatuur[20];  // is een char array, een string
	CPhidgetInterfaceKitHandle ifKit =0;
	CPhidgetInterfaceKit_create(&ifKit);

	CPhidget_open((CPhidgetHandle)ifKit,-1);

	//Wacht maximaal 1000 ms tot dat de phidgetkit is aangesloten.
    	CPhidget_waitForAttachment((CPhidgetHandle)ifKit,1000);


	//Lees sensor nummer 0 in en plaats de waarde in de variabele val.
 	//&val = geheugenadres van de variabele val.
     	CPhidgetInterfaceKit_getSensorValue(ifKit,0,&sensorValue);

	//sensorwaarde omzetten naar temperatuur celsius
	temperatuurValue = (sensorValue*0.2222)- 61.1111; 


	//weergave in het terminalvenster; voor %d wordt de decimale waarde
 	//van de variabele val ingevuld; \n (new line) betekent: overgang naar
	//een nieuwe regel.
	printf("Temperatuur %d \n", temperatuurValue);

    
        //Temperatuur integer omzetten naar  char m.b.v. sprintf()
        int	i; // extra variabel voor duidelijkheid
        i = temperatuurValue;
        sprintf(temperatuur, "%d", i);

	//delay van 800 ms
	usleep(800000);

	CPhidget_close((CPhidgetHandle)ifKit);
	CPhidget_delete((CPhidgetHandle)ifKit);

	//mysql verbinding openzetten
        MYSQL *conn;

        //constants met verbindingsinfo
  const char*	server	 = "get.rekt.info" ;
  const char*	user 	 = "upload";
  const char*	password =  "jemoeder123";
  const char*	database = "Project_FYS";

        //verbinding met de database aanmaken
        conn = mysql_init(NULL);
        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
                fprintf(stderr, "%s\n", mysql_error(conn));
                exit(1);
        }

	//waarden injecten in de table
	//char* zijn char arrays. Er bestaat geen string datatype in C.
  const char* 	insert	= "INSERT INTO Temperature VALUES ('";
  const char* 	insert2 = argv[1];
  const char* 	insert3 = "'," ;
  const char* 	insert4 = temperatuur;
  const char* 	insert5 = ")";

        //nieuwe char array aanmaken voor de uiteindelijke insert command
        char* 	statement;

        //memory allocaten voor de totale string
        statement = malloc(strlen(insert) + 1 + 50 + 1 + 50);

        //strcpy: string copy, strcat: string concatenate
        strcpy(statement, insert);
        strcat(statement, insert2);
        strcat(statement, insert3);
        strcat(statement, insert4);
        strcat(statement, insert5);

        //uitprinten van de gegevens
	printf("%s",argv[1], "\n");
        printf("%s", statement, "\n"); //debugfunctie, print de geformuleerde statement naar de commandline

        //de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
        if (mysql_query(conn, ((char*) statement)) !=0)
	{
                exit(1);
        }

	//mysql verbinding sluiten.
        mysql_close(conn);
        return 0;
}