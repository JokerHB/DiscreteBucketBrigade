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
#include "./Include/Model/ProductionLine.h"
#include "./Include/Simulation/Simulation.h"

using namespace std;

int main(int argc, const char *argv[])
{
    vector<Station> stations;
    vector<Worker> workers;
    Simulation simulation = Simulation();

    double v0 = 5;
    double v1 = 1.0;

    Worker worker0 = Worker(0, Idle, v0, 3, vector<int>({0, 1, 2, 3, 4}));
    Worker worker1 = Worker(1, Idle, v1, 4, vector<int>({0, 1, 2, 3, 4}));
    Station station0 = Station(0, Idle, 1.0);
    Station station1 = Station(1, Idle, 1.0);
    Station station2 = Station(2, Idle, 1.0);
    Station station3 = Station(3, Idle, 1.0);
    Station station4 = Station(4, Idle, 1.0);

    stations.push_back(station0);
    stations.push_back(station1);
    stations.push_back(station2);
    stations.push_back(station3);
    stations.push_back(station4);
    workers.push_back(worker0);
    workers.push_back(worker1);

    ProductionLine productionLine = ProductionLine(stations, workers);
    double throughput = productionLine.Run();
    // simulation.Run(stations, workers);
    cout << "Maxmim Throughput is " << (v0 + v1) / 5.0 << endl;
    cout << "Rate is " << throughput / ((v0 + v1) / 5.0) << endl;
    return 0;
}
