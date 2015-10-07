import glob
import csv
import numpy as np
import matplotlib.pyplot as plt

for file in glob.glob("*.csv"):
    data = np.genfromtxt(file, delimiter = ',', names = True)

    plt.figure(figsize=(10,20))
    plt.suptitle(file)

    num_plots = len(data.dtype.names)
    count = 1
    for col_name in data.dtype.names:
        plt.subplot(num_plots, 1, count)
        plt.plot(data[col_name], label=col_name)
        plt.legend()
        count += 1

    mng = plt.get_current_fig_manager()
    if plt.get_backend() == 'TkAgg':
        mng.window.state('zoomed')
    elif plt.get_backend() == 'wxAgg':
        mng.frame.Maximize(True)
    elif plt.get_backend() == 'QT4Agg':
        mng.window.showMaximized()

    plt.savefig(file.rstrip('.csv') + '.pdf')
    plt.show()



