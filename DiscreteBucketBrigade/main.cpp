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

bool cmp(Worker a, Worker b)
{
    return a.GetID() < b.GetID();
}

vector<double> GetSpeedList(vector<Worker> &workers)
{
    vector<double> ret;

    for (int i = 0; i < workers.size(); i++)
    {
        ret.push_back(workers[i].GetSpeed());
    }

    return ret;
}

void SetNewSpeed(vector<Worker> &workers, vector<double> &speedList)
{
    for (int i = 0; i < workers.size(); i++)
    {
        workers[i].ResetWorker();
        workers[i].SetSpeed(speedList[i]);
    }
}

int mainTest(int argc, const char *argv[])
{
    for (int i = 5; i < 7; i += 2)
    {
        for (int j = 3; j < i; j += 2)
        {
            Simulation simulation = Simulation(i, j, true, 2);
            simulation.GenerateWorkers();
            simulation.NormalizationSpeed();
            vector<Worker> workers = simulation.GetWorkers();
            vector<double> speedList = GetSpeedList(workers);
            sort(speedList.begin(), speedList.end());
            while (next_permutation(speedList.begin(), speedList.end()))
            {
                for (int k = 0; k < 1; k++)
                {
                    SetNewSpeed(workers, speedList);
                    vector<Worker> _workers = vector<Worker>(workers);
                    simulation.ClearStation();
                    simulation.GenerateStations();
                    simulation.NormalizationWorkContent();
                    vector<Station> stations = simulation.GetStations();
                    ProductionLine productionLine = ProductionLine(stations, _workers, 10);
                    double throughput = productionLine.Run();
                    vector<double> speedOrder = productionLine.GetSpeedOrder();
                    for (int i = 0; i < speedOrder.size(); i++)
                    {
                        cout << speedOrder[i] << " ";
                    }
                    cout << endl;
                    cout << throughput << endl;
                }
            }

            sort(speedList.begin(), speedList.end());
            for (int k = 0; k < 1; k++)
            {
                SetNewSpeed(workers, speedList);
                vector<Worker> _workers = vector<Worker>(workers);
                simulation.ClearStation();
                simulation.GenerateStations();
                simulation.NormalizationWorkContent();
                vector<Station> stations = simulation.GetStations();
                ProductionLine productionLine = ProductionLine(stations, _workers, 10);
                double throughput = productionLine.Run();
                vector<double> speedOrder = productionLine.GetSpeedOrder();
                for (int i = 0; i < speedOrder.size(); i++)
                {
                    cout << speedOrder[i] << " ";
                }
                cout << endl;
                cout << throughput << endl;
            }
        }
    }

    return 0;
}
