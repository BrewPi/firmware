import glob
import re
import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.backends.backend_pdf
pdf = matplotlib.backends.backend_pdf.PdfPages("test-results-plots.pdf")


# each name in the header is of format like: 5b#beer temp
# 5: subplot number
# b: optional plot type specifier
# after #: legend name

class header_decoder:
    def __init__(self):
        self.matcher = re.compile("\s*([0-9]+)([a-z]*)#(.*)\s*")
        self.names = []
        self.indices = []
        self.types = []

    def decode(self, file_name):
        f = open(file, "rb")
        names = csv.reader(f).next()
        f.close()

        self.names = []
        self.indices = []
        self.types = []

        for name in names:
            match = self.matcher.match(name)
            if match:
                self.indices.append(match.group(1))
                self.types.append(match.group(2))
                self.names.append(match.group(3))
            else:
                print("Cannot decode name '{0}' CSV header of {1}".format(name, file_name))
                exit(1)

hd = header_decoder()


for file in glob.glob("*.csv"):
    data = np.genfromtxt(file, delimiter = ',', names = True)

    plt.figure(figsize=(10,20))
    plt.suptitle(file)

    num_plots = len(data.dtype.names)
    count = 0

    hd.decode(file)
    num_subplots = int(max(hd.indices))
    subplot_ymin = [float("inf")] * num_subplots
    subplot_ymax = [float("-inf")] * num_subplots
    for col_name in data.dtype.names:
        plt.hold(True)
        plot_nr = int(hd.indices[count])
        plt.subplot(num_subplots, 1, plot_nr)

        if hd.types[count] == 'a': # area plot
            line = plt.plot(data[col_name], label=hd.names[count])
            x = range(0, len(data[col_name]))
            last_color = line[-1].get_color()
            plt.fill_between(x, 0, data[col_name], facecolor=last_color, alpha=0.5)
        else:
            plt.plot(data[col_name], label=hd.names[count])
            median = np.median(data[col_name])
            mad = np.median(np.abs(data[col_name] - median))

            ymin = median - 4*mad
            ymax = median + 4*mad
            if ymin < subplot_ymin[plot_nr-1]:
                subplot_ymin[plot_nr-1] = ymin
            if ymax > subplot_ymax[plot_nr-1]:
                subplot_ymax[plot_nr-1] = ymax

        plt.legend()
        count += 1

    for i, val in enumerate(subplot_ymin):
        plt.subplot(num_subplots, 1, i+1)
        if not np.isinf(subplot_ymin[i]) and not np.isinf(subplot_ymax[i]):
            plt.ylim(subplot_ymin[i], subplot_ymax[i])
        if subplot_ymin[i] < 0 < subplot_ymax[i]:
            plt.axhline(0, hold=True, color='grey') # plot line through zero

    pdf.savefig()

    mng = plt.get_current_fig_manager()
    if plt.get_backend() == 'TkAgg':
        (width, height) = mng.window.maxsize()
        if width > 2 * height:
            width = width / 2
        mng.resize(width, height)
    elif plt.get_backend() == 'wxAgg':
        mng.frame.Maximize(True)
    elif plt.get_backend() == 'QT4Agg':
        mng.window.showMaximized()


    plt.show()
    plt.close()

pdf.close()




