# -*- coding: utf-8 -*-

from math import ceil


def getHandoff3(pos, speed):
    ret = [0.0]
    ret.append(ceil(((10.0 - pos[2]) / speed[2]) * speed[0]))
    ret.append(pos[1] + ceil(((10.0 - pos[2]) / speed[2]) * speed[1]))
    # ret.append((((10.0 - pos[2]) / speed[2]) * speed[0]))
    # ret.append(pos[1] + (((10.0 - pos[2]) / speed[2]) * speed[1]))

    for i in range(1, len(ret)):
        ret[i] = ret[i] if ret[i] >= 1.0 else 1.0
        ret[i] = 9.0 if ret[i] >= 9.0 else ret[i]

    return ret


def getHandoff2(pos, speed):
    ret = [0.0]
    # ret.append(pos[0] + ceil(((10.0 - pos[1]) / speed[1]) * speed[0]))
    ret.append(pos[0] + (((10.0 - pos[1]) / speed[1]) * speed[0]))

    for i in range(1, len(ret)):
        ret[i] = ret[i] if ret[i] >= 1.0 else 1.0
        ret[i] = 9.0 if ret[i] >= 9.0 else ret[i]

    return ret


if __name__ == '__main__':
    pos2 = [1.0, 2.0]
    speed2 = [0.1, 0.2]
    pos3 = [1.0, 2.0, 6.0]
    speed3 = [0.8, 0.5, 0.9]
    cnt = 0

    while cnt < 1000:
        # print(pos2)
        # pos2 = getHandoff2(pos=pos2, speed=speed2)
        print(pos3)
        pos3 = getHandoff3(pos=pos3, speed=speed3)
        cnt += 1
