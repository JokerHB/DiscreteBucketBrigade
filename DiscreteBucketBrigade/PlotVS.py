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


def PlotFastvsOther(xData, yData, yyData, stationNum, r, cf):
    fig = plt.gcf()
    fig.set_size_inches(10, 10 * 0.7518796992481203)
    plt.title("Average efficiency of different worker orders, %d stations" %
              int(stationNum))
    plt.xlabel("Worker number \n (r=%.1f, cf=%.1f)" % (r, cf))
    plt.ylabel("Efficiency")
    plt.xticks(xData, [str(i) for i in xData])

    plt.plot(xData, yData, marker='o', linestyle='--', label='Fast')
    plt.plot(xData, yyData, marker='.', linestyle=':', label='Other')

    plt.legend()

    plt.savefig('./r-%.1f-cf-%.1f-FastVsOther-%d.png' %
                (r, cf, int(stationNum)),
                dpi=120)
    plt.close()
    # plt.show()


def PlotFastvsSlow(xData, yData, yyData, stationNum, r, cf):
    fig = plt.gcf()
    fig.set_size_inches(10, 10 * 0.7518796992481203)
    plt.title("Average efficiency of different worker orders, %d stations" %
              int(stationNum))
    plt.xlabel("Worker number \n (r=%.1f, cf=%.1f)" % (r, cf))
    plt.ylabel("Efficiency")
    plt.xticks(xData, [str(i) for i in xData])

    plt.plot(xData, yData, marker='o', linestyle='--', label='Fast')
    plt.plot(xData, yyData, marker='.', linestyle=':', label='Slow')

    plt.legend()

    plt.savefig('./r-%.1f-cf-%.1f-FastVsSlow-%d.png' %
                (r, cf, int(stationNum)),
                dpi=120)
    plt.close()
    # plt.show()


def PlotSlowvsOther(xData, yData, yyData, stationNum, r, cf):
    fig = plt.gcf()
    fig.set_size_inches(10, 10 * 0.7518796992481203)
    plt.title("Average efficiency of different worker orders, %d stations" %
              int(stationNum))
    plt.xlabel("Worker number \n (r=%.1f, cf=%.1f)" % (r, cf))
    plt.ylabel("Efficiency")
    plt.xticks(xData, [str(i) for i in xData])

    plt.plot(xData, yData, marker='o', linestyle='--', label='Slow')
    plt.plot(xData, yyData, marker='.', linestyle=':', label='Other')

    plt.legend()

    plt.savefig('./r-%.1f-cf-%.1f-SlowVsOther-%d.png' %
                (r, cf, int(stationNum)),
                dpi=120)
    plt.close()
    # plt.show()


if __name__ == "__main__":
    for r in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
        for cf in [0.1, 0.5, 0.9]:
            resultPath = './ExperResult/result-r-%.1f-wc-cf-%.1f.csv' % (r, cf)
            lines = {}
            with open(resultPath, 'r') as f:
                data = f.readlines()
            pos = 0
            for stationNum in range(3, 11, 2):
                vsXdata = []
                vsYdataFast = []
                vsYYdataFast = []
                vsYdataSlow = []
                vsYYdataSlow = []
                vsYdataFastSlow = []
                vsYYdataFastSlow = []
                for workerNum in range(2, stationNum):
                    vsXdata.append(workerNum)
                    insNum = factorial(workerNum)
                    fast = []
                    otherFast = []
                    otherSlow = []
                    slow = []
                    tmpMax = ProcessRow(data[pos])[0]
                    tmpMin = min(tmpMax)
                    tmpMax = max(tmpMax)
                    for i in range(insNum):
                        (speedList, mean, stdev, _min,
                         _max) = ProcessRow(data[pos + i])
                        if tmpMax == speedList[0]:
                            fast.append(mean)
                            otherSlow.append(mean)
                        elif tmpMin == speedList[0]:
                            slow.append(mean)
                            otherFast.append(mean)
                        else:
                            otherFast.append(mean)
                            otherSlow.append(mean)
                    vsYdataFast.append(sum(fast) / len(fast))
                    vsYYdataFast.append(sum(otherFast) / len(otherFast))

                    vsYdataSlow.append(sum(slow) / len(slow))
                    vsYYdataSlow.append(sum(otherSlow) / len(otherSlow))

                    vsYdataFastSlow.append(sum(fast) / len(fast))
                    vsYYdataFastSlow.append(sum(slow) / len(slow))
                    pos += insNum
                PlotFastvsOther(vsXdata, vsYdataFast, vsYYdataFast, stationNum,
                                r, cf)
                PlotFastvsSlow(vsXdata, vsYdataFastSlow, vsYYdataFastSlow,
                               stationNum, r, cf)
                PlotSlowvsOther(vsXdata, vsYdataSlow, vsYYdataSlow, stationNum,
                                r, cf)
