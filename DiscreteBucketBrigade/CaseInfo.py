# -*- coding: utf-8 -*-

from math import pow
from math import sqrt
from math import factorial
from collections import namedtuple
import numpy as np
import pandas as pd
import itertools as it
import matplotlib.pylab as plt
import scipy.cluster.hierarchy as sch

Info = namedtuple('Info', ['order', 'mean', 'stdev', 'min', 'max'])
Data = namedtuple('Data', ['workerNum', 'order', 'efficiency'])
BestInfo = namedtuple('BestInfo',
                      ['order', 'workerNum', 'stationNum', 'cf', 'r'])


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


def GetSpeedList(workerNum, r):
    speedList = []
    an = 2.0 / (workerNum * (1. + r))
    a1 = r * an
    delta = (an - a1) / (workerNum - 1)

    for i in range(workerNum):
        speedList.append(a1 + delta * i)

    return speedList


def DisplayBestInfo(bestList):
    for best in bestList:
        print(best.order, best.workerNum, best.stationNum, best.cf, best.r)


def DisplayInfo(bestDic, worstDic):
    for key in bestDic:
        if bestDic[key] == 0 and worstDic[key] == 0:
            continue
        print(key, bestDic[key], worstDic[key])


def OrderToString(order):
    ret = ''
    for o in order:
        ret += '%.2f, ' % o
    return ret[:len(ret) - 2]


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


def TexTableGenerate(bestDic, worstDic, workerNum):
    title = 'Best and worst cases of different orders, %d workers' % (
        workerNum)
    # info = '\\begin{table}[!htbp]\n'
    # info += '\t\\centering\n'
    info = '\\begin{center}\n'
    info += '\t\\small\n'
    info += '\t\\begin{longtable}{clcc}\n'
    info += '\t\t\\caption{%s}\\\\\n' % title
    info += '\t\t\\toprule\n'
    info += '\t\t\\setlength{\\tabcolsep}{1mm}\n'
    info += '\t\t\\renewcommand\\baselinestretch{0.5}\\selectfont\n'
    # info += '\t\t\\resizebox{\\textwidth}{!}{\n'
    info += '\t\t$\\frac{\\max\\{v_i\\}}{\\min\\{v_i\\}}$ & Order & Best cases(\\%) & Worst cases(\\%) \\\\\n'
    info += '\t\t\t\\midrule\n'

    for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0][::-1]:
        info += '\t\t%.1f' % (1.0 / r)
        flag = True
        for key in bestDic[r]:
            if bestDic[r][key] == 0 and worstDic[r][key] == 0:
                continue
            info += '\t\t\t&%s&%.2f&%.2f\\\\\n' % (
                Highlight(key), bestDic[r][key], worstDic[r][key])
            flag = False
        if flag:
            info = info[:len(info) - len('\t\t\t%.1f' % r)]
        else:
            info += '\t\t&&&\\\\\n'

    info = info[:len(info) - len('\t\t&&&\\\\\n')]
    info += '\t\t\\bottomrule\n'
    info += '\t\\end{longtable}\n'
    info += '\\end{center}\n'
    # info += '\\end{table}\n'
    info += '\\newpage'

    return info


if __name__ == "__main__":
    bestDataList = {}
    worstDataList = {}
    cnt = 0

    tex = '''\\documentclass[10pt,a4paper]{report}
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

    for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]:
        if r not in bestDataList:
            bestDataList[r] = {}
        if r not in worstDataList:
            worstDataList[r] = {}

        for cf in [0.0, 0.1, 0.5, 0.9]:
            if cf not in bestDataList[r]:
                bestDataList[r][cf] = {}
            if cf not in worstDataList[r]:
                worstDataList[r][cf] = {}

            bestList = []

            resultPath = './ExperResult-20210121/result-r-%.1f-wc-cf-%.1f.csv' % (
                r, cf)
            with open(resultPath, 'r') as f:
                data = f.readlines()
            pos = 0

            for stationNum in range(3, 11, 2):
                if stationNum not in bestDataList[r][cf]:
                    bestDataList[r][cf][stationNum] = {}
                if stationNum not in worstDataList[r][cf]:
                    worstDataList[r][cf][stationNum] = {}

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
                    pos += insNum
                    cnt += 1
                    best = sorted(best, key=lambda x: x.mean, reverse=True)
                    average = sum([x.mean for x in best]) / len(best)

                    if best[0].mean == best[-1].mean:
                        continue

                    if workerNum not in bestDataList[r][cf][stationNum]:
                        bestDataList[r][cf][stationNum][workerNum] = best[0]
                    else:
                        print('Error %d %d %.1f %.1f' %
                              (stationNum, workerNum, r, cf))
                        exit(-1)

                    if workerNum not in worstDataList[r][cf][stationNum]:
                        worstDataList[r][cf][stationNum][workerNum] = best[-1]
                    else:
                        print('Error %d %d %.1f %.1f' %
                              (stationNum, workerNum, r, cf))
                        exit(-1)

    for workerNum in range(2, 9):
        bestPerDic = {}
        worstPerDic = {}
        for r in [i / 10.0 for i in range(3, 11)][::]:
            if r not in bestPerDic:
                bestPerDic[r] = {}
            if r not in worstPerDic:
                worstPerDic[r] = {}

            speedList = GetSpeedList(workerNum=workerNum, r=r)
            per = it.permutations(speedList)

            for p in per:
                pp = tuple([float('%.6f' % num) for num in p])
                bestPerDic[r][tuple(pp)] = 0
                worstPerDic[r][tuple(pp)] = 0

            caseCnt = 0

            for stationNum in range(3, 11, 2):
                if stationNum <= workerNum:
                    continue

                for cf in [0.0, 0.1, 0.5, 0.9]:
                    if workerNum not in bestDataList[r][cf][stationNum]:
                        break
                    bestOrder = bestDataList[r][cf][stationNum][
                        workerNum].order
                    worstOrder = worstDataList[r][cf][stationNum][
                        workerNum].order

                    pBest = tuple([float('%.6f' % num) for num in bestOrder])
                    pWorst = tuple([float('%.6f' % num) for num in worstOrder])

                    bestPerDic[r][pBest] += 1
                    worstPerDic[r][pWorst] += 1
                    caseCnt += 1

            if caseCnt != 0:
                for key in bestPerDic[r]:
                    bestPerDic[r][key] = 100 * (bestPerDic[r][key] /
                                                float(caseCnt))
                    worstPerDic[r][key] = 100 * (worstPerDic[r][key] /
                                                 float(caseCnt))

            # print('---------- %d %.1f----------' % (workerNum, r))
            # print(bestPerDic)
            # print(worstPerDic)
            # print('Order\tBest\tWorst')
            # DisplayInfo(bestDic=bestPerDic[r], worstDic=worstPerDic[r])

        tables = TexTableGenerate(bestDic=bestPerDic,
                                  worstDic=worstPerDic,
                                  workerNum=workerNum)
        tex += tables
    tex += '\\end{document}\n'

    print(tex)
