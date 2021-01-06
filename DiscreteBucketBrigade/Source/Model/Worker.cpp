//
//  Worker.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Worker_cpp
#define Worker_cpp

#include "../../Include/Model/Worker.h"

Worker::Worker(int id, State state, double speed, int currentStation, std::vector<int> operateZone)
{
    this->id = id;
    this->state = state;
    this->speed = speed;
    this->currentStation = currentStation;
    this->currentPosition = 0.0;
    this->operateZone = operateZone;
    this->direction = Forward;
}

Worker::Worker(const Worker &newWorker)
{
    this->id = newWorker.id;
    this->state = newWorker.state;
    this->speed = newWorker.speed;
    this->direction = newWorker.direction;
    this->currentStation = newWorker.currentStation;
    this->currentPosition = newWorker.currentPosition;
    this->operateZone = std::vector<int>(newWorker.operateZone);
    this->handoffPoint = std::vector<int>(newWorker.handoffPoint);
}

Worker::~Worker()
{
    this->operateZone.clear();
    this->handoffPoint.clear();
}

int Worker::GetID()
{
    return this->id;
}

double Worker::GetSpeed()
{
    return this->speed;
}

void Worker::SetSpeed(double speed)
{
    this->speed = speed;
}

State Worker::GetState()
{
    return this->state;
}

void Worker::SetState(const State &state)
{
    this->state = state;
}

Direction Worker::GetDirection()
{
    return this->direction;
}

void Worker::SetDirection(Direction direction)
{
    this->direction = direction;
}

double Worker::GetCurrentPosition()
{
    return this->currentPosition;
}

void Worker::SetCurrentPosition(double newPosition)
{
    this->currentPosition = newPosition;
}

int Worker::GetCurrentStation()
{
    return this->currentStation;
}

void Worker::SetCurrentStation(int newStation)
{
    this->currentStation = newStation;
}

std::vector<int> Worker::GetOperateZone()
{
    return this->operateZone;
}

std::vector<int> Worker::GetHandoffPoint()
{
    return this->handoffPoint;
}

void Worker::AddHandoffPoint()
{
    this->handoffPoint.push_back(this->currentStation);
}

void Worker::AddHandoffPoint(int point)
{
    this->handoffPoint.push_back(point);
}

void Worker::ResetPosition()
{
    this->SetCurrentPosition(0.0);
}

void Worker::MoveToNewStation(int stationID)
{
    this->SetCurrentStation(stationID);
    this->ResetPosition();
    this->SetState(Busy);
    this->SetDirection(Forward);
}

bool Worker::IsAvailable(int station)
{
    std::vector<int> operateZone = this->GetOperateZone();
    int _min = *std::min_element(operateZone.begin(), operateZone.end());
    int _max = *std::max_element(operateZone.begin(), operateZone.end());
    return station >= _min && station <= _max;
}

std::string Worker::ToString()
{
    // id state speed, currentStation, operatingzone
    std::ostringstream fmt;
    fmt << this->id << "\t" << this->state << "\t" << this->speed << "\t" << this->currentStation;
    for (int i = 0; i < this->operateZone.size(); i++)
    {
        fmt << "\t" << this->operateZone[i];
    }
    fmt << std::endl;
    return fmt.str();
}

#endif /* Worker_cpp */