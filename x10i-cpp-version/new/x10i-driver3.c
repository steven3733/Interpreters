

//  /usr/bin/gcc -pedantic -Wall -O2 -DX10_LINUX_BUILD -std=gnu99 -I../include -L../lib  ../lib/libfflyusb.so ../lib/libxlinecapi.so -lpthread ../lib/unlockioc.o ../lib/linuxkb.o ../lib/xlineauthenticate.o ../lib/xlinecexit.o -o x10i-driver3.x x10i-driver3.c 



#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../include/linuxkb.h"

#include "../include/xlinecapi.h"
#include "../include/xlineauthenticate.h"
#include "../include/unlockioc.h"
#include "../include/xlinecexit.h"
#include "../include/x15key.h"


#define TRUE 1					/* needed for 'C' */
#define FALSE 0

usbOutput SetOutputs; // define *SetOutputs as an object for the STRUCT "usbOutput"
usbOutput UnSetOutputs; // define *UnSetOutputs as an object for the STRUCT "usbOutput"


int drive[1000000], stop[1000000];
int run_size = 0, run_speed, step_size = 40;
int pulse_count = 0;
int mv, st;

FILE *fp;

void load_data(void *xBoard);


void do_direction(void *xBoard, int dir_pin);
void do_step(void *xBoard);
int decimal_to_binary(int n);

int main(int argc, char* argv[]) {
    /*X10I BOARD*/
    void *xBoard;
    Authenticate x15Authenticate;
    BYTE fittedBoard;

    /* initialise the firefly device */
    xBoard = XlineInitBoard();
    if (xBoard == NULL) {
        return ( XlineExit("initialisation failed.", xBoard, &x15Authenticate, 1));
    }
    /* Determine which board type is fitted, and unlock the IO security accordingly */


    XlineGetFittedBoard(xBoard, &fittedBoard);

    if (fittedBoard == X10I_BOARD)
        UnlockX10c(xBoard);
    else
        return ( XlineExit("unknown board fitted.", xBoard, &x15Authenticate, 1));

    printf("success.\n\n");

#ifdef X10_LINUX_BUILD
    InitialiseConsole();
#endif
    /*
    ===========================================================================================
     */


    load_data(xBoard);
    //do_step(xBoard);
    //do_direction(xBoard, X_DIR_PIN  );

    fclose(fp);
    /*
    ===========================================================================================
     */
    return ( XlineExit("Leaving program.", xBoard, &x15Authenticate, 0));

}

void load_data(void *xBoard) {

    fp = fopen("signals-new.txt", "r");
    run_size = 0;
    // Read signal file and generate memory array drive[] and stop[]
    while (fscanf(fp, "%d %d", &mv, &st) != EOF) {
        drive[run_size] = mv;
        stop[run_size] = st;
        run_size++;
    }
    printf("run_size = %d \n", run_size);

    do_step(xBoard);
    //        printf("pulse count = %d \n", pulse_count);
}

void do_step(void *xBoard) {

    int i, k;
    for (i = 0; i < run_size; i++) {
        for (k = 0; k < step_size; k++) {

            printf("LINE[%d] DRIVE BIT [%d]   ", i, drive[i]);
            decimal_to_binary(drive[i]);
            printf(" STOP BIT [%d] ",stop[i]);
            decimal_to_binary(stop[i]);
            printf("\n");
        
            /*
                        decimal_to_binary(drive[i]);
                        printf(" ");
                        decimal_to_binary(stop[i]) ;   
                        printf("\n");
             */
            SetOutputs.byOut[0] = drive[i];
            UnSetOutputs.byOut[0] = (255 - drive[i]);
            XlineModifyOutputs(xBoard, &UnSetOutputs, &SetOutputs);
            usleep(3);

            SetOutputs.byOut[0] = stop[i];
            UnSetOutputs.byOut[0] = (255 - stop[i]);
            XlineModifyOutputs(xBoard, &UnSetOutputs, &SetOutputs);
            usleep(3);
        }
    }
}

int decimal_to_binary(int n) {
    int k, c;
    for (c = 8; c >= 0; c--) {
        k = n >> c;

        if (k & 1)
            printf("1");
        else
            printf("0");
    }

    // printf("\n");
    return 0;
}