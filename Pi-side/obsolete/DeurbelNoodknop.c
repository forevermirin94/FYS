#include <stdio.h>   
#include <phidget21.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <time.h>

int CCONV AttachHandler(CPhidgetHandle IFK, void *userptr)
{
        int serialNo;
        const char *name;

        CPhidget_getDeviceName(IFK, &name);
        CPhidget_getSerialNumber(IFK, &serialNo);

        printf("%s %10d attached!\n", name, serialNo);

        return 0;
}

int CCONV DetachHandler(CPhidgetHandle IFK, void *userptr)
{
        int serialNo;
        const char *name;

        CPhidget_getDeviceName (IFK, &name);
        CPhidget_getSerialNumber(IFK, &serialNo);

        printf("%s %10d detached!\n", name, serialNo);

        return 0;
}


//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int CCONV InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
        printf("Button: %d > State: %d\n", Index, State);

if (Index ==0 && State == 1){
printf("Noodknop is Ingedrukt \n");
MYSQL *conn;

	char* jeMoeder;
	jeMoeder = malloc(150);
	char* jeVader = "er heeft iemand op de noodknop gedrukt";

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
}

if (Index == 1 && State == 1){
printf("De deurbel is ingedrukt iemand staat voor de deur bruh \n");
MYSQL *conn;

	char* jeMoeder;
	jeMoeder = malloc(150);
	char* jeVader = "er heeft iemand op de deurbel gedrukt";

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
	system("/home/pi/cameraScript.sh");
}
        return 0;
}


int interfacekit_simple()
{
        int result, numSensors, i;
        const char *err;

        //Declare an InterfaceKit handle
        CPhidgetInterfaceKitHandle ifKit = 0;

        //create the InterfaceKit object
        CPhidgetInterfaceKit_create(&ifKit);

        //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
        CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, AttachHandler, NULL);
        CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, DetachHandler, NULL);

        //Registers a callback that will run if an input changes.
        //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
        CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, InputChangeHandler, NULL);



        //open the interfacekit for device connections
        CPhidget_open((CPhidgetHandle)ifKit, -1);

        //get the program to wait for an interface kit device to be attached
        printf("Waiting for interface kit to be attached....");
        if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
        {
                CPhidget_getErrorDescription(result, &err);
                printf("Problem waiting for attachment: %s\n", err);
                return 0;
        }



        //get the number of sensors available
        CPhidgetInterfaceKit_getSensorCount(ifKit, &numSensors);

        getchar();

        CPhidgetInterfaceKit_setRatiometric(ifKit, 0);
}
int main(int argc, char* argv[])
{
        interfacekit_simple();
        return 0;
}
