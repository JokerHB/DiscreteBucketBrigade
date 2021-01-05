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

void Simulation::SetStationNum(int stationNum)
{
    this->stationNum = stationNum;
}

int Simulation::GetStationNum()
{
    return this->stationNum;
}

void Simulation::SetWorkerNum(int workerNum)
{
    this->workerNum = workerNum;
}

int Simulation::GetWorkerNum()
{
    return this->workerNum;
}

void Simulation::SetFullorPartial(bool isFullCross)
{
    this->isFullCross = isFullCross;
}

bool Simulation::GetFullorPartial()
{
    return this->isFullCross;
}

void Simulation::GenerateStations()
{
    // TODO Generate Stations
}

void Simulation::GenerateWorkers()
{
    // TODO Generate Workers
}

std::vector<Station> Simulation::GetStations()
{
    return this->stations;
}

std::vector<Worker> Simulation::GetWorkers()
{
    return this->workers;
}

void Simulation::StationWriteToFile(std::string filePath)
{
    if (filePath == "")
    {
        filePath = this->stationPath;
    }
}

void Simulation::WorkerWriteToFile(std::string filePath)
{
    if (filePath == "")
    {
        filePath = this->workerPath;
    }
}

#endif /* Simulation_cpp */
