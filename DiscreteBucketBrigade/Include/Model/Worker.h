//
//  Worker.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Worker_h
#define Worker_h

#include <algorithm>
#include <iostream>
#include <vector>
#include "../Tool/State.h"

class Worker
{
private:
    int id;
    State state;
    double speed;
    double currentPosition;
    int currentStation;
    std::vector<int> operateZone;
    std::vector<int> handoffPoint;

public:
    Worker(int id, State state, double speed, int currentStation, std::vector<int> operateZone);

    Worker(const Worker &newWorker);

    ~Worker();

    int GetID();

    double GetSpeed();

    State GetState();

    void SetState(const State &state);

    double GetCurrentPosition();

    void SetCurrentPosition(double newPosition);

    int GetCurrentStation();

    void SetCurrentStation(int newStation);

    std::vector<int> GetOperateZone();

    std::vector<int> GetHandoffPoint();

    void AddHandoffPoint();

    void AddHandoffPoint(int point);

    void MoveToNewStation(int stationID);

    bool IsAvailable(int station);
};

#endif /* Worker_h */
