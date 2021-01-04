//
//  ProductionLine.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2021/1/3.
//

#ifndef ProductionLine_cpp
#define ProductionLine_cpp

#include "../../Include/Model/ProductionLine.h"

ProductionLine::ProductionLine(std::vector<Station> &stations, std::vector<Worker> &workers, int productNum)
{
    this->time = 0.0;
    this->productNum = productNum;

    for (int i = 0; i < stations.size(); i++)
    {
        this->stations.push_back(&stations[i]);
    }

    for (int i = 0; i < workers.size(); i++)
    {
        this->workers.push_back(&workers[i]);
    }
}

ProductionLine::~ProductionLine()
{
}

int ProductionLine::FindStation(int workerID)
{
    return this->workers[workerID]->GetCurrentStation();
}

double ProductionLine::GetMinWorkTime()
{
    double minWorkTime = DBL_MAX;

    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i]->GetState() == Idle)
            continue;

        double _tmp = stations[i]->GetProcessTime();
        if (_tmp != -1.0)
        {
            minWorkTime = std::min(minWorkTime, _tmp);
        }
    }
    return minWorkTime == DBL_MAX ? 0.0 : minWorkTime;
}

void ProductionLine::ProcessWork(double minWorkTime)
{
    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i]->GetState() == Idle)
            continue;
        stations[i]->Process(minWorkTime);
    }
}

void ProductionLine::MoveForward(Worker *worker)
{
    int currentStationID = worker->GetCurrentStation();
    int nextStationID = currentStationID + 1;
    nextStationID = std::min(nextStationID, (int)this->stations.size() - 1);
    Station *nextStation = this->stations[nextStationID];
    nextStation->AddWaitWorker(worker);
}

void ProductionLine::MoveBackward(Worker *worker)
{
    int preWorkerID = worker->GetID() - 1;
    int nextStationID = -1;
    preWorkerID = std::max(0, preWorkerID);
    nextStationID = this->FindStation(preWorkerID);
    Station *nextStation = this->stations[nextStationID];
    if (worker->GetID() == 0)
    {
        this->stations[0]->AddWaitWorker(worker);
    }
    nextStation->AddHandoffWorker(worker);
}

bool cmp(Worker *a, Worker *b)
{
    return a->GetID() - b->GetID();
}

void ProductionLine::ArrangeWorker(std::vector<Worker *> idleWorkers)
{
    std::sort(idleWorkers.begin(), idleWorkers.end(), cmp);

    for (int i = 0; i < idleWorkers.size(); i++)
    {
        Worker *worker = idleWorkers[i];
        int currentStationID = worker->GetCurrentStation();
        int nextStationID = -1;

        if (worker->GetDirection() == Forward)
        {
            nextStationID = currentStationID + 1;
            if (worker->IsAvailable(nextStationID))
            {
                this->MoveForward(worker);
            }
            else
            {
                worker->SetDirection(Backward);
                this->MoveBackward(worker);
            }
        }
        else
        {
            this->MoveBackward(worker);
        }
    }
}

void ProductionLine::Run()
{
    int productCnt = 0;

    for (int i = 0; i < this->workers.size(); i++)
    {
        int currentStation = this->workers[i]->GetCurrentStation();
        Station *station = this->stations[currentStation];
        station->AddWaitWorker(this->workers[i]);
    }

    for (int i = 0; i < this->stations.size(); i++)
    {
        stations[i]->ArrangeWorker();
    }

    while (productCnt < this->productNum)
    {
        double minWorkTime = this->GetMinWorkTime();
        this->time += minWorkTime;
        this->ProcessWork(minWorkTime);
        std::vector<Worker *> idleWorkers;

        for (int i = 0; i < this->stations.size(); i++)
        {
            Station *station = this->stations[i];
            std::vector<Worker *> _tmp = station->Handoff(this->stations.size());
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
            this->ArrangeWorker(idleWorkers);
            idleWorkers.clear();
            for (int i = 0; i < this->stations.size(); i++)
            {
                Station *station = this->stations[i];
                std::vector<Worker *> _tmp = station->Handoff(this->stations.size());
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

        for (int i = 0; i < this->stations.size(); i++)
        {
            this->stations[i]->ArrangeWorker();
        }
    }

    std::cout << "Throughput " << this->productNum / this->time << std::endl;
}

#endif /* ProductionLine_cpp */
