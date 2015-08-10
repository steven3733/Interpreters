//#include "interpreter.hpp"


// COMPILATION
//  g++ x10i-interpreter9.cpp -o x10i-interpreter9.x

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <math.h>

//#include <Point.h>

using namespace std;

// G00 - Rapid linear move
// G01 - Linear interpolation
// G2 -  Clockwise circular interpolation
// G3 -  Counterclockwise circular interpolation
// G20 - Inches
// G21 - Millimetres
// G28 - return to home position
// G30 - return to home position through intermediate point
// G90 - Absolute distance mode
// G91 - Incremental distance mode
// G92 - set current as absolute zero point
// M00 - Pause
// M02 - Stop

// Global variables
ofstream myfile;

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

//string cmds[] = {"G0", "G1", "G2", "G3", "G20", "G21", "G28", "G30", "G92", "G90", "G91", "M02", "M00", "G90.1", "G91.1"};
int numcmd = 15; // number of possible commands
Point cur, next;
double ioff, joff;
string str;


Point points[1000]; // array to store points between two points
int drive[10000]; // array of direction moves(1\0) necessary to reach endpoint
int stop[10000];
int sizet = 0; // size of points and moves
int psn = 10; // x*=psn	// precision

int xmov, ymov, zmov, xstop, ystop, zstop; // directions x(ccw)=1, x(cw)=3, y(ccw)=4 ...

/*
This function is implementation of 3D Bresenham's(Midpoint) line generation algorithm.
In order to use floating points with this algorithm they are multiplied
by given precision before passing arguments to the function;
 */
void bresenham_3D(Point p1, Point p2);

void drawCircle(Point start, Point end, int i_off, int j_off, int cw);

//======================================================================================================


bool is_present(string &);
double get_num(int);
void getXYZ();
void getIJ();
//void start_interpreter(char *file_name, int *move_bits,int *stop_bits)

int main(int argc, char *argv[]) {
    myfile.open("signals-new2.txt");
    ifstream infile;
    infile.open(argv[1]);
    cur.setPoint(0, 0, 0);
    int mode = 0; // 0 for absolute, 1 for incremental mode
    int mode_cir = 1; // 0 for absolute, 1 for incremental IJK offsets
    int linenum = 1;
    int cmd_num;
    while (!infile.eof()) {
        getline(infile, str);
        //cout<<"Processing line #"<<linenum<<" command: "<<str<<endl;
        linenum++;
         if (str[0] == '(' || str[0] == '/')
            continue;
        if (str[0] == 'G') {
            double cm = get_num(1);
            if (cm == 90.1) {
                mode_cir = 0;
            } else if (cm == 91.1) {
                mode_cir = 1;
            } else {
                
                cmd_num = (int) cm;
                
                switch (cmd_num) {
                    case 0:
                        getXYZ();
                        bresenham_3D(cur, next);
                        cur.copy(next);
                        break;
                    case 1:
                        getXYZ();
                        bresenham_3D(cur, next);
                        cur.copy(next);
                        break;
                    case 2:
                        getXYZ();
                        getIJ();
                        drawCircle(cur, next, ioff, joff, 2);
                        break;
                    case 3:
                        getXYZ();
                        getIJ();
                        drawCircle(cur, next, ioff, joff, 3);
                        break;
                    case 20:
                        //inches
                        break;
                    case 21:
                        // millimetres
                        break;
                    case 28:
                        break;
                    case 30:
                        break;
                    case 90:
                        mode = 0; // absolute mode
                        break;
                    case 91:
                        mode = 1; // inremental mode
                        break;
                    case 92:
                        break;
                    default:
                        break;
                }
            }

        } else if (str[0] == ' ') {
            getXYZ();
            bresenham_3D(cur, next);
            cur.copy(next);
        }
    }

}

//bool is_present(string & val) {
//    bool ok = false;
//    for (int i = 0; i < numcmd; i++) {
//        if (cmds[i] == val)
//            ok = true;
//    }
//    return ok;
//}

double get_num(int pos) {
    char num[10];
    int j = 0;
    for (int i = pos; i < str.length(); i++) {
        if (str[i] == ' ')
            break;
        num[j] = str[i];
        j++;
    }
    double n = (floor(atof(num) + 0.5));
    return n;
}

void getXYZ() {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == 'X')
            next.x = get_num(i + 1);
        if (str[i] == 'Y')
            next.y = get_num(i + 1);
        if (str[i] == 'Z')
            next.z = get_num(i + 1);
    }
}

void getIJ() {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == 'I')
            ioff = get_num(i + 1);
        if (str[i] == 'J')
            joff = get_num(i + 1);
    }
}



// =================================

void bresenham_3D(Point p1, Point p2)
// =================================
{
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
        }
    }
    sizet = i;
}

// ------------------------------------- start

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




