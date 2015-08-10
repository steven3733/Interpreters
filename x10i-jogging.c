
// ==============================================
// Description:
//
// This program drives the CNC servo motors to
// manually set the CNC spindle starting location.
//
// This program allows the user to select the
// direction to move the CNC spindle on-the-run
// in fixed steps of 500 pulses. The directions
// cover y-up, y-down, x-right, x-left, z-up
// and z-down. We use 500 pulses for z-up and z-down.

// This adjustment for the starting location
// in CNC jargon is called jogging.
//
// This program runs continuously, until the
// key 'q' on the keyboard is pressed.
/*
// ==============================================
// COMPILATION AND EXECUTION INSTRUCTIONS
// /usr/bin/gcc -pedantic -Wall -O2 -DX10_LINUX_BUILD -std=gnu99 -I../include -L../lib -o x10i-jogging.x x10i-jogging.c ../lib/libfflyusb.so ../lib/libxlinecapi.so -lpthread ../lib/unlockioc.o ../lib/linuxkb.o ../lib/xlineauthenticate.o ../lib/xlinecexit.o


*/

// HEADERS FOR KEYBOARD_HIT
#include <termios.h>
#include <term.h>
#include <curses.h>

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

//#include "pins.h"

#define TRUE 1					/* needed for 'C' */
#define FALSE 0


static struct termios initial_settings, new_settings;
static int peek_character = -1;

// VARIABLE DECLARATIONS
int count;
usbOutput outputs;

// PROTOTYPE FUNCTION DEFINITIONS
void init_keyboard();
void close_keyboard();
int keyboard_hit();
int read_charkey();

// FUNCTION PROTOTYPES DECLARATION

void cmd_interpreter(void *xBoard,int);
void run_menu(void);

// DRIVE CNC ALONG Y-AXIS
void drive_up(void *xBoard, int valdrive); // DRIVE (12,8 CW)
void drive_down(void *xBoard, int valdrive); // DRIVE (4,0, CCW)

// DRIVE CNC ALONG X-AXIS
void drive_right(void *xBoard, int valdrive); // DRIVE (3,2 CW)
void drive_left(void *xBoard, int valdrive); // DRIVE (1,0 CCW)

// DRIVE CNC ALONG Z-AXIS
void pen_up(void *xBoard, int valdrive); // DRIVE (16,0  CW)
void pen_down(void *xBoard, int valdrive); // DRIVE (48,32 CCW)

time_t t;

// ================================================

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
    printf("Bismillah start X10i jogging. \n\n");



    int charkey = 0;
    run_menu();
    init_keyboard();


    for (;;) {
        if (keyboard_hit()) {
            charkey = read_charkey();
            printf("You hit keyboard key: char = %c or int = %d \n", charkey, charkey);
            cmd_interpreter(xBoard,charkey);
        } //END IF
    } // END FOR



    close_keyboard();
    printf("Alhamdulillah. finish Raspberry Pi jogging. \n\n");

    return ( XlineExit("Leaving program.", xBoard, &x15Authenticate, 0));

}
// ================================================

/*
   void reset_CNC(void *xBoard) {
   usbOutput outputs;

   outputs.byOut[1] = 0x00;

   if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
   printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
   }
   if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
   printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
   }
   }
   */


// ================================================

void cmd_interpreter( void *xBoard,int pressed_key) {
    // ================================================
    //printf("I am inside  cmd_interpreter() before switch. \n");
    switch (pressed_key) {

        case 117:
            // pressed_key char = u or int = 117
            // DRIVE CNC ALONG Y-AXIS
            printf("u \tdrive_up    (500) y-axis (12,8   CW)\t==> Y-axis PINS = (0)(0) (1/0)(1) (0)(0) running ... ");
            drive_up(xBoard, 500); // DRIVE (12,8 CW)
            printf("done.\n");
            break;

        case 100:
            // pressed_key char = d or int = 100
            // DRIVE CNC ALONG Y-AXIS
            printf("d \tdrive_down  (500) y-axis (4,0   CCW)\t==> Y-axis PINS = (0)(0) (1)(0) (0)(0)   running ... ");
            drive_down(xBoard, 500); // DRIVE (4,0 CCW)
            printf("done.\n");
            break;

        case 114:
            // pressed_key char = r or int = 114
            // DRIVE CNC ALONG X-AXIS
            printf("r \tdrive_right (500) x-axis (3,2    CW)\t==> X-axis PINS = (1/0)(1) (0)(0) (0)(0) running ... ");
            drive_right(xBoard, 500); // DRIVE (3,2 CW)
            printf("done.\n");
            break;

        case 108:
            // pressed_key char = l or int = 108
            // DRIVE CNC ALONG X-AXIS
            printf("l \tdrive_left  (500) x-axis (1,0   CCW)\t==> X-axis PINS = (1)(0) (0)(0) (0)(0)   running ... ");
            drive_left(xBoard, 500); // DRIVE (1,0 CCW)
            printf("done.\n");
            break;

        case 122:
            // pressed_key char = z or int = 122
            // DRIVE CNC ALONG Z-AXIS
            printf("z \tdrivepen_up  (500) z-axis (16,0   CW)\t==> Z-axis PINS = (0)(0) (0)(0) (1)(0)   running ... ");
            pen_up(xBoard, 500); // DRIVE (16,0 CW)
            printf("done.\n");
            break;

        case 115:
            // pressed_key char = s or int = 115
            // DRIVE CNC ALONG Z-AXIS
            printf("s \tdrivepen_down(500) z-axis (48,32 CCW)\t==> Z-axis PINS = (0)(0) (0)(0) (1/0)(1) running ... ");
            pen_down(xBoard, 500); // DRIVE (48,32 CCW)
            printf("done.\n");
            break;

            //===========
        case 113:
            // pressed_key char = q or int = 113
            // QUIT AND EXIT PROGRAM
            printf("q \tQuit and exit. \t\t==> Alhamdulillah. Done. \n\n");
            // reset_CNC();
            // RELEASE IO_PORTADDRESS WITH VALUE 0
            printf("Alhamdulillah. Parallel port successfully released. \n\n");
            close_keyboard();
            exit(0);

        default:
            printf("%c \tERROR: Invalid command: ==> char %c or int %d \n", pressed_key, pressed_key, pressed_key);
    }
    //printf("I am exiting cmd_interpreter() after switch. \n");
}
// ================================================

void run_menu(void) {
    // ------------------------------------------------

    printf("\n\tMENU OF COMMANDS (ACTIONS). \n");
    printf("\t===========================\n");

    printf("\tr Drive right the x-axis (3,2    CW) PINS = (1/0)(1) (0)(0) (0)(0)\n");
    printf("\tl Drive left  the x-axis (1,0   CCW) PINS =   (1)(0) (0)(0) (0)(0)\n");

    printf("\tu Drive up    the y-axis (12,8   CW) PINS = (0)(0) (1/0)(1) (0)(0)\n");
    printf("\td Drive down  the y-axis (4,0   CCW) PINS = (0)(0)   (1)(0) (0)(0)\n");

    printf("\ts Pen   down  the z-axis (48,32 CCW) PINS = (0)(0) (0)(0) (1/0)(0)\n");
    printf("\tz Pen   up    the z-axis (16,0   CW) PINS = (0)(0) (0)(0)   (1)(0)\n");
    printf("\tg random selection of pins  and axis \n");

    printf("\tq Quit and exit this program.\n\n");

    printf("Enter your command: (please wait after pressing a key). \n\n");
}
// ==============================================

void init_keyboard() {
    // ==============================================
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
// ===============================================

void close_keyboard() {
    // ===============================================
    tcsetattr(0, TCSANOW, &initial_settings);
}
// ==============================================

int keyboard_hit() {
    // ==============================================
    char ch;
    int nread;

    if (peek_character != -1) {
        return 1;
    }

    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    if (nread == 1) {
        peek_character = ch;
        return 1;
    }
    return (0);
}
// ==============================================

int read_charkey() {
    // ==============================================
    char ch;
    if (peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    //read(0, &ch, 1);
    return (ch);
}
// ==============================================
// DRIVE CNC ALONG Y-AXIS
// ==============================================

void drive_up(void *xBoard, int valdrive) { // DRIVE (12,8 CW)
    for (count = 0; count < valdrive; count++) {

        outputs.byOut[1] = 0x04;
        // outputs.byOut[1] = 0x0C;
        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

        //outputs.byOut[1] = index_hex[0];
        outputs.byOut[1] = 0x00;

        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
    }
    outputs.byOut[1] = 0x00;
}

void drive_down(void *xBoard, int valdrive) { // DRIVE (4,0, CCW)
    for (count = 0; count < valdrive; count++) {
        outputs.byOut[1] = 0x0C;
        // outputs.byOut[1] = 0x0C;
        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

        //outputs.byOut[1] = index_hex[0];
        outputs.byOut[1] = 0x00;

        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }


    }
    outputs.byOut[1] = 0x00;
}
// ==============================================
// DRIVE CNC ALONG X-AXIS
// ==============================================

void drive_right(void *xBoard, int valdrive) { // DRIVE (3,2 CW)
    for (count = 0; count < valdrive; count++) {
        outputs.byOut[1] = 0x01;
        // outputs.byOut[1] = 0x0C;
        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

        //outputs.byOut[1] = index_hex[0];
        outputs.byOut[1] = 0x00;

        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

    }
    outputs.byOut[1] = 0x00;

}

void drive_left(void *xBoard, int valdrive) { // DRIVE (1,0 CCW)
    for (count = 0; count < valdrive; count++) {
        outputs.byOut[1] = 0x03;
        // outputs.byOut[1] = 0x0C;
        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

        // outputs.byOut[1] = index_hex[0];
        outputs.byOut[1] = 0x00;

        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

    }
    outputs.byOut[1] = 0x00;
}
// ==============================================
// DRIVE CNC ALONG Z-AXIS
// ==============================================

void pen_up(void *xBoard, int valdrive) { // DRIVE (16,0  CW)
    for (count = 0; count < valdrive; count++) {
        outputs.byOut[1] = 0x10;
        // outputs.byOut[1] = 0x0C;
        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

        //outputs.byOut[1] = index_hex[0];
        outputs.byOut[1] = 0x00;

        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
    }
    outputs.byOut[1] = 0x00;
}

void pen_down(void *xBoard, int valdrive) { // DRIVE (48,32 CCW)
    for (count = 0; count < valdrive; count++) {

        outputs.byOut[1] = 0x30;
        // outputs.byOut[1] = 0x0C;
        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

        // outputs.byOut[1] = index_hex[0];
        outputs.byOut[1] = 0x00;

        if (!XlineSetOnPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }
        if (!XlineSetOffPeriodOutputs(xBoard, outputs)) {
            printf("Failure %d setting outputs.\n", (int) XlineGetLastError(xBoard));
        }

    }

    outputs.byOut[1] = 0x00;
}
// ==============================================


