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


def GetLatexTableAvg(stationNum, f, data):
    title = 'Efficiency of different worker orders, %d stations, $\\frac{\min_{1 \\leq i \\leq n}{v_i}}{\\max_{1 \\leq j \\leq n}{v_j}}$ = %.1f' % (
        stationNum, r)
    info = '\\begin{table}[!htbp]\n'
    info += '\t\\caption{%s}\n' % title
    info += '\t\\begin{center}\n'
    info += '\t\t\\begin{tabular}{ccc}\n'
    info += '\t\t\t\\toprule\n'
    info += '\t\t\tWorker number & Worker order & Efficiency \\\\\n'
    info += '\t\t\t\\midrule\n'

    for lines in data:
        info += '\t\t\t%d (Best case)\t&%s\t&%.2f\\\\\n' % (
            lines[0].workerNum, lines[0].order, lines[0].efficiency)
        info += '\t\t\t%d (Worst case)\t&%s\t&%.2f\\\\\n' % (
            lines[0].workerNum, lines[1].order, lines[1].efficiency)
        info += '\t\t\t\\textit{Average}\t&-\t&%.2f\\\\\n' % (lines[2])

    info += '\t\t\t\\bottomrule\n'
    info += '\t\t\\end{tabular}\n'
    info += '\t\\end{center}\n'
    info += '\\end{table}\n'

    print(info)
    return info


if __name__ == "__main__":
    for r in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
        for cf in [0.1, 0.5, 0.9]:
            resultPath = './ExperResult/result-r-%.1f-wc-cf-%.1f.csv' % (r, cf)
            with open(resultPath, 'r') as f:
                data = f.readlines()
            pos = 0
            for stationNum in range(3, 11, 2):
                dataList = []
                for workerNum in range(2, stationNum):
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
                    dataList.append((dataBest, dataWorst, average))
                    pos += insNum

                GetLatexTableAvg(stationNum, r, dataList)
