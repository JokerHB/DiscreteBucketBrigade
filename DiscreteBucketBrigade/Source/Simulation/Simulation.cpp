//
//  Simulation.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Simulation_cpp
#define Simulation_cpp

#include "../../Include/Simulation/Simulation.h"

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> res;
    if ("" == str)
        return res;
    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p)
    {
        std::string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }

    return res;
}

Simulation::Simulation()
{
    this->stationPath = "./station.txt";
    this->workerPath = "./worker.txt";
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
    Station station0 = Station(0, Idle, 1.0);
    Station station1 = Station(1, Idle, 1.0);
    Station station2 = Station(2, Idle, 1.0);
    Station station3 = Station(3, Idle, 1.0);
    Station station4 = Station(4, Idle, 1.0);

    this->stations.push_back(station0);
    this->stations.push_back(station1);
    this->stations.push_back(station2);
    this->stations.push_back(station3);
    this->stations.push_back(station4);
}

void Simulation::GenerateWorkers()
{
    // TODO Generate Workers
    double v0 = 0.5;
    double v1 = 1.0;
    double v2 = 1.5;
    Worker worker0 = Worker(0, Idle, v0, 2, std::vector<int>({0, 1, 2}));
    Worker worker1 = Worker(1, Idle, v1, 3, std::vector<int>({2, 3}));
    Worker worker2 = Worker(2, Idle, v2, 4, std::vector<int>({3, 4}));

    this->workers.push_back(worker0);
    this->workers.push_back(worker1);
    this->workers.push_back(worker2);
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
    std::ofstream writer;

    if (filePath == "")
    {
        filePath = this->stationPath;
    }

    writer.open(filePath);
    if (!writer.is_open())
    {
        std::cout << "Error, can not write to " << filePath << std::endl;
        exit(-8);
    }

    writer << "ID\tState\tWorkContent\n";

    for (int i = 0; i < this->stations.size(); i++)
    {
        std::string info = this->stations[i].ToString();
        writer << info;
    }

    writer.close();
}

void Simulation::WorkerWriteToFile(std::string filePath)
{
    std::ofstream writer;

    if (filePath == "")
    {
        filePath = this->workerPath;
    }

    writer.open(filePath);
    if (!writer.is_open())
    {
        std::cout << "Error, can not write to " << filePath << std::endl;
        exit(-9);
    }

    writer << "ID\tState\tSpeed\tCurrentStation\tOperatingZone\n";

    for (int i = 0; i < this->workers.size(); i++)
    {
        std::string info = this->workers[i].ToString();
        writer << info;
    }

    writer.close();
}

void Simulation::StationReadFromFile(std::string filePath)
{
    std::ifstream reader;

    reader.open(filePath);
    if (!reader.is_open())
    {
        std::cout << "Error, can not read " << filePath << std::endl;
        exit(-10);
    }
    std::string tmp;
    std::getline(reader, tmp);
    while (reader.good() && !reader.eof())
    {
        std::string info;
        std::getline(reader, info);
        if (info == "")
            continue;
        std::vector<std::string> infos = split(info, "\t");

        if (infos.size() != 3)
        {
            std::cout << "Error, file " << filePath << " error" << std::endl;
            exit(-11);
        }

        int id = std::stoi(infos[0]);
        State state = std::stoi(infos[1]) == 1 ? Idle : Busy;
        double workContent = std::stod(infos[2]);
        this->stations.push_back(Station(id, state, workContent));
    }

    reader.close();
}

void Simulation::WorkerReadFromFile(std::string filePath)
{
    std::ifstream reader;

    reader.open(filePath);
    if (!reader.is_open())
    {
        std::cout << "Error, can not read " << filePath << std::endl;
        exit(-12);
    }

    std::string tmp;
    std::getline(reader, tmp);
    while (reader.good() && !reader.eof())
    {
        std::string info;
        std::getline(reader, info);
        if (info == "")
            continue;
        std::vector<std::string> infos = split(info, "\t");

        if (infos.size() < 5)
        {
            std::cout << "Error, file " << filePath << " error" << std::endl;
            exit(-13);
        }

        int id = std::stoi(infos[0]);
        State state = std::stoi(infos[1]) == 1 ? Idle : Busy;
        double speed = std::stod(infos[2]);
        int currentStation = std::stoi(infos[3]);
        std::vector<int> operatingZone;
        for (int i = 4; i < infos.size(); i++)
        {
            operatingZone.push_back(std::stoi(infos[i]));
        }
        this->workers.push_back(Worker(id, state, speed, currentStation, operatingZone));
    }

    reader.close();
}

#endif /* Simulation_cpp */
