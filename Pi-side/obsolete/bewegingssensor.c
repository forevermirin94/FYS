//compilen met "gcc movesensor.c -o movesensor $(mysql_config --cflags) $(mysql_config --libs) -std=gnu99 -lphidget21"
#include <phidget21.h>                                                  // voor phidgetfucties
#include <unistd.h>                                                     // voor usleep()
#include <stdio.h>                                                      // voor printf()
#include <stdlib.h> 	                                                // voor printf en overige c bibliotheek functies
#include <time.h>                                                       // voor extra tijd functies
#include <mysql/mysql.h>                                                // voor mysql
#include <string.h>                                                     // voor handige string functies

void mySQL () 
{
    MYSQL *conn;

    char* jeMoeder;
    jeMoeder = malloc(150);
    char* sqlBericht = "Licht aan";

    //constants met verbindingsinfo
    const char *server = "get.rekt.info" ;
    const char *user = "upload";
    const char *password =  "IT_101-FYS";
    const char *database = "Project_FYS";
    
    //verbinding met de database aanmaken
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    //datestamp formuleren voor de sql database
    //bufbaf is de tijdelijke bufferstring voor het opbouwen van de timestamp
    time_t timer;
    char bufbaf[50];
    struct tm* tm_info;
    
    time(&timer);
    tm_info = localtime(&timer);
    
    //strftime: print de tijd als een string
    strftime(bufbaf, 30, "%Y-%m-%d_%H:%M:%S", tm_info);
    puts(bufbaf);
    
    const char* insert = "INSERT INTO Notifications (Notification_id, Notification, Timestamp_Notification) VALUES (NULL, '";
    const char* insert2 = "'";
    const char* insert3 = ",'";
    const char* insert5 = "')";

    char* statementOne;
    statementOne = malloc(strlen(insert) + 1 + 300);

    strcpy(statementOne, insert);
    strcat(statementOne, sqlBericht);
    strcat(statementOne, insert2);
    strcat(statementOne, insert3);
    strcat(statementOne, bufbaf);
    strcat(statementOne, insert5);
    
    printf("%s", statementOne, "\n");
    {mysql_error(conn);}
    printf("%s", mysql_error(conn));
    if (mysql_query(conn, ((char*) statementOne)) !=0){
        mysql_error(conn);
    }
}
void main (int argc, char* argv[]) 
{
	//waarde uit bewegingssensor
	int moveValue, bigValue, smallValue, err, outputState;
	//maak var voor een interfacekit handle
    CPhidgetInterfaceKitHandle IFK = 0;

    //zet logging aan voor in de console
    CPhidget_enableLogging(PHIDGET_LOG_VERBOSE, NULL);

    //maak dat de interfacekit handle ook echt gezien wordt als interfacekit
    CPhidgetInterfaceKit_create(&IFK);

    //verbind de interfacekit
    CPhidget_waitForAttachment((CPhidgetHandle)IFK, 10000);

    //open de interface voor apparaat verbindingen
    CPhidget_open((CPhidgetHandle)IFK, -1);

    //als het verbinden langer duurt dan 5 seconden, stop het programma
    if ((err = CPhidget_waitForAttachment((CPhidgetHandle)IFK, 0)) != EPHIDGET_OK )
    {
        const char *errStr;
        CPhidget_getErrorDescription(err, &errStr);
        printf("Error waiting for attachment: (%d): %s\n",err,errStr);
        goto exit;
    }
    //loop die kijkt of er beweging is binnen het bereik van de sensor
    while (1) 
    {
        //sla de waarde van de sensor op in moveValue
        CPhidgetInterfaceKit_getSensorValue(IFK, 3, &moveValue);
        printf("%d\n", moveValue);
        if (moveValue <= 400 || moveValue >= 600) 
        {
            //licht aan, wacht 10 seconden, licht uit
            CPhidgetInterfaceKit_setOutputState(IFK, 2, 1);
            printf("licht aan\n");
            mySQL();
            printf("SQL insert klaar");
            usleep(100000000);
            CPhidgetInterfaceKit_setOutputState(IFK, 2, 0);
            printf("licht uit\n");
        } else {
            printf("no new move, value is: ");
        }
        usleep(100000);
    }
	//einde van het programma, sluit de verbinding en verwijder de handle
    exit:
    CPhidget_close((CPhidgetHandle)IFK);
    CPhidget_delete((CPhidgetHandle)IFK);
}
