# -*- coding: utf-8 -*-

from math import factorial
from collections import namedtuple

Info = namedtuple('Info', ['order', 'mean', 'stdev', 'min', 'max'])
Data = namedtuple('Data', ['workerNum', 'order', 'efficiency'])


def ProcessRow(data):
    data = data.split(',')
    speedList = data[0].split(' ')
    for i in range(len(speedList)):
        speedList[i] = float(speedList[i].strip())
    mean = float(data[1].strip())
    stdev = float(data[2].strip())
    _min = float(data[3].strip())
    _max = float(data[4].strip())

    return (speedList, mean, stdev, _min, _max)


def Highlight(order):
    ret = '('
    _max = max(order)
    _min = min(order)
    for o in order:
        if o == _max:
            ret += '\\textcolor{red}{%.2f}, ' % o
        elif o == _min:
            ret += '\\textcolor{black}{%.2f}, ' % o
        else:
            ret += '%.2f, ' % o
    ret = ret[:len(ret) - 2]
    ret += ')'
    return ret


def GetLatexTableAvg(stationNum, dataList):
    title = 'Efficiency of different worker orders, %d stations' % (stationNum)
    info = '\\begin{table}[!htbp]\n'
    # info += '\t\\centering\n'
    info += '\t\\small\n'
    info += '\t\\caption{%s}\n' % title
    info += '\t\\setlength{\\tabcolsep}{1mm}\n'
    info += '\t\\renewcommand\\baselinestretch{0.5}\\selectfont\n'
    info += '\t\\begin{center}\n'
    info += '\t\t\\resizebox{\\textwidth}{!}{\n'
    info += '\t\t\\begin{tabular}{ccllcccc}\n'
    info += '\t\t\t\\toprule\n'
    info += '\t\t\tN &$\\frac{\\max\{v_i\}}{\\min\{v_i\}}$ &Best(\\textit{Efficiency}/Sequence) &Worst(\\textit{Efficiency}/Sequence) &Ave(\\textit{Efficiency}) \\\\\n'
    info += '\t\t\t\t\\midrule\n'

    for workerNum in dataList:
        info += '\t\t\t%d' % workerNum
        for r in dataList[workerNum]:
            info += '\t\t\t&%.1f&%.2f/%s&%.2f/%s&%.2f\\\\\n' % (
                1. / r, dataList[workerNum][r][0].efficiency,
                dataList[workerNum][r][0].order,
                dataList[workerNum][r][1].efficiency,
                dataList[workerNum][r][1].order, dataList[workerNum][r][2])
        info += '\t\t\t&&&&\\\\\n'
    info = info[:len(info) - len('\t\t\t&&&&\\\\\n')]
    info += '\t\t\t\\bottomrule\n'
    info += '\t\t\\end{tabular}}\n'
    info += '\t\\end{center}\n'
    info += '\\end{table}\n'

    print(info)
    return info


if __name__ == "__main__":
    dataList = {}
    for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]:
        if r not in dataList:
            dataList[r] = {}

        resultPath = './ExperResult/result-r-%.1f-wc-cf-0.0.csv' % (r)
        with open(resultPath, 'r') as f:
            data = f.readlines()
        pos = 0

        for stationNum in range(3, 11, 2):
            if stationNum not in dataList[r]:
                dataList[r][stationNum] = {}

            for workerNum in range(2, stationNum):
                if r == 1.0:
                    insNum = 1
                else:
                    insNum = factorial(workerNum)
                best = []
                for i in range(insNum):
                    (speedList, mean, stdev, _min,
                     _max) = ProcessRow(data[pos + i])
                    best.append(
                        Info(order=speedList,
                             mean=mean,
                             stdev=stdev,
                             min=_min,
                             max=_max))
                best = sorted(best, key=lambda x: x.mean, reverse=True)
                average = sum([x.mean for x in best]) / len(best)

                dataBest = Data(workerNum=workerNum,
                                order=Highlight(best[0].order),
                                efficiency=best[0].mean)
                dataWorst = Data(workerNum=workerNum,
                                 order=Highlight(best[-1].order),
                                 efficiency=best[-1].mean)
                if workerNum not in dataList[r][stationNum]:
                    dataList[r][stationNum][workerNum] = (dataBest, dataWorst,
                                                          average)
                else:
                    print('Error %d %d %.1f' % (stationNum, workerNum, r))
                    exit(-1)
                pos += insNum

    texHead = '''\\documentclass[10pt,a4paper]{report}
\\usepackage[utf8]{inputenc}
\\usepackage[T1]{fontenc}
\\usepackage{amsmath}
\\usepackage{amsfonts}
\\usepackage{amssymb}
\\usepackage{graphicx}
\\usepackage{amsmath}
\\usepackage{amsfonts}
\\usepackage{amssymb}
\\usepackage{makeidx}
\\usepackage{graphicx}
\\usepackage{booktabs}
\\usepackage{diagbox}
\\usepackage{rotating}
\\usepackage{enumerate}
\\usepackage{color}
\\usepackage{lscape}
\\usepackage{longtable}
\\usepackage[maxfloats=1024]{morefloats}
\\maxdeadcycles=1000

\\begin{document}\n
    '''

    infoList = {}
    for stationNum in range(3, 11, 2):
        if stationNum not in infoList:
            infoList[stationNum] = {}
        for workerNum in range(2, stationNum):
            if workerNum not in infoList[stationNum]:
                infoList[stationNum][workerNum] = {}
            for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0][::-1]:
                infoList[stationNum][workerNum][r] = dataList[r][stationNum][
                    workerNum]
        GetLatexTableAvg(stationNum=stationNum, dataList=infoList[stationNum])

    texHead += '\\end{document}\n'

    print(texHead)
