//gcc -o RFID $(mysql_config --cflags) RFID.c $(mysql_config --libs) -std=gnu99 -lphidget21
#include <stdio.h>   
#include <phidget21.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <time.h>

char tagVal1[10];


int CCONV TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, char *TagVal, CPhidgetRFID_Protocol proto) //Tag wordt gelezen en LED gaat aan
{
	CPhidgetRFID_setLEDOn(RFID, 1);
	 

	printf("Tag Read: %s\n", TagVal);
	sprintf(tagVal1, "%s", TagVal);
}

int CCONV TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, char *TagVal, CPhidgetRFID_Protocol proto) //Tag is verwijderd van RFID lezer
{
	CPhidgetRFID_setLEDOn(RFID, 0);

	printf("Tag Lost: %s\n", TagVal);
	return 0;
}



int rfid_simple()
{
	int result;
	const char *err;

	//Declare an RFID handle
	CPhidgetRFIDHandle rfid = 0;

	//RFID object wordt aangemaakt
	CPhidgetRFID_create(&rfid);

	//Wanneer de tag gelezen wordt, wordt het uitgeprint
	CPhidgetRFID_set_OnTag2_Handler(rfid, TagHandler, NULL);

	//Wanneer de tag  verwijderd wordt, wordt het uitgeprint
	CPhidgetRFID_set_OnTagLost2_Handler(rfid, TagLostHandler, NULL);

	//RFID wordt geopend voor connectie
	CPhidget_open((CPhidgetHandle)rfid, -1);

	//Programma wacht totdat RFID is aangesloten
	printf("Waiting for RFID to be attached....\n");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)rfid, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}


	CPhidgetRFID_setAntennaOn(rfid, 1);

	//read RFID event data
	printf("RFID reader is aangesloten! Druk op enter om het programma af te sluiten\n");

	//keep displaying RFID event data until user input is read
	printf("RFID tag nodig\n");
	getchar();


	//RFID wordt afgesloten zodra er op Enter wordt gedrukt
	printf("Ik ga afsluiten vriend...\n");
	CPhidget_close((CPhidgetHandle)rfid);
	CPhidget_delete((CPhidgetHandle)rfid);
	return 0;
}
void finish_with_error(MYSQL *conn)
{
  fprintf(stderr, "%s\n", mysql_error(conn)); //foutmelding voor mmysql connectie
  mysql_close(conn);
  exit(1);
}
int main(int argc, char* argv[])
{
MYSQL *conn; rfid_simple();

	char* jeMoeder;
	jeMoeder = malloc(150);
	char* jeVader = "Open Deur";
	char *employeeID[20];

	//constants met verbindingsinfo
	const char *server = "get.rekt.info" ;
	const char *user = "upload";
	const char *password =  "IT_101-FYS";
	const char *database = "Project_FYS";
	
	//verbinding met de database aanmaken
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	
	
	 if (mysql_query(conn, "SELECT (Employee_id) FROM Employee")){finish_with_error(conn);} // gewoon een select query

  MYSQL_RES *result = mysql_store_result(conn);// opgehaalde data  in results storen

  if (result == NULL){finish_with_error(conn);}

    //We get the result set using the mysql_store_result() function. The MYSQL_RES is a structure for holding a result set.
    //geen zin om dit te vertalen naar NL

  int num_fields = mysql_num_fields(result); // We get the number of fields (columns) in the table.
  const char* rfidTags[num_fields]; // string array declararen

    //We fetch the rows and print them to the screen. en slaat het op in string array

  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result)))
  {
      for(int i = 0; i < num_fields; i++)
      {
          printf("%s ", row[i] ? row[i] : "NULL");
                  rfidTags[i] = row[i];//opslaan in rf[idtag arrays
		if(strcmp(rfidTags[i], tagVal1 ) == 0){
		printf("Tags zijn gelijk, open de deur.");
		

	
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
	const char* insert4 = argv[1];
	const char* insert5 = "')";

	char* statementOne;
	statementOne = malloc(strlen(insert) + 1 + 300);

	strcpy(statementOne, insert);
	strcat(statementOne, jeVader);
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
	
	int lastid = mysql_insert_id(conn);
	printf("%d\n", lastid);
	sprintf(jeMoeder, "%ld", lastid);

	const char* insert6 = "INSERT INTO Access(Employee_id, Notification_id) VALUES('";
	const char* insert7 = "',";
	const char* insert8 = ")";
	
	//nieuwe char array aanmaken voor de uiteindelijke insert command
	char* statementTwo;
	//memory allocaten voor de totale string... ja c is raar en ouderwets
	statementTwo = malloc(strlen(insert6) + 1 + 150);
	//strcpy: string copy, strcat: string concatenate
	strcpy(statementTwo, insert6);
	strcat(statementTwo, tagVal1);
	strcat(statementTwo, insert7);
	strcat(statementTwo, jeMoeder);
	strcat(statementTwo, insert8);
	
	printf("%s", statementTwo, "\n"); //debugfunctie, print de geformuleerde statement naar de commandline
	//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
	if (mysql_query(conn, ((char*) statementTwo)) !=0){
		mysql_error(conn);
	}
	mysql_close(conn);
	
	//printf("%s\n",tagVal1);
	return 0;
		}else{
		printf("Tags zijn onjuist, geen toegang tot de deur");
		}
      }
          printf("\n");

	//for(int a = 0; a < num_fields; a++){
	 //printf("%s", rfidTags[a],"\n");}// testen van array: rfidtags
  }
  mysql_free_result(result);
  

return 0;
}

