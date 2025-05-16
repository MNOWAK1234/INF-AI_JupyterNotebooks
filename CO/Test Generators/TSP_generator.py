import random
import os
import numpy
import matplotlib.pyplot as plt
plt.rcParams.update({'figure.max_open_warning': 0})#IDK how this works, but it shuts all errors

n=int(input())#how many datasets
k=int(input())#how many points in each datasets
path='D:/Mikołaj/Zadania/Studia/CO_datasets'

for i in range (n):
    os.mkdir(path+'/'+'dataset'+str(i))
    f = open(path+'/'+'dataset'+str(i)+'/coords.txt', 'w' )#w stands for write
    f.write(str(k)+'\n')
    #w stands for write
    #Coordinates
    x = [random.randint(0,100000) for i in range(k)]
    y = [random.randint(0,100000) for i in range(k)]
    # Setting the figure size
    fig = plt.figure(figsize=(10,5))
    # Plotting
    plt.plot(x,y,'r*')
    # Displaying grid
    plt.grid()
    # Controlling axis
    plt.axis([0, 100000, 0, 100000])
    #Saving the plot as an image
    fig.savefig(path+'/'+'dataset'+str(i)+'/plot.jpg', bbox_inches='tight', dpi=150)
    # Displaying plot
    #plt.show()
    for i in range(k):
        currx=x[i]
        curry=y[i]
        f.write(str(currx))
        f.write(' ')
        f.write(str(curry))
        f.write('\n')
    f.close()


