#include <stdio.h>                                                              // voor printf()
#include <phidget21.h>                                                          // voor phidgetfucties
#include <unistd.h>                                                             // voor usleep()

main (int argc, char* argv[]) {
        int doorLocked, outPutState, err;

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
        if((err = CPhidget_waitForAttachment((CPhidgetHandle)IFK, 0)) != EPHIDGET_OK )
        {
                const char *errStr;
                CPhidget_getErrorDescription(err, &errStr);
                printf("Error waiting for attachment: (%d): %s\n",err,errStr);
                goto exit;
        }
        /*zet output 0 (relay 0) aan, wacht 0.2 seconden en zet hem weer uit.
        *dit maakt een puls voor de electromagneet zodat de pin wordt bewogen.
        *na 5 seconden gebeurt dit weer zodat de deur weer op slot gaat*/
        CPhidgetInterfaceKit_setOutputState(IFK, 1, 1);
        usleep(200000);
        CPhidgetInterfaceKit_setOutputState(IFK, 1, 0);
        usleep(5000000);
        CPhidgetInterfaceKit_setOutputState(IFK, 0, 1);
        usleep(200000);
        CPhidgetInterfaceKit_setOutputState(IFK, 0, 0);

        //einde van het programma, sluit de verbinding en verwijder de handle
        exit:
        CPhidget_close((CPhidgetHandle)IFK);
        CPhidget_delete((CPhidgetHandle)IFK);

        return 0;
}