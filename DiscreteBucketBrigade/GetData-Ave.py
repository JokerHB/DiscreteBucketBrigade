# -*- coding: utf-8 -*-

from math import factorial
from collections import namedtuple

Info = namedtuple('Info', ['order', 'mean', 'stdev', 'min', 'max'])


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


if __name__ == "__main__":
    cf = 0.0
    for r in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
        resultPath = './ExperResult/result-r-%.1f-wc-cf-%.1f.csv' % (r, cf)
        lines = {}
        with open(resultPath, 'r') as f:
            data = f.readlines()
        pos = 0
        for stationNum in range(3, 11, 2):
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
                print('****** r-%.1f cf-%.1f, %d stations, %d workers ******' %
                      (r, cf, stationNum, workerNum))
                print(sum([x.mean for x in best]) / len(best))
                print(best[0].order, best[0].mean)
                print(best[-1].order, best[-1].mean)

                pos += insNum
