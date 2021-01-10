# -*- coding: utf-8 -*-

from matplotlib import pyplot as plt
from math import factorial


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


def PlotFigure(xData, yData, stationNum, workerNum):
    plt.title("Throughput of different worker orders, %d stations" %
              int(stationNum))
    plt.xlabel("Worker order")
    plt.ylabel("Throughput")
    plt.xticks(xData, [str(i) for i in xData])
    fig = plt.gcf()
    fig.set_size_inches(20, 20 * 0.7518796992481203)
    plt.plot(xData, yData)
    plt.savefig('./station-%d-worker%d-max.png' % (stationNum, workerNum), dpi=120)
    plt.close()
    # plt.show()


if __name__ == "__main__":
    resultPath = './result-sp-cf-0.1.csv'
    lines = {}
    with open(resultPath, 'r') as f:
        data = f.readlines()
    pos = 0
    for stationNum in range(3, 11, 2):
        for workerNum in range(2, stationNum):
            if stationNum == 9 and workerNum == 8:
                break
            insNum = factorial(workerNum)
            xData = [i for i in range(insNum)]
            yData = []
            for i in range(insNum):
                (speedList, mean, stdev, _min,
                 _max) = ProcessRow(data[pos + i])
                yData.append(_max)
            # print(pos, yData, xData, stationNum)
            PlotFigure(xData, yData, stationNum, workerNum)
            pos += insNum
