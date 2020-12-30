//
//  Station.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Station_cpp
#define Station_cpp

#include "../../Include/Model/Station.h"
#include "../../Include/Tool/State.h"

const double Station::EPS = 1e-4;

Station::Station(int id, double workContent, State state)
{
    this->id = id;
    this->workContent = workContent;
    this->state = state;
    this->worker = NULL;
}

Station::Station(int id, State state, double workContent)
{
    this->id = id;
    this->workContent = workContent;
    this->state = state;
    this->worker = NULL;
}

Station::Station(int id, State state, Worker *worker, double workContent)
{
    this->id = id;
    this->workContent = workContent;
    this->state = state;
    this->worker = worker;
}

Station::Station(const Station &newStation)
{
    this->id = newStation.id;
    this->workContent = newStation.workContent;
    this->state = newStation.state;
    this->worker = newStation.worker;
    this->waitingQueue = std::queue<Worker *>(newStation.waitingQueue);
    this->handoffStack = std::stack<Worker *>(newStation.handoffStack);
    this->finishStack = std::stack<Worker *>(newStation.finishStack);
}

Station::~Station()
{
    this->worker = NULL;
}

int Station::GetID()
{
    return this->id;
}

void Station::SetWorker(Worker *woker)
{
    if (this->worker != NULL || this->state == Busy)
    {
        std::cout << "Error, can not assign worker " << woker->GetID() << " at station " << this->id << std::endl;
        exit(-1);
    }
    this->worker = woker;
    this->worker->SetState(Busy);
    this->SetState(Busy);
}

State Station::GetState()
{
    return this->state;
}

void Station::SetState(const State &state)
{
    this->state = state;
}

double Station::GetWorkContent()
{
    return this->workContent;
}

double Station::GetProcessTime()
{
    if (this->worker == NULL)
    {
        return -1.0;
    }
    double currentPosition = this->worker->GetCurrentPosition();

    return (this->workContent - currentPosition) / this->worker->GetSpeed();
}

void Station::AddWaitWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in waiting queue" << std::endl;
        exit(-4);
    }
    this->waitingQueue.push(worker);
}

Worker *Station::GetWatiWorker()
{
    if (!this->waitingQueue.empty())
    {
        Worker *ret = this->waitingQueue.front();
        this->waitingQueue.pop();
        return ret;
    }
    return NULL;
}

void Station::AddHandoffWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in handoff stack" << std::endl;
        exit(-5);
    }
    this->handoffStack.push(worker);
}

Worker *Station::GetHandoffWorker()
{
    if (!this->handoffStack.empty())
    {
        Worker *ret = this->handoffStack.top();
        this->handoffStack.pop();
        return ret;
    }
    return NULL;
}

void Station::AddFinishWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in finish stack" << std::endl;
        exit(-5);
    }
    this->finishStack.push(worker);
}

Worker *Station::GetFinishWorker()
{
    if (!this->finishStack.empty())
    {
        Worker *ret = this->finishStack.top();
        this->finishStack.pop();
        return ret;
    }
    return NULL;
}

Worker *Station::Station::Process(double workTime)
{
    if (this->worker == NULL)
    {
        std::cout << "Error, no worker at station " << this->GetID() << std::endl;
        exit(-2);
    }

    double currentPosition = this->worker->GetCurrentPosition();
    currentPosition += this->worker->GetSpeed() * workTime;

    if (currentPosition > this->GetWorkContent())
    {
        std::cout << "Error, worker " << this->worker->GetID() << " exceed station " << this->GetID() << std::endl;
        exit(-3);
    }

    if (currentPosition < this->GetWorkContent())
    {
        this->worker->SetCurrentPosition(currentPosition);
    }

    double delta = currentPosition - this->GetWorkContent();
    delta = delta > 0 ? delta : -delta;

    if (delta <= this->EPS)
    {
        this->SetState(Idle);
        this->worker->SetState(Idle);
        return this->worker;
    }
    return NULL;
}

Worker *Station::ArrangeWorker()
{
    Worker *_worker = NULL;
    if (this->handoffStack.empty())
    {
        _worker = this->GetWatiWorker();
    }
    
    return _worker;
}

#endif /* Station_cpp */