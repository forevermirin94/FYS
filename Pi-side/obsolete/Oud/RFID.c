//hier ook... purpose, args, etcetera
//ook: de volgorde van dingen is raar, main het liefst bovenaan je programma, variabelen definen aan het begin van je loops


#include <stdio.h>
#include <phidget21.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

char	tagVal1[10];

int CCONV TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, char *TagVal, CPhidgetRFID_Protocol proto) //Tag wordt gelezen en LED gaat aan
{
	CPhidgetRFID_setLEDOn(RFID, 1); 

	printf("Tag Read: %s\n", TagVal);
	sprintf(tagVal1, "%s", TagVal); //tagVal wegzetten in tagVal1
	return 0;
}

int CCONV TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, char *TagVal, CPhidgetRFID_Protocol proto) //Tag is verwijderd van RFID lezer
{
	CPhidgetRFID_setLEDOn(RFID, 0);

	printf("Tag Lost: %s\n", TagVal);
	return 0;
}


int rfid_simple()
{
	int	result;
  const char*	err;

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
	if( (result = CPhidget_waitForAttachment((CPhidgetHandle)rfid, 10000)) )
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

int main(int argc, char* argv[])
{
	MYSQL *conn; rfid_simple();
	
	char*	jeMoeder;
	jeMoeder = malloc(150);
	
	//constants met verbindingsinfo
  const char*	server		= "get.rekt.info";
  const char*	user		= "upload";
  const char*	password	= "jemoeder123";
  const char*	database 	= "Project_FYS";
	
	//verbinding met de database aanmaken
	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	
	//waarden injecten in de table
	if (mysql_query(conn, "INSERT INTO Notifications(Notification_id, Notification) VALUES(NULL,'Open Deur')" )!=0)
		mysql_error(conn);

	printf("%s", mysql_error(conn));
	int	lastid	= mysql_insert_id(conn);
	
	printf("%d\n", lastid);
	sprintf(jeMoeder, "%ld", lastid); //lastid converteren naar een leesbare string

  const char* 	insert 	= "INSERT INTO Access(Employee_id, Notification_id) VALUES('";
  const char* 	insert2 = "',";
  const char* 	insert3 = ")";
	
	//nieuwe char array aanmaken voor de uiteindelijke insert command
	char* statement;
	
	//memory allocaten voor de totale string
	statement = malloc(strlen(insert) + 1 + 150);
	
	//strcpy: string copy, strcat: string concatenate
	strcpy(statement, insert);
	strcat(statement, tagVal1);
	strcat(statement, insert2);
	strcat(statement, jeMoeder);
	strcat(statement, insert3);
	
	//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
	if (mysql_query(conn, ((char*) statement)) !=0)
	{
		mysql_error(conn);
	}
	
	mysql_close(conn);
	
	return 0;
}

