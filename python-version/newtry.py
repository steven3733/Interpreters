#!/usr/bin/python

#from Breserham1 import Breserhams
import re
import time
#from numpy import pi, sin, cos, sqrt, arccos, arcsin, abs, sqrt, fabs
import math


global x0; 
x0 = 0.0;
global y0;
y0 = 0.0;

global x_pos; 
x_pos= 0.0;
global y_pos;
y_pos = 0.0;
global z_pos;
z_pos = 0.0;
global dx; 
dx2 = 0.0;
global dy;
dy2 = 0.0;
global dz;
dz2 = 0.0;
def stepFromCurrentToGoal(x0, y0, x1, y1):
   #base case
   if (x0==x1 and y0==y1):
       return

   xIter = 1 if (x1 > x0) else -1

   yIter = 1 if (x1 > x0) else -1

   #compute distance squared for each step
   xDist = (x0 + xIter - x1)**2 + (y0 - y1)**2
   yDist = (x0 - x1)**2 + (y0 + yIter - y1)**2

   #step to minimize distance
   if xDist <= yDist:
       print "Step in X From: " + str(x0) + " to: " + str(x0+xIter)
       x0+=xIter
   else:
       print "Step in Y From: " + str(y0) + " to: " + str(y0+yIter)
       y0+=yIter

   #keep going recursively
   return stepFromCurrentToGoal(x0, y0, x1, y1)


	
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
		
			stepFromCurrentToGoal(x0, y0, x_pos, y_pos)	
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
