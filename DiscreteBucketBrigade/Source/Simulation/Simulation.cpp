//
//  Simulation.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Simulation_cpp
#define Simulation_cpp

#include "../../Include/Simulation/Simulation.h"

void Simulation::Run(const std::vector<Station> &stations, const std::vector<Worker> &workers)
{
    int stationNum = stations.size();
    int workerNum = workers.size();

    while (true)
    {
        double minInterval = (double)(1 << 32);

        for (int i = 0; i < workerNum; i++)
        {
        }
    }
}

#endif /* Simulation_cpp */
