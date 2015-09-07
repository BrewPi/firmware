import glob
import csv
import numpy as np
import matplotlib.pyplot as plt

for file in glob.glob("*.csv"):
    data = np.genfromtxt(file, delimiter = ',', names = True)

    plt.figure()
    num_plots = len(data.dtype.names)
    count = 1
    for col_name in data.dtype.names:
        plt.subplot(num_plots, 1, count)
        plt.plot(data[col_name], label=col_name)
        plt.legend()
        count += 1

    plt.show()
