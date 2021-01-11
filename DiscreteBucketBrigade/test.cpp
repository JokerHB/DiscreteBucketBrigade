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
        // Worker worker0 = Worker(0, Idle, 0.11471859053148356, 0, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        // Worker worker1 = Worker(1, Idle, 0.13879637860907498, 1, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        // Worker worker2 = Worker(2, Idle, 0.062336482774343303, 2, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        // Worker worker3 = Worker(3, Idle, 0.1523751284601299, 3, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        // Worker worker4 = Worker(4, Idle, 0.18727799731730074, 4, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        // Worker worker5 = Worker(5, Idle, 0.18355352346865111, 5, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        // Worker worker6 = Worker(6, Idle, 0.16094189883901641, 6, vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        Worker worker0 = Worker(0, Idle, 1.0, 0, vector<int>({0, 1, 2, 3, 4}));
        Worker worker1 = Worker(1, Idle, 1.0, 1, vector<int>({0, 1, 2, 3, 4}));
        Worker worker2 = Worker(2, Idle, 1.0, 2, vector<int>({0, 1, 2, 3, 4}));
        Worker worker3 = Worker(3, Idle, 1.0, 3, vector<int>({0, 1, 2, 3, 4}));
        Worker worker4 = Worker(4, Idle, 1.0, 4, vector<int>({0, 1, 2, 3, 4}));
        
        

        //       Station station0 = Station(0, Idle, 0.098481985523746623);
        //       Station station1 = Station(1, Idle, 0.064461508053420574);
        //       Station station2 = Station(2, Idle, 0.056807992117188559);
        //       Station station3 = Station(3, Idle, 0.14248038625100407);
        //       Station station4 = Station(4, Idle, 0.14377251574939126);
        //       Station station5 = Station(5, Idle, 0.084358812286077015);
        //       Station station6 = Station(6, Idle, 0.14489208869859624);
        //       Station station7 = Station(7, Idle, 0.1047476501342775);
        //       Station station8 = Station(8, Idle, 0.15999706118629817);
        Station station0 = Station(0, Idle, 1.0);
        Station station1 = Station(1, Idle, 1.0);
        Station station2 = Station(2, Idle, 1.0);
        Station station3 = Station(3, Idle, 1.0);
        Station station4 = Station(4, Idle, 3.0);

        stations.push_back(station0);
        stations.push_back(station1);
        stations.push_back(station2);
        stations.push_back(station3);
        stations.push_back(station4);
        //       stations.push_back(station5);
        //       stations.push_back(station6);
        //       stations.push_back(station7);
        //       stations.push_back(station8);

        workers.push_back(worker0);
        workers.push_back(worker1);
        workers.push_back(worker2);
        workers.push_back(worker3);
        workers.push_back(worker4);
        //       workers.push_back(worker2);
        //       workers.push_back(worker3);
        //       workers.push_back(worker4);
        //       workers.push_back(worker5);
        //       workers.push_back(worker6);

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
        cout << "Maximal Throughput is " << (5.0) / 7.0 << endl;
        cout << throughput <<endl;
        exit(-1);
        
        cout << v0 / v1 << endl;
        cout << "Maxmal Throughput is " << (v0 + v1) / 5.0 << endl;
        cout << "Rate is " << throughput / ((v0 + v1) / 5.0) << endl;

        //       cout << "Maximal Throughput is 1.0" << endl;
        //       cout << "Rate is " << throughput << endl;
    }

    return 0;
}
