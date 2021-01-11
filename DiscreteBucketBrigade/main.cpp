//
//  main.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
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

double CalStdev(vector<double> resultSet)
{
    double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
    double mean = sum / resultSet.size();
    double accum = 0.0;
    std::for_each(std::begin(resultSet), std::end(resultSet), [&](const double d) {
        accum += (d - mean) * (d - mean);
    });
    double stdev = sqrt(accum / (resultSet.size() - 1));
    return stdev;
}

double CalMean(vector<double> resultSet)
{
    double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
    double mean = sum / resultSet.size();
    return mean;
}

double GetMax(vector<double> resultSet)
{
    double ret = resultSet[0];

    for (int i = 1; i < resultSet.size(); i++)
    {
        ret = ret < resultSet[i] ? resultSet[i] : ret;
    }
    return ret;
}

double GetMin(vector<double> resultSet)
{
    double ret = resultSet[0];

    for (int i = 1; i < resultSet.size(); i++)
    {
        ret = ret > resultSet[i] ? resultSet[i] : ret;
    }
    return ret;
}

void WriteToFile(string filePath, double mean, double stdev, double min, double max, vector<double> speedList)
{
    ofstream writer;

    writer.open(filePath, ios::app);
    if (!writer.is_open())
    {
        std::cout << "Error, can not write to " << filePath << std::endl;
        exit(-14);
    }
    ostringstream fmt;
    for (int i = 0; i < speedList.size(); i++)
    {
        if (i != speedList.size() - 1)
            fmt << speedList[i] << " ";
        else
            fmt << speedList[i] << ", ";
    }
    fmt << mean << ", " << stdev << ", " << min << ", " << max << endl;
    writer << fmt.str();
    writer.close();
}

int main(int argc, const char *argv[])
{
    int cnt = 0;
    string filePath = "result.csv";

    for (int i = 3; i < 11; i += 2)
    {
        for (int j = 2; j < i; j ++)
        {
            Simulation simulation = Simulation(i, j, true, 2);
            simulation.GenerateWorkers(0.5);
            simulation.NormalizationSpeed();
            vector<Worker> workers = simulation.GetWorkers();
            vector<double> speedList = GetSpeedList(workers);
            sort(speedList.begin(), speedList.end());
            int iterNum = 30;

            vector<double> throughputs;
            for (int k = 0; k < iterNum; k++)
            {
                SetNewSpeed(workers, speedList);
                vector<Worker> _workers = vector<Worker>(workers);
                simulation.ClearStation();
                simulation.GenerateStations();
                simulation.NormalizationWorkContent();
                vector<Station> stations = simulation.GetStations();
                ProductionLine productionLine = ProductionLine(stations, _workers, 1000);
                double throughput = productionLine.Run();
                throughputs.push_back(throughput);
                cout << i << " " << j << " " << k << endl;
            }
            double stdev = CalStdev(throughputs);
            double mean = CalMean(throughputs);
            double min = GetMin(throughputs);
            double max = GetMax(throughputs);
            WriteToFile(filePath, mean, stdev, min, max, speedList);

            while (next_permutation(speedList.begin(), speedList.end()))
            {
                vector<double> throughputs;
                for (int k = 0; k < iterNum; k++)
                {
                    SetNewSpeed(workers, speedList);
                    vector<Worker> _workers = vector<Worker>(workers);
                    simulation.ClearStation();
                    simulation.GenerateStations();
                    simulation.NormalizationWorkContent();
                    vector<Station> stations = simulation.GetStations();
                    ProductionLine productionLine = ProductionLine(stations, _workers, 1000);
                    double throughput = productionLine.Run();
                    throughputs.push_back(throughput);
                    cout << i << " " << j << " " << k << endl;
                }
                double stdev = CalStdev(throughputs);
                double mean = CalMean(throughputs);
                double min = GetMin(throughputs);
                double max = GetMax(throughputs);
                WriteToFile(filePath, mean, stdev, min, max, speedList);
            }

            cout << cnt++ << endl;
        }
    }

    return 0;
}
