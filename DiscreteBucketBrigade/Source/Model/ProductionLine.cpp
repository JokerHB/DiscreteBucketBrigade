//
//  ProductionLine.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2021/1/3.
//

#ifndef ProductionLine_cpp
#define ProductionLine_cpp

#include "../../Include/Model/ProductionLine.h"

bool cmp(Worker *a, Worker *b)
{
    return a->GetID() > b->GetID();
}

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
    this->stations.clear();
    this->workers.clear();
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

void ProductionLine::ProcessWork(double minWorkTime, int &productCnt)
{
    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i]->GetState() == Idle)
            continue;
        stations[i]->Process(minWorkTime);
    }

    Station *lastStation = this->stations[this->stations.size() - 1];
    while (!lastStation->IsFinishEmpty())
    {
        Worker *worker = lastStation->GetFinishWorker();
        worker->SetDirection(Backward);
        this->MoveBackward(worker);
        productCnt++;
    }
}

void ProductionLine::MoveForward(Worker *worker)
{
    int currentStationID = worker->GetCurrentStation();
    int nextWorkerID = worker->GetID() + 1 == (int)this->workers.size() ? (int)this->workers.size() - 1 : worker->GetID() + 1;
    Worker *nextWorker = this->workers[nextWorkerID];
    int nextStationID = currentStationID + 1;
    if (nextWorkerID == worker->GetID())
    {
        nextStationID = std::min(nextStationID, (int)this->stations.size() - 1);
    }
    else
    {
        nextStationID = currentStationID + 1 > nextWorker->GetCurrentStation() ? nextWorker->GetCurrentStation() : currentStationID + 1;
    }
    Station *nextStation = this->stations[nextStationID];
    nextStation->AddWaitWorker(worker);
}

void ProductionLine::MoveBackward(Worker *worker)
{
    int nextWorkerID = worker->GetID() - 1 == -1 ? 0 : worker->GetID() - 1;
    Worker *nextWorker = this->workers[nextWorkerID];
    int nextStationID = nextWorker->GetCurrentStation();

    if (nextWorkerID == worker->GetID() && worker->GetID() == 0)
    {
        worker->SetDirection(Forward);
        Station *nextStation = this->stations[0];
        nextStation->AddWaitWorker(worker);
    }
    else
    {
        Station *nextStation = this->stations[nextStationID];
        nextStation->AddHandoffWorker(worker);
    }
}

void ProductionLine::MoveStay(Worker *worker)
{
    Station *station = this->stations[worker->GetCurrentStation()];
    worker->SetDirection(Forward);
    station->AddWaitWorker(worker);
}

void ProductionLine::ArrangeWorker(std::vector<Worker *> idleWorkers)
{
    std::sort(idleWorkers.begin(), idleWorkers.end(), cmp);

    for (int i = 0; i < idleWorkers.size(); i++)
    {
        Worker *worker = idleWorkers[i];
        int currentStationID = worker->GetCurrentStation();

        if (worker->GetDirection() == Forward)
        {
            int nextWorkerID = worker->GetID() + 1 == (int)this->workers.size() ? (int)this->workers.size() - 1 : worker->GetID() + 1;
            Worker *nextWorker = this->workers[nextWorkerID];
            int nextStationID = currentStationID + 1 > nextWorker->GetCurrentStation() ? nextWorker->GetCurrentStation() : currentStationID + 1;

            if (worker->IsAvailable(nextStationID))
            {
                this->MoveForward(worker);
            }
        }
        else if (worker->GetDirection() == Backward)
        {
            this->MoveBackward(worker);
        }
        else
        {
            this->MoveStay(worker);
        }
    }
}

void ProductionLine::GetIdleWorker(std::vector<Worker *> &idleWorkers)
{
    for (int i = (int)this->stations.size() - 1; i >= 0; i--)
    {
        Station *station = this->stations[i];
        std::vector<Worker *> _tmp = station->Handoff((int)this->stations.size());
        for (int j = 0; j < _tmp.size(); j++)
        {
            idleWorkers.push_back(_tmp[j]);
        }
    }
}

void ProductionLine::ArrangeHandoff()
{
    std::vector<Worker *> idleWorkers;

    this->GetIdleWorker(idleWorkers);

    while (!idleWorkers.empty())
    {
        this->ArrangeWorker(idleWorkers);
        idleWorkers.clear();
        this->GetIdleWorker(idleWorkers);
    }
}

void ProductionLine::ArrangeFinish()
{
    for (int i = 0; i < this->stations.size() - 1; i++)
    {
        Station *station = this->stations[i];
        Station *nextStation = this->stations[i + 1];
        while (!station->IsFinishEmpty())
        {
            Worker *worker = station->GetFinishWorker();
            if (worker->IsAvailable(station->GetID() + 1))
            {
                nextStation->AddWaitWorker(worker);
            }
            else
            {
                station->AddFinishWorker(worker);
                break;
            }
        }
    }
}

void ProductionLine::ArrangeWait()
{
    for (int i = 0; i < this->stations.size(); i++)
    {
        this->stations[i]->ArrangeWorker();
    }
}

double ProductionLine::Run()
{
    int productCnt = 0;

    for (int i = 0; i < this->workers.size(); i++)
    {
        int currentStation = this->workers[i]->GetCurrentStation();
        Station *station = this->stations[currentStation];
        station->AddWaitWorker(this->workers[i]);
    }

    this->ArrangeWait();
    while (productCnt < this->productNum)
    {
        double minWorkTime = this->GetMinWorkTime();

        this->time += minWorkTime;

        this->ProcessWork(minWorkTime, productCnt);

        this->ArrangeHandoff();

        this->ArrangeFinish();
        
        this->ArrangeHandoff();

        this->ArrangeWait();
        
        this->ArrangeHandoff();
    }

    // std::cout << "Throughput " << this->productNum / this->time << std::endl;

    return this->productNum / this->time;
}

std::vector<double> ProductionLine::GetSpeedOrder()
{
    std::vector<double> ret;
    for (int i = 0; i < this->workers.size(); i++)
    {
        ret.push_back(this->workers[i]->GetSpeed());
    }
    return ret;
}

#endif /* ProductionLine_cpp */
