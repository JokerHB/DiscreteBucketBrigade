//
//  ProductionLine.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2021/1/3.
//

#ifndef ProductionLine_h
#define ProductionLine_h

#include <iostream>
#include <algorithm>
#include <cfloat>
#include <vector>
#include "Worker.h"
#include "Station.h"
#include "../Tool/Direction.h"
#include "../Tool/State.h"

class ProductionLine
{
private:
    std::vector<Station *> stations;
    std::vector<Worker *> workers;
    double time;
    int productNum;

public:
    ProductionLine(std::vector<Station> &stations, std::vector<Worker> &workers, int productNum = 1000);

    ~ProductionLine();

    int FindStation(int workerID);

    double GetMinWorkTime();

    void ProcessWork(double minWorkTime);

    void MoveForward(Worker *worker);

    void MoveBackward(Worker *worker);

    void ArrangeHandoff(int &productCnt);

    void ArrangeFinish();
    
    void ArrangeWait();

    void ArrangeWorker(std::vector<Worker *> idleWorkers);

    double Run();
};

#endif /* ProductionLine_h */