//---------------------------------------//
//***************************************//
//*************SQL INSERT IN C***********//
//***************TOESOE 2015*************//
//***************************************//
//---------------------------------------//

//Dit programmaatje pakt het eerste commandline argument (c.q. ./sql >jemoer< jetante jeopa whatever)
//en zet het weg in de database in de table Camera onder de Timestamp_Photo entry.
//Totaal unsecured maar het werkt.

//gcc -o sql $(mysql_config --cflags) CameraSql.c $(mysql_config --libs) -std=gnu99

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h> //strcpy, strcat
#include <stdlib.h> //exit staat in stdlib

//argc = argument count, argv = argument value
//heeft te maken met de commandline arguments
int main(int argc, char *argv[])
{
    	MYSQL *conn;
    	
    	//constants met verbindingsinfo
  const char*	server      = "get.rekt.info" ;
  const char*	user        = "upload";
  const char*   password    = "IT_101-FYS";
  const char*	database    = "Project_FYS";
    	
    	//define conn als een mysql_init object voor mysql_real_connect
    	conn = mysql_init(NULL);
    	
    	//als mysql_real_connect niets doet klopt de database connection niet
    	//tijd om de boel te terminaten
    	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    	{
		fprintf(stderr, "%s\n", mysql_error(conn));
    		fprintf(stderr, "i'll be back");
  		exit(1);
    	}
    	
    	//waarden injecten in de table
    	//eerst gaan we de delen van de string in chararrays zetten
  const char*	insert     = "INSERT INTO Camera (id, Timestamp_Photo) VALUES (NULL, '";
  const char*	insert2    = argv[1];
  const char*	insert3    = "')";
    	
    	//nieuwe char array aanmaken waarin we het uiteindelijke commando formuleren
    	char*	statement;
    	
    	//memory allocaten voor de totale string; 
	    //+1 voor de end pointer en +50 voor de length van argv[1]
    	statement = malloc (strlen(insert) + 1 + 50);
    	
    	//strcpy: string copy, strcat: string concatenate
    	//let op: strcpy vervangt de hele inhoud, strcat voegt toe
    	strcpy(statement, insert);
    	strcat(statement, insert2);
    	strcat(statement, insert3);
    	
    	//debugfunctie, print de geformuleerde statement naar de commandline
    	//printf("%s", statement, "\n");
    	
    	//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
    	//deze moet wel explicit gedefinieerd worden als een char array anders schiet gcc uit z'n slof
    	if (mysql_query(conn, ((char*) statement)) !=0)
                exit(1);
    	
    	//zijn we klaar? niet vergeten de sql connection te sluiten
    	mysql_close(conn);
    	return 0;
}