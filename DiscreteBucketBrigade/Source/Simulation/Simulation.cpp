//
//  Simulation.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Simulation_cpp
#define Simulation_cpp

#include "../../Include/Simulation/Simulation.h"

Simulation::Simulation()
{
}

Simulation::~Simulation()
{
}

double Simulation::GetMinWorkTime(std::vector<Station> &stations, std::vector<Worker> &workers)
{
    double minWorkTime = DBL_MAX;

    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i].GetState() == Idle)
            continue;

        double _tmp = stations[i].GetProcessTime();
        if (_tmp != -1.0)
        {
            minWorkTime = std::min(minWorkTime, _tmp);
        }
    }
    return minWorkTime;
}

std::vector<Worker *> Simulation::GetIdleWorker(std::vector<Station> &stations, std::vector<Worker> &workers, double minWorkTime)
{
    std::vector<Worker *> idleWorkers;

    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i].GetState() == Idle)
            continue;
        Worker *_tmp = stations[i].Process(minWorkTime);
        if (_tmp != NULL)
        {
            idleWorkers.push_back(_tmp);
        }
    }
    return idleWorkers;
}

void Simulation::Run(std::vector<Station> &stations, std::vector<Worker> &workers, int productNumber)
{
    int stationNum = stations.size();
    int workerNum = workers.size();
    int productNum = 0;

    while (productNum < productNumber)
    {
        double minWorkTime = this->GetMinWorkTime(stations, workers);
        std::vector<Worker *> idleWorkers = this->GetIdleWorker(stations, workers, minWorkTime);

        for (int i = 0; i < idleWorkers.size(); i++)
        {
            Worker *worker = idleWorkers[i];
            int stationID = worker->GetCurrentStation();

            if (worker->GetCurrentStation() == stationNum - 1)
            {
                productNum++;
            }

            // TODO arrange workers
        }
    }
}

#endif /* Simulation_cpp */
