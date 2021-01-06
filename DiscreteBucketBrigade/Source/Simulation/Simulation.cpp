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

Simulation::Simulation(int stationNum, int workerNum, bool isFullCross, int overlapNum, std::string stationPath, std::string workerPath)
{
    this->stationNum = stationNum;
    this->workerNum = workerNum;
    this->isFullCross = isFullCross;
    this->stationPath = stationPath;
    this->workerPath = workerPath;
    this->overlapNum = overlapNum;
}

Simulation::~Simulation()
{
    this->stations.clear();
    this->workers.clear();
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
    RandomGenerator workContent = RandomGenerator(1.0, 0.5, 0.6, 6);
    for (int i = 0; i < this->stationNum; i++)
    {
        this->stations.push_back(Station(i, Idle, workContent()));
    }
}

void Simulation::GenerateWorkers()
{
    RandomGenerator speed = RandomGenerator(1.0, 0.5, 0.5, 2);

    for (int i = 0; i < this->workerNum; i++)
    {
        // ID\tState\tSpeed\tCurrentStation\tOperatingZone\n
        double workerSpeed = speed();
        int currentStation = 0;
        if (i == 0)
        {
            currentStation = RandomGenerator::GenerateRandomInt(0, this->stationNum - this->workerNum);
        }
        else
        {
            currentStation = RandomGenerator::GenerateRandomInt(this->workers[i - 1].GetCurrentStation() + 1, this->stationNum - this->workerNum + i);
        }

        if (this->isFullCross)
        {
            std::vector<int> operatingZone;
            for (int j = 0; j < this->stationNum; j++)
            {
                operatingZone.push_back(j);
            }
            this->workers.push_back(Worker(i, Idle, workerSpeed, currentStation, operatingZone));
        }
        else
        {
            // TODO Add operating zone generate method
            // std::vector<int> operatingZone;
            // int len = this->stationNum / this->workerNum + this->overlapNum;
            // len = this->stationNum % this->workerNum == 0 ? len : len + 1;

            // for (int j = 0; j < len; j++)
            // {
            //     operatingZone.push_back(len -  + j + j);
            // }
        }
    }
}

void Simulation::NormalizationWorkContent()
{
    double totalWorkContent = 0.0;

    for (int i = 0; i < this->stationNum; i++)
    {
        totalWorkContent += this->stations[i].GetWorkContent();
    }

    for (int i = 0; i < this->stationNum; i++)
    {
        this->stations[i].SetWorkContent(this->stations[i].GetWorkContent() / totalWorkContent);
    }
}

void Simulation::NormalizationSpeed()
{
    double totalSpeed = 0.0;

    for (int i = 0; i < this->workerNum; i++)
    {
        totalSpeed += this->workers[i].GetSpeed();
    }

    for (int i = 0; i < this->workerNum; i++)
    {
        this->workers[i].SetSpeed(this->workers[i].GetSpeed() / totalSpeed);
    }
}

void Simulation::ClearWorker()
{
    this->workers.clear();
}

void Simulation::ClearStation()
{
    this->stations.clear();
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

    if (filePath == "")
    {
        filePath = this->workerPath;
    }

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

    if (filePath == "")
    {
        filePath = this->workerPath;
    }

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
