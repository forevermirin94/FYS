/*
*  combined programmacode voor alle Pi-side functies
*  code (c) 2016 Stefan Schokker
*  SQL injection (c) Thijs vd Boogaard
*  compilen met "gcc -o main $(mysql_config --cflags) main.c $(mysql_config --libs) -std=gnu99 -lphidget21 -lpthread"
*  aub aangepaste code pas invoegen als je zeker weet dat het werkt en compatible is met de rest van de code hier :)
*  voor SQL: als je een notification insert wil doen: mySQL("bericht voor in notificatie");. Als je een ander soort query wilt uitvoeren moet je zelf het hele verhaal typen
*/

#include <phidget21.h>   			// voor phidgetfucties
#include <unistd.h>  				// voor sleep() en overige functies
#include <stdio.h>   				// voor basis i/o functies
#include <stdlib.h>  				// voor basis functies
#include <time.h>					// voor extra tijd en datum functies
#include <mysql/mysql.h> 			// voor mysql
#include <string.h>  				// voor handige string functies
#include <pthread.h> 	 			// voor multithreading support

char tagVal1[10];					// var om de waarde van de RFID tags in op te slaan

// constants met verbindingsinfo, wordt door alle subthreads gebruikt
const char *server    =  "get.rekt.info";
const char *user      =  "upload";
const char *password  =  "IT_101-FYS";
const char *database  =  "Project_FYS";

//©️Thijs, Stefan
//mysql functie die een insert doet in notifications met de gegeven tekst
int sqlNotification (char *messageString)
{
	MYSQL *conn;

	//verbinding met de database aanmaken
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	//datestamp formuleren voor de sql database
	//buffer = de tijdelijke bufferstring voor het opbouwen van de timestamp
	time_t timer;
	char buffer[50];
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);

	//strftime: print de tijd als een string
	strftime(buffer, 30, "%Y-%m-%d_%H:%M:%S", tm_info);
	puts(buffer);

	const char* insert = "INSERT INTO Notifications (Notification_id, Notification, Timestamp_Notification) VALUES (NULL, '";
	const char* insert2 = "','";
	const char* insert3 = "')";

	//statementOne = sql insert statement
	//niet vergeten memory allocaten
	char* statementOne;
	statementOne = malloc(strlen(insert) + 1 + 300);

	//strcpy = stringcopy
	//strcat = string concatenate (samentrekken/samenvoegen)
	strcpy(statementOne, insert);
	strcat(statementOne, messageString);
	strcat(statementOne, insert2);
	strcat(statementOne, buffer);
	strcat(statementOne, insert3);

	if (mysql_query(conn, ((char*) statementOne)) != 0)
	{
		mysql_error(conn);
	}
	printf("query: ", statementOne, "is completed\n");
}

//©️Youssef
//deurbel
void* deurbel (void* IFK)
{
	printf("Thread 2 created successfully: deurbel\n");
	sleep(2);
	printf("Thread deurbel started\n");
	char* messageString = "Deurbel is ingedrukt";
	int input;
	
	while (1)
	{
		//sla de waarde van input 0 & 1 op in variables
		CPhidgetInterfaceKit_getInputState(IFK, 0, &input);
		//als de deurbel is ingedrukt
		if (input)
		{
			printf("Deurbel is ingedrukt\n");
			sqlNotification("Deurbel is ingedrukt");

			//wacht totdat de knop weer is losgelaten, ivm spam in database
			do
			{
				sleep(0.5);
				CPhidgetInterfaceKit_getInputState(IFK, 0, &input);
			} while (input);

		}
	}
}

//©️Stefan
//knopje dat voelt of de deur open of dicht staat
void* doorSensor (void* IFK)
{
	printf("Thread 5 created succesfully: doorSensor\n");
	sleep(2);
	printf("Thread doorSensor started\n");

	int closed = 0;

	while (1)
	{
		//sla de waarde van input 2 op in closed
		CPhidgetInterfaceKit_getInputState(IFK, 2, &closed);
		//als de deur dicht zit, doe em op slot
		if (closed)
		{
			//doe de deur op slot
			CPhidgetInterfaceKit_setOutputState(IFK, 0, 1);
			usleep(200000);
			CPhidgetInterfaceKit_setOutputState(IFK, 0, 0);
			//zolang de knop ingedrukt is, doe niks
			do
			{
				CPhidgetInterfaceKit_getInputState(IFK, 2, &closed);
				sleep(0.5);
			} while (closed);
		}
	}
}

//©️Stefan
//functie die de bewegingssensor in de gaten houdt
void* lightSwitch (void* IFK)
{
	printf("Thread 1 created succesfully: lightSwitch\n");
	sleep(2);
	printf("Thread lichtSwitch started\n");
	int moveValue = 0;
	while (1)
	{
		//sla de waarde van de sensor op in moveValue
		CPhidgetInterfaceKit_getSensorValue(IFK, 3, &moveValue);

		//als de waarde kleiner dan 500 of groter dan 550 is, is er duidelijk beweging.
		if (moveValue <= 500 || moveValue >= 550)
		{
			//licht aan zetten
			CPhidgetInterfaceKit_setOutputState(IFK, 2, 1);
			printf("%d", moveValue);
			printf("licht aan\n");
			sqlNotification("Licht aan");

			sleep(300);

			//zet licht uit
			CPhidgetInterfaceKit_setOutputState(IFK, 2, 0);
			printf("licht uit\n");
			sqlNotification("Licht uit");
		}
		sleep(1);
	}
}

//©️Youssef
void* noodknop (void* IFK)
{
	printf("Thread 3 created succesfully: noodknop\n");
	sleep(2);
	printf("Thread noodknop started\n");
	int input;
	//voor de insert
	char* messageString = "Noodknop is ingedrukt";
	
	//sla de status van input 1 op in input
	CPhidgetInterfaceKit_getInputState(IFK, 1, &input);

	MYSQL *conn;

	while (1)
	{
		//sla elke keer dat de loop loopt de waarde opnieuw op
		CPhidgetInterfaceKit_getInputState(IFK, 1, &input);
		//als de noodknop is ingedrukt
		if (input)
		{
			printf("Noodknop is ingedrukt\n");
			sqlNotification("Noodknop is ingedrukt");

			//verbinding met de database aanmaken
			conn = mysql_init(NULL);
			if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

			//waarden injecten in de table
			//char* zijn char arrays. Er bestaat geen string datatype in C.
			const char* insert = "UPDATE Setting SET noodsituatie = 1";

			//nieuwe char array aanmaken voor de uiteindelijke insert command
			char* statementOne;

			statementOne = malloc(strlen(insert) + 1 + 300);
			strcpy(statementOne, insert);

			//printf("%s", statementOne, "\n"); //debugfunctie, print de geformuleerde statement naar de commandline

			//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
			if (mysql_query(conn, ((char*) statementOne)) !=0)
			{
				exit(1);
			}

			//mysql verbinding sluiten.
			mysql_close(conn);

			//ga pas dooor wanneer de knop is losgelaten
			do 
			{
				sleep(1);
				CPhidgetInterfaceKit_getInputState(IFK, 1, &input);
			} while (input);

		}
	}
}

//afsluit functie voor als het misgaat met de sql verbinding
void finish_with_error(MYSQL *conn)
{
	printf("%s\n", mysql_error(conn)); //foutmelding voor mmysql connectie
	mysql_close(conn);
	exit(1);
}

//callback handler voor wanneer een tag wordt gedecteerd
//deze functies zijn wat onduidelijk maar maken leven een stuk makkelijker
//ze besparen ook resources omdat ze event-driven zijn

int CCONV TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, char *TagVal, CPhidgetRFID_Protocol proto)
{
	//zet led aan
	CPhidgetRFID_setLEDOn(RFID, 1);
	printf("Tag Read: %s\n", TagVal);

	//hier wordt de waarde van de tag in tagVal1 opgeslagen
	sprintf(tagVal1, "%s", TagVal);
}

//callback voor als de tag weer uit het zicht verdwijnt

int CCONV TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, char *TagVal, CPhidgetRFID_Protocol proto)
{
	//zet led uit
	CPhidgetRFID_setLEDOn(RFID, 0);
	printf("Tag Lost: %s\n", TagVal);

	sleep(1);

	//ze de waarde van tagVal1 op iets wat niet klopt, zodat de deur niet open blijft als de tag uit de buurt is
	sprintf(tagVal1, "%s", "0000000000");
}

//©️Jonathan, Stefan, youssef, hung.
void* rfidCheck (void* IFK)
{
	printf("Thread 4 created succesfully: rfid\n");
	sleep(2);
	printf("Thread rfid started\n");

	//Declare an RFID handle
	CPhidgetRFIDHandle rfid = 0;

	//RFID object wordt aangemaakt
	CPhidgetRFID_create(&rfid);

	//RFID wordt geopend voor connectie
	CPhidget_open((CPhidgetHandle)rfid, -1);

	//Programma wacht totdat RFID is aangesloten
	printf("Waiting for RFID to be attached....\n");

	//wacht voor verbinding met met de RFID kit
	if (CPhidget_waitForAttachment((CPhidgetHandle)rfid, 10000))
	{
		printf("Problem waiting for attachment: %s\n");
		return 0;
	}

	//stel de TagHandler in als callback voor wanneer er een tag wordt gezien
	//dit maakt dus dat tagVal1 de waarde van de tag krijgt als er een tag in de buurt van de scanner is
	CPhidgetRFID_set_OnTag2_Handler(rfid, TagHandler, NULL);

	//stel de TagLostHandler in als callback voor wanneer de tag verdwijnt
	//deze zet de waarde van tagVal1 weer terug wanneer de tag weer weg is
	CPhidgetRFID_set_OnTagLost2_Handler(rfid, TagLostHandler, NULL);

	printf("RFID attached!\n");

	//zet antenne aan
	CPhidgetRFID_setAntennaOn(rfid, 1);

	//conn wordt gedeclareerd als een sql verbinding
	MYSQL *conn;

	//verbinding met de database aanmaken
	conn = mysql_init(NULL);

	//als de verbinding mislukt moet het programma stoppen
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		printf("SQL connection failed\n");
		finish_with_error(conn);
	}

	//haal de authenticatie gegevens uit de database
	if (mysql_query(conn, "SELECT Employee_id FROM Employee"))
	{
		finish_with_error(conn);
	}

	//sla het resultaat van de query op in result
	MYSQL_RES *result = mysql_store_result(conn);

	//als er niks in de result set zit: sluit het programma af
	if (result == NULL)
	{
		finish_with_error(conn);
	}

	//het aantal velden uit de result set opslaan in num_rows
	const int num_rows = mysql_num_rows(result);

	//print info naar de console (debug dingetje)
	printf("aantal tags:\n");
	printf("%d\n", num_rows, "\n");

	//array van char arrays om de tags in op te slaan
	const char* rfidTags[num_rows];

	printf("tag data:\n");

	//declareer row als een var waar we sql data in kunnen opslaan
	MYSQL_ROW row;

	int i = 0;
	//mysql_fetch_row() haalt de eerstvolgende entry uit de result set en gooit hem weg
	while ((row = mysql_fetch_row(result)))
	{
		//zet de data van row over in rfidTags zodat we het kunnen hergebruiken
		rfidTags[i] = row[0];
		printf(rfidTags[i]);
		printf("\n");
		i++;
	}

	//verwijder de resultaten set omdat we toch alles hebben overgezet in de rfidTags array


	char* idString;
	idString = malloc(150);

    //vul tagVal1 met een incorrecte waarde voor de zekerheid
	sprintf(tagVal1, "%s", "0000000000");

	//de check loop
	while (1)
	{
		for (int i = 0; i < num_rows; i ++)
		{
			//vergelijk de tag uit de database met de tag bij de scanner
			if (strcmp(rfidTags[i], tagVal1) == 0)
			{
				printf("Tag is legit \n");

				//unlocked de deur door de relais eerst in te schakelen en daarna weer uit
				//dit maakt een puls die de electromagneet van het slot activeert

				CPhidgetInterfaceKit_setOutputState(IFK, 1, 1);
				usleep(200000);
				CPhidgetInterfaceKit_setOutputState(IFK, 1, 0);

				//notification insert
				sqlNotification("Deur geopend");

				int lastid = mysql_insert_id(conn);
				printf("%d\n", lastid);
				sprintf(idString, "%d", lastid);

				const char* insert = "INSERT INTO Access(Employee_id, Notification_id) VALUES('";
				const char* insert1 = "',";
				const char* insert2 = "(SELECT Notification_id FROM Notifications WHERE Notification='Deur geopend' ORDER BY Notification_id DESC LIMIT 1))";

				//nieuwe char array aanmaken voor de uiteindelijke insert command
				char* statement;
				// memory allocaten voor de totale string... ja c is raar en ouderwets
				statement = malloc(strlen(insert) + 1 + 150);


				//rfidTags naar var overzetten voor strcat
				//strcpy: string copy, strcat: string concatenate
				strcpy(statement, insert);
				strcat(statement, rfidTags[i]);
				strcat(statement, insert1);
				strcat(statement, insert2);

				printf("Statement RFID:\n");
				//debugfunctie, print de geformuleerde statement naar de commandline
				printf("%s", statement, "\n");

				//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
				if (mysql_query(conn, ((char*) statement)) !=0)
				{
					mysql_error(conn);
				}

				//ze de waarde van tagVal1 op iets wat niet klopt, zodat de deur niet open blijft als de tag uit de buurt is
				sprintf(tagVal1, "%s", "0000000000");
				sleep(1);
			}
		}
		sleep(0.1);
	}

	//einde van thread
	exit:
	mysql_free_result(result);
	CPhidget_close((CPhidgetHandle)rfid);
	CPhidget_delete((CPhidgetHandle)rfid);
}

//©️Stefan
void* doorCheck (void* IFK)
{
	printf("Thread 6 created succesfully: doorCheck\n");
	sleep(2);
	printf("Thread doorCheck started\n");

	//mysql verbinding openzetten
	MYSQL *conn;
	//verbinding met de database aanmaken
	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	//var om de result set in op te slaan
	MYSQL_RES *result;
	//var de data uit de result set in op te slaan
	MYSQL_ROW row;

	int status;

	for (;;)
	{
		if (mysql_query(conn, "SELECT Door FROM Setting") != 0)
		{
			mysql_error(conn);
		}

		result = mysql_store_result(conn);

		if (result == NULL)
		{
			finish_with_error(conn);
		}

		while (row = mysql_fetch_row(result))
		{
			status = *row[0];
		}
        
        // ©️Youssef, Hung
        // het karakter "0" heeft nummer 48 in de ASCII table en het karakter "1" heeft nummer 49 in de ASCII table
		// 49 is dan open en 48 is dicht
		if (status == 49)
		{
			printf("Open deur\n");
			//Open de deur
			CPhidgetInterfaceKit_setOutputState(IFK, 1, 1);
			usleep(200000);
			CPhidgetInterfaceKit_setOutputState(IFK, 1, 0);

			sqlNotification("Deur via webUI geopend");
			//waarden injecten in de table
			//char* zijn char arrays. Er bestaat geen string datatype in C.
			const char* insert = "UPDATE Setting SET Door = 0";

			//nieuwe char array aanmaken voor de uiteindelijke insert command
			char* statementOne;

			statementOne = malloc(strlen(insert) + 1 + 300);
			strcpy(statementOne, insert);

			//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
			if (mysql_query(conn, ((char*) statementOne)) !=0)
			{
				exit(1);
			}

            //wacht voor de zekerheid totdat de database geupdated is
			do {
				if (mysql_query(conn, "SELECT Door FROM Setting") != 0)
				{
					mysql_error(conn);
				}

				result = mysql_store_result(conn);

				if (result == NULL)
				{
					finish_with_error(conn);
				}

				while (row = mysql_fetch_row(result))
				{
					status = *row[0];
				}
			} while (status == 49);
		}
		//resultaat weggooien zodat we de query opnieuw uit kunnen voeren
		mysql_free_result(result);
		sleep(1);
	}
}

//©️Hung
//functie die temperatuur meet en insert in temperatuur table. Heeft eigen sql vanwege alternatieve insert
void* tempSensor (void* IFK)
{
	printf("Thread 0 created succesfully: Temperature\n");
	sleep(2);
	printf("Thread tempSensor started\n");
	int sensorValue;        // sensorwaarde opslag
	int temperatuurValue;   // temperatuurwaarde opslag
	char temperatuur[20];   // is een char array, een string
	while (1)
	{
		//Lees sensor nummer 0 in en plaats de waarde in de variabele val.
		//&val = geheugenadres van de variabele val.
		CPhidgetInterfaceKit_getSensorValue(IFK,2,&sensorValue);

		//sensorwaarde omzetten naar temperatuur celsius
		temperatuurValue = (sensorValue*0.2222)- 61.1111;

		//weergave in het terminalvenster; voor %d wordt de decimale waarde
		//van de variabele val ingevuld; \n (new line) betekent: overgang naar
		//een nieuwe regel.
		printf("Temperatuur: %d", temperatuurValue, "\n");

		//Temperatuur integer omzetten naar  char m.b.v. sprintf()
		int i; // extra variabel voor duidelijkheid
		i = temperatuurValue;
		sprintf(temperatuur, "%d", i);

		//delay van 800 ms
		usleep(800000);

		//mysql verbinding openzetten
		MYSQL *conn;

		//verbinding met de database aanmaken
		conn = mysql_init(NULL);
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		//datestamp formuleren voor de sql database
		//buffer is de tijdelijke bufferstring voor het opbouwen van de timestamp
		time_t timer;
		char buffer[50];
		struct tm* tm_info;

		time(&timer);
		tm_info = localtime(&timer);

		//strftime: print de tijd als een string
		strftime(buffer, 30, "%Y-%m-%d_%H:%M:%S", tm_info);
		puts(buffer);

		//waarden injecten in de table
		//char* zijn char arrays. Er bestaat geen string datatype in C.
		const char* insert  = "INSERT INTO Temperature VALUES ('";
		const char* insert2 = "'," ;
		const char* insert3 = ")";

		//nieuwe char array aanmaken voor de uiteindelijke insert command
		char* statement;

		//memory allocaten voor de totale string
		statement = malloc(strlen(insert) + 1 + 50 + 1 + 50);

		//strcpy: string copy, strcat: string concatenate
		strcpy(statement, insert);
		strcat(statement, buffer);
		strcat(statement, insert2);
		strcat(statement, temperatuur);
		strcat(statement, insert3);

		//uitprinten van de gegevens
		printf("%s",buffer, "\n");
		//printf("%s", statement, "\n"); //debugfunctie, print de geformuleerde statement naar de commandline

		//de query: conn is de verbinding met server, statement is de geprepareerde char array met de insert statement
		if (mysql_query(conn, ((char*) statement)) !=0)
		{
			exit(1);
		}
		//mysql verbinding sluiten.
		mysql_close(conn);
		//30 seconden wachten voordat er een nieuwe meting wordt gedaan
		sleep(300);
	}
}

//©️Stefan
//main: koppelt de interfacekit en opent alle functies in een eigen thread
//todo: koppel RFID aan en zet authenticatie aan
//vereist: 1 extra thread definen en openen
int main ()
{
	//waarde om op te slaan of phigdet goed is verbonden
	int err;

	//maak thread0 t/m thread 3 vars voor de pthread_create functie
	pthread_t thread0, thread1, thread2, thread3, thread4, thread5, thread6;

	//zet logging aan voor in de console
	CPhidget_enableLogging(PHIDGET_LOG_VERBOSE, NULL);

	//maak var voor een interfacekit handle
	CPhidgetInterfaceKitHandle IFK = 0;

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

	/*
	*  de functie pthread_create(x1, x2, x3, x4); wordt gebruikt voor het maken van nieuwe threads en werkt als volgt:
	*  x1 is een verwijzing naar een thread variable (wordt gemaakt met pthread_t x1;, R 640)
	*  x2 is voor attributes, die zijn in ons geval niet nodig;
	*  x3 is de naam van de functie die je in je nieuwe thread wilt uitvoeren
	*  x4 is de plek voor eventuele variables die je naar je functie wil sturen (in ons geval de link naar de InterfaceKitHandle)
	*  als het maken van een nieuwe thread misgaat, returned pthread_create een error code: -1 of 1 wanneer dit gebeurt, stop het programma
	*/
	if (pthread_create(&thread0, NULL, tempSensor, IFK) != 0)
	{
		goto exit;
	}
	if (pthread_create(&thread1, NULL, lightSwitch, IFK) != 0)
	{
		goto exit;
	}
	if (pthread_create(&thread2, NULL, deurbel, IFK) != 0)
	{
		goto exit;
	}
	if (pthread_create(&thread3, NULL, noodknop, IFK) != 0)
	{
		goto exit;
	}
	if (pthread_create(&thread4, NULL, rfidCheck, IFK) != 0)
	{
		goto exit;
	}
	if (pthread_create(&thread5, NULL, doorSensor, IFK) != 0)
	{
		goto exit;
	}
	if (pthread_create(&thread6, NULL, doorCheck, IFK) != 0)
	{
		goto exit;
	}

	//start het camera script
	//met system() call je een executable buiten het huidige programma
	system("/home/pi/cameraScript.sh");

	//stelt afsluiting van het programma uit totdat de opgegeven thread klaar is
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);
	pthread_join(thread6, NULL);

	//einde van het programma, sluit de verbinding en verwijder de handle
	exit:
	CPhidget_close((CPhidgetHandle)IFK);
	CPhidget_delete((CPhidgetHandle)IFK);
	pthread_exit(NULL);
	return 0;
}