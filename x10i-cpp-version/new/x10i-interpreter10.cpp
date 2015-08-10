
// COMPILATION
//  g++ x10i-interpreter10.cpp -o x10i-interpreter10.x


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <sstream>


using namespace std;

class Point {
public:
    float x;
    float y;
    float z;

    Point() {
    }

    Point(float tx, float ty, float tz) {
        x = tx;
        y = ty;
        z = tz;
    }

    void setPoint(float tx, float ty, float tz = 0) {
        x = tx;
        y = ty;
        z = tz;
    }

    void copy(Point &p2) {
        x = p2.x;
        y = p2.y;
        z = p2.z;
    }
};

// Global variables
ofstream myfile;

void bresenham_3D(Point, Point);
void drawCircle(Point, Point, int, int, int);
int scan_int(char *str, int *valp, unsigned int *seen, unsigned int flag);
int scan_float(char *str, float *valp, unsigned int *seen, unsigned int flag);
void readFile();

Point cur, next;
int ioff, joff;
string str;


Point points[1000]; // array to store points between two points
int drive[10000]; // array of direction moves(1\0) necessary to reach endpoint
int stop[10000];
int sizet = 0; // size of points and moves
int psn = 10; // x*=psn	// precision

int xmov, ymov, zmov, xstop, ystop, zstop; // directions x(ccw)=1, x(cw)=3, y(ccw)=4 ...

/*
/*
 * opening an input file in the globle 
 */
FILE * ifp;

/* gcode line parse results */
typedef struct GcodeParser {
    unsigned int seen;
    int G;
    int M;
    float P;
    float X;
    float Y;
    float Z;
    float I;
    float J;
    float F;
} GcodeParser;




/*
 * Boolean datatype is not defined in C programming language, so had to define it 
 */


bool abs_mode = true; //0 = incremental; 1 = absolute

int last_gcode_g = -1;

/* bit-flags for commands and parameters */
#define GCODE_G	(1<<0)
#define GCODE_M	(1<<1)
#define GCODE_P	(1<<2)
#define GCODE_X	(1<<3)
#define GCODE_Y	(1<<4)
#define GCODE_Z	(1<<5)
#define GCODE_I	(1<<6)
#define GCODE_J	(1<<7)
#define GCODE_K	(1<<8)
#define GCODE_F	(1<<9)


#define PARSE_INT(ch, str, len, val, seen, flag) \
	case ch: \
		len = scan_int(str, &val, &seen, flag); \
		break;

#define PARSE_FLOAT(ch, str, len, val, seen, flag) \
	case ch: \
		len = scan_float(str, &val, &seen, flag); \
		break;

/*
 * I should put the inside code of process_string() and parse_string() inside 
 * my readFile() so I can check the "G", "M", and the extract the values of (X,Y,Z)
 *  
 */


int main(int argc, char *argv[]) {

    myfile.open("signals.txt");
    readFile();

}

void readFile() {

    //char * line = NULL;
    //size_t len = 0;
    char dataLine[50];
    int sizeLine = 50;
    /* file name to open */
    char filename[50];
    printf("please enter the G-code file name: ");
    scanf("%s", filename);

    /* check if the file not empty */
    ifp = fopen(filename, "r");
    if (ifp == NULL)
        exit(EXIT_FAILURE);


    //init 
        Point fp;
        fp.x = 0.0;
        fp.y = 0.0;
        fp.z = 0.0;


    GcodeParser gc;

    int ind;
    int len; /* length of parameter argument */

    gc.seen = 0;

    while (!feof(ifp)) {
        fgets(dataLine, sizeLine, ifp);


        /* scan the string for commands and parameters, recording the arguments for each,
         * and setting the seen flag for each that is seen
         */
        for (ind = 0; ind < sizeLine; ind += (1 + len)) {
            len = 0;
            switch (dataLine[ind]) {
                    PARSE_INT('G', &dataLine[ind + 1], len, gc.G, gc.seen, GCODE_G);
                    PARSE_INT('M', &dataLine[ind + 1], len, gc.M, gc.seen, GCODE_M);

                    PARSE_FLOAT('P', &dataLine[ind + 1], len, gc.P, gc.seen, GCODE_P);
                    PARSE_FLOAT('X', &dataLine[ind + 1], len, gc.X, gc.seen, GCODE_X);
                    PARSE_FLOAT('Y', &dataLine[ind + 1], len, gc.Y, gc.seen, GCODE_Y);
                    PARSE_FLOAT('Z', &dataLine[ind + 1], len, gc.Z, gc.seen, GCODE_Z);
                    PARSE_FLOAT('I', &dataLine[ind + 1], len, gc.I, gc.seen, GCODE_I);
                    PARSE_FLOAT('J', &dataLine[ind + 1], len, gc.J, gc.seen, GCODE_J);
                    PARSE_FLOAT('F', &dataLine[ind + 1], len, gc.F, gc.seen, GCODE_F);

                default:
                    break;
            }
        }



        if ((!(gc.seen & (GCODE_G | GCODE_M))) &&
                ((gc.seen != 0) &&
                (last_gcode_g >= 0))
                ) {
            /* yes - so use the previous command with the new parameters */
            gc.G = last_gcode_g;
            gc.seen |= GCODE_G;
        }
        //did we get a gcode?
        if (gc.seen & GCODE_G) {
            last_gcode_g = gc.G; /* remember this for future instructions */
             fp = cur;
            if (abs_mode) {
                if (gc.seen & GCODE_X)
                    fp.x = gc.X;
                if (gc.seen & GCODE_Y)
                    fp.y = gc.Y;
                if (gc.seen & GCODE_Z)
                    fp.z = gc.Z;
            } else {
                if (gc.seen & GCODE_X)
                    fp.x += gc.X;
                if (gc.seen & GCODE_Y)
                    fp.y += gc.Y;
                if (gc.seen & GCODE_Z)
                    fp.z += gc.Z;
            }

            /*
                        // Get feedrate if supplied
                        if (gc.seen & GCODE_F)
                            feedrate = gc.F;
             */

            //do something!
            switch (gc.G) {
                    //Rapid Positioning
                    //Linear Interpolation
                    //these are basically the same thing.
                case 0:
                    //cout << "Rapid linear move  \n";
                    //cout << "X = " << fp.x << " Y = " << fp.y << " Z = " << fp.z << "\n";
                    //                    next.x = fp.x;
                    //                    next.y = fp.y;
                    //                    next.z = fp.z;
                    bresenham_3D(cur, next);
                    cur.copy(next);
                case 1:

                    //cout << "Linear interpolation  \n";
                    // cout << "X = " << fp.x << " Y = " << fp.y << " Z = " << fp.z << "\n";
                    //                    next.x = fp.x;
                    //                    next.y = fp.y;
                    //                    next.z = fp.z;
                    bresenham_3D(cur, next);
                    cur.copy(next);
                case 2:
                    //Counterclockwise arc
                    // cout << "Clockwise circular interpolation  \n";
                    //cout << "X = " << fp.x << " Y = " << fp.y << " Z = " << fp.z << "\n";
                    //                    next.x = fp.x;
                    //                    next.y = fp.y;
                    //                    next.y = fp.z;
                    drawCircle(cur, next, ioff, joff, 2);

                case 3:
                    //cout << "counterClockwise circular  interpolation  \n";
                    //cout << "X = " << fp.x << " Y = " << fp.y << " Z = " << fp.z << "\n";
                    next.x = fp.x;
                    next.y = fp.y;
                    next.y = fp.z;
                    drawCircle(cur, next, ioff, joff, 3);

                    break;
                default:
                    break;
            }

        }
    }

    fclose(ifp);
}

void bresenham_3D(Point p1, Point p2) {
    // =================================
    int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    Point pixel;
    pixel.copy(p1);
    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    dz = p2.z - p1.z;
    if (dx < 0) //-> 1) determine directions for each axis: +/-  
    {
        x_inc = -1;
        xmov = 1; //  left
        xstop = 0;
    } else {
        x_inc = 1; // right
        xmov = 3;
        xstop = 2;
    }
    if (dy < 0) {
        y_inc = -1;
        ymov = 4; // backward	
        ystop = 0;
    } else {
        y_inc = 1; // forward
        ymov = 12;
        ystop = 8;
    }
    if (dz < 0) {
        z_inc = -1; // down
        zmov = 48;
        zstop = 32;
    } else {
        z_inc = 1; // up
        zmov = 16;
        zstop = 0;
    }


    l = abs(dx); //-> 2) find absolute values in order to determine leading axis
    m = abs(dy); // 		the largest will be leading axis
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;
    int mov_bit = 0, stop_bit = 0;
    if ((l >= m) && (l >= n)) // x is leading
    {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i <= l; i++) //-> 3) every time increment by 1 leading axis and check err value
        { // to decide to increment other axes
            drive[i] = mov_bit;
            stop[i] = stop_bit;
            mov_bit = 0;
            stop_bit = 0;
            points[i].copy(pixel);
            if (err_1 > 0) {
                pixel.y += y_inc;
                err_1 -= dx2;
                mov_bit |= ymov; //-> 4) capture the direction 
                stop_bit |= ystop;
            }
            if (err_2 > 0) {
                pixel.z += z_inc;
                err_2 -= dx2;
                mov_bit |= zmov;
                stop_bit |= zstop;
            }
            err_1 += dy2;
            err_2 += dz2;
            pixel.x += x_inc;
            mov_bit |= xmov;
            stop_bit |= xstop;

            myfile << mov_bit << " " << stop_bit << endl;
            //cout << mov_bit << " " << stop_bit << endl;

        }
    } else if ((m >= l) && (m >= n)) // y is leading
    {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i <= m; i++) {
            drive[i] = mov_bit;
            stop[i] = stop_bit;
            mov_bit = 0;
            stop_bit = 0;
            points[i].copy(pixel);
            if (err_1 > 0) {
                pixel.x += x_inc;
                err_1 -= dy2;
                mov_bit |= xmov;
                stop_bit |= xstop;
            }
            if (err_2 > 0) {
                pixel.z += z_inc;
                err_2 -= dy2;
                mov_bit |= zmov;
                stop_bit |= zstop;
            }
            err_1 += dx2;
            err_2 += dz2;
            pixel.y += y_inc;
            mov_bit |= ymov;
            stop_bit |= ystop;
            myfile << mov_bit << " " << stop_bit << endl;

            //cout << mov_bit << " " << stop_bit << endl;

        }
    } else // z is leading
    {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i <= n; i++) {
            drive[i] = mov_bit;
            stop[i] = stop_bit;
            mov_bit = 0;
            stop_bit = 0;
            points[i].copy(pixel);
            if (err_1 > 0) {
                pixel.y += y_inc;
                err_1 -= dz2;
                mov_bit |= ymov;
                stop_bit |= ystop;
            }
            if (err_2 > 0) {
                pixel.x += x_inc;
                err_2 -= dz2;
                mov_bit |= xmov;
                stop_bit |= xstop;
            }
            err_1 += dy2;
            err_2 += dx2;
            pixel.z += z_inc;
            mov_bit |= zmov;
            stop_bit |= zstop;
            myfile << mov_bit << " " << stop_bit << endl;

            //cout << mov_bit << " " << stop_bit << endl;

        }
    }



    sizet = i;
}

void drawCircle(Point start, Point end, int i_off, int j_off, int cw) {
    Point cent;
    float angleA, angleB, angle, radius, length, aX, aY, bX, bY;
    cent.setPoint(start.x + i_off, start.y + j_off);
    //-> 1) Find the angles of starting and ending point 
    aX = (start.x - cent.x);
    aY = (start.y - cent.y);
    bX = (end.x - cent.x);
    bY = (end.y - cent.y);
    if (cw == 2) // Clockwise 
    {
        angleA = atan2(bY, bX);
        angleB = atan2(aY, aX);
    } else // Counterclockwise 
    {
        angleA = atan2(aY, aX);
        angleB = atan2(bY, bX);
    }
    if (angleB <= angleA) // make sure start_point angle is smaller than endpoint one.
    {
        angleB += 2 * M_PI;
    }
    angle = angleB - angleA;
    radius = sqrt(aX * aX + aY * aY);
    length = radius * angle; // length of arc
    int steps, s, step;
    float curve_length = 0.5;
    steps = (int) ceil(length / curve_length);
    Point newPoint, oldPoint;
    oldPoint.copy(start);
    // -> 2) from start_angle increment angle and calculate new point
    for (s = 1; s <= steps; s++) {
        step = (cw == 3) ? s : steps - s; // go backwards for CW
        newPoint.x = cent.x + radius * cos(angleA + angle * ((float) step / steps));
        newPoint.y = cent.y + radius * sin(angleA + angle * ((float) step / steps));
        newPoint.z = start.z;
        oldPoint.setPoint(oldPoint.x*psn, oldPoint.y*psn, oldPoint.z * psn);
        newPoint.setPoint(newPoint.x*psn, newPoint.y*psn, oldPoint.z * psn);
        bresenham_3D(oldPoint, newPoint);
        oldPoint.setPoint(newPoint.x / psn, newPoint.y / psn, oldPoint.z / psn);
    }

}

int scan_float(char *str, float *valp, unsigned int *seen, unsigned int flag) {
    float res;
    int len;
    char *end;

    res = (float) strtod(str, &end);

    len = end - str;

    if (len > 0) {
        *valp = res;
        *seen |= flag;
    } else
        *valp = 0;

    return len; /* length of number */
}

int scan_int(char *str, int *valp, unsigned int *seen, unsigned int flag) {
    int res;
    int len;
    char *end;

    res = (int) strtol(str, &end, 10);
    len = end - str;

    if (len > 0) {
        *valp = res;
        *seen |= flag;
    } else
        *valp = 0;

    return len; /* length of number */
}
