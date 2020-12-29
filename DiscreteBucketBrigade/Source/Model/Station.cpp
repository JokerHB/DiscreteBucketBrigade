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

Station::Station(int id, double workContent, State state)
{
    this->id = id;
    this->workContent = workContent;
    this->state = state;
}

Station::Station(int id, State state, double workContent)
{
    this->id = id;
    this->workContent = workContent;
    this->state = state;
}

Station::Station(const Station &newStation)
{
    this->id = newStation.id;
    this->workContent = newStation.workContent;
    this->state = newStation.state;
}

Station::~Station() {}

int Station::GetID()
{
    return this->id;
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

#endif /* Station_cpp */