//
//  Simulation.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Simulation_h
#define Simulation_h

#include <algorithm>
#include <vector>
#include <cfloat>
#include "../Tool/State.h"
#include "../Model/Station.h"
#include "../Model/Worker.h"

class Simulation
{
private:
    double time;

public:
    Simulation();

    ~Simulation();

    double GetMinWorkTime(std::vector<Station> &stations, std::vector<Worker> &workers);

    std::vector<Worker *> GetIdleWorker(std::vector<Station> &stations, std::vector<Worker> &workers, double minWorkTime);

    void Run(std::vector<Station> &stations, std::vector<Worker> &workers, int productNumber = 1000);
};

#endif /* Simulation_h */
