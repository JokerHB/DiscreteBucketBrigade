//
//  test.cpp
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

int mainTest(int argc, const char *argv[])
{
    Simulation simulation = Simulation(5, 3, true, 2);

    // simulation.GenerateStations();
    // simulation.GenerateWorkers();

    // simulation.NormalizationSpeed();
    // simulation.NormalizationWorkContent();

    // simulation.StationWriteToFile();
    // simulation.WorkerWriteToFile();

    // simulation.StationReadFromFile("./station.txt");
    // simulation.WorkerReadFromFile("./worker.txt");

    vector<Station> stations = simulation.GetStations();
    vector<Worker> workers = simulation.GetWorkers();

    for (int i = 1; i < 101; i++)
    {
        double v0 = (double)i / 10.0;
        double v1 = 1.0;
        // double v2 = 1.5;
        stations.clear();
        workers.clear();
        Worker worker0 = Worker(0, Idle, v0, 0, vector<int>({0, 1, 2, 3, 4}));
        Worker worker1 = Worker(1, Idle, v1, 1, vector<int>({0, 1, 2, 3, 4}));
        // Worker worker2 = Worker(2, Idle, v2, 4, vector<int>({0, 1, 2, 3, 4}));
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
        // workers.push_back(worker2);

        // for (int i = 0; i < stations.size(); i++)
        // {
        //     cout << stations[i].ToString() << endl;
        // }

        // for (int i = 0; i < workers.size(); i++)
        // {
        //     cout << workers[i].ToString() << endl;
        // }

        ProductionLine productionLine = ProductionLine(stations, workers, 10000);
        double throughput = productionLine.Run();
        cout << v0 / v1 << endl;
        cout << "Maxmim Throughput is " << (v0 + v1) / 5.0 << endl;
        cout << "Rate is " << throughput / ((v0 + v1) / 5.0) << endl;
        // cout << "Maximal Throughput is 1.0" << endl;
        // cout << "Rate is " << throughput << endl;
    }

    return 0;
}
