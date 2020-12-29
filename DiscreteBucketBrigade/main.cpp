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

using namespace std;

int main(int argc, const char *argv[])
{
    Worker worker = Worker(1, Idle, 1.0, 1, vector<int>({1, 2, 3}));
    Station station = Station(1, Idle, 1.0);

    cout << "Hello World" << endl;
    cout << worker.GetID() << endl;
    cout << station.GetID() << endl;
    return 0;
}
