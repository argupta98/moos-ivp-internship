
#!/usr/bin/python

import sys
import math

theta_x=25.06
theta_y=24.78
grid_size=10
#function for descritizing coordinates
def descritize(xi, yi):
   x=float(xi)*math.cos(theta_x)+83
   y=float(yi)*math.cos(theta_y)+114
   x=x/grid_size
   y=y/grid_size
   return (math.ceil(x),math.ceil(y))
   
print 'Converting file: ', str(sys.argv[1])

#read alog file line by line into array
with open(sys.argv[1]) as inp:
   data = inp.read().splitlines() 

#split each line at spaces keeping only the node report
reports = []
for report in data:
   split_report = report.split()
   for term in split_report:
      if(len(term)>30):
         reports.append(term)

#isolate x,y coordinates for each node report and write to .csv
x=0
y=0
time=0
group=''
newfile = open(str(sys.argv[2]), 'w')
if(len(reports)>75):
   for index in range(20, 75, 11):
      elements=reports[index].split(',')
      for term in elements:
         pair=term.split('=')
         if(pair[0]=='X'):
            x=pair[1]
         elif(pair[0]=='Y'):
            y=pair[1]
         elif(pair[0]=='TIME'):
            time=pair[1]
         elif(pair[0]=='GROUP'):
            group=pair[1]
      location=descritize(x,y)
      print str(location),'    ', str(time), '      ' , str(len(reports))
      newfile.write(str(location[0])+','+str(location[1])+',')
      
   newfile.write(group+','+str(sys.argv[3])+'\n')
newfile.close()
   
      
      

#print 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)
 
