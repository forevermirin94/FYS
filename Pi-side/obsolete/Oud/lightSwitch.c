#include <stdio.h>                                                              // voor printf()
#include <phidget21.h>                                                          // voor phidgetfucties
#include <unistd.h>                                                             // voor usleep()
#include <mysql/mysql.h>                                                        // voor mysql


main (int argc, char* argv[]) {
        int outputState, err;

        //Declare an InterfaceKit handle
        CPhidgetInterfaceKitHandle IFK = 0;

        //enable logging
        //CPhidget_enableLogging(PHIDGET_LOG_VERBOSE, NULL);

        //create the InterfaceKit object
        CPhidgetInterfaceKit_create(&IFK);

        //attach the interface
        CPhidget_waitForAttachment((CPhidgetHandle)IFK, 10000);

        //open the interface for device connections
        CPhidget_open((CPhidgetHandle)IFK, -1);

        //wacht 5 seconden voor attachement
        if((err = CPhidget_waitForAttachment((CPhidgetHandle)IFK, 0)) != EPHIDGET_OK )
        {
                const char *errStr;
                CPhidget_getErrorDescription(err, &errStr);
                printf("Error waiting for attachment: (%d): %s\n",err,errStr);
                goto exit;
        }

        //sla de output state van relay 2 op in outputState
        CPhidgetInterfaceKit_getOutputState(IFK, 2, &outputState);
        //if on turn off, else turn on
        if (outputState) {
                CPhidgetInterfaceKit_setOutputState(IFK, 2, 0);
                printf("licht uit\n");
        } else {
                CPhidgetInterfaceKit_setOutputState(IFK, 2, 1);
                printf("licht aan\n");
        }
        //exit handler
        exit:
        CPhidget_close((CPhidgetHandle)IFK);
        CPhidget_delete((CPhidgetHandle)IFK);

        return 0;
}