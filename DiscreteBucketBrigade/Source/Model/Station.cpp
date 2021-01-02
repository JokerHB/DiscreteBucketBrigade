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
    this->waitingQueue = std::priority_queue<Node>(newStation.waitingQueue);
    this->handoffStack = std::priority_queue<Node>(newStation.handoffStack);
    this->finishStack = std::priority_queue<Node>(newStation.finishStack);
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

void Station::FreeWorker()
{
    this->SetState(Idle);
    this->worker->SetState(Idle);
    this->worker->ResetPosition();
    this->worker = NULL;
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
    this->waitingQueue.push(Node(worker));
}

Worker *Station::GetWatiWorker()
{
    if (!this->waitingQueue.empty())
    {
        Worker *ret = this->waitingQueue.top().worker;
        this->waitingQueue.pop();
        return ret;
    }
    return NULL;
}

bool Station::IsWaitQueueEmpty()
{
    return this->waitingQueue.empty();
}

void Station::AddHandoffWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in handoff stack" << std::endl;
        exit(-5);
    }
    this->handoffStack.push(Node(worker));
}

Worker *Station::GetHandoffWorker()
{
    if (!this->handoffStack.empty())
    {
        Worker *ret = this->handoffStack.top().worker;
        this->handoffStack.pop();
        return ret;
    }
    return NULL;
}

bool Station::IshandoffStackEmpty()
{
    return this->handoffStack.empty();
}

void Station::AddFinishWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in finish stack" << std::endl;
        exit(-5);
    }
    this->finishStack.push(Node(worker));
}

Worker *Station::GetFinishWorker()
{
    if (!this->finishStack.empty())
    {
        Worker *ret = this->finishStack.top().worker;
        this->finishStack.pop();
        return ret;
    }
    return NULL;
}

bool Station::IsFinishStackEmpty()
{
    return this->finishStack.empty();
}

void Station::Station::Process(double workTime)
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

    if (currentPosition <= this->GetWorkContent())
    {
        this->worker->SetCurrentPosition(currentPosition);
    }
}

std::vector<Worker *> Station::Handoff(int stationNum)
{
    std::vector<Worker *> workers;

    if (this->worker != NULL)
    {
        double delta = this->worker->GetCurrentPosition() - this->GetWorkContent();
        delta = delta > 0 ? delta : -delta;

        if (delta <= this->EPS)
        {
            this->finishStack.push(Node(this->worker));
            this->FreeWorker();
        }
    }

    if (this->GetID() == stationNum - 1)
    {
        if (this->finishStack.empty())
        {
            return workers;
        }

        if (this->waitingQueue.empty())
        {
            Worker *forward = this->finishStack.top().worker;
            forward->SetDirection(Forward);
            workers.push_back(forward);
            this->finishStack.pop();
            return workers;
        }
        else
        {
            std::vector<Worker *> tmp;
            while (!this->waitingQueue.empty())
            {
                tmp.push_back(this->waitingQueue.top().worker);
                this->waitingQueue.pop();
            }

            Worker *forward = tmp[tmp.size() - 1];
            forward->AddHandoffPoint(this->GetID());
            forward->SetDirection(Forward);
            workers.push_back(forward);

            for (int i = 0; i < tmp.size() - 1; i++)
            {
                this->waitingQueue.push(tmp[i]);
            }

            Worker *backward = this->finishStack.top().worker;
            backward->AddHandoffPoint(this->GetID());
            backward->SetDirection(Backward);
            workers.push_back(backward);
            this->finishStack.pop();
        }

        return workers;
    }

    if (this->finishStack.empty())
    {
        return workers;
    }

    if (this->handoffStack.empty() && this->GetID() != stationNum - 1)
    {
        Worker *backward = this->finishStack.top().worker;
        backward->SetDirection(Backward);
        workers.push_back(backward);
        this->finishStack.pop();
        return workers;
    }

    Worker *forward = this->finishStack.top().worker;
    Worker *backward = this->handoffStack.top().worker;
    forward->AddHandoffPoint(this->GetID());
    forward->SetDirection(Forward);
    backward->AddHandoffPoint(this->GetID());
    backward->SetDirection(Backward);
    workers.push_back(forward);
    workers.push_back(backward);
    this->finishStack.pop();
    this->handoffStack.pop();

    return workers;
}

void Station::ArrangeWorker()
{
    if (this->worker == NULL)
    {
        if (!waitingQueue.empty())
        {
            Worker *_worker = this->waitingQueue.top().worker;
            this->SetWorker(_worker);
            this->waitingQueue.pop();
            std::cout << "------------------" << std::endl;
            std::cout << "Worker " << _worker->GetID() << " at Station " << this->GetID() << std::endl;
        }
    }
}

#endif /* Station_cpp */
