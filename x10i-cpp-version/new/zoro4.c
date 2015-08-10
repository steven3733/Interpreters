
//  /usr/bin/gcc -pedantic -Wall -O2 -DX10_LINUX_BUILD -std=gnu99 -I../include -L../lib  ../lib/libfflyusb.so ../lib/libxlinecapi.so -lpthread ../lib/unlockioc.o ../lib/linuxkb.o ../lib/xlineauthenticate.o ../lib/xlinecexit.o -lm -o zoro4.x zoro4.c 

#include <string.h>
#include <math.h>
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







const double TWOPIE = 6.2831853072;
const double STEP = 0.0090; // REDUCE BY HALF ACTUAL SIZE OF 0.0045 SETTING

double SQR(double a) {
    return pow(a, 2);
}

//double NEXT_STEP(double DELTA);

double NEXT_STEP(double DELTA) {
    double m_DELTA;
    if (DELTA > 0.5 * STEP)
        m_DELTA = STEP;
    else if (DELTA < -0.5 * STEP)
        m_DELTA = -STEP;
    else
        m_DELTA = 0.0;
    return m_DELTA;
}

double lXFUN(double u, double cx1, double cx2) {
    return cx1 + (cx2 - cx1) * u;
}

double lYFUN(double u, double cy1, double cy2) {
    return cy1 + (cy2 - cy1) * u;
}

double lZFUN(double u, double cz1, double cz2) {
    return cz1 + (cz2 - cz1) * u;
}

double lXFUN_1ST(double u, double cx1, double cx2) {
    return cx2 - cx1;
}

double lYFUN_1ST(double u, double cy1, double cy2) {
    return cy2 - cy1;
}

double lZFUN_1ST(double u, double cz1, double cz2) {
    return cz2 - cz1;
}

void pcline(void *xBoard, double cx2, double cy2, double cz2) {
    double cx1 = 0.0;
    double cy1 = 0.0;
    double cz1 = 0.0;
    double X = cx1;
    double Y = cy1;
    double Z = cz1;
    double x = cx1;
    double y = cy1;
    double z = cz1;
    double xp, yp, zp;
    double xDist, yDist, zDist;
    double u = 0.0;
    int XF, YF, ZF, GF1, GF2;
    int SelectedPlane;
    int SX1, SY1, SZ1, SX2, SY2, SZ2;
    double DX, DY, DZ, INC_X, INC_Y, INC_Z;
    unsigned long TP = 0;

    int valout_pulse = 0;
    int valout_dir = 0;

    while (u <= 1.0) {
        xp = lXFUN_1ST(u, cx1, cx2);
        yp = lYFUN_1ST(u, cy1, cy2);
        zp = lZFUN_1ST(u, cz1, cz2);
        if (xp >= 0) {
            xDist = fabs(x - X - STEP);
            XF = 10000;
        } else {
            xDist = fabs(x - X + STEP);
            XF = 0;
        }
        if (yp >= 0) {
            yDist = fabs(y - Y - STEP);
            YF = 1000;
        } else {
            yDist = fabs(y - Y + STEP);
            YF = 0;
        }
        if (zp >= 0) {
            zDist = fabs(z - Z - STEP);
            ZF = 100;
        } else {
            zDist = fabs(z - Z + STEP);
            ZF = 0;
        }
        if (fabs(yp * xDist) >= fabs(xp * yDist)) GF1 = 10;
        else GF1 = 0;
        if (SQR(zp)*(SQR(xDist) + SQR(yDist)) >= SQR(zDist)*(SQR(xp) + SQR(yp))) GF2 = 1;
        else GF2 = 0;
        SelectedPlane = XF + YF + ZF + GF1 + GF2;
        switch (SelectedPlane) {
            case (11000):case (11100):case (10100):case (10000): u = u + (xDist / xp);
                break; //Plane 1
            case (11010):case (1110):case (11110): u = u + (yDist / yp);
                break; //Plane 2
            case (1100):case (1000):case (100):case (0): u = u - (xDist / xp);
                break; //Plane 3
            case (10110):case (10010):case (1010):case (110):case (10): u = u - (yDist / yp);
                break; //Plane 4
            case (11111):case (11101):case (10111):case (10101):case (1111):case (1101):case (1011):
            case (1001):case (111):case (101): u = u + (zDist / zp);
                break; //Plane 5
            case (11001):case (10011):case (10001):case (11):case (1):case (11011): u = u - (zDist / zp);
                break; //Plane 6
            default: u = 1000;
                break;
        }


        x = lXFUN(u, cx1, cx2);
        y = lYFUN(u, cy1, cy2);
        z = lZFUN(u, cz1, cz2);
        DX = x - X;
        DY = y - Y;
        DZ = z - Z;
        INC_X = NEXT_STEP(DX);
        INC_Y = NEXT_STEP(DY);
        INC_Z = NEXT_STEP(DZ);
        X = X + INC_X;
        Y = Y + INC_Y;
        Z = Z + INC_Z;
        if (INC_X > 0.0) {
            SX1 = 1;
            SX2 = 0;
        } else if (INC_X < 0.0) {
            SX1 = 3;
            SX2 = 2;
        } else {
            SX1 = 0;
            SX2 = 0;
        }
        if (INC_Y > 0.0) {
            SY1 = 4;
            SY2 = 0;
        } else if (INC_Y < 0.0) {
            SY1 = 12;
            SY2 = 8;
        } else {
            SY1 = 0;
            SY2 = 0;
        }
        if (INC_Z > 0.0) {
            SZ1 = 16;
            SZ2 = 0;
        } else if (INC_Z < 0.0) {
            SZ1 = 48;
            SZ2 = 32;
        } else {
            SZ1 = 0;
            SZ2 = 0;
        }
        valout_pulse = SX1 + SY1 + SZ1;
        valout_dir = SX2 + SY2 + SZ2;

        printf("valout_pulse = %d\tvalout_dir = %d \n", valout_pulse, valout_dir);


        /*
                // REALTIME DRIVER FOR PARALLEL PORT OUTPUTS
                // 1. MODIFIED FOR LINUX BY WRY

                outb(valout_pulse + on, BASEPORT);
                // outb(valout_pulse+on, 888);
                // usleep(700);
                rt_sleep(nano2count(TICKS));

                outb(valout_pulse + on, BASEPORT);
                // outb(valout_dir+on, 888);
                // usleep(700);
                rt_sleep(nano2count(TICKS));
         */

        SetOutputs.byOut[0] = valout_pulse;
        UnSetOutputs.byOut[0] = (255 - valout_pulse);
        XlineModifyOutputs(xBoard, &UnSetOutputs, &SetOutputs);
        usleep(3);

        SetOutputs.byOut[0] = valout_dir;
        UnSetOutputs.byOut[0] = (255 - valout_dir);
        XlineModifyOutputs(xBoard, &UnSetOutputs, &SetOutputs);
        usleep(3);


        TP++;
    }
    return;
}

FILE *fpData;

int main(int argc, char* argv[]) {

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

    //  ===========================================================================================

    const unsigned long int maxsize = 100000;
    double XP[maxsize];
    double YP[maxsize];
    double ZP[maxsize];
    // int laser[maxsize];
    // INITIALIZATION
    int i;
    for (i = 0; i < maxsize; i++) {
        XP[i] = 0.0;
        YP[i] = 0.0;
        ZP[i] = 0.0;
    }

    char str[100] = "";
    //char gcodefile[50] = "";
    char gcodefile[50];
    long int num;
    long int j;
    char X[20] = "";
    char Y[20] = "";
    char Z[20] = "";
    long int line = 1;
    //printf("Please enter G_Code file: ");
    //scanf("%s", gcodefile);
    //fgets(gcodefile, 256, stdin);
    //int i;
    /*
        for (i = 0; i < 50; i++) {
            if (gcodefile[i] == '\n') { // END-OF-LINE character
                gcodefile[i] = '\0'; // Replace with NULL character
                break;
            }
        }
     */
    fpData = fopen("mmu-modified.ngc", "r");
    //printf("Converting G code to controller...\n");
    /*
        while (!feof(fpData)) {
            fgets(str, sizeof (str), fpData);
     */
    while (fgets(str, sizeof (str), fpData)) {
        XP[line] = XP[line - 1];
        YP[line] = YP[line - 1];
        ZP[line] = ZP[line - 1];
        /*
                if (str[0] == 'R') laser[line] = 0;
                if (str[0] == 'L') laser[line] = 128;
         */
        for (num = 0; num < strlen(str); num++) {
            if (str[num] == 'X') {
                j = 1;
                while (str[num + j] != ' ') {
                    X[j - 1] = str[num + j];
                    j++;
                }
                XP[line] = atof(X);
            }
        }
        for (num = 0; num < strlen(str); num++) {
            if (str[num] == 'Y') {
                j = 1;
                while (str[num + j] != ' ') {
                    Y[j - 1] = str[num + j];
                    j++;
                }
                YP[line] = atof(Y);
            }
        }

        for (num = 0; num < strlen(str); num++) {
            if (str[num] == 'Z') {
                j = 1;
                while (str[num + j] != ' ') {
                    Z[j - 1] = str[num + j];
                    j++;
                }
                ZP[line] = atof(Z);
            }
        }
        pcline(xBoard, XP[line] - XP[line - 1], YP[line] - YP[line - 1], ZP[line] - ZP[line - 1]);

        line++;
    }

    printf("DONE! ...\n");
    fclose(fpData);

    //================================================================================
    return ( XlineExit("Leaving program.", xBoard, &x15Authenticate, 0));
}



