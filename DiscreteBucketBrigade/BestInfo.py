# -*- coding: utf-8 -*-

from math import factorial
from collections import namedtuple

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


def IsSF(info):
    sortedOrder = sorted(info.order)
    for i in range(len(sortedOrder)):
        if sortedOrder[i] != info.order[i]:
            return False
    return True


def IsFS(info):
    sortedOrder = sorted(info.order, reverse=True)
    for i in range(len(sortedOrder)):
        if sortedOrder[i] != info.order[i]:
            return False
    return True


def IsFSF(info):
    slow = min(info.order)
    index = info.order.index(slow)
    _len = len(info.order)
    if _len % 2 != 0 and index == int(_len / 2):
        for i in range(1, index):
            if info.order[i - 1] < info.order[i]:
                return False
        for i in range(index + 1, len(info.order)):
            if info.order[i - 1] > info.order[i]:
                return False
        return True
    elif _len % 2 == 0 and (index == int(_len / 2) or index == int(
        (_len - 1) / 2)):
        for i in range(1, index):
            if info.order[i - 1] < info.order[i]:
                return False
        for i in range(index + 1, len(info.order)):
            if info.order[i - 1] > info.order[i]:
                return False
        return True
    return False


def IsSFS(info):
    fast = max(info.order)
    index = info.order.index(fast)
    _len = len(info.order)
    if _len % 2 != 0 and index == int(_len / 2):
        for i in range(1, index):
            if info.order[i - 1] > info.order[i]:
                return False
        for i in range(index + 1, len(info.order)):
            if info.order[i - 1] < info.order[i]:
                return False
        return True
    elif _len % 2 == 0 and (index == int(_len / 2) or index == int(
        (_len - 1) / 2)):
        for i in range(1, index):
            if info.order[i - 1] > info.order[i]:
                return False
        for i in range(index + 1, len(info.order)):
            if info.order[i - 1] < info.order[i]:
                return False
        return True
    return False


def IsFH(info):
    _max = max(info.order)
    if info.order.index(_max) == 0:
        return True
    return False


def IsFT(info):
    _max = max(info.order)
    if info.order.index(_max) == len(info.order) - 1:
        return True
    return False


def DisplayBestInfo(bestList):
    for best in bestList:
        print(best.order, best.workerNum, best.stationNum, best.cf, best.r)


if __name__ == "__main__":
    dataList = {}
    cnt = 0
    SF = []
    FS = []
    SFS = []
    FSF = []
    Other = []
    FH = []
    FT = []

    for r in [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]:
        if r not in dataList:
            dataList[r] = {}

        for cf in [0.0, 0.1, 0.5, 0.9]:
            if cf not in dataList[r]:
                dataList[r][cf] = {}

            resultPath = './ExperResult-20210117/result-r-%.1f-wc-cf-%.1f.csv' % (
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
                    best = sorted(best, key=lambda x: x.mean, reverse=True)
                    average = sum([x.mean for x in best]) / len(best)
                    bestInfo = BestInfo(order=best[0].order,
                                        workerNum=workerNum,
                                        stationNum=stationNum,
                                        cf=cf,
                                        r=r)

                    if IsFH(best[0]):
                        FH.append(bestInfo)
                    elif IsFT(best[0]):
                        FT.append(bestInfo)

                    if IsSF(best[0]):
                        SF.append(bestInfo)
                    elif IsFS(best[0]):
                        FS.append(bestInfo)
                    elif IsFSF(best[0]):
                        FSF.append(bestInfo)
                    elif IsSFS(best[0]):
                        SFS.append(bestInfo)
                    elif IsSF(best[0]) is False and IsFS(
                            best[0]) is False and IsFSF(
                                best[0]) is False and IsSFS(best[0]) is False:
                        Other.append(bestInfo)

                    cnt += 1

    print('---------- FH %d----------' % (len(FH)))
    print(DisplayBestInfo(FH))

    print('---------- FT %d----------' % (len(FT)))
    print(DisplayBestInfo(FT))

    print('---------- SF %d----------' % (len(SF)))
    print(DisplayBestInfo(SF))

    print('---------- FS %d----------' % (len(FS)))
    print(DisplayBestInfo(FS))

    print('---------- FSF %d----------' % (len(FSF)))
    print(DisplayBestInfo(FSF))

    print('---------- SFS %d----------' % (len(SFS)))
    print(DisplayBestInfo(SFS))

    print('---------- Other %d----------' % (len(Other)))
    print(DisplayBestInfo(Other))

    print(cnt)
