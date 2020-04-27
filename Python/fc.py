import serial as ser
import matplotlib.pyplot as plt
import numpy as np
import pyqtgraph as pg
from drawnow import *
import time
from time import sleep
from matplotlib.lines import Line2D
btData=ser.Serial('com15',921600)

cnt=0
time_list=[]
var2_list=[]
var3_list=[]
var4_list=[]
var5_list=[]
var6_list=[]
var7_list=[]
var8_list=[]
var9_list=[]
var10_list=[]
var11_list=[]
var12_list=[]

plt.ion()

def test():
    time=0
    var2=0
    var3=0
    var4=0
    var5=0
    var6=0
    var7=0
    var8=0
    var9=0
    var10=0
    var11=0
    var12=0
 
    cnt=0
    counter=0

    
    while(True):
        while(counter<10):
            current_line=btData.readline()
            if(len(str(current_line))!=34):
                current_line=btData.readline()
        
            time=signed_int(int(current_line[0:8],16))/1000
            var2=signed_int(int(current_line[8:16],16))/1000
            var3=signed_int(int(current_line[16:24],16))/1000
            var4=signed_int(int(current_line[24:32],16))/1000
            var5=signed_int(int(current_line[32:40],16))/1000
            var6=signed_int(int(current_line[40:48],16))/1000
            var7=signed_int(int(current_line[48:56],16))/1000
            var8=signed_int(int(current_line[56:64],16))/1000
            var9=signed_int(int(current_line[64:72],16))/1000
            var10=signed_int(int(current_line[72:80],16))/1000
            var11=signed_int(int(current_line[80:88],16))/1000
            var12=signed_int(int(current_line[88:96],16))/1000
            
            
      
    
            time_list.append(time)
            var2_list.append(var2)
            var3_list.append(var3)
            var4_list.append(var4)
            var5_list.append(var5)
            var6_list.append(var6)
            var7_list.append(var7)
            var8_list.append(var8)
            var9_list.append(var9)
            var10_list.append(var10)
            var11_list.append(var11)
            var12_list.append(var12)
            
            cnt=cnt+1
            if(cnt>100000):                            
                time_list.pop(0)
                var2_list.pop(0)
                var3_list.pop(0)
                var4_list.pop(0)
                var5_list.pop(0)
                var6_list.pop(0)
                var7_list.pop(0)
                var8_list.pop(0)
                var9_list.pop(0)
                var10_list.pop(0)
                var11_list.pop(0)
                var12_list.pop(0)

            counter+=1
        


            
        drawnow(makeFig)                       #Call drawnow to update our live graph
        counter=0
        plt.pause(.000001)                     #Pause Briefly. Important to keep drawnow from crashing
       
        btData.flushInput()

     
def test2():
    fig, ax = plt.subplots()
    ax.set_xlim(0,100)
    ax.set_ylim(-200,200)
    time=0
    var2=0
    var3=0
    var4=0
    cnt=0

    

    
    num_plots = 0

    while(True):
        
        current_line=btData.readline()
        if(len(str(current_line))!=34):
            current_line=btData.readline()
        
        time=signed_int(int(current_line[0:8],16))/1000
        var2=signed_int(int(current_line[8:16],16))/1000
        time_list.append(time)
        var2_list.append(var2)
        line=Line2D(time_list,var2_list)
        ax.add_line(line)
        print(line)
        
        #fig.canvas.draw()
        #fig.canvas.flush_events()
        cnt=cnt+1
        if(cnt>50):                            #If you have 50 or more points, delete the first one from the array
            time_list.pop(0)
            var2_list.pop(0)
       
        btData.flushInput()
        num_plots += 1
        btData.flushInput()
        print(var2)
    print(num_plots)

def test3():
    time=0
    var2=0
    var3=0
    var4=0
    cnt=0
    handle=pg.plot()

    
    while(True):
        
        current_line=btData.readline()
        if(len(str(current_line))!=34):
            current_line=btData.readline()
        
        time=signed_int(int(current_line[0:8],16))/1000
        var2=signed_int(int(current_line[8:16],16))/1000
      
    
        time_list.append(time)
        var2_list.append(var2)
        handle.plot(time_list,var2_list)
        cnt=cnt+1
        if(cnt>1000):                            #If you have 50 or more points, delete the first one from the array
            time_list.pop(0)
            var2_list.pop(0)

        btData.flushInput()
        sleep(0.01)



def signed_int(var):
    if var>0x7FFFFFFF:
        var=var-0x100000000
    return var

def makeFig(): #Create a function that makes our desired plot
    #plt.plot(time_list, var2_list, 'r')       #plot the temperature
    #plt.plot(time_list, var3_list, 'b')       #plot the temperature
    #plt.plot(time_list, var4_list, 'r')       #anglez
    plt.plot(time_list, var5_list, 'c')       #errorz
    #plt.plot(time_list, var6_list, 'm')       #error PID1 z
    #plt.plot(time_list, var7_list, 'y')       #pid2z
    plt.plot(time_list, var8_list, 'k')       #pid1z
    plt.plot(time_list, var9_list, 'g')       #desiredz
    #plt.plot(time_list, var10_list, 'g')       #plot the temperature
    #plt.plot(time_list, var11_list, 'r')       #plot the temperature
    #plt.plot(time_list, var12_list, 'b')       #plot the temperature

