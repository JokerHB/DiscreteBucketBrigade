//
//  main.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#include <iostream>
#include <vector>
#include "./Include/Tool/State.h"
#include "./Include/Model/Event.h"
#include "./Include/Model/Station.h"
#include "./Include/Model/Worker.h"
#include "./Include/Simulation/Simulation.h"

using namespace std;

int main(int argc, const char *argv[])
{
    vector<Station> stations;
    vector<Worker> workers;
    Simulation simulation = Simulation();

    Worker worker0 = Worker(0, Idle, 1.0, 1, vector<int>({0, 1, 2}));
    Worker worker1 = Worker(1, Idle, 1.0, 2, vector<int>({0, 1, 2}));
    Station station0 = Station(0, Idle, 1.0);
    Station station1 = Station(1, Idle, 1.0);
    Station station2 = Station(2, Idle, 1.0);

    stations.push_back(station0);
    stations.push_back(station1);
    stations.push_back(station2);
    workers.push_back(worker0);
    workers.push_back(worker1);

    simulation.Run(stations, workers);

    return 0;
}
