//waar is m'n about? wat doet het programma? met welke arguments moet ik 'm compilen?


#include <stdio.h>                                                              //voor printf()
#include <phidget21.h>                                                          //voor phidgetfuncties
#include <unistd.h>                                                             //voor usleep()

int main (int argc, char* argv[]) 
{
        //variabelen definen
        int doorLocked, outPutState, err;

        //maak var voor een interfacekit handle
        CPhidgetInterfaceKitHandle IFK = 0;

        //zet verbosemode aan voor debugoutput in de console
        CPhidget_enableLogging(PHIDGET_LOG_VERBOSE, NULL);

        //zorg dat de interfacekit handle ook echt gezien wordt als interfacekit
        CPhidgetInterfaceKit_create(&IFK);

        //verbind de interfacekit
        CPhidget_waitForAttachment((CPhidgetHandle)IFK, 10000);

        //open de interface voor verbindingen met phidgets
        CPhidget_open((CPhidgetHandle)IFK, -1);

        //als het verbinden langer duurt dan 5 seconden, stop het programma
        if ( (err = CPhidget_waitForAttachment((CPhidgetHandle) IFK, 0)) != EPHIDGET_OK )
        {
                const char *errStr;
                CPhidget_getErrorDescription(err, &errStr);
                printf("Error waiting for attachment: (%d): %s\n",err,errStr);
                goto exit;
        }
        
        //zet output 1 (relay 1) aan, wacht 0.2 seconden en zet hem weer uit.
        //dit maakt een puls voor de electromagneet zodat de pin wordt bewogen
        CPhidgetInterfaceKit_setOutputState(IFK, 1, 1);
        usleep(200000);
        CPhidgetInterfaceKit_setOutputState(IFK, 1, 0);

        //einde van het programma, sluit de verbinding en verwijder de handle
        exit:
            CPhidget_close((CPhidgetHandle)IFK);
            CPhidget_delete((CPhidgetHandle)IFK);
            return 0;
}