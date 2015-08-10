#!/usr/bin/python

#from Breserham1 import Breserhams
import re
import time
#from numpy import pi, sin, cos, sqrt, arccos, arcsin, abs, sqrt, fabs
import math


global x2;x2= 0.0;
global y2;y2 = 0.0;
global z2;z2 = 0.0;

global x1;x1= 0.0;
global y1;y1 = 0.0;
global z1;z1 = 0.0;

global x_pos;x_pos= 0.0;
global y_pos;y_pos = 0.0;
global z_pos;z_pos = 0.0;

global dx;dx = 0.0;
global dy;dy = 0.0;
global dz;dz = 0.0;

global dx2;dx2 = 0;
global dy2;dy2 = 0;
global dz2;dz2 = 0;

		
#points[]
# HOW TO MAKE A LIST OF CLASS INSTANCES 
#my_objects = []
#for i in range(100) :
    #my_objects.append(MyClass(i))

listofpoints = []
ftemp = open('temp.txt', 'w')

def Breserhams(x1,y1,z1,x2,y2,z2):
	dx = x2 - x1;
	dy = y2 - y1;
	dz = z2 - z1;
	
	if dx < 0:  #-> 1) determine directions for each axis: +/-  
	  x_inc = -1;
	  xmov = 1; #left
	  xstop = 0;
	else:
	  x_inc = 1; #right
	  xmov = 3;
	  xstop = 2;
	
	if dy < 0 :
	  y_inc = -1;
	  ymov = 4; # backward	
	  ystop = 0;
	else:
	  y_inc = 1; #forward
	  ymov = 12;
	  ystop = 8;
	
	if dz < 0 :
	  z_inc = -1; #down
	  zmov = 48;
	  zstop = 32;
	else:
	  z_inc = 1; #up
	  zmov = 16;
	  zstop = 0;
	  
	l = abs(dx); #-> 2) find absolute values in order to determine leading axis
	m = abs(dy); #the largest will be leading axis
	n = abs(dz);

	#dx2 = (l << 1);
	#dy2 = (m << 1);
	#dz2 = (n << 1);

	mov_bit = 0;
	stop_bit = 0;
	
	if l >= m and l >= n: #x is leading
		err_1 = dy2 - l;
		err_2 = dz2 - l;
		i = 0
		while i<= l: #-> 3) every time increment by 1 leading axis and check err value
					#to decide to increment other axes
			drive = []
			stop = []
			drive.append(mov_bit);
			stop.append(stop_bit);
			mov_bit = 0;
			stop_bit = 0;
			#points[i].copy(pixel);
			#listofpoints.append(Point())
			if err_1 > 0:
				y1 += y_inc;
				err_1 -= dx2;
				mov_bit |= ymov; #-> 4) capture the direction 
				stop_bit |= ystop;
			
			if err_2 > 0:
				z1 += z_inc;
				err_2 -= dx2;
				mov_bit |= zmov;
				stop_bit |= zstop;
			
			err_1 += dy2;
			err_2 += dz2;
			x1 += x_inc;
			mov_bit |= xmov;
			stop_bit |= xstop;

			print '%d %d' % (mov_bit, stop_bit);
			#ftemp.write('mov_bit, stop_bit')
			print >>ftemp, mov_bit, stop_bit			#PRINT INTO "temp.txt" FILE
			i += 1;
	elif m >= l and m >= n: #y is leading
		err_1 = dx2 - m;
		err_2 = dz2 - m;
		i = 0
		while i <= m : 
			drive = []
			stop = []
			drive.append(mov_bit);
			stop.append(stop_bit);
			mov_bit = 0;
			stop_bit = 0;
			#points[i].copy(pixel);
			if err_1 > 0: 
				x1 += x_inc;
				err_1 -= dy2;
				mov_bit |= xmov;
				stop_bit |= xstop;
			
			if err_2 > 0: 
				z1 += z_inc;
				err_2 -= dy2;
				mov_bit |= zmov;
				stop_bit |= zstop;
			
			err_1 += dx2;
			err_2 += dz2;
			y1 += y_inc;
			mov_bit |= ymov;
			stop_bit |= ystop;

			print '%d %d' % (mov_bit, stop_bit);
			#ftemp.write('mov_bit, stop_bit')
			print >>ftemp, mov_bit, stop_bit			#PRINT INTO "temp.txt" FILE
			i += 1;
	else :#z is leading
		err_1 = dy2 - n;
		err_2 = dx2 - n;
		i = 0
		while i <= n : 
			drive = []
			stop = []
			drive.append(mov_bit);
			stop.append(stop_bit);
			mov_bit = 0;
			stop_bit = 0;
			#points[i].copy(pixel);
			if err_1 > 0: 
				y1 += y_inc;
				err_1 -= dz2;
				mov_bit |= ymov;
				stop_bit |= ystop;
				
			if err_2 > 0: 
				x1 += x_inc;
				err_2 -= dz2;
				mov_bit |= xmov;
				stop_bit |= xstop;
				
			err_1 += dy2;
			err_2 += dx2;
			z1 += z_inc;
			mov_bit |= zmov;
			stop_bit |= zstop;

			print '%d %d' % (mov_bit, stop_bit);
			print >>ftemp, mov_bit, stop_bit			#PRINT INTO "temp.txt" FILE
			i += 1;
	sizet = i;
	return;
	
####################################
##### 		open file          #####
####################################
filename = raw_input("Enter the name of .ngc file: ")
fo = open(filename, "r")
####################################
###### GET THE AXES POSITIONS ######
####################################
def XPosition(lines):
  	  xchar_loc=lines.index('X')
	  i=xchar_loc+1
	  while (47<ord(lines[i])<58)|(lines[i]=='.')|(lines[i]=='-'):
		  i+=1
	  x_pos=float(lines[xchar_loc+1:i])   
	  print "X = ",x_pos
	  return x_pos
	
def YPosition(lines):
  	  ychar_loc=lines.index('Y')
	  i=ychar_loc+1
	  while (47<ord(lines[i])<58)|(lines[i]=='.')|(lines[i]=='-'):
		  i+=1
	  y_pos=float(lines[ychar_loc+1:i])   
	  print "Y = ",y_pos
	  return y_pos
	
def ZPosition(lines):
	  zchar_loc=lines.index('Z')
	  i=zchar_loc+1
	  while (47<ord(lines[i])<58)|(lines[i]=='.')|(lines[i]=='-'):
		  i+=1
	  z_pos=float(lines[zchar_loc+1:i])
	  print "Z = ",z_pos
	  return z_pos
#################################
#given a G02 or G03 movement command line, return the I J position
def IPosition(lines):
  ichar_loc=lines.index('I')
  i=ichar_loc+1
  while (47<ord(lines[i])<58)|(lines[i]=='.')|(lines[i]=='-'):
	  i+=1
  i_pos=float(lines[ichar_loc+1:i])
  print "I = ",i_pos
  return i_pos

def JPosition(lines):
  jchar_loc=lines.index('J')
  i=jchar_loc+1
  while (47<ord(lines[i])<58)|(lines[i]=='.')|(lines[i]=='-'):
	  i+=1
  j_pos=float(lines[jchar_loc+1:i])
  print "J = ",j_pos
  return j_pos
  
 
try:#read and execute G code
	for lines in fo:
		if lines==[]:
			1 #blank lines
		elif lines[0:3]=='G90':
			print "start"
			
		elif lines[0:3]=='G20':# working in inch
			dx/=25.4
			dy/=25.4
			print 'Working in inch'
			  
		elif lines[0:3]=='G21':# working in mm
			print 'Working in mm'  
			
		elif lines[0:3]=='M05':
			print 'Laser turned off'
			
		elif lines[0:3]=='M03':
			GPIO.output(Laser_switch,True)
			print 'Laser turned on'

		elif lines[0:3]=='M02':
			GPIO.output(Laser_switch,False)
			print 'finished. shuting down'
			break
		elif (lines[0:3]=='G1F')|(lines[0:4]=='G1 F'):
			1#do nothing
		elif (lines[0:3]=='G0 ')|(lines[0:3]=='G1 ')|(lines[0:3]=='G01'): 
			##linear movement
			print 'linear engraving movement'
			
			if 'X' in lines:
			  x_pos = XPosition(lines)
			if 'Y' in lines:
			  y_pos = YPosition(lines)
			if 'Z' in lines:
			  z_pos = ZPosition(lines)
			
			Breserhams(x1,y1,z1, x2=x_pos, y2=y_pos, z2=z_pos)	
			
			#point1.x = point2.x 
			#point1.y = point2.y 
			#point1.z = point2.z 
			
		elif (lines[0:3]=='G02')|(lines[0:3]=='G03'): #circular erpolation
			print "circular erpolation"
						
			#if 'X' in lines:
			  #x_pos = XPosition(lines)
			#if 'Y' in lines:
			  #y_pos = YPosition(lines)
			#if 'Z' in lines:
			  #z_pos = ZPosition(lines)
			#if 'I' in lines:
			  #i_pos = IPosition(lines)
			#if 'J' in lines:
			  #j_pos = JPosition(lines)

except KeyboardInterrupt:
    pass
 


fo.close()  
