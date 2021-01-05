//
//  Simulation.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Simulation_h
#define Simulation_h

#include <algorithm>
#include <cfloat>
#include <vector>
#include <string>
#include "../Tool/State.h"
#include "../Model/Station.h"
#include "../Model/Worker.h"

class Simulation
{
private:
    int stationNum;
    int workerNum;
    bool isFullCross;
    std::string stationPath;
    std::string workerPath;
    std::vector<Station> stations;
    std::vector<Worker> workers;

public:
    Simulation();

    ~Simulation();

    void SetStationNum(int stationNum);

    int GetStationNum();

    void SetWorkerNum(int workerNum);

    int GetWorkerNum();

    void SetFullorPartial(bool isFullCross);

    bool GetFullorPartial();

    void GenerateStations();

    void GenerateWorkers();

    std::vector<Station> GetStations();

    std::vector<Worker> GetWorkers();

    void StationWriteToFile(std::string filePath = "");

    void WorkerWriteToFile(std::string filePath = "");
};

#endif /* Simulation_h */
