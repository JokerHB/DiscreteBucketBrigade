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
    this->waiting = std::vector<Worker *>(newStation.waiting);
    this->handoff = std::vector<Worker *>(newStation.handoff);
    this->finish = std::vector<Worker *>(newStation.finish);
}

Station::~Station()
{
    this->worker = NULL;
    this->waiting.clear();
    this->handoff.clear();
    this->finish.clear();
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
    this->worker->MoveToNewStation(this->GetID());
    this->SetState(Busy);
}

void Station::FreeWorker()
{
    if (this->worker == NULL || this->state == Idle)
    {
        std::cout << "Error, can not free worker at station " << this->id << std::endl;
        exit(-7);
    }
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

void Station::SetWorkContent(double workContent)
{
    this->workContent = workContent;
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

void Station::RemoveItem(std::vector<Worker *> &vec, Worker *item)
{
    for (std::vector<Worker *>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        if (*it == item)
        {
            vec.erase(it);
            break;
        }
    }
}

void Station::AddWaitWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in waiting queue" << std::endl;
        exit(-4);
    }
    worker->SetCurrentStation(this->GetID());
    this->waiting.push_back(worker);
}

Worker *Station::GetWatiWorker()
{
    if (!this->waiting.empty())
    {
        Worker *ret = this->waiting[0];

        for (int i = 1; i < this->waiting.size(); i++)
        {
            if (ret->GetID() < this->waiting[i]->GetID())
            {
                ret = this->waiting[i];
            }
        }
        this->RemoveItem(this->waiting, ret);

        return ret;
    }
    return NULL;
}

Worker *Station::GetLastWatiWorker()
{
    if (!this->waiting.empty())
    {
        Worker *ret = this->waiting[0];

        for (int i = 1; i < this->waiting.size(); i++)
        {
            if (ret->GetID() > this->waiting[i]->GetID())
            {
                ret = this->waiting[i];
            }
        }
        this->RemoveItem(this->waiting, ret);

        return ret;
    }
    return NULL;
}

bool Station::IsWaitEmpty()
{
    return this->waiting.empty();
}

void Station::AddHandoffWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in handoff stack" << std::endl;
        exit(-5);
    }
    worker->SetCurrentStation(this->GetID());
    this->handoff.push_back(worker);
}

Worker *Station::GetHandoffWorker()
{
    if (!this->handoff.empty())
    {
        Worker *ret = this->handoff[0];

        for (int i = 1; i < this->handoff.size(); i++)
        {
            if (ret->GetID() < this->handoff[i]->GetID())
            {
                ret = this->handoff[i];
            }
        }
        this->RemoveItem(this->handoff, ret);
        return ret;
    }
    return NULL;
}

bool Station::IsHandoffEmpty()
{
    return this->handoff.empty();
}

void Station::AddFinishWorker(Worker *worker)
{
    if (worker == NULL)
    {
        std::cout << "Error, worker is NULL, can not be added in finish stack" << std::endl;
        exit(-6);
    }
    worker->SetCurrentStation(this->GetID());
    this->finish.push_back(worker);
}

Worker *Station::GetFinishWorker()
{
    if (!this->finish.empty())
    {
        Worker *ret = this->finish[0];

        for (int i = 1; i < this->finish.size(); i++)
        {
            if (ret->GetID() > this->finish[i]->GetID())
            {
                ret = this->finish[i];
            }
        }
        this->RemoveItem(this->finish, ret);
        return ret;
    }
    return NULL;
}

bool Station::IsFinishEmpty()
{
    return this->finish.empty();
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

    if (currentPosition - this->GetWorkContent() > this->EPS)
    {
        std::cout << "Error, worker " << this->worker->GetID() << " exceed station " << this->GetID() << std::endl;
        exit(-3);
    }

    double delta = currentPosition - this->GetWorkContent();
    this->worker->SetCurrentPosition(currentPosition);
    delta = delta > 0 ? delta : -delta;

    if (delta <= this->EPS)
    {
        this->AddFinishWorker(this->worker);
        this->FreeWorker();
    }
}

bool Station::CanWalkBack(Worker *worker)
{
    int preWorkerID = worker->GetID() - 1;
    if (preWorkerID == -1)
    {
        return true;
    }

    if (this->IsWaitEmpty() && this->IsFinishEmpty())
    {
        return true;
    }
    return false;
}

std::vector<Worker *> Station::Handoff(int stationNum)
{
    std::vector<Worker *> handOffWorkers;

    if (this->GetID() == stationNum - 1)
    {
        while (!this->IsWaitEmpty() && !this->IsFinishEmpty())
        {
            Worker *wait = this->GetLastWatiWorker();
            Worker *finish = this->GetFinishWorker();
            wait->SetDirection(Backward);
            finish->SetDirection(Forward);
            // wait->AddHandoffPoint();
            // finish->AddHandoffPoint();
            handOffWorkers.push_back(wait);
            handOffWorkers.push_back(finish);
        }
        while (!this->IsFinishEmpty())
        {
            Worker *finish = this->GetFinishWorker();
            finish->SetDirection(Backward);
            handOffWorkers.push_back(finish);
        }
        return handOffWorkers;
    }

    while (!this->IsHandoffEmpty() && !this->IsFinishEmpty())
    {
        Worker *handoff = this->GetHandoffWorker();
        Worker *finish = this->GetFinishWorker();
        handoff->SetDirection(Forward);
        finish->SetDirection(Backward);
        // handoff->AddHandoffPoint();
        // finish->AddHandoffPoint();
        handOffWorkers.push_back(finish);
        handOffWorkers.push_back(handoff);
    }

    while (this->IsFinishEmpty() && !this->IsHandoffEmpty() && !this->IsWaitEmpty())
    {
        Worker *handoff = this->GetHandoffWorker();
        if (handoff->IsAvailable(this->GetID()))
        {
            Worker *wait = this->GetLastWatiWorker();
            handoff->SetDirection(Forward);
            wait->SetDirection(Backward);
            // handoff->AddHandoffPoint();
            // wait->AddHandoffPoint();
            handOffWorkers.push_back(wait);
            handOffWorkers.push_back(handoff);
        }
        else
        {
            this->AddHandoffWorker(handoff);
            break;
        }
    }

    return handOffWorkers;
}

void Station::ArrangeWorker()
{
    if (this->worker == NULL)
    {
        if (!waiting.empty())
        {
            Worker *_worker = this->GetWatiWorker();
            this->SetWorker(_worker);
            // std::cout << "------------------" << std::endl;
            // std::cout << "Worker " << _worker->GetID() << " at Station " << this->GetID() << std::endl;
        }
    }
}

std::string Station::ToString()
{
    // id state workContent
    std::ostringstream fmt;
    fmt << this->id << "\t" << this->state << "\t" << this->workContent << std::endl;
    return fmt.str();
}

#endif /* Station_cpp */
