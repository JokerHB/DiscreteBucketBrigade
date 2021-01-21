# -*- coding: utf-8 -*-

from math import pow
from math import sqrt
from math import factorial
from collections import namedtuple
import numpy as np
import pandas as pd
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


def CalDistance(pointA, pointB):
    distance = 0.0
    for a, b in zip(pointA, pointB):
        distance += pow(a - b, 2)
    return sqrt(distance)


def DisplayBestInfo(bestList):
    for best in bestList:
        print(best.order, best.workerNum, best.stationNum, best.cf, best.r)


if __name__ == "__main__":
    dataList = {}
    cnt = 0

    for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]:
        if r not in dataList:
            dataList[r] = {}

        for cf in [0.0, 0.1, 0.5, 0.9]:
            if cf not in dataList[r]:
                dataList[r][cf] = {}

            bestList = []

            resultPath = './ExperResult-20210120/result-r-%.1f-wc-cf-%.1f.csv' % (
                r, cf)
            with open(resultPath, 'r') as f:
                data = f.readlines()
            pos = 0

            for stationNum in range(3, 11, 2):
                if stationNum not in dataList[r][cf]:
                    dataList[r][cf][stationNum] = {}

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

                    if workerNum not in dataList[r][cf][stationNum]:
                        dataList[r][cf][stationNum][workerNum] = best[0]
                    else:
                        print('Error %d %d %.1f %.1f' %
                              (stationNum, workerNum, r, cf))
                        exit(-1)

    for workerNum in range(2, 9):
        for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]:
            orderSet = set()
            for stationNum in range(3, 11, 2):
                if workerNum >= stationNum:
                    continue

                for cf in [0.0, 0.1, 0.5, 0.9]:
                    orderSet.add(
                        tuple(dataList[r][cf][stationNum][workerNum].order))

            variables = []
            for i in range(workerNum):
                variables.append('v%d' % i)

            labels = []
            for e in orderSet:
                labels.append('%s' % str(e))

            matrix = []
            for e in orderSet:
                matrix.append(e)

            if len(matrix) <= 1:
                continue

            _flag = False
            for m in matrix:
                if len(m) <= 1:
                    _flag = True
                    break
            if _flag:
                continue

            print(matrix)

            df = pd.DataFrame(matrix, columns=variables, index=labels)

            disMat = sch.distance.pdist(df, 'euclidean')
            disMat = sch.distance.squareform(disMat)
            Z = sch.linkage(disMat, method='average')

            fig = plt.gcf()
            fig.set_size_inches(30, 30 * 0.7518796992481203)
            ax = fig.add_subplot(111)
            P = sch.dendrogram(Z)
            xLabel = ax.get_xticklabels()
            _orderSet = list(orderSet)
            print('---------- %d ----------' % workerNum)
            newXLabel = []

            for x in xLabel:
                tmp = x.get_text()
                print(int(tmp), type(tmp))
                print(_orderSet[int(tmp)])
                _label = ''
                for _ in _orderSet[int(tmp)]:
                    _label += '%.2f, ' % _
                newXLabel.append(_label[:-2])

            fontsize = 20
            plt.xticks(fontsize=fontsize)
            plt.yticks(fontsize=fontsize)
            plt.title('Clustering of %d workers, r = %.1f' %
                      (workerNum, 1.0 / r),
                      fontsize=fontsize)
            if workerNum >= 4:
                ax.set_xticklabels(newXLabel,
                                   rotation=15,
                                   horizontalalignment='right')
            else:
                ax.set_xticklabels(newXLabel,
                                   rotation=0,
                                   horizontalalignment='right')
            plt.savefig('./plot_dendrogram-%d-%.1f.png' % (workerNum, 1.0 / r))
            cluster = sch.fcluster(Z, 1, criterion='maxclust')
            plt.close()
