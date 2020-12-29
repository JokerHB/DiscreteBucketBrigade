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
    this->currentPosition = currentStation - 1.0;
    this->operateZone = operateZone;
}

Worker::Worker(const Worker &newWorker)
{
    this->id = newWorker.id;
    this->state = newWorker.state;
    this->speed = newWorker.speed;
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

State Worker::GetState()
{
    return this->state;
}

void Worker::SetState(const State &state)
{
    this->state = state;
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
    // TODO Update hand off list
}

void Worker::AddHandoffPoint(int point)
{
    this->handoffPoint.push_back(point);
}

#endif /* Worker_cpp */