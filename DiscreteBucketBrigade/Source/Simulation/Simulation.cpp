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
    this->time = 0.0;
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
    return minWorkTime == DBL_MAX ? 0.0 : minWorkTime;
}

void Simulation::ProcessWork(std::vector<Station> &stations, std::vector<Worker> &workers, double minWorkTime)
{
    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i].GetState() == Idle)
            continue;
        stations[i].Process(minWorkTime);
    }
}

void Simulation::ArrangeBackward(std::vector<Station> &stations, Worker *worker)
{
    int currentStation = worker->GetCurrentStation();
    Station *station = &stations[currentStation];

    if (currentStation == stations.size() - 1 || !station->IsFinishEmpty() || !station->IsHandoffEmpty())
    {
        station->AddWaitWorker(worker);
    }
    else
    {
        Station *station = &stations[currentStation];
        Station *nextStation = &stations[currentStation + 1];
        if (worker->IsAvailable(nextStation->GetID()))
        {
            nextStation->AddWaitWorker(worker);
        }
        else
        {
            station->AddFinishWorker(worker);
        }
    }
}

void Simulation::ArrangeForward(std::vector<Station> &stations, Worker *worker)
{
    int currentStation = worker->GetCurrentStation();
    Station *station = &stations[currentStation];

    if (currentStation == 0 || !station->IsWaitEmpty() || !station->IsFinishEmpty())
    {
        station->AddWaitWorker(worker);
    }
    else
    {
        Station *station = &stations[currentStation];
        Station *nextStation = &stations[currentStation - 1];
        if (worker->IsAvailable(nextStation->GetID()))
        {
            nextStation->AddHandoffWorker(worker);
        }
        else
        {
            station->AddFinishWorker(worker);
        }
    }
}

void Simulation::Run(std::vector<Station> &stations, std::vector<Worker> &workers, int productNumber)
{
    int stationNum = (int)stations.size();
    int workerNum = (int)workers.size();
    int productNum = 0;

    for (int i = 0; i < workerNum; i++)
    {
        Station *station = &stations[workers[i].GetCurrentStation()];
        station->AddWaitWorker(&workers[i]);
    }

    for (int i = 0; i < stationNum; i++)
    {
        stations[i].ArrangeWorker();
    }

    while (productNum < productNumber)
    {
        double minWorkTime = this->GetMinWorkTime(stations, workers);
        this->time += minWorkTime;
        this->ProcessWork(stations, workers, minWorkTime);
        std::vector<Worker *> idleWorkers;

        for (int i = 0; i < stationNum; i++)
        {
            std::vector<Worker *> _tmp = stations[i].Handoff(stationNum);
            for (int j = 0; j < _tmp.size(); j++)
            {
                idleWorkers.push_back(_tmp[j]);
                if (_tmp[j]->GetCurrentStation() == stations.size() - 1)
                {
                    productNum++;
                }
            }
        }

        while (!idleWorkers.empty())
        {
            Worker *worker = idleWorkers.back();
            idleWorkers.pop_back();
            if (worker->GetDirection() == Forward)
            {
                this->ArrangeForward(stations, worker);
            }
            else
            {
                this->ArrangeBackward(stations, worker);
            }

            for (int i = 0; i < stationNum; i++)
            {
                std::vector<Worker *> _tmp = stations[i].Handoff(stationNum);
                for (int j = 0; j < _tmp.size(); j++)
                {
                    idleWorkers.push_back(_tmp[j]);
                    if (_tmp[j]->GetCurrentStation() == stations.size() - 1)
                    {
                        productNum++;
                    }
                }
            }
        }

        for (int i = 0; i < stationNum; i++)
        {
            stations[i].ArrangeWorker();
        }
    }
    std::cout << "Throughput " << productNumber / this->time << std::endl;
}

#endif /* Simulation_cpp */
