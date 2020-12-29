//
//  Simulation.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Simulation_h
#define Simulation_h

#include <vector>
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

    void Run(const std::vector<Station> &stations, const std::vector<Worker> &workers);
};

#endif /* Simulation_h */
